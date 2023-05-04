	.arch msp430g2553
	.p2align 2
	.text

	.global collision
	.global lost

	.extern buzzer_set_period

collision:
	mov #4357 , r12
	call #buzzer_set_period
	mov #20000, r12
	jmp delay
	ret

lost:
	mov #8849 , r12
	call #buzzer_set_period
	mov #20000, r12
	jmp delay
	ret
	
delay:
	sub #1 , r12
	jnz delay

	mov #0, r12

	call #buzzer_set_period
	ret

	
