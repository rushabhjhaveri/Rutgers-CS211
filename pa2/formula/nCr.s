	.file	"nCr.c"
	.text
	.globl	_Factorial
	.def	_Factorial;	.scl	2;	.type	32;	.endef
_Factorial:
LFB17:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	cmpl	$0, 8(%ebp)
	je	L2
	cmpl	$1, 8(%ebp)
	jne	L3
L2:
	movl	$1, %eax
	jmp	L4
L3:
	movl	8(%ebp), %eax
	subl	$1, %eax
	movl	%eax, (%esp)
	call	_Factorial
	imull	8(%ebp), %eax
L4:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE17:
	.globl	_nCr
	.def	_nCr;	.scl	2;	.type	32;	.endef
_nCr:
LFB18:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%esi
	pushl	%ebx
	subl	$32, %esp
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	call	_Factorial
	movl	%eax, %ebx
	movl	12(%ebp), %eax
	movl	%eax, (%esp)
	call	_Factorial
	movl	%eax, %esi
	movl	8(%ebp), %eax
	subl	12(%ebp), %eax
	movl	%eax, (%esp)
	call	_Factorial
	movl	%esi, %edx
	imull	%eax, %edx
	movl	%edx, %ecx
	movl	%ebx, %eax
	cltd
	idivl	%ecx
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	addl	$32, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE18:
	.ident	"GCC: (GNU) 5.3.0"
