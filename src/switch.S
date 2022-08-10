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

    movups [rcx + 8 * 12 + 0 * 16], xmm6
    movups [rcx + 8 * 12 + 1 * 16], xmm7
    movups [rcx + 8 * 12 + 2 * 16], xmm8
    movups [rcx + 8 * 12 + 3 * 16], xmm9
    movups [rcx + 8 * 12 + 4 * 16], xmm10
    movups [rcx + 8 * 12 + 5 * 16], xmm11
    movups [rcx + 8 * 12 + 6 * 16], xmm12
    movups [rcx + 8 * 12 + 7 * 16], xmm13
    movups [rcx + 8 * 12 + 8 * 16], xmm14
    movups [rcx + 8 * 12 + 9 * 16], xmm15

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

    movups xmm6, [rcx + 8 * 12 + 0 * 16]
    movups xmm7, [rcx + 8 * 12 + 1 * 16]
    movups xmm8 ,[rcx + 8 * 12 + 2 * 16]
    movups xmm9 ,[rcx + 8 * 12 + 3 * 16]
    movups xmm10 ,[rcx + 8 * 12 + 4 * 16]
    movups xmm11 ,[rcx + 8 * 12 + 5 * 16]
    movups xmm12 ,[rcx + 8 * 12 + 6 * 16]
    movups xmm13 ,[rcx + 8 * 12 + 7 * 16]
    movups xmm14 ,[rcx + 8 * 12 + 8 * 16]
    movups xmm15 ,[rcx + 8 * 12 + 9 * 16]

    mov rcx, [rcx + 8 * 10]
    pushq r8
    xor rax, rax
    ret