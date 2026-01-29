section .text
global _start

func_add:
    push rbp
    mov rbp, rsp
    push QWORD [rbp + 16]
    push QWORD [rbp + 24]
    mov rax, QWORD [rsp + 8]
    push rax
    mov rax, QWORD [rsp + 8]
    push rax
    pop rbx
    pop rax
    add rax, rbx
    push rax
    pop rax
    leave
    ret
    add rsp, 0
    mov rax, 0
    leave
    ret

_start:
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_0]
    mov rdx, 35
    syscall
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel newline_const]
    mov rdx, 1
    syscall
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_1]
    mov rdx, 35
    syscall
    mov rax, 10
    push rax
    mov rax, 5
    push rax
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_2]
    mov rdx, 7
    syscall
    mov rax, QWORD [rsp + 8]
    push rax
    pop rdi
    call print_int
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_3]
    mov rdx, 6
    syscall
    mov rax, QWORD [rsp + 0]
    push rax
    pop rdi
    call print_int
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_4]
    mov rdx, 22
    syscall
    mov rax, QWORD [rsp + 8]
    push rax
    mov rax, QWORD [rsp + 8]
    push rax
    pop rbx
    pop rax
    add rax, rbx
    push rax
    pop rdi
    call print_int
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_5]
    mov rdx, 24
    syscall
    mov rax, QWORD [rsp + 8]
    push rax
    mov rax, QWORD [rsp + 8]
    push rax
    pop rbx
    pop rax
    sub rax, rbx
    push rax
    pop rdi
    call print_int
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_6]
    mov rdx, 28
    syscall
    mov rax, QWORD [rsp + 8]
    push rax
    mov rax, QWORD [rsp + 8]
    push rax
    pop rbx
    pop rax
    mul rbx
    push rax
    pop rdi
    call print_int
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_7]
    mov rdx, 21
    syscall
    mov rax, QWORD [rsp + 8]
    push rax
    mov rax, QWORD [rsp + 8]
    push rax
    pop rbx
    pop rax
    xor rdx, rdx
    div rbx
    push rax
    pop rdi
    call print_int
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_8]
    mov rdx, 32
    syscall
    mov rax, 10
    push rax
    mov rax, 20
    push rax
    mov rax, QWORD [rsp + 8]
    push rax
    mov rax, QWORD [rsp + 8]
    push rax
    pop rbx
    pop rax
    cmp rax, rbx
    setl al
    movzx rax, al
    push rax
    pop rax
    test rax, rax 
    jz label1
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_9]
    mov rdx, 24
    syscall
    add rsp, 0
    jmp label0
label1:
label0:
    mov rax, QWORD [rsp + 8]
    push rax
    mov rax, QWORD [rsp + 8]
    push rax
    pop rbx
    pop rax
    cmp rax, rbx
    setg al
    movzx rax, al
    push rax
    pop rax
    test rax, rax 
    jz label3
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_10]
    mov rdx, 27
    syscall
    add rsp, 0
    jmp label2
label3:
label2:
    mov rax, QWORD [rsp + 8]
    push rax
    mov rax, 10
    push rax
    pop rbx
    pop rax
    cmp rax, rbx
    sete al
    movzx rax, al
    push rax
    pop rax
    test rax, rax 
    jz label5
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_11]
    mov rdx, 24
    syscall
    add rsp, 0
    jmp label4
label5:
label4:
    mov rax, QWORD [rsp + 8]
    push rax
    mov rax, QWORD [rsp + 8]
    push rax
    pop rbx
    pop rax
    cmp rax, rbx
    setne al
    movzx rax, al
    push rax
    pop rax
    test rax, rax 
    jz label7
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_12]
    mov rdx, 23
    syscall
    add rsp, 0
    jmp label6
label7:
label6:
    mov rax, QWORD [rsp + 8]
    push rax
    mov rax, 10
    push rax
    pop rbx
    pop rax
    cmp rax, rbx
    setle al
    movzx rax, al
    push rax
    pop rax
    test rax, rax 
    jz label9
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_13]
    mov rdx, 24
    syscall
    add rsp, 0
    jmp label8
label9:
label8:
    mov rax, QWORD [rsp + 0]
    push rax
    mov rax, 20
    push rax
    pop rbx
    pop rax
    cmp rax, rbx
    setge al
    movzx rax, al
    push rax
    pop rax
    test rax, rax 
    jz label11
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_14]
    mov rdx, 24
    syscall
    add rsp, 0
    jmp label10
label11:
label10:
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_15]
    mov rdx, 48
    syscall
    mov rax, 50
    push rax
    mov rax, QWORD [rsp + 0]
    push rax
    mov rax, 10
    push rax
    pop rbx
    pop rax
    cmp rax, rbx
    sete al
    movzx rax, al
    push rax
    pop rax
    test rax, rax 
    jz label13
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_16]
    mov rdx, 21
    syscall
    add rsp, 0
    jmp label12
label13:
    mov rax, QWORD [rsp + 0]
    push rax
    mov rax, 50
    push rax
    pop rbx
    pop rax
    cmp rax, rbx
    sete al
    movzx rax, al
    push rax
    pop rax
    test rax, rax 
    jz label14
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_17]
    mov rdx, 29
    syscall
    add rsp, 0
    jmp label12
label14:
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_18]
    mov rdx, 23
    syscall
    add rsp, 0
label12:
    mov rax, QWORD [rsp + 0]
    push rax
    mov rax, 999
    push rax
    pop rbx
    pop rax
    cmp rax, rbx
    sete al
    movzx rax, al
    push rax
    pop rax
    test rax, rax 
    jz label16
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_19]
    mov rdx, 22
    syscall
    add rsp, 0
    jmp label15
label16:
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_20]
    mov rdx, 33
    syscall
    add rsp, 0
label15:
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_21]
    mov rdx, 22
    syscall
    mov rax, 20
    push rax
    mov rax, 10
    push rax
    call func_add
    add rsp, 16
    push rax
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_22]
    mov rdx, 19
    syscall
    mov rax, QWORD [rsp + 0]
    push rax
    pop rdi
    call print_int
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_23]
    mov rdx, 23
    syscall
    mov rax, 3
    push rax
label17:
    mov rax, QWORD [rsp + 0]
    push rax
    mov rax, 0
    push rax
    pop rbx
    pop rax
    cmp rax, rbx
    setg al
    movzx rax, al
    push rax
    pop rax
    test rax, rax 
    jz label18
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_24]
    mov rdx, 8
    syscall
    mov rax, QWORD [rsp + 0]
    push rax
    pop rdi
    call print_int
    mov rax, QWORD [rsp + 0]
    push rax
    mov rax, 1
    push rax
    pop rbx
    pop rax
    sub rax, rbx
    push rax
    pop rax
    mov [rsp + 0], rax
    add rsp, 0
    jmp label17
label18:
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_25]
    mov rdx, 14
    syscall
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_26]
    mov rdx, 24
    syscall
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_27]
    mov rdx, 58
    syscall
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_28]
    mov rdx, 31
    syscall
    mov rax, 0
    push rax
    mov rax, 60
    pop rdi
    syscall
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel msg_29]
    mov rdx, 21
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
newline_const: db 10, 0
msg_29: db "THIS SHOULD NOT PRINT", 0
msg_11: db "a == 10 (True - Correct)", 0
msg_9: db "10 < 20 (True - Correct)", 0
msg_8: db "--- 2. Logic and Comparisons ---", 0
msg_7: db "Division (x / y = 2):", 0
msg_23: db "--- 4. Loops (wait) ---", 0
msg_6: db "Multiplication (x * y = 50):", 0
msg_20: db "val is not 999 (Correct - moveon)", 0
msg_22: db "add(10, 20) result:", 0
msg_27: db "You should not see any errors from the hidden block above.", 0
msg_5: db "Subtraction (x - y = 5):", 0
msg_28: db "=== TEST COMPLETE - EXITING ===", 0
msg_4: db "Addition (x + y = 15):", 0
msg_18: db "val is else (Incorrect)", 0
msg_12: db "a != b (True - Correct)", 0
msg_15: db "--- 3. Control Flow (maybe, ormaybe, moveon) ---", 0
msg_10: db "10 > 20 (False - Incorrect)", 0
msg_0: db "=== STARTING COMPREHENSIVE TEST ===", 0
msg_2: db "x (10):", 0
msg_1: db "--- 1. Variables and Arithmetic ---", 0
msg_13: db "a <= 10 (True - Correct)", 0
msg_21: db "--- 3.5. Functions ---", 0
msg_14: db "b >= 20 (True - Correct)", 0
msg_16: db "val is 10 (Incorrect)", 0
msg_3: db "y (5):", 0
msg_17: db "val is 50 (Correct - ormaybe)", 0
msg_19: db "val is 999 (Incorrect)", 0
msg_24: db "Counter:", 0
msg_25: db "Loop Finished!", 0
msg_26: db "--- 5. Comments Test ---", 0
