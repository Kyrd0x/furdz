;--------------------------ORIGINAL----------


_start:
    cld
    and rsp,0xfffffffffffffff0
    call save_beginning

metasploit_block_api:
    push r9
    push r8
    xor rax,rax
    mov rax,0xff
    push rdx
    xor rdx,rdx
    xor rax,0x9f
    mov rax,[gs:rax] ; 0x60
    xor rax,%RANDOM_1%
    push rcx
    mov rdx,rax
    xor rdx,%RANDOM_1%
    ; mov rdx,[rax+0x60]
    mov rdx,[rdx+0x18]
    xor rdx,rax
    push rsi
    xor rdx,rax
    nop
    mov rdx,[rdx+0x20]

load_module_name:
    mov rsi,[rdx+0x50]     
    movzx rcx, word [rdx+0x4a]
    xor r9,r9  
    shl rcx, 1

module_hash_loop:
    xor rax,rax 
    lodsb 
    cmp al,0x61
    jl skip_case_adjustment
    sub al,0x20
skip_case_adjustment:
    ror r9d,%ROR_VALUE%
    dec rcx
    add r9d,eax
    loop module_hash_loop

is_valid_module:
    push rdx
    push r9
    mov rdx,[rdx+0x20]   ; 
    mov eax,[rdx+0x3c]   ; offset de l'entete PE (e_lfanew)
    add rax,rdx          ; RAX situé à l'entete PE
    cmp word [rax+0x18],0x20b   ; Vérifier si c'est une image 64 bits (0x20b correspond à PE64)
    jnz load_next_module        ; Si pas le cas, on passe au module suivant

    mov eax,[rax+0x88]   ; offset de IMAGE_EXPORT_DIRECTORY
    test rax,rax         ; si pas d'offset pour IMAGE_EXPORT_DIRECTORY, le module n'est pas le bon, on passe au suivant
    jz load_next_module

    add rax,rdx          ; On recupere l'adresse de IMAGE_EXPORT_DIRECTORY, elle contient des adresses relatives (offsets) de tables dont nous allons avoir besoin
    mov ecx,[rax+0x18]   ; NumberOfNames
    mov r8d,[rax+0x20]   ; AddressOfNames (offset de la table des RVA des noms des fonctions)
    add r8,rdx
    push rax             ; On sauvegarde l'adresse de IMAGE_EXPORT_DIRECTORY

find_function:
    jrcxz pre_load_next_module
    xor r9,r9
    dec rcx
    mov esi,[r8+rcx*4] 
    add rsi,rdx

function_hash_loop:
    xor rax,rax
    lodsb
    ror r9d,%ROR_VALUE%
    add r9d,eax
    cmp al,ah
    jnz function_hash_loop

check:
    add r9,[rsp+0x8]    ; on aoute au hash celui du module
    cmp r9d,r10d        ; afin d'essayer de trouver celui stocké en dur
    jnz find_function   ; Si une correspondance a été trouvée, on passe à la suite
    
final:                  
    pop rax             ; On recupere adresse de IMAGE_EXPORT_DIRECTORY
    mov r8d,[rax+0x24]  ; AddressOfNameOrdinals, à IMAGE_EXPORT_DIRECTORY + 0x24
    add r8,rdx
    mov cx,[r8+rcx*2]   ; rcx est l'index de la fonction trouvée precedement dans AddressOfNames, on recupere la valeur correspondante dans AddressOfNameOrdinals
    mov r8d,[rax+0x1c]  ; EAT, à IMAGE_EXPORT_DIRECTORY + 0x1C (aussi appelé AddressOfNames)
    add r8,rdx   
    mov eax,[r8+rcx*4] 
    pop r8
    add rax,rdx 
    xor rax,%RANDOM_2%
    pop r8
    pop rsi
    pop rcx 
    pop rdx  
    pop r8 
    pop r9 
    pop r10
    sub rsp,byte 0x20
    push r10
    xor rax,%RANDOM_2%
    jmp rax 

pre_load_next_module:
    pop rax             
load_next_module:
    pop r9
    pop rdx
    mov rdx, [rdx]
    jmp load_module_name

save_beginning:
    pop rbp               ; rbp = @->beginning (metasploit_block_api)
    call load_USER32DLL
    db `user32.dll\0`

load_USER32DLL:
    pop rcx   
    mov r10d,%HASH_kernel32.dll_LoadLibraryA%   ; Hash de kernel32.LoadLibraryA 
    call rbp 
    mov r9, 0x0     
    call load_HELLO
    db `Hello\0`

load_HELLO:
    pop rdx
    call load_INFO
    db `Info\0`

load_INFO:
    pop r8

load_hashes:
    xor rcx,rcx
    mov r10d,%HASH_user32.dll_MessageBoxA%    ; Hash de user32.MessageBoxA
    call rbp
    xor rcx,rcx
    mov r10d,%HASH_kernel32.dll_ExitProcess%   ; Hash de kernel32.ExitProcess
    call rbp