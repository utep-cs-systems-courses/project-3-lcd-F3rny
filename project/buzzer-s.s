	.arch msp430g2553
	.p2align 2
	.text

	.global buzzer_init
	.global buzzer_set_period
	
	.extern P2SEL2
	.extern P2SEL
	.extern P2DIR

	.extern CCR0
	.extern CCR1

	.extern timerAUpmode
	
	
BIT6:	.word 64 		; BIT6 == 64
BIT7:	.word 128 		; BIT7 == 128
Both:	.word 192		; BIT6 | BIT7
	
	
buzzer_init:

	call #timerAUpmode

	bic &Both , &P2SEL2 	; P2SEL2 &= ~(BIT6 | BIT7);
	bic &BIT7 , &P2SEL	; P2SEL &= ~BIT7; 
	mov &BIT6 , &P2DIR
	ret
buzzer_set_period:

	mov r12 , &CCR0
	rra r12
	mov r12 , &CCR1
	ret
