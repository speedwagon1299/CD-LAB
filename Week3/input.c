#include <stdio.h>

int main() {
    int a = 5, b = 10;
    int c;
    a = a + b;      // Simple addition
    a += b;         // Compound addition
    a++;            // Increment
    b--;            // Decrement
    b = a / 3;
    const char* str = "this is a \'STRONG\' message\n";
    /*
    This is a multiline comment
    */
    if ( a  <  b  &&  b == 10 ) {
        b = a;
    }

    return 0;
}