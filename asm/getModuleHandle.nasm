mov r10d,0xCCE6C0C4 ; input hash
xor rdx,rdx
mov rdx,[gs:rdx+0x60]
mov rdx,[rdx+0x18]
mov rdx,[rdx+0x20]

load_module_name:
    mov rsi,[rdx+0x50]     
    movzx rcx, word [rdx+0x4a]  
    xor r9,r9  

module_hash_loop:
    xor rax,rax 
    lodsb 
    cmp al,0x61
    jl skip_case_adjustment
    sub al,0x20
skip_case_adjustment:
    ror r9d,byte 0xd
    add r9d,eax
    loop module_hash_loop
then:
    cmp r9d,r10d
    jz found
load_next_module:
    mov rdx, [rdx]
    jmp load_module_name
found:
    ; rdx contains the module base address