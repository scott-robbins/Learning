; division
section	.text
global main
extern printf     

main:
	xor eax, eax
	xor ebx, ebx
	xor ecx, ecx
	xor edx, edx
	mov eax, 1337
	mov ecx, 42
	div ecx

	mov ecx, eax

	; result is in rax
	push eax
	push format
	call printf
	add esp, 8
	ret

section	.data               
	remainder db 0x0
	format db "1337/42 = %d", 10, 0


; nasm -f elf div.asm && gcc -m32 -o divide div.o