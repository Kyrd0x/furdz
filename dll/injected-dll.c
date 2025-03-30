



#include <windows.h>
#include <shellapi.h>

// UTF16("str") == L"str" # Wide Character String / UTF-16
#define _UTF16(x)   L##x
#define UTF16(x)    _UTF16(x)
// This function is our 'dummy' malware, juste to help
// visualize when our process is actually running.

void RenameTrashFiles() {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char trashPath[MAX_PATH];
    char oldFilePath[MAX_PATH];
    char newFilePath[MAX_PATH];

    // Récupérer le chemin de la corbeille
    snprintf(trashPath, MAX_PATH, "%s\\*", getenv("USERPROFILE"));
    strcat(trashPath, "\\AppData\\Local\\Microsoft\\Windows\\Recycle Bin\\*.*");

    // Recherche de fichiers dans la corbeille
    hFind = FindFirstFile(trashPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Aucun fichier trouvé dans la corbeille.\n");
        return;
    }

    do {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            snprintf(oldFilePath, MAX_PATH, "%s\\%s", trashPath, findFileData.cFileName);
            snprintf(newFilePath, MAX_PATH, "%s\\%s.enc", trashPath, findFileData.cFileName);
            
            if (MoveFile(oldFilePath, newFilePath)) {
                printf("Renommé: %s -> %s\n", findFileData.cFileName, newFilePath);
            } else {
                printf("Erreur de renommage pour: %s\n", findFileData.cFileName);
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}


__declspec(dllexport) DWORD WINAPI Notify(LPVOID param) {
    (void)param;
    NOTIFYICONDATAW nid = {0};
    nid.cbSize = sizeof(nid);

    // dummy msg-only window
    HWND hWnd = CreateWindowW(L"Static", L"Dummy", WS_OVERLAPPEDWINDOW,
                              0, 0, 0, 0, HWND_MESSAGE, NULL, 0, NULL);
    nid.hWnd = hWnd;
    nid.uID = 1;
    nid.uFlags = NIF_INFO;
    nid.hIcon = LoadIconW(0, MAKEINTRESOURCEW(IDI_APPLICATION));
    wsprintfW(nid.szInfoTitle, UTF16(__FILE__));
    nid.dwInfoFlags = NIIF_INFO | NIIF_NOSOUND;

    while (1) {
        SYSTEMTIME st;
        GetLocalTime(&st);
        wsprintfW(nid.szInfo,
                L"[%04d-%02d-%02d %02d:%02d:%02d] Running on PID %lu",
                st.wYear, st.wMonth, st.wDay,
                st.wHour, st.wMinute, st.wSecond,
                GetCurrentProcessId());
        Shell_NotifyIconW(NIM_DELETE, &nid);
        Shell_NotifyIconW(NIM_ADD, &nid);
        Sleep(5000);
    }
}

__declspec(dllexport) void CALLBACK TestFunction() {
    MessageBoxA(NULL, "TestFunction() called!", __FILE__, MB_OK);
    Sleep(10000);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    (void)hinstDLL; (void)lpvReserved;
    if (fdwReason == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, (void*)Notify, 0, 0, 0);
    return TRUE;
}

