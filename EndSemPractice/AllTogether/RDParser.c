#include "getNextToken.h"

struct Token* token = NULL;

void invalidType(char* type) {
    if(token) {
        printf("\nExpected %s in row %d col %d. Got %s. Ignoring...\n", type, token->row, token->col, token->type);
    }
    else {
        printf("\nNULL Token\n");
        exit(1);
    }
}

void invalidSymb(char* symb) {
    if(token) {
        printf("\nExpected %s in row %d col %d. Got %s. Ignoring...\n", symb, token->row, token->col, token->val);
    }
    else {
        printf("\nNULL Token\n");
        exit(1);
    }
}

void invalid() {
    printf("\nINVALID\n");
    exit(1);
}

void valid() {
    printf("\nVALID\n");
    exit(0);
}

void Program();
void RecordDecl();
void VarDeclList();
void VarDecl();
void StaticOpt();
void Type();
void VarDefList();
void VarDefListTail();
void VarDef();
void ArrayOpt();
void InitOpt();
void Val();

void Program() {
    RecordDecl();
}

void RecordDecl() {
    if(!(token && strcmp(token->val,"record") == 0)) {
        invalidSymb("record");
    }
    token = getToken();
    if(!(token && strcmp(token->type, "ID") == 0)) {
        invalidType("ID");
    }
    token = getToken();
    if(!(token && strcmp(token->val,"{") == 0)) {
        invalidSymb("{");
    }
    token = getToken();
    VarDeclList();
    token = getToken();
    if(!(token && strcmp(token->val,"}") == 0)) {
        invalidSymb("}");
    }
}

void VarDeclList() {
    if(token && (strcmp(token->val, "static") == 0 || strcmp(token->val, "int") == 0 || strcmp(token->val, "char") == 0 || strcmp(token->val, "bool") == 0)) {
        VarDecl();
        token = getToken();
        VarDeclList();
        return;
    }
    ungetToken();
}

void VarDecl() {
    StaticOpt();
    token = getToken();
    Type();
    token = getToken();
    VarDefList();
    token = getToken();
    if(!(token && strcmp(token->val,";") == 0)) {
        invalidSymb(";");
    }
}

void StaticOpt() {
    if(token && strcmp(token->val, "static") == 0) {
        return;
    }
    ungetToken();
}

void Type() {
    if(!(token && (strcmp(token->val, "int") == 0 || strcmp(token->val, "char") == 0 || strcmp(token->val, "bool") == 0))) {
        invalidType("int|char|bool");
    }
}

void VarDefList() {
    VarDef();
    token = getToken();
    VarDefListTail();
}

void VarDefListTail() {
    if(token && strcmp(token->val, ",") == 0) {
        token = getToken();
        VarDef();
        token = getToken();
        VarDefListTail();
        return;
    }
    ungetToken();
}

void VarDef() {
    if(!(token && strcmp(token->type, "ID") == 0)) {
        invalidType("ID");
    }
    token = getToken();
    ArrayOpt();
    token = getToken();
    InitOpt();
}

void ArrayOpt() {
    if(token && strcmp(token->val, "[") == 0) {
        token = getToken();
        if(!(token && strcmp(token->type, "NUM") == 0)) {
            invalidType("NUM");
        }
        token = getToken();
        if(!(token && strcmp(token->val,"]") == 0)) {
            invalidSymb("]");
        }
        return;
    }
    ungetToken();
}

void InitOpt() {
    if(token && strcmp(token->val, "=") == 0) {
        token = getToken();
        Val();
        return;
    }
    ungetToken();
}

void Val() {
    if(!(token && (strcmp(token->val, "true") == 0 || strcmp(token->val, "false") == 0 || strcmp(token->type, "NUM") == 0 || strcmp(token->type, "LIT") == 0))) {
        printf("\nEnter a valid constant to initialize to. Skipping...\n");
    }
}

int main() {
    char* fin = "input.txt";
    fp = fopen(fin, "r");
    token = getToken();
    Program();
    valid();
}