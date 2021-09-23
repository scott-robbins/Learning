; make a file using argument in
; open(path, O_WRONLY | O_CREAT | O_TRUNC, mod);
; O_RDONLY         00
; O_WRONLY         01
; O_RDWR           02
; O_CREAT        0100

section .data
fname   db  "test.txt"

section .text
	global _start

_start:
    ; create file named test.txt
    mov rdi, fname
    mov rsi, 0102o     ; O_CREAT
    mov rdx, 0666o     ; mode
    mov rax, 2
    syscall

    ; exit
    mov eax, 1
    int 0x80
