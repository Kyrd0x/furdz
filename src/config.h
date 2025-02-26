#ifndef CONFIG_H
#define CONFIG_H

#include <windows.h>

#define HOSTNAME_PREFIX "CYCORP-"
#define DEBUG_MODE 1

#define TRUE 1
#define FALSE 0

void XOR(unsigned char *data, size_t len, WORD key);

// HMODULE CustomGetModuleHandle(unsigned int module_hash);
// FARPROC CustomGetProcAdress(IN HMODULE hModule, unsigned int function_hash);
// void v();
// int d();
// int z();
// char* h();
// char* c();

#endif // CONFIG_H
