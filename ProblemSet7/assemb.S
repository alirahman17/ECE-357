.global _start

_start:
	mov     $500, %rax
	mov     $1, %rdi
	mov     $msg, %rsi
	mov     $13, %rdx
	syscall

	mov     $60, %rax
	mov     $0, %rdi
	syscall
	msg:
	.ascii  "Test Output!\n"
