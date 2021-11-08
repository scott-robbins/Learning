; destroy a file using commandline argument 
; remove(const char *path)

section .text
	global _start

_start:
	; delete file name test.txt
	mov rdi, [rsp+16]			; get commandline argument of filename
	mov rax, 87					; unlink
	syscall

	; exit
	mov eax, 1
	int 0x80
