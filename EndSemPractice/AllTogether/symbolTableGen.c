#include "getNextToken.h"
#define TB_SZ 50 

struct STNode {
    char lex[50], token[50], dtype[20];
    struct STNode* next; 
};

struct STNode** ST = NULL;

void initST() {
    ST = (struct STNode**) malloc(TB_SZ * sizeof(struct STNode*));
    for(int i = 0; i < TB_SZ; i++) {
        ST[i] = NULL;
    }
}

int hash(char* str) {
    int res = 0;
    for(int i = 0; str[i] != '\0'; i++) {
        res += str[i];
    }
    return res % TB_SZ;
}

void insertST(char* lex, char* token, char* dtype) {
    int ind = hash(lex);
    struct STNode* entry = ST[ind];
    // only to check for duplicate entry 
    while(entry) {
        if(strcmp(entry->lex, lex) == 0) {
            printf("Duplicate Entry, Skipping...");
            return;
        }
        entry = entry->next;
    }
    struct STNode* nn = (struct STNode*) malloc(sizeof(struct STNode));
    strcpy(nn->lex, lex);
    strcpy(nn->token, token);
    strcpy(nn->dtype, dtype);
    nn->next = ST[ind];
    ST[ind] = nn;
}

void printST() {
    printf("\nSYMBOL TABLE:\n");
    printf("%-20s %-15s %-10s\n", "Lexeme Name", "Token Type", "Data Type");
    for(int i = 0; i < TB_SZ; i++) {
        struct STNode* entry = ST[i];
        while(entry) {
            printf("%-20s %-15s %-10s\n", entry->lex, entry->token, entry->dtype);
            entry = entry->next;
        }
    }
    printf("\n\n");
}

int main() {
    char* fin = "input.txt";
    fp = fopen(fin, "r");
    initST();
    struct Token* token = NULL;
    char interDtype[20];
    while(token = getToken()) {
        if(strcmp(token->val, "int") == 0 || strcmp(token->val, "char") == 0 || strcmp(token->val, "bool") == 0) {
            strcpy(interDtype, token->val);
            token = getToken();
            while(strcmp(token->val,";") != 0) {
                if(strcmp(token->type, "ID") != 0) {
                    token = getToken();
                    continue;
                }
                insertST(token->val, token->type, interDtype);
                token = getToken();
            }
        }
    }
    printST();
    fclose(fp);
}