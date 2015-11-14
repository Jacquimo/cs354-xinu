
	.text
	.globl	host2netl

host2netl:
	# save and update stack information
	pushl %ebp
	movl %esp, %ebp

	# do the work
	movl 8(%ebp), %eax		# get the argument value from the stack
	bswapl %eax				# swap the bit values of the argument

	# return
	popl %ebp
	ret

