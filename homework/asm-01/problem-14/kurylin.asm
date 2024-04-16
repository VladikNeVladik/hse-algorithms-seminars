bits 32

extern io_print_udec, io_get_udec, io_get_dec, io_newline, io_print_dec


section .bss
varN: resd 1
varM: resd 1
varK: resd 1
varD: resd 1
varX: resd 1
varY: resd 1
numbox: resd 1
numcheck: resd 1
needcheck: resd 1

section .text
global main
main:
	call io_get_udec
	mov dword [varN], eax

	call io_get_udec
	mov dword [varM], eax

	call io_get_udec
	mov dword [varK], eax

	call io_get_udec
	mov dword [varD], eax

	call io_get_udec
	mov dword [varX], eax

	call io_get_udec
	mov dword [varY], eax
	
	mov eax, dword [varN]
	mov ebx, dword [varM]
	mov ecx, dword [varK]
	mul ebx
	mul ecx
	mov ebx, dword [varD]
	div ebx
	mov ecx, 1
	sub edx, ecx
	shr edx, 31
	neg edx
	add edx, ecx
	add eax, edx
	mov dword [numbox], eax

	mov eax, dword [varX]
	mov ebx, 6
	sub eax, ebx
	shr eax, 31
	neg eax
	add eax, ecx
	mov dword [needcheck], eax

	mov eax, dword [numbox]
	sub eax, ecx
	mov ebx, 3
	div ebx
	mov ebx, dword [needcheck]
	mul ebx

	mov ebx, dword [numbox]
	sub ebx, eax
	mov eax, ebx
	mov ebx, dword [needcheck]
	sub eax, ebx

	call io_print_dec


	xor eax, eax 
	ret