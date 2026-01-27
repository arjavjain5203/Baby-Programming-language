global _start
_start:
    mov rax, 10
    push rax
    mov rax, 2
    push rax
    mov rax, 3
    push rax
    pop rbx
    pop rax
    mul rbx
    push rax
    pop rbx
    pop rax
    sub rax, rbx
    push rax
    mov rax, 2
    push rax
    pop rbx
    pop rax
    div rbx
    push rax
    mov rax, 2
    push rax
    mov rax, QWORD [rsp + 8]
    push rax
    pop rbx
    pop rax
    mul rbx
    push rax
    mov rax, QWORD [rsp + 0]
    push rax
    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    mov rdi, 0
    syscall
