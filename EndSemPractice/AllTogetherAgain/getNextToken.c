#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Token {
    int row, col;
    char type[50], val[50];
};

struct Token* makeToken(int row, int col, char* type, char* val) {
    struct Token* token = (struct Token*) malloc(sizeof(struct Token));
    token->row = row;
    token->col = col;
    strcpy(token->type, type);
    strcpy(token->val, val);
    return token;
}

FILE* fp = NULL;
int row = 1, col = 1, ind = 0, ungetFlag = 0;
char buf[100];
struct Token* lastToken = NULL;
char keywords[7][10] = {"int", "bool", "function", "true", "false", "if", "else"};

void ungetToken() {
    ungetFlag = 1;
}

void printToken(struct Token* token) {
    if(token) {
        printf("<%d,%d,%s,%s>", row, col, token->type, token->val);
    }
    else {
        printf("Segmentation FAULT at row %d, col %d", row, col);
        exit(1);
    }
}

void handleComments(char in);
void handleSpace(char in);
struct Token* getLiteral(char in);
struct Token* getNum(char in);
struct Token* getKWorID(char in);

void handleComments(char in) {
    char c = fgetc(fp);
    if(c == '/') {
        c = fgetc(fp);
        while(c != EOF && c != '\n') {
            c = fgetc(fp);
        }
        row++;
        col = 1;
        return;
    }
    ungetc(c, fp);
}

void handleSpace(char in) {
    char c = in;
    while(c != EOF && isspace(c)) {
        if(c == '\n') {
            row++;
            col = 1;
            printf("\n");
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
    ind = 0;
    char c = fgetc(fp);
    while(c != EOF && c != in) {
        col++;
        buf[ind++] = c;
        c = fgetc(fp);
    }
    buf[ind] = '\0';
    col++;
    return makeToken(row, col, "LIT", buf);
}

struct Token* getNum(char in) {
    col++;
    ind = 0;
    buf[ind++] = in;
    char c = fgetc(fp);
    while(isdigit(c)) {
        col++;
        buf[ind++] = c;
        c = fgetc(fp);
    }
    if(c == '.') {
        col++;
        buf[ind++] = '.';
        c = fgetc(fp);
        while(isdigit(c)) {
            col++;
            buf[ind++] = c;
            c = fgetc(fp);
        }
        if(c == 'E' || c == 'e') {
            col++;
            buf[ind++] = c;
            c = fgetc(fp);
            if(c == '-' || c == '+') {
                col++;
                buf[ind++] = c;
                c = fgetc(fp);
            }
            while(isdigit(c)) {
                col++;
                buf[ind++] = c;
                c = fgetc(fp);
            }
            ungetc(c, fp);
            buf[ind] = '\0';
            return makeToken(row, col, "FLOAT", buf);
        }
        else {
            ungetc(c, fp);
            buf[ind] = '\0';
            return makeToken(row, col, "FLOAT", buf);
        }
    }
    else {
        ungetc(c, fp);
        buf[ind] = '\0';
        return makeToken(row, col, "INT", buf);
    }
}

struct Token* getKWorID(char in) {
    col++;
    ind = 0;
    buf[ind++] = in;
    char c = fgetc(fp);
    while(isalnum(c) || c == '_') {
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

struct Token* getToken() {
    if(ungetFlag) {
        ungetFlag = 0;
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
        else if(isdigit(c)) {
            return lastToken = getNum(c);
        }
        else if(isalpha(c) || c == '_') {
            return lastToken = getKWorID(c);
        }
        else {
            col++;
            ind = 0;
            buf[ind++] = c;
            buf[ind] = '\0';
            return lastToken = makeToken(row, col, "SPEC", buf);
        }
        c = fgetc(fp);
    }
    return NULL;
}

int main() {
    char* fin = "input.txt";
    fp = fopen(fin, "r");
    if(!fp) {
        perror("Error opening file");
        exit(1);
    }
    struct Token* tok = NULL;
    while(tok = getToken()) {
        printToken(tok);
    }
    fclose(fp);
    exit(0);
}