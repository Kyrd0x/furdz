mov r10d,0xCCE6C0C4 ; input hash
mov rdx, "PEB" ; handle
mov rdx,[rdx+0x20]
mov eax,[rdx+0x3c]
add rax,rdx
mov eax,[rax+0x88]
add rax,rdx
mov ecx,[rax+0x18]
mov r8d,[rax+0x20]
add r8,rdx

find_function:
    jrcxz final
    xor r9,r9
    dec rcx
    mov esi,[r8+rcx*4] 
    add rsi,rdx

function_hash_loop:
    xor rax,rax
    lodsb
    ror r9d,byte 0xD
    add r9d,eax
    cmp al,ah
    jnz function_hash_loop

check:
    add r9,[rsp+0x8]    ; on aoute au hash celui du module
    cmp r9d,r10d        ; afin d'essayer de trouver celui stocké en dur
    jnz find_function   ; Si une correspondance a été trouvée, on passe à la suite

final:
    mov r8d,[rax+0x24]  ; AddressOfNameOrdinals, à IMAGE_EXPORT_DIRECTORY + 0x24
    add r8,rdx
    mov cx,[r8+rcx*2]   ; rcx est l'index de la fonction trouvée precedement dans AddressOfNames, on recupere la valeur correspondante dans AddressOfNameOrdinals
    mov r8d,[rax+0x1c]  ; EAT, à IMAGE_EXPORT_DIRECTORY + 0x1C (aussi appelé AddressOfNames)
    add r8,rdx   
    mov eax,[r8+rcx*4] 
    add rax,rdx
    ; return rax




