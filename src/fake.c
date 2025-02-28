#include "definitions.h"

int divide(int a, int b) { return a / b; }
int multiply(int a, int b) { return a * b; }
int add(int a, int b) { return a + b; }
int substract(int a, int b) { return a - b; }

FILE* clean_init() {
    return NULL;
}

void clean_exit(FILE* file) {
    fprintf(file, "Time is running, the bank will close soon\n");
    fclose(file);
    exit(0);
}