#include <stdio.h>
#include <stdlib.h>

int main() {
    const char* ifname = "file_input.txt";
    const char* ofname = "file_output.txt";
    FILE *fin, *fout; 
    if((fin = fopen(ifname, "r")) == NULL) {
        perror("Error Opening Input File");
        exit(0);
    }
    if((fout = fopen(ofname, "w")) == NULL) {
        perror("Error Opening Output File");
        exit(0);
    }
    fseek(fin, 0, SEEK_END);
    long fsize = ftell(fin);
    printf("\nFile Size: %ld\n", fsize);
    for(long i = fsize-1; i >= 0; i--) {
        fseek(fin, i, SEEK_SET);
        char c = fgetc(fin);
        fputc(c, fout);
    }
    fclose(fin);
    fclose(fout);
}