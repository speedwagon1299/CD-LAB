#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char LHS[3] = {'P', 'S', 'S'};
const char RHS[3][5] = {"S", "(S)S", ""};

const char open[6][3] = {"s2", ".", "s2", ".", "s2", "."};
const char close[6][3] = {"r2", ".", "r2", "s4", "r2", "r1"};
const char dollar[6][3] = {"r2", "a", "r2", ".", "r2", "r1"};
const char S[6][3] = {"1", ".", "3", ".", "5", "."};

const char input[10] = "(())()$";

char stack[50][3] = {"0"};
char symbols[50] = "\0";

int main() {
    int iptr = 0, sptr = 0, top = 0, accept = 0;
    while(iptr < strlen(input)) {
        char c = input[iptr];
        int stval = atoi(stack[top]);
        char temp[3] = "\0";
        switch(c) {
            case '(':
                strcpy(temp, open[stval]);
                break;
            case ')':
                strcpy(temp, close[stval]);
                break;
            case '$':
                strcpy(temp, dollar[stval]);
                break;
        }
        if(temp[0] == 's') {
            iptr++;
            strcpy(stack[++top], temp+1);
            symbols[sptr++] = c;
        }
        else if(temp[0] == 'r') {
            char nextval[3] = "\0";
            strcpy(nextval, temp+1);
            int ind = atoi(nextval);
            char lhs = LHS[ind];
            int len = strlen(RHS[ind]);
            sptr -= len;
            top -= len;
            symbols[sptr++] = lhs;
            int val = atoi(stack[top]);
            switch(lhs) {
                case 'S':
                    strcpy(stack[++top], S[val]);
                    break;
            }
        }
        else if(temp[0] == 'a') {
            accept = 1;
            break;
        }
        else {
            printf("\nInvalid Entry!\n");
            break;
        }
    }
    if(accept) {
        printf("\nAccepted!\n");
    }
    exit(0);
}


// int main() {
//     int iptr = 0, top = 0, sptr = 0, accept = 0;
//     while(iptr < strlen(input)) {
//         char c = input[iptr];
//         int stval = atoi(stack[top]);
//         char temp[3] = "\0";
//         switch(c) {
//             case '(':
//                 strcpy(temp, open[stval]);
//                 break;
//             case ')':
//                 strcpy(temp, close[stval]);
//                 break;
//             case '$':
//                 strcpy(temp, dollar[stval]);
//                 break;
//         }
//         if(temp[0] == 's') {
//             iptr++;
//             strcpy(stack[++top], temp+1);
//             symbols[sptr++] = c;
//         }
//         else if(temp[0] == 'r') {
//             char nextval[3] = "\0";
//             strcpy(nextval, temp+1);
//             int val = atoi(nextval);
//             char lhs = LHS[val];
//             int len = strlen(RHS[val]);
//             sptr -= len;
//             top -= len;
//             symbols[sptr++] = lhs;
//             int cur = atoi(stack[top]);
//             switch(lhs) {
//                 case 'S':
//                     strcpy(stack[++top], S[cur]);
//                     break;
//             }
//         }
//         else if(temp[0] == 'a') {
//             accept = 1;
//             break;
//         }
//         else {
//             printf("\nErraneous entry\n");
//             break;
//         }
//     }
//     if(accept) {
//         printf("\nValid");
//     }
//     else {
//         printf("\nInvalid");
//     }
//     return 0;
// }