

# sum(n), a0 -> arg; a0 -> ret
# first step:
sum_to:
	mv t0 a0 	# a0 : arg/ret 0 -> t0: tmp 0, here t0 is set as input `n`


for:
	addi t0, t0, -1
	add  a0, a0, t0	# do sum
	bnez t0, for 
	ret

