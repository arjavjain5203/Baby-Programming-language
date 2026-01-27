section .text
global _start
_start:
    mov rax, 18
    push rax
    mov rax, 9
    push rax
    pop rbx
    pop rax
    mul rbx
    push rax
    mov rax, 7
    push rax
    pop rbx
    pop rax
    sub rax, rbx
    push rax
    mov rax, QWORD [rsp + 0]
    push rax
    pop rdi
    call print_int
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_0]
    mov rdx, 6
    syscall
    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    mov rdi, 0
    syscall

print_int:
    push rbp
    mov rbp, rsp
    sub rsp, 32
    mov rax, rdi
    lea rsi, [rel buffer]
    mov rcx, 0
    cmp rax, 0
    jge .L1
    neg rax
    mov byte [rsi], '-'
    inc rsi
    inc rcx
.L1:
    mov r8, 10
    mov r9, 0
.L2:
    xor edx, edx
    div r8
    add dl, '0'
    push rdx
    inc r9
    cmp rax, 0
    jne .L2
.L3:
    pop rax
    mov byte [rsi], al
    inc rsi
    inc rcx
    dec r9
    jne .L3
    mov byte [rsi], 10
    inc rcx
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel buffer]
    mov rdx, rcx
    syscall
    leave
    ret

section .bss
buffer: resb 32

section .data
msg_0: db "love u", 0
