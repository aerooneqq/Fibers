.intel_syntax noprefix
.section .text
.globl FillContext
.globl SetContext

FillContext:
    mov r8, [rsp]
    mov [rcx], r8
    lea r8, [rsp + 8]
    mov [rcx + 8 * 1], r8

    mov [rcx + 8 * 2], rbx
    mov [rcx + 8 * 3], rbp
    mov [rcx + 8 * 4], r12
    mov [rcx + 8 * 5], r13
    mov [rcx + 8 * 6], r14
    mov [rcx + 8 * 7], r15
    mov [rcx + 8 * 8], rdi
    mov [rcx + 8 * 9], rsi

    xor rax, rax
    ret

SetContext:
    mov r8, [rcx]
    mov rsp, [rcx + 8 * 1]

    mov rbx, [rcx + 8 * 2]
    mov rbp, [rcx + 8 * 3]
    mov r12, [rcx + 8 * 4]
    mov r13, [rcx + 8 * 5]
    mov r14, [rcx + 8 * 6]
    mov r15, [rcx + 8 * 7]
    mov rdi, [rcx + 8 * 8]
    mov rsi, [rcx + 8 * 9]

    mov rdx, [rcx + 8 * 11]
    mov rcx, [rcx + 8 * 10]

    pushq r8
    xor rax, rax
    ret