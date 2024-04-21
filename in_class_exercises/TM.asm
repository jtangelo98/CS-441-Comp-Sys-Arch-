alpha 'a'
alpha 'b'
alpha 'A'
alpha 'B'
alpha '$'
!1 
	cmp BLANK
	je !1_BLANK
	right
	jmp !2
!1_BLANK
	stop
!2
	cmp BLANK
	jne !2_^BLANK
	cmp BLANK
	je !2_BLANK
!2_^BLANK
	right
	jmp !2
!2_BLANK
	left
	jmp !3
!3
	cmp 'A'
	OR  'B'
	je !3_AB
	cmp 'a'
	je !3_a
	cmp 'b'
	je !3_b
	cmp BLANK
	je !3_BLANK
!3_AB
	left
	jmp !3
!3_a
	draw 'A'
	right
	jmp !4
!3_b
	draw 'B'
	right
	jmp !5
!3_BLANK
	right
	jmp !6
!4
	cmp BLANK
	jne !4_^BLANK
	cmp BLANK
	je !4_BLANK
!4_^BLANK
	right
	jmp !4
!4_BLANK
	draw 'A'
	left
	jmp !3
!5
	cmp BLANK
	jne !5_^BLANK
	cmp BLANK
	je !5_BLANK
!5_^BLANK
	right
	jmp !5
!5_BLANK
	draw 'B'
	left
	jmp !3
!6 
	cmp 'A'
	je !6_A
	cmp 'B'
	je !6_B
	cmp BLANK
	je !6_BLANK
!6_A
	draw 'a'
	right
	jmp !6
!6_B
	draw 'b'
	right
	jmp !6
!6_BLANK
	left
	jmp !7
!7
	cmp BLANK
	jne !7_^BLANK
	cmp BLANK
	je !7_BLANK
!7_^BLANK
	left
	jmp !7
!7_BLANK
	draw '$'
	right
	stop