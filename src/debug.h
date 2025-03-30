#include <windows.h>
#include <winnt.h>
#include <stdio.h>
#include <winternl.h>
#include <stdarg.h>
#include <shellapi.h>
#include <tlhelp32.h>
#include <tlhelp32.h>

#define FORCE_INLINE __attribute__((always_inline)) static inline
#define CUR_PROC (HANDLE)-1

typedef void (*DllEntryPoint)(HINSTANCE, DWORD, LPVOID);




// structures for base relocation
typedef struct _RELOC_BLOCK {
    DWORD PageAddress;
    DWORD BlockSize;
} RELOC_BLOCK;

typedef struct _RELOC_ENTRY {
    USHORT Offset : 12;
    USHORT Type   : 4;
} RELOC_ENTRY;



#define Debug(fmt, ...) _Debug(0, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define Fail(fmt, ...) _Debug(1, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
void _Debug(
        BOOL fail,
        const char *file, const char* func, int line,
        const char *fmt, ...)
{
    char buf[1024] = {0};
    char *ptr = buf;
    char *prefix = fail ? "ERROR" : "DEBUG";
    ptr += wsprintfA(ptr, "[%s - PID %lu] ", prefix, GetCurrentProcessId());

    va_list args;
    va_start(args, fmt);
    ptr += wvsprintfA(ptr, fmt, args);
    va_end(args);

    ptr += wsprintfA(ptr, " (%s() @ %s:%d)\r\n",
            func, file, line);

    HANDLE hFile = CreateFileA(
        "Z:\\sandbox\\debug.log",
        FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
    );
    if (hFile == INVALID_HANDLE_VALUE) {
        MessageBoxA(NULL, "Can't open DEBUG file !", __FILE__, MB_ICONERROR);
        ExitProcess(1);
    }
    else {
        WriteFile(hFile, buf, ptr-buf, NULL, NULL);
        CloseHandle(hFile);
    }
    if (fail) {
        MessageBoxA(NULL, buf, __FILE__, MB_ICONERROR);
        ExitProcess(1);
    }
}