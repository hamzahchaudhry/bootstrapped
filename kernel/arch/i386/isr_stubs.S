.section .text
.code32
.extern exception_handler
.globl isr_stub_table

.macro ISR_NOERR n
	.globl isr_stub_\n
isr_stub_\n:
	call exception_handler
	iret
.endm

.macro ISR_ERR n
	.globl isr_stub_\n
isr_stub_\n:
	call exception_handler
	add $4, %esp /* discard error code */
	iret
.endm

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR 8
ISR_NOERR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR 17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_ERR 30
ISR_NOERR 31

isr_stub_table:
	.long isr_stub_0
	.long isr_stub_1
	.long isr_stub_2
	.long isr_stub_3
	.long isr_stub_4
	.long isr_stub_5
	.long isr_stub_6
	.long isr_stub_7
	.long isr_stub_8
	.long isr_stub_9
	.long isr_stub_10
	.long isr_stub_11
	.long isr_stub_12
	.long isr_stub_13
	.long isr_stub_14
	.long isr_stub_15
	.long isr_stub_16
	.long isr_stub_17
	.long isr_stub_18
	.long isr_stub_19
	.long isr_stub_20
	.long isr_stub_21
	.long isr_stub_22
	.long isr_stub_23
	.long isr_stub_24
	.long isr_stub_25
	.long isr_stub_26
	.long isr_stub_27
	.long isr_stub_28
	.long isr_stub_29
	.long isr_stub_30
	.long isr_stub_31

.extern keyboard_irq_handler
.globl irq_stub_1
irq_stub_1:
	pusha /* preserve all general regs */
	call keyboard_irq_handler
	popa	/* restore regs */
	iret	/* return to interrupted code */
