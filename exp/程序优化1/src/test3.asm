global t1
global t2
        section .text
t1:
	mfence
	rdtsc
	mov r8, rdx
	sal r8, 32
	or	r8, rax
	mfence

	%rep    10
	mov rax, [rsi]
	add rax, [rdi]
	mov [rdi], rax
	add rax, [rsi]
	mov [rdi], rax
	%endrep

	mfence
	rdtsc
	mov r9, rdx
	sal r9, 32
	or	r9, rax
	mfence

	sub r9, r8
	mov rax, r9

	ret

t2:
	mfence
	rdtsc
	mov r8, rdx
	sal r8, 32
	or	r8, rax
	mfence

	%rep    10
	mov rax, [rsi]
	add rax, rax
	add [rdi], rax
	%endrep

	mfence
	rdtsc
	mov r9, rdx
	sal r9, 32
	or	r9, rax
	mfence

	sub r9, r8
	mov rax, r9

	ret