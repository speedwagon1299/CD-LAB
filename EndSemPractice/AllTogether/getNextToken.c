#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Token {
    int row, col;
    char type[50], val[50];
};

struct Token* lastToken = NULL;
char buf[100];
int ind = 0, row = 1, col = 1, ungetFlag = 0;
FILE* fp = NULL;

char keywords[7][10] = {
    "record", "int", "char", "bool", "true", "false", "static"
};

struct Token* makeToken(int row, int col, char* type, char* val) {
    struct Token* token = (struct Token*) malloc(sizeof(struct Token));
    token->row = row;
    token->col = col;
    strcpy(token->type, type);
    strcpy(token->val, val);
    return token;
}

void ungetToken() {
    ungetFlag = 1;
}

void printToken(struct Token* token) {
    printf("<%d,%d,%s,%s>", token->row, token->col, token->type, token->val);
}

void handleComments(char in);
void handleSpace(char in);
struct Token* getLiteral(char in);
struct Token* getIdentifierOrKeyword(char in);
struct Token* getNum(char in);

void handleComments(char in) {
    col++;
    char c = fgetc(fp);
    if(c == '/') {
        while(c != '\n' && c != EOF) {
            c = fgetc(fp);
        }
        row++;
        col = 1;
    }
}

void handleSpace(char in) {
    if(in == '\n') {
        row++;
        printf("\n");
        col = 1;
    }
    else {
        col++;
    }
    char c = fgetc(fp);
    while(isspace(c) && c != EOF) {
        if(c == '\n') {
            row++;
            printf("\n");
            col = 1;
        }
        else if(c == '\t') {
            col += 4;
        }
        else {
            col++;
        }
        c = fgetc(fp);
    }
    ungetc(c, fp);
}

struct Token* getLiteral(char in) {
    col++;
    char c = fgetc(fp);
    ind = 0;
    while(c != EOF && c != in) {
        buf[ind++] = c;
        col++;
        c = fgetc(fp);
    }
    col++;
    buf[ind] = '\0';
    return makeToken(row, col, "LIT", buf);
}

struct Token* getIdentifierOrKeyword(char in) {
    col++;
    ind = 0;
    buf[ind++] = in;
    char c = fgetc(fp);
    while(c != EOF && (isalnum(c) || c == '_')) {
        col++;
        buf[ind++] = c;
        c = fgetc(fp);
    }
    ungetc(c, fp);
    buf[ind] = '\0';
    for(int i = 0; i < 7; i++) {
        if(strcmp(keywords[i], buf) == 0) {
            return makeToken(row, col, "KW", buf);
        }
    }
    return makeToken(row, col, "ID", buf);
}

struct Token* getNum(char in) {
    col++;
    ind = 0;
    buf[ind++] = in;
    char c = fgetc(fp);
    while(c != EOF) {
        if(isdigit(c)) {
            col++;
            buf[ind++] = c;
            c = fgetc(fp);
        }
        else if(c == '.') {
            col++;
            buf[ind++] = c;
            c = fgetc(fp);
            while(isdigit(c)) {
                col++;
                buf[ind++] = c;
                c = fgetc(fp);
            }
            if(c == 'e' || c == 'E') {
                col++;
                buf[ind++] = c;
                c = fgetc(fp);
                if(c == '+' || c == '-') {
                    col++;
                    buf[ind++] = c;
                    c = fgetc(fp);
                }
                if(isdigit(c)) {
                    while(isdigit(c)) {
                        col++;
                        buf[ind++] = c;
                        c = fgetc(fp);
                    }
                }
            }
            buf[ind] = '\0';
            ungetc(c, fp);
            return makeToken(row, col, "FLOAT", buf);
        }
        else {
            buf[ind] = '\0';
            ungetc(c, fp);
            return makeToken(row, col, "NUM", buf);
        }
    }
}

struct Token* getToken() {
    if(ungetFlag) {
        return lastToken;
    }
    char c = fgetc(fp);
    while(c != EOF) {
        if(c == '/') {
            handleComments(c);
        }
        else if(isspace(c)) {
            handleSpace(c);
        }
        else if(c == '\'' || c == '\"') {
            return lastToken = getLiteral(c);
        }
        else if(isalpha(c)) {
            return lastToken = getIdentifierOrKeyword(c);
        }
        else if(isdigit(c)) {
            return lastToken = getNum(c);
        }
        else {
            ind = 0;
            col++;
            buf[ind++] = c;
            buf[ind] = '\0';
            return lastToken = makeToken(row, col, "SPEC", buf);
        }
        c = fgetc(fp);
    }
    return NULL;
}

int main() {
    const char* fin = "input.txt";
    fp = fopen(fin, "r");
    struct Token* token = NULL;
    while((token = getToken())) {
        printToken(token);
    }
    fclose(fp);
    return 0;
}