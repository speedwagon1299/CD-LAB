#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char LHS[7] = {'S','E','E','T','T','F','F'};
const char RHS[7][4] = {"E","E+T","T","T*F","F","(E)","a"};

const char open_bracket[12][3] = {"s4",".",".",".","s4",".","s4","s4",".",".",".","."};
const char close_bracket[12][3] = {".",".","r2","r4",".","r6",".",".","s11","r1","r3","r5"};
const char plus[12][3] = {".","s6","r2","r4",".","r6",".",".","s6","r1","r3","r5"};
const char mult[12][3] = {".",".","s7","r4",".","r6",".",".",".","s7","r3","r5"};
const char id[12][3] = {"s5",".",".",".","s5",".","s5","s5",".",".",".","."};
const char dollar[12][3] = {".","a","r2","r4",".","r6",".",".",".","r1","r3","r5"};
const char F[12][3] = {"3",".",".",".","3",".","3","10",".",".",".","."};
const char T[12][3] = {"2",".",".",".","2",".","9",".",".",".",".","."};
const char E[12][3] = {"1",".",".",".","8",".",".",".",".",".",".","."};

const char input[20] = "a*a+a$";  
char stack[50][3] = {"0"};   // Stack for storing states
char symbols[50] = "\0";     // Symbol stack

int main() {
    int iptr = 0;   // indicates current symbol read
    int top = 0;    // indicates current top of stack
    int sptr = 0;   // flexible symbol pointer, points to empty space
    int accept = 0; // accept flag
    
    while (iptr < strlen(input)) {
        char c = input[iptr];   // current input character
        int stval = atoi(stack[top]);  // value at top of stack
        char temp[3] = "\0";    // SLR Table entry value
        char nextval[3] = "\0"; // next dfa state to go to
        switch(c) {
            case '(': strcpy(temp, open_bracket[stval]); break;
            case ')': strcpy(temp, close_bracket[stval]); break;
            case '+': strcpy(temp, plus[stval]); break;
            case '*': strcpy(temp, mult[stval]); break;
            case 'a': strcpy(temp, id[stval]); break;
            case '$': strcpy(temp, dollar[stval]); break;
        }
        if(temp[0] == 's') {   // shift operation
            strcpy(nextval, temp+1);
            strcpy(stack[++top], nextval);
            symbols[sptr++] = c;
            iptr++;
        }
        else if(temp[0] == 'r') {   // reduce operation
            strcpy(nextval, temp+1);
            int val = atoi(nextval); 
            // Reducing symbols 
            char lhs = LHS[val];  
            int len = strlen(RHS[val]);  
            // pop from stack and symbol
            top -= len;
            sptr -= len;
            symbols[sptr++] = lhs;
            // get next stack symbol from GOTO of current top
            stval = atoi(stack[top]);
            switch(lhs) {
                case 'F': 
                    strcpy(stack[++top], F[stval]); 
                    break;
                case 'T': 
                    strcpy(stack[++top], T[stval]); 
                    break;
                case 'E': 
                    strcpy(stack[++top], E[stval]); 
                    break;
            }
        }
        else if(temp[0] == 'a') {   // accept condition
            // Input pointer must be at $
            // Symbol must be at real start symbol (not S' but S)
            // Stack must have 2 elements (top at 1)
            if(input[iptr] == '$' && symbols[sptr-1] == LHS[1] && top == 1) {
                accept = 1;
                printf("Accepted\n");
                break;
            }
        }
        else {
            printf("Error: '%c' at state %d\n", c, stval);
            return 1;
        }
    }
    if(!accept) {
        printf("Rejected\n");
    }
    return 0;
}
