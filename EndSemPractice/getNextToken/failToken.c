#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Token {
    char type[50];
    char value[50];
};

struct Token* makeToken(char type[], char value[]) {
    struct Token* token = (struct Token*) malloc(sizeof(struct Token));
    strcpy(token->type, type);
    strcpy(token->value, value);
    return token;
}

void printToken(struct Token* token) {
    printf("<%s,%s>", token->type, token->value);
}

FILE* fp;
char buf[100];
int ind = 0;

char opcode[6][20] = {
    "MOVLW", "MOVWF", "ADDWF", "INCF", "CALL", "GOTO"
};

char reg[4][20] = {
    "PORTB", "WREG", "W", "F"
};

void handleSpaces(char in) {
    if(in == '\n') {
        printf("\n");
    }
    char c = fgetc(fp);
    while(c != EOF) {
        if(isspace(c)) {
            if(c == '\n') {
                continue;
            }
        }
        else {
            ungetc(c, fp);
            break;
        }
        c = fgetc(fp);
    }
}

void handleComDir() {
    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            printf("\n");
            break;
        }
    }
}

struct Token* getNum(char in) {
    ind = 0;
    buf[ind++] = in;
    int x = 0;
    char c;
    while((c = fgetc(fp)) != EOF) {
        if(isalnum(c)) {
            buf[ind++] = c;
        }
        else if(c == 'x' && x == 0) {
            x = 1;
            buf[ind++] = c;
        }
        else if(c == '\n') {
            printf("\n");
            break;
        }
        else {
            ungetc(c, fp);
            break;
        }
    }
    buf[ind] = '\0';
    return makeToken("CONST_LITERAL", buf);
}

struct Token* others(char in) {
    ind = 0;
    buf[ind++] = in;
    char c = fgetc(fp);
    while(c != EOF) {
        if(isalnum(c) || c == '_') {
            buf[ind++] = c;
        }
        else if(c == ':') {
            ungetc(c, fp);
            return makeToken("LABEL", buf);
        }
        else {
            ungetc(c, fp);
            break;
        }
        c = fgetc(fp);
    }
    buf[ind] = '\0';
    for(int i = 0; i < 4; i++) {
        if(strcmp(buf, reg[i]) == 0) {
            return makeToken("REG", buf);
        }
    }
    for(int i = 0; i < 6; i++) {
        if(strcmp(buf, opcode[i]) == 0) {
            return makeToken("OPCODE", buf);
        }
    }
    return makeToken("LABEL",buf);
}

struct Token* getNextToken() {
    char c = fgetc(fp);
    while(c != EOF) {
        if(isspace(c)) {
            handleSpaces(c);
        }
        else if(c == '#' || c == ';') {
            handleComDir();
        }
        else if(isdigit(c)) {
            return getNum(c);
        }
        else if(isalpha(c)) {
            return others(c);
        }
        else {
            buf[0] = c;
            buf[1] = '\0';
            return makeToken("SPEC_SYMBOL", buf);
        }
        c = fgetc(fp);
    }
    return NULL;
}

int main() {
    fp = fopen("failinput.txt","r");
    printf("\nOpen");
    if(fp == NULL) {
        printf("\nError Opening");
        return 1;
    }
    struct Token* token = NULL;
    while((token = getNextToken()) != NULL) {
        printToken(token);
    }
    return 0;
}
