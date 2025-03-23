#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Token {
    int row, col;
    char type[50], val[50];
};

struct Token* makeToken(int r, int c, char* t, char* v) {
    struct Token* nn = (struct Token*) malloc(sizeof(struct Token));
    nn->row = r;
    nn->col = c;
    strcpy(nn->type, t);
    strcpy(nn->val, v);
    return nn;
}

void printToken(struct Token* token) {
    printf("<%s,%s,%d,%d>", token->type, token->val, token->row, token->col);
}

char keywords[9][10] = {"int", "float", "char", "long", "for", "if", "else", "while", "struct"};

char buf[100];

FILE* fp;
int row = 1, col = 1, ind = 0;

int handleComments();
void handleSpaces(char in);
void handleDirectives();
struct Token* getLiteral(char in);
struct Token* getArithOp(char in);
struct Token* getRelOp(char in);
struct Token* getLogOp(char in);
struct Token* getKeyOrId(char in);
struct Token* getNum(char in);

/*    
    If single line, keep reading till '\n'
    If multiline, keep prev and current, dont stop till * and /
    If neither, return 0 flag ready to see division
*/
int handleComments() {
    char c = fgetc(fp);
    col++;
    int flag = 0;
    if(c == '/') {
        flag = 1;
        do {
            c = fgetc(fp);
            col++;
        }
        while(c != '\n' && c != EOF);
        printf("\n");
        row++;
        col = 1;
    }
    else if(c == '*') {
        char prev = '\0';
        do {
            c = fgetc(fp);
            col++;
            if(prev == '*' && c == '/') {
                flag = 1;
                break;
            }
            else if(c == '\n') {
                printf("\n");
                row++;
                col = 1;
            }
            prev = c;
        }
        while(c != EOF);
    }
    else {
        ungetc(c, fp);
        col--;
    }
    return flag;
}

/*
    If '\n', account for it
    Else if any other space, keep moving till no space, then unget
*/
void handleSpaces(char in) {
    if(in == '\n') {
        printf("\n");
        row++;
        col = 1;
    }
    char c;
    do {
        c = fgetc(fp);
        col++;
        if(isspace(c)) {
            if(c == '\n') {
                printf("\n");
                row++;
                col = 1;
            }
        }
        else {
            ungetc(c, fp);
            col--;
            break;
        }
    }
    while(c != EOF);
}

/*
    If #, read till '\n'
*/
void handleDirectives() {
    char c;
    do {
        c = fgetc(fp);
        col++;
        if(c == '\n') {
            printf("\n");
            col = 1;
            row++;
            break;
        }
    }
    while(c != EOF);
}

/*
    When quotes, read till same quotes come back
    Account for newline
    USE BUFFER
*/
struct Token* getLiteral(char in) {
    char c = fgetc(fp);
    col++;
    ind = 0;
    while(c != in && c != EOF) {
        buf[ind++] = c;
        if(c == '\n') {
            row++;
            col = 1;
        }
        c = fgetc(fp);
        col++;
    }
    buf[ind] = '\0';
    if(c == EOF) {
        return NULL;
    } 
    return makeToken(row, col, "STRING_LITERAL", buf);
}

/*
    Check for +,-,/,*,% with =, ++, --
    USE BUFFER
*/
struct Token* getArithOp(char in) {
    char c;
    c = fgetc(fp);
    col++;
    buf[0] = in;
    // ++, --, +=, -=, +, -
    if(strchr("+-", in)) {
        if(c == '=' || c == in) {
            buf[1] = c;
            buf[2] = '\0';
            return makeToken(row, col, "DOUBLE_ARITH_OP", buf);
        }
        buf[1] = '\0';
        ungetc(c, fp);
        col--;
        return makeToken(row, col, "ARITH_OP", buf);
    }
    // /=, *=, %=, *, / , %
    else if(strchr("/*%", in)) {
        if(c == '=') {
            buf[1] = c;
            buf[2] = '\0';
            return makeToken(row, col, "DOUBLE_ARITH_OP", buf);
        }
        buf[1] = '\0';
        ungetc(c, fp);
        col--;
        return makeToken(row, col, "ARITH_OP", buf);
    }
    return NULL;
}

/*
    <,>,=,!, with =
    USE BUFFER
*/
struct Token* getRelOp(char in) {
    char c = fgetc(fp);
    col++;
    buf[0] = in;
    // <=, >=, ==, !=
    if(c == '=') {
        buf[1] = c;
        buf[2] = '\0';
    }
    // <, >, !, =
    else {
        buf[1] = '\0';
        ungetc(c, fp);
        col--;
    }
    return makeToken(row, col, "REL_OP", buf);
}

/*
    &,|, rep
    USE BUFFER
*/
struct Token* getLogOp(char in) {
    char c = fgetc(fp);
    col++;
    buf[0] = in;
    if(c == in) {
        buf[1] = in;
        buf[2] = '\0';
    }
    else {
        buf[1] = '\0';
        ungetc(c, fp);
        col--;
    }
    return makeToken(row, col, "LOG_OP", buf);
}

/*
    Starts with alphabet, rest is alphanum or _
    Sequential search with keywords
    USE BUFFER
*/
struct Token* getKeyOrId(char in) {
    ind = 0;
    buf[ind++] = in;
    char c;
    do {
        c = fgetc(fp);
        col++;
        if(!isalnum(c) && c != '_') {
            ungetc(c, fp);
            col--;
            break;
        }
        else { 
            buf[ind++] = c;
        }
    }
    while(c != EOF);
    buf[ind] = '\0';
    for(int i = 0; i < 9; i++) {
        if(strcmp(buf, keywords[i]) == 0) {
            return makeToken(row, col, "KEYWORD", buf);
        }
    }
    return makeToken(row, col, "IDENTIFIER", buf);
}

/*
    Starts with digit
    One . allowed (is_float)
    Continue digit
    E / e, +/-/none
    digits
    [0-9]+[.]?[0-9]*[eE]?[+-]?[0-9]+
    USE BUFFER
*/
struct Token* getNum(char in) {
    ind = 0;
    buf[ind++] = in;
    char c;
    int is_float = 0;
    do {
        c = fgetc(fp);
        col++;
        if(isdigit(c)) {
            buf[ind++] = c;
        }
        else if(c == '.' && !is_float) {
            is_float = 1;
            buf[ind++] = c;
        }
        else if(c == 'e' || c == 'E') {
            buf[ind++] = c;
            c = fgetc(fp);
            col++;
            if(c == '+' || c == '-') {
                buf[ind++] = c;
            }
            do {
                c = fgetc(fp);
                if(isdigit(c)) {
                    col++;
                    buf[ind++] = c;
                }
                else {
                    col--;
                    ungetc(c, fp);
                    break;
                }
            }
            while(c != EOF);
        }
        else {
            col--;
            ungetc(c, fp);
            break;
        }
    }
    while(c != EOF);
    buf[ind] = '\0';
    return makeToken(row, col, "NUMBER", buf);
}

struct Token* getNextToken() {
    char c;
    do {
        c = fgetc(fp);
        col++;
        if(c == '/') {
            int flag = handleComments();
            if(flag == 0) {
                return getArithOp(c);
            }
        }
        else if(isspace(c)) {
            handleSpaces(c);
        }
        else if(c == '#') {
            handleDirectives();
        }
        else if(c == '\"' || c == '\'') {
            return getLiteral(c);
        }
        else if(strchr("+-*%", c) != NULL) {
            return getArithOp(c);
        }
        else if(strchr("><!=", c) != NULL) {
            return getRelOp(c);
        }
        else if(strchr("&|", c) != NULL) {
            return getLogOp(c);
        }
        else if(strchr("()[]{};,", c) != NULL) {
            ind = 0;
            buf[ind++] = c;
            buf[ind] = '\0';
            return makeToken(row, col, "SPECIAL_SYMBOL", buf);
        }
        else if(isalpha(c) || c == '_') {
            return getKeyOrId(c);
        }
        else if(isdigit(c)) {
            return getNum(c);
        }
    }
    while(c != EOF);
    return NULL;
}