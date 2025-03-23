#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "getNT.h"

#define TABLE_SIZE 50

struct STNode {
	char lexName[50], tokenType[50], dtype[20];
	struct STNode* next;
};

struct STNode** ST = NULL;

void initializeST() {
	ST = (struct STNode**) malloc(TABLE_SIZE * sizeof(struct STNode*));
	for(int i = 0; i < 50; i++) {
    	ST[i] = NULL;
	}
}

int hash(char* name) {
	int val = 0;
	for(int i = 0; name[i] != '\0'; i++) {
    	val += name[i];
	}
	return val % TABLE_SIZE;
}

void insertST(char* lexName, char* tokenType, char* dtype) {
	int index = hash(lexName);
	struct STNode* entry = ST[index];
	while (entry != NULL) {
    	if (strcmp(entry->lexName, lexName) == 0) {
        	printf("Duplicate entry found: %s, skipping insertion.\n", lexName);
        	return;
    	}
    	entry = entry->next;
	}
	struct STNode* nn = (struct STNode*) malloc(sizeof(struct STNode));
	strcpy(nn->lexName, lexName);
	strcpy(nn->tokenType, tokenType);
	strcpy(nn->dtype, dtype);
	nn->next = ST[index];
	ST[index] = nn;
}

void printST() {
	printf("\nSYMBOL TABLE:\n");
	printf("-----------------------------------------\n");
	printf("%-20s %-15s %-10s\n", "Lexeme", "Token Type", "Data Type");
	printf("-----------------------------------------\n");
	for (int i = 0; i < TABLE_SIZE; i++) {
    	struct STNode* entry = ST[i];
    	while (entry) {
        	printf("%-20s %-15s %-10s\n", entry->lexName, entry->tokenType, entry->dtype);
        	entry = entry->next;
    	}
	}
	printf("-----------------------------------------\n");
}

int main() {
	fp = fopen("input.c", "r");
	if (fp == NULL) {
    	printf("Error: Unable to open file\n");
    	exit(1);
	}
	initializeST();
	struct Token* token;
	char interDtype[50];
	char dbuf[10][25];
	int dbuf_ind = 0;
	while ((token = getNextToken()) != NULL) {
    	if(strcmp(token->type,"KEYWORD") == 0) {
        	if(strcmp(token->val, "int") == 0 || strcmp(token->val, "float") == 0 || strcmp(token->val, "short") == 0 ||
                	strcmp(token->val, "long") == 0 || strcmp(token->val, "double") == 0 || strcmp(token->val, "char") == 0) {
            	strcpy(interDtype, token->val);
            	dbuf_ind = 0;
            	while((token = getNextToken()) != NULL) {
                	if (strcmp(token->type, "IDENTIFIER") == 0) {
                    	strcpy(dbuf[dbuf_ind++], token->val);
                	}
                	else if (strcmp(token->val, ",") == 0) {
                    	continue;
                	}
                	else if (strcmp(token->val, "(") == 0) {
                    	insertST(dbuf[0], "FUNC_ID", interDtype);
                    	break;
                	}
                	else if (strcmp(token->val, ";") == 0 || strcmp(token->val, ")") == 0) {
                    	for(int i = 0; i < dbuf_ind; i++) {
                        	insertST(dbuf[i], "VAR_ID", interDtype);
                    	}
                    	break;
                	}
            	}
        	}
    	}
	}
	printST();
	fclose(fp);
	return 0;
}
