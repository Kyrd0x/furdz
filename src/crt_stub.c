#include <windows.h>

extern int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

void* mark_section_writable(void* p) { return p; }

int WinMainCRTStartup(void) {
    return WinMain(NULL, NULL, "", 1);
}