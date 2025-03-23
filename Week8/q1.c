#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "getNextToken.h"

struct Token* token = NULL;

void valid() {
    printf("-----------------VALID-------------------\n");
}

void invalid() {
    printToken(token);
    printf("-----------------INVALID-------------------\n");
    exit(1);
}

/*

Take Aways to replicate on an LL(1) Grammar:

1.  Eliminate Left recursion to avoid infinite loop and
    Left Factor to prevent need for Backtracking

2.  For Productions having only one rule in RHS (No OR '|')
    use if(!(token && condition)) { invalid(); } to avoid nested if loop

3.  For multiple Productions in RHS (One or more OR '|')
    use if(token && FIRST) { ... return; } for all elements in the FIRST set.
    In each if block, use the above mentioned if(!()) rule

4.  Before calling a function or verifying non-terminal,
    always call token = getNextToken() to advance to next token

5.  If an epsilon production is present, after rule 3, put a
    ungetTok(); that is called if none of the previous if are satisfied.
    If there is no epsilon, put invalid(); instead of ungetting token

*/

void Program();
void Declarations();
void Data_Type();
void Identifier_List();
void Identifier_List_1();
void Identifier_List_2();
void Statement_List();
void Statement();
void Assign_Stat();
void Assign_Stat_2();
void Decision_Stat();
void Dprime();
void Loop_Stat();
void Expn();
void Simple_Expn();
void SEPrime();
void Eprime();
void Term();
void Tprime();
void Factor();
void Addop();
void Mulop();
void Relop();

void Program() {
    if(!(token && strcmp("main", token->value) == 0)) {
        invalid();
    }
    token = getNextToken();
    if (!(token && strcmp("(", token->value) == 0)) {
        invalid();
    }
    token = getNextToken();
    if (!(token && strcmp(")", token->value) == 0)) {
        invalid();
    }
    token = getNextToken();
    if (!(token && strcmp("{", token->value) == 0)) {
        invalid();
    }
    token = getNextToken();
    Declarations();
    token = getNextToken();
    Statement_List();
    token = getNextToken();
    if (!(token && strcmp("}", token->value) == 0)) {
        invalid();
    }
}

void Declarations() {
    if (token && (strcmp("int", token->value) == 0 || strcmp("char", token->value) == 0)) {
        Data_Type();         
        token = getNextToken();
        Identifier_List();
        token = getNextToken();
        if(!(token && strcmp(";", token->value) == 0)) {
            invalid();
        }
        token = getNextToken();
        Declarations();
        return;
    }
    ungetTok();
}

void Data_Type() {
    if (!(token && (strcmp("int", token->value) == 0 || strcmp("char", token->value) == 0))) {
        invalid();
    }
}

void Identifier_List() {
    if (!(token && strcmp("IDENTIFIER", token->type) == 0)) {
        invalid();
    }
    token = getNextToken();
    Identifier_List_1();
}

void Identifier_List_1() {
    if(token && strcmp(",", token->value) == 0) {
        token = getNextToken();
        Identifier_List();
        return;
    }
    if(token && strcmp("[", token->value) == 0) {
        token = getNextToken();
        if(!(token && strcmp("INTEGER", token->type) == 0)) {
            invalid();
        }
        token = getNextToken();
        if(!(token && strcmp("]", token->value) == 0)) {
            invalid();
        }
        token = getNextToken();
        Identifier_List_2();
        return;
    }
    ungetTok();
}

void Identifier_List_2() {
    if (token && strcmp(",", token->value) == 0) {
        token = getNextToken();
        Identifier_List();
        return;
    }
    ungetTok();
}

void Statement_List() {
    if (token && (strcmp("IDENTIFIER", token->type) == 0 || strcmp("if", token->value) == 0)) {
        Statement();
        token = getNextToken();
        Statement_List();
        return;
    }
    ungetTok();
}

void Statement() {
    if (token && strcmp("IDENTIFIER", token->type) == 0) {
        Assign_Stat();
        token = getNextToken();
        if(!(token && strcmp(";", token->value) == 0)) {
            invalid();
        }
        return;
    }
    if(token && strcmp("if", token->value) == 0) {
        Decision_Stat();
        return;
    }
    invalid();
}

void Assign_Stat() {
    if (!(token && strcmp("IDENTIFIER", token->type) == 0)) {
        invalid();
    }
    token = getNextToken();
    if (!(token && strcmp("=", token->value) == 0)) {
        invalid();
    }
    token = getNextToken();
    Expn();
}

void Expn() {
    if(token && (strcmp("IDENTIFIER", token->type) == 0 || strcmp("INTEGER", token->type) == 0)) {
        Simple_Expn();
        token = getNextToken();
        Eprime();
        return;
    }
    invalid();
}

void Eprime() {
    if(token && strcmp("RELATIONAL_OPERATOR", token->type) == 0) {
        Relop();
        token = getNextToken();
        Simple_Expn();
        return;
    }
    ungetTok();
}

void Simple_Expn() {
    if(token && (strcmp("IDENTIFIER", token->type) == 0 || strcmp("INTEGER", token->type) == 0)) {  
        Term();
        token = getNextToken();
        SEPrime();
        return;
    }
    invalid();
}

void SEPrime() {
    if(token && (strcmp("+", token->value) == 0 || strcmp("-", token->value) == 0)) {
        Addop();
        token = getNextToken();
        Term();
        token = getNextToken();
        SEPrime();
        return;
    }
    ungetTok();
}

void Term() {
    if(token && (strcmp("IDENTIFIER", token->type) == 0 || strcmp("INTEGER", token->type) == 0)) {
        Factor();
        token = getNextToken();
        Tprime();
        return;
    }
    invalid();
}

void Tprime() {
    if(token && (strcmp("*", token->value) == 0 || strcmp("/", token->value) == 0 || strcmp("%", token->value) == 0)) {
        Mulop();
        token = getNextToken();
        Factor();
        token = getNextToken();
        Tprime();
        return;
    }
    ungetTok();
}

void Factor() {
    if(!(token && (strcmp("IDENTIFIER", token->type) == 0 || strcmp("INTEGER", token->type) == 0))) {
        invalid();
    }
}

void Decision_Stat() {
    if(!(token && strcmp("if", token->value) == 0)) {
        invalid();
    }
    token = getNextToken();
    if(!(token && strcmp("(", token->value) == 0)) {
        invalid();
    }
    token = getNextToken();
    Expn();
    token = getNextToken();
    if(!(token && strcmp(")", token->value) == 0)) {
        invalid();
    }
    token = getNextToken();
    if(!(token && strcmp("{", token->value) == 0)) {
        invalid();
    }
    token = getNextToken();
    Statement_List();
    if(!(token && strcmp("}", token->value) == 0)) {
        invalid();
    }
    token = getNextToken();
    Dprime();
}

void Dprime() {
    if(token && strcmp("else", token->value) == 0) {
        token = getNextToken();
        if(!(token && strcmp("{", token->value) == 0)) {
            invalid();
        }
        token = getNextToken();
        Statement_List();
        token = getNextToken();
        if(!(token && strcmp("}", token->value) == 0)) {
            invalid();
        }
        token = getNextToken();
        Dprime();
        return;
    }
    ungetTok();
}

void Relop() {
    if(!(token && strcmp("RELATIONAL_OPERATOR", token->type) == 0)) {
        invalid();
    }

}

void Addop() {
    if(!(token && (strcmp("+", token->value) == 0 || strcmp("-", token->value) == 0))) {
        invalid();
    }
}

void Mulop() {
    if(!(token && (strcmp("*", token->value) == 0 || strcmp("/", token->value) == 0 || strcmp("%", token->value) == 0))) {
        invalid();
    }
}

int main() {
    fp = fopen("input.c", "r");
    if(fp == NULL) {
        return 1;
    }
    token = getNextToken();
    Program();
    valid();
    return 0;
}