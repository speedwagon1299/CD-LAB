#include "getNextToken.h"

struct Token* token = NULL;

void invalidType(char* msg) {
    if(token) {
        printf("\nExpected Type %s, got %s at row %d, col %d", msg, token->type, token->row, token->col);
        ungetToken();
    }
    else {
        printf("\nInvalid Type: Segmentation Fault\n");
        exit(1);
    }
}

void invalidSymb(char* msg) {
    if(token) {
        printf("\nExpected Symbol %s, got %s at row %d, col %d", msg, token->val, token->row, token->col);
        ungetToken();
    }
    else {
        printf("\nInvalid Symbol: Segmentation Fault\n");
        exit(1);
    }
} 

void invalid(char* msg) {
    if(token) {
        printf("\nInvalid: Error at row %d, col %d. Expected %s\n", token->row, token->col, msg);
    }
    else {
        printf("\nInvalid: Segmentation Fault\n");
        exit(1);
    }
}

void valid() {
    printf("\nValid\n");
    exit(0);
}

void Program();
void FunctionDecl();
void StmtList();
void Stmt();
void Declaration();
void Assignment();
void IfStmt();
void ElsePart();
void Type();
void InitOpt();
void Expr();

void Program() {
    FunctionDecl();
}

void FunctionDecl() {
    if(!(token && strcmp(token->val, "function") == 0)) {
        invalidSymb("function");
    }
    token = getToken();
    if(!(token && strcmp(token->type, "ID") == 0)) {
        invalidType("ID");
    }
    token = getToken();
    if(!(token && strcmp(token->val, "(") == 0)) {
        invalidSymb("(");
    }
    token = getToken();
    if(!(token && strcmp(token->val, ")") == 0)) {
        invalidSymb(")");
    }
    token = getToken();
    if(!(token && strcmp(token->val, "{") == 0)) {
        invalidSymb("{");
    }
    token = getToken();
    StmtList();
    token = getToken();
    if(!(token && strcmp(token->val, "}") == 0)) {
        invalidSymb("}");
    }
}

void StmtList() {
    if(token && (strcmp(token->val, "int") == 0 || strcmp(token->val, "char") == 0 || strcmp(token->val, "bool") == 0 || strcmp(token->type, "ID") == 0 || strcmp(token->val, "if") == 0)) {
        Stmt();
        token = getToken();
        StmtList();
    }
    else {
        ungetToken();
    }
}

void Stmt() {
    if(token && (strcmp(token->val, "int") == 0 || strcmp(token->val, "char") == 0 || strcmp(token->val, "bool") == 0)) {
        Declaration();
    }
    else if(token && strcmp(token->type, "ID") == 0) {
        Assignment();
    }
    else if(token && strcmp(token->val, "if") == 0) {
        IfStmt();
    }
    else {
        invalid("Declaration, Assignment or If Statement");
    }
}

void Declaration() {
    Type();
    token = getToken(); 
    if(!(token && strcmp(token->type, "ID") == 0)) {
        invalidType("ID");
    }
    token = getToken();
    InitOpt();
    token = getToken(); 
    if(!(token && strcmp(token->val, ";") == 0)) {
        invalidSymb(";");
    }
}

void Assignment() {
    if(!(token && strcmp(token->type, "ID") == 0)) {
        invalidType("ID");
    }
    token = getToken();
    if(!(token && strcmp(token->val, "=") == 0)) {
        invalidSymb("=");
    }
    token = getToken();
    Expr();
    token = getToken(); 
    if(!(token && strcmp(token->val, ";") == 0)) {
        invalidSymb(";");
    }
}

void IfStmt() {
    if(!(token && strcmp(token->val, "if") == 0)) {
        invalidSymb("if");
    }
    token = getToken();
    if(!(token && strcmp(token->val, "(") == 0)) {
        invalidSymb("(");
    }
    token = getToken();
    Expr();
    token = getToken();
    if(!(token && strcmp(token->val, ")") == 0)) {
        invalidSymb(")");
    }
    token = getToken();
    if(!(token && strcmp(token->val, "{") == 0)) {
        invalidSymb("{");
    }
    token = getToken();
    StmtList();
    token = getToken();
    if(!(token && strcmp(token->val, "}") == 0)) {
        invalidSymb("}");
    }
    token = getToken();
    ElsePart();
}

void ElsePart() {
    if(token && strcmp(token->val, "else") == 0) {
        token = getToken();
        if(!(token && strcmp(token->val, "{") == 0)) {
            invalidSymb("{");
        }
        token = getToken();
        StmtList();
        token = getToken();
        if(!(token && strcmp(token->val, "}") == 0)) {
            invalidSymb("}");
        }
    }
    else {
        ungetToken();
    }
}

void Type() {
    if(!(token && (strcmp(token->val, "int") == 0 || strcmp(token->val, "char") == 0 || strcmp(token->val, "bool") == 0))) { 
        invalidSymb("int, char, bool");
    }
}

void InitOpt() {
    if(token && strcmp(token->val, "=") == 0) {
        token = getToken();
        Expr();
    }
    else {
        ungetToken();
    }
}

void Expr() {
    if(!(token && (strcmp(token->type, "INT") == 0 || strcmp(token->type, "FLOAT") == 0 || strcmp(token->type, "LIT") == 0 || strcmp(token->val, "true") == 0 || strcmp(token->val, "false") == 0 || strcmp(token->type, "ID") == 0))) {
        invalid("INT, FLOAT, LIT, true, false or ID");
    }
}

int main() {
    char* fin = "input.txt";
    fp = fopen(fin, "r");
    if(!fp) {
        perror("Error opening file");
        exit(1);
    }
    token = getToken();
    Program();
    fclose(fp);
    valid();
    exit(0);
}