#include "getNextToken.h"
#define TB_SZ 50

struct STNode {
    char lex[50], type[50], dtype[50]; 
    struct STNode* next;
};

struct STNode** ST = NULL;

void initST() {
    ST = (struct STNode**) malloc(TB_SZ * sizeof(struct STNode*));
    for(int i = 0; i < TB_SZ; i++) {
        ST[i] = NULL;
    }
}

int hash(char* lex) {
    int res = 0;
    for(int i = 0; lex[i] != '\0'; i++) {
        res += lex[i];
    }
}

void insertST(char* lex, char* type, char* dtype) { 
    int ind = hash(lex);
    struct STNode* entry = ST[ind];
    while(entry) {
        if(strcmp(entry->lex, lex) == 0) {
            printf("\nDuplicate entry, skipping...\n");
            return;
        }
        entry = entry->next;
    }
    struct STNode* nn = (struct STNode*) malloc(sizeof(struct STNode));
    strcpy(nn->lex, lex);
    strcpy(nn->type, type);
    strcpy(nn->dtype, dtype);
    nn->next = ST[ind];
    ST[ind] = nn;
}

void printST() {
    printf("\nSYMBOL TABLE\n");
    printf("%-20s %-15s %-10s\n", "Lexeme", "Token", "Data Type");
    for(int i = 0; i < TB_SZ; i++) {
        struct STNode* entry = ST[i];
        while(entry) {
            printf("%-20s %-15s %-10s\n", entry->lex, entry->type, entry->dtype);
            entry = entry->next;
        }
    }
    printf("\n");
}

int main() {
    char* fin = "input.txt";
    fp = fopen(fin, "r");
    if(!fp) {
        perror("\nError opening file");
        exit(1);
    }
    initST();
    char interDtype[20];
    struct Token* token = NULL;
    while(token = getToken()) {
        // allowing function to be equivalent to void
        if(strcmp(token->val, "int") == 0 || strcmp(token->val, "bool") == 0 || strcmp(token->val, "char") == 0 || strcmp(token->val, "function") == 0) {
            strcpy(interDtype, token->val);
            token = getToken();
            // function or variable
            while(strcmp(token->val, ";") != 0 && strcmp(token->val, "(") != 0) {
                if(strcmp(token->type, "ID") == 0) {
                    insertST(token->val, token->type, interDtype);
                }
                token = getToken();
            }
            if(strcmp(token->val, "(") == 0) {
                token = getToken();
                while(strcmp(token->val, ")") != 0) {
                    strcpy(interDtype, token->val);
                    token = getToken();
                    insertST(token->val, token->type, interDtype);
                    token = getToken();
                    if(strcmp(token->val, ",") == 0) {
                        token = getToken();
                    }
                }
            }
        }
    }
    fclose(fp);
    printST();
    exit(0);
}