#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Token {
    int row, col;
    char type[25];
    char val[25];
};

struct Token* makeToken(int row, int col, char* type, char* val) {
    struct Token* token = (struct Token*) malloc(sizeof(struct Token));
    token->row = row;
    token->col = col;
    strcpy(token->type, type);
    strcpy(token->val, val);
    return token;
}

void printToken(struct Token* token) {
    printf("<%d, %d, %s, %s> ",token->row, token->col, token->type, token->val);
}

FILE* fp;
int row = 1, col = 1, ind = 0, unget = 0;
char buf[100];
struct Token* lastToken = NULL;

char keywords[6][10] = {"if", "else", "elsif", "sub", "return", "while"};

void ungetToken() {
    unget = 1;
}

void handleComments(char in);
void handleSpaces(char in);
struct Token* getLiteral(char in);
struct Token* getRelOp(char in);
struct Token* getArithOp(char in);
struct Token* getLogOp(char in);
struct Token* getNumber(char in);
struct Token* getKeywordOrIdentifier(char in);

void handleComments(char in) {
    char c = fgetc(fp);
    while(c != '\n') {
        c = fgetc(fp);
    }
    row++; col = 1;
}

void handleSpaces(char in) {
    col++;
    if(in == '\n') {
        row++;
        col = 1;
    }
    col++;
    char c = fgetc(fp);
    while(isspace(c)) {
        if(c == '\n') {
            row++;
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
    col--;
}

struct Token* getLiteral(char in) {
    ind = 0;
    char c = fgetc(fp);
    col++;
    while(c != in) {
        buf[ind++] = c;
        c = fgetc(fp);
        col++;
    }
    buf[ind] = '\0';
    return makeToken(row, col, "LITERAL", buf);
}

struct Token* getRelop(char in) {
    ind = 0; col++;
    char c;
    if(strchr("!<>=",in) != NULL) {
        buf[ind++] = in;
        c = fgetc(fp);
        col++;
        if(c == '=') {
            buf[ind++] = c;
            buf[ind] = '\0';
            return makeToken(row, col, "RELOP", buf);
        }
        else {
            buf[ind] = '\0';
            col--;
            ungetc(c, fp);
            return makeToken(row, col, "RELOP", buf);
        }
    }
}

struct Token* getArithOp(char in) {
    ind = 0; col++;
    char c;
    buf[ind++] = in;
    col++;
    c = fgetc(fp);
    if(((c == '+' || c == '-') && c == in) || (c == '=')) {
        buf[ind++] = c;
        buf[ind] = '\0';
        return makeToken(row, col, "ARITHOP", buf);
    }
    else {
        ungetc(c, fp);
        col--;
        buf[ind] = '\0';
        return makeToken(row, col, "ARITHOP", buf);
    }
}

struct Token* getLogOp(char in) {
    ind = 0; col++;
    buf[ind++] = in;
    char c = fgetc(fp);
    if(c == in) {
        col++;
        buf[ind++] = c;
        buf[ind] = '\0';
        return makeToken(row, col, "LOGOP", buf);
    }
    ungetc(c, fp);
    buf[ind] = '\0';
    return makeToken(row, col, "LOGOP", buf);
}

struct Token* getNumber(char in) {
    ind = 0; col++;
    buf[ind++] = in;
    char c = fgetc(fp);
    while(isdigit(c)) {
        col++;
        buf[ind++] = c;
        c = fgetc(fp);
    }
    col--;
    ungetc(c, fp);
    buf[ind] = '\0';
    return makeToken(row, col, "NUM", buf);
}

struct Token* getKeywordOrFunc(char in) {
    ind = 0; col++;
    buf[ind++] = in;
    char c = fgetc(fp);
    while(isalnum(c)) {
        buf[ind++] = c;
        col++;
        c = fgetc(fp);
    }
    col--;
    ungetc(c, fp);
    buf[ind] = '\0';
    for(int i = 0; i < 6; i++) {
        if(strcmp(buf, keywords[i]) == 0) {
            return makeToken(row, col, "KEYWORD", buf);
        }
    }
    return makeToken(row, col, "FUNC", buf);
}

struct Token* getIdentifier(char in) {
    ind = 0; col++;
    buf[ind++] = in;
    char c = fgetc(fp);
    while(isalnum(c)) {
        buf[ind++] = c;
        col++;
        c = fgetc(fp);
    }
    col--;
    ungetc(c, fp);
    buf[ind] = '\0';
    if(in == '$') {
        return makeToken(row, col, "$_ID", buf);
    }
    return makeToken(row, col, "@_ID", buf);
}

struct Token* getToken() {
    char c;
    if(unget == 1) {
        unget = 0;
        return lastToken;
    }
    while((c = fgetc(fp)) != EOF) {
        struct Token* token = NULL;
        if(c == '#') {
            handleComments(c);
        }
        else if(isspace(c)) {
            handleSpaces(c);
        }
        else if(c == '\"' || c == '\'') {
            return lastToken = getLiteral(c);
        }
        else if(strchr("<>=!",c) != NULL) {
            return lastToken = getRelop(c);
        }
        else if(strchr("+-/*",c) != NULL) {
            return lastToken = getArithOp(c);
        }
        else if(c == '@' || c == '$') {
            return lastToken = getIdentifier(c);
        }
        else if(isdigit(c)) {
            return lastToken = getNumber(c);
        }
        else if(isalpha(c)) {
            return lastToken = getKeywordOrFunc(c);
        }
        else {
            ind = 0;
            buf[ind++] = c;
            col++;
            buf[ind] = '\0';
            return lastToken = makeToken(row, col, "SPEC", buf);
        }
    }
    return NULL;
}

int main() {
    fp = fopen("input.txt", "r");
    if(fp == NULL) {
        perror("Error reading");
        exit(1);
    }
    struct Token* token = NULL;
    while((token = getToken()) != NULL) {
        printToken(token);
    }
    fclose(fp);
    return 0;
}