	.text
	.file	"tmp.c"
	.globl	signum                          # -- Begin function signum
	.p2align	4, 0x90
	.type	signum,@function
signum:                                 # @signum
	.cfi_startproc
# %bb.0:
	xorl	%ecx, %ecx
	testl	%edi, %edi
	sete	%cl
	negl	%ecx
	testl	%edi, %edi
	movl	$1, %eax
	cmovlel	%ecx, %eax
	retq
.Lfunc_end0:
	.size	signum, .Lfunc_end0-signum
	.cfi_endproc
                                        # -- End function
	.ident	"Ubuntu clang version 12.0.0-3ubuntu1~20.04.4"
	.section	".note.GNU-stack","",@progbits
	.addrsig
