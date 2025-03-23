#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Token {
    int row, col;
    char type[50];
    char value[50];
};

struct Token* makeToken(int row, int col, char type[], char value[]) {
    struct Token* token = (struct Token*) malloc(sizeof(struct Token));
    token->row = row;
    token->col = col;
    strcpy(token->type, type);
    strcpy(token->value, value);
    return token;
}

void printToken(struct Token* token) {
    printf("<%s,%s,%d,%d>", token->type, token->value, token->row, token->col);
}

char keywords[26][10] = {
    "break", "case", "char", "const", "continue", "default",
    "do", "double", "else", "enum", "float", "for",
    "if", "int", "long", "return", "short", "signed",
    "sizeof", "static", "struct", "switch", "typedef",
    "unsigned", "void", "while"
};

char buf[100];

FILE *fp;
int row = 1, col = 1, ind = 0;

int handleComments();
void handleSpaces(char in);
void handleDirectives();
struct Token* handleQuotes(char in);
struct Token* getArithOp(char in);
struct Token* getRelOp(char in);
struct Token* getLogOp(char in);
struct Token* getKeyOrId(char in);
struct Token* getNumber(char c);

int isKeyword(char* str) {
    for (int i = 0; i < 32; i++) {
        if (strcmp(keywords[i], str) == 0) {
            return 1;
        }
    }
    return 0;
}

int handleComments() {
    char c = fgetc(fp);
    int flag = 0;
    // single line comment
    if (c == '/') {
        while ((c = fgetc(fp)) != '\n');
        printf("\n");
        row++;
        flag = 1;
    }
    // multi line comment
    else if (c == '*') {
        char prev = '\0';
        while ((c = fgetc(fp)) != EOF) {
            if (prev == '*' && c == '/') {
                flag = 1;
                break;
            }
            if (c == '\n') {
                printf("\n");
                row++;
                col = 1;
            }
            else {
                col++;
            }
            prev = c;
        }
    }
    // not a comment
    else {
        ungetc(c, fp);
    }
    return flag;
}

void handleSpaces(char in) {
    if(in == '\n') {
        printf("\n");
        row++;
        col = 1;
    }
    char c;
    while ((c = fgetc(fp)) != EOF) {
        // if c is space, tab, newline etc.
        if (isspace(c)) {
            // deal with newline
            if (c == '\n') {
                printf("\n");
                row++;
                col = 1;
            }
            else {
                col++;
            }
        }
        else {
            ungetc(c, fp);
            break;
        }
    }
}

void handleDirectives() {
    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            printf("\n");
            row++;
            col = 1;
            break;
        }
        else {
            col++;
        }
    }
}

struct Token* handleQuotes(char in) {
    ind = 0;
    buf[ind++] = in;
    char c;
    while ((c = fgetc(fp)) != EOF) {
        col++;
        buf[ind++] = c;
        if (c == in) {
            buf[ind] = '\0';
            return makeToken(row, col, "STRING", buf);
        }
        // escape sequences
        if (c == '\\') {
            char next = fgetc(fp);
            if (next == EOF) {
                printf("Error: Unexpected EOF\n");
                exit(1);
            }
            col++;
            buf[ind++] = next;
        }
        else if (c == '\n') {
            printf("\n");
            row++;
            col = 1;
        }
    }
    return NULL;
}

struct Token* getArithOp(char in) {
    buf[ind++] = in;
    char c = fgetc(fp);
    col++;
    // ++, +=, --, -=
    if ((in == '+' || in == '-') && ((c == in) || (c == '='))) {
        buf[ind++] = c;
        col++;
    }
    // /=, %=, *=
    else if ((in == '*' || in == '/' || in == '%') && c == '=') {
        buf[ind++] = c;
        col++;
    }
    // +, -, *, /, %
    else {
        ungetc(c, fp);
    }
    buf[ind] = '\0';
    return makeToken(row, col, "ARITHMETIC_OPERATOR", buf);
}

struct Token* getRelOp(char in) {
    buf[ind++] = in;
    char c = fgetc(fp);
    col++;
    // ==, !=, <=, >=
    if (c == '=') {
        buf[ind++] = c;
        col++;
    }
    // =, <, >, !
    else {
        ungetc(c, fp);
    }
    buf[ind] = '\0';
    return makeToken(row, col, "RELATIONAL_OPERATOR", buf);
}

struct Token* getLogOp(char in) {
    buf[ind++] = in;
    char c = fgetc(fp);
    col++;
    // &&, ||
    if (c == in) {
        buf[ind++] = c;
        col++;
    }
    // &, |
    else {
        ungetc(c, fp);
    }
    buf[ind] = '\0';
    return makeToken(row, col, "LOGICAL_OPERATOR", buf);
}

struct Token* getKeyOrId(char in) {
    buf[ind++] = in;
    char c;
    col++;
    while ((c = fgetc(fp)) != EOF && (isalnum(c) || c == '_')) {
        buf[ind++] = c;
        col++;
    }
    ungetc(c, fp); 
    buf[ind] = '\0';
    if (isKeyword(buf)) {
        return makeToken(row, col, "KEYWORD", buf);
    }
    return makeToken(row, col, "IDENTIFIER", buf);
}

struct Token* getNumber(char c) {
    ind = 0;
    buf[ind++] = c;
    col++;
    int is_float = 0; 
    while ((c = fgetc(fp)) != EOF) {
        if (isdigit(c)) {
            buf[ind++] = c;
            col++;
        } 
        else if (c == '.' && !is_float) {
            is_float = 1;
            buf[ind++] = c;
            col++;
        } 
        else if ((c == 'e' || c == 'E') && (isdigit(buf[ind - 1]) || buf[ind - 1] == '.')) {
            buf[ind++] = c;
            col++;
            c = fgetc(fp);
            // optional sign
            if (c == '+' || c == '-') {  
                buf[ind++] = c;
                col++;
            }
            while (isdigit(c)) {
                buf[ind++] = c;
                col++;
                c = fgetc(fp);
            }
            ungetc(c, fp);  
        } 
        else {
            ungetc(c, fp); 
            break;
        }
    }
    buf[ind] = '\0';  // Null-terminate the string
    return makeToken(row, col, is_float ? "FLOAT" : "INTEGER", buf);
}


struct Token* getNextToken() {
    char c;
    while ((c = fgetc(fp)) != EOF) {
        struct Token* token = NULL;
        if (c == '/') {
            int stat = handleComments();
            if(stat == 0) {
                token = getArithOp(c);
                ind = 0;
                return token;
            }
        }
        else if (isspace(c)) {
            handleSpaces(c);
        }
        else if (c == '#') {
            handleDirectives();
        }
        else if (c == '\"' || c == '\'') {
            token = handleQuotes(c);
            ind = 0;
            return token;
        }
        else if (strchr("+-*%", c) != NULL) {
            token = getArithOp(c);
            ind = 0;
            return token;
        }
        else if (strchr("<>=!", c) != NULL) {
            token = getRelOp(c);
            ind = 0;
            return token;
        }
        else if (strchr("&|", c) != NULL) {
            token = getLogOp(c);
            ind = 0;
            return token;
        }
        else if (strchr("(){}[];,.", c) != NULL) {
            buf[ind++] = c;
            buf[ind] = '\0';
            ind = 0;
            return token = makeToken(row, col, "SPECIAL_SYMBOL", buf);
        }
        else if (isalpha(c) || c == '_') {
            token = getKeyOrId(c);
            ind = 0;
            return token;
        }
        else if (isdigit(c)) {
            token = getNumber(c);
            ind = 0;
            return token;
        }
    }
    return NULL;
}

int main() {
    fp = fopen("input.c", "r");
    if (fp == NULL) {
        printf("Error: Unable to open file\n");
        exit(1);
    }
    struct Token* token;
    while ((token = getNextToken()) != NULL) {
        printToken(token);
    }
    fclose(fp);
    return 0;
}