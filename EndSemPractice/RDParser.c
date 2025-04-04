#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "getNextToken.h"

struct Token* token = NULL;

void invalid(struct Token* token) {
    if(token) {
        printf("\nInvalid\n");
        printToken(token);
    }
    else {
        printf("\n-----INVALID-----\n");
    }
    exit(0);
}

void invalidVal(char in, struct Token* token) {
    if(token) {
        printf("\nExpected %c. Got %s in row %d col %d\n", in, token->val, token->row, token->col);
    }
    else {
        printf("\nUnexpected EOF\n");
        exit(0);
    }
}

void invalidType(char* in, struct Token* token) {
    if(token) {
        printf("\nExpected %c. Got %s in row %d col %d\n", in, token->type, token->row, token->col);
    }
    else {
        printf("\nUnexpected EOF\n");
        exit(0);
    }
}


void Program();
void StmtList();
void StmtTail();
void Stmt();
void ElseOpt();
void Block();
void Expr();
void ExprTail();
void Term();
void TermTail();
void Factor();
void NumList();
void NumListTail();

void Program() {
    StmtList();
}

void StmtList() {
    Stmt();
    token = getToken();
    StmtTail();
}

void StmtTail() {
    if(token && (strcmp(token->type, "$_ID") == 0 || strcmp(token->type, "@_ID") == 0 || strcmp(token->type, "FUNC") == 0 || strcmp(token->val, "sub") == 0 || strcmp(token->val, "if") == 0)) {
        Stmt();
        token = getToken();
        StmtTail();
        return;
    }
    ungetToken();
}

void Stmt() {
    if(token && strcmp(token->type, "$_ID") == 0) {
        token = getToken();
        if(!(token && strcmp(token->val, "=") == 0)) {
            invalidVal('=', token);
        }
        token = getToken();
        Expr();
        token = getToken();
        if(!(token && strcmp(token->val, ";") == 0)) {
            invalidVal(';', token);
        }
        return;
    }
    if(token && strcmp(token->type, "@_ID") == 0) {
        token = getToken();
        if(!(token && strcmp(token->val, "=") == 0)) {
            invalidVal('=', token);
        }
        token = getToken();
        if(!(token && strcmp(token->val, "(") == 0)) {
            invalidVal('(', token);
        }
        token = getToken();
        NumList();
        token = getToken();
        if(!(token && strcmp(token->val, ")") == 0)) {
            invalidVal(')', token);
        }
        token = getToken();
        if(!(token && strcmp(token->val, ";") == 0)) {
            invalidVal(';', token);
        }
        return;
    }
    if(token && strcmp(token->type, "FUNC") == 0) {
        token = getToken();
        if(!(token && strcmp(token->val, "(") == 0)) {
            invalidVal('(', token);
        }
        token = getToken();
        if(!(token && strcmp(token->val, ")") == 0)) {
            invalidVal(')', token);
        }
        token = getToken();
        if(!(token && strcmp(token->val, ";") == 0)) {
            invalidVal(';', token);
        }
        return;
    }
    if(token && strcmp(token->val, "sub") == 0) {
        token = getToken();
        if(!(token && strcmp(token->type, "FUNC") == 0)) {
            invalidType("FUNC", token);
        }
        token = getToken();
        Block();
        return;
    }
    if(token && strcmp(token->val, "if") == 0) {
        token = getToken();
        if(!(token && strcmp(token->val, "(") == 0)) {
            invalidVal('(', token);
        }
        token = getToken();
        Expr();
        token = getToken();
        if(!(token && strcmp(token->val, ")") == 0)) {
            invalidVal(')', token);
        }
        token = getToken();
        Block();
        token = getToken();
        ElseOpt();
        return;
    }
    invalid(token);
}

void ElseOpt() {
    if(token && strcmp(token->val, "if") == 0) {
        token = getToken();
        Block();
        return;
    }
    ungetToken();
}

void Block() {
    if(!(token && strcmp(token->val, "{") == 0)) {
        invalidVal('{', token);
    }
    token = getToken();
    StmtList();
    token = getToken();
    if(!(token && strcmp(token->val, "}") == 0)) {
        invalidVal('}', token);
    }
}

void Expr() {
    Term();
    token = getToken();
    ExprTail();
}

void ExprTail() {
    if(token && strcmp(token->val, "+") == 0) {
        token = getToken();
        Term();
        token = getToken();
        ExprTail();
        return;
    }
    if(token && strcmp(token->val, "-") == 0) {
        token = getToken();
        Term();
        token = getToken();
        ExprTail();
        return;
    }
    ungetToken();
}

void Term() {
    Factor();
    token = getToken();
    TermTail();
}

void TermTail() {
    if(token && strcmp(token->val, "*") == 0) {
        token = getToken();
        Factor();
        token = getToken();
        TermTail();
        return;
    }
    if(token && strcmp(token->val, "/") == 0) {
        token = getToken();
        Factor();
        token = getToken();
        TermTail();
        return;
    }
    ungetToken();
}

void Factor() {
    if(token && strcmp(token->type, "$_ID") == 0) {
        token = getToken();
        if(token && strcmp(token->val, "[") == 0) {
            token = getToken();
            Expr();
            token = getToken();
            if(!(token && strcmp(token->val, "]") == 0)) {
                invalidVal(']', token);
            }
            return;
        }
        return;
    }
    if(token && strcmp(token->type, "NUM") == 0) {
        return;
    }
    if(token && strcmp(token->val, "(") == 0) {
        token = getToken();
        Expr();
        token = getToken();
        if(!(token && strcmp(token->val, ")") == 0)) {
            invalidVal(')', token);
        }
        return;
    }
    invalid(token);
}

void NumList() {
    if(!(token && strcmp(token->type, "NUM") == 0)) {
        invalidType("NUM", token);
    }
    token = getToken();
    NumListTail();
}

void NumListTail() {
    if(token && strcmp(token->val,",") == 0) {
        token = getToken();
        if(!(token && strcmp(token->type, "NUM") == 0)) {
            invalidType("NUM", token);
        }
        token = getToken();
        NumListTail();
        return;
    }
    ungetToken();
}

int main() {
    fp = fopen("input.txt", "r");
    if(fp == NULL) {
        perror("Error opening file");
        exit(1);
    }
    token = getToken();
    Program();
    printf("-----VALID-----");
    exit(0);
}