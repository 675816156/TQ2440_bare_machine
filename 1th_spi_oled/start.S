.global _start
_start:
	b	reset
	ldr	pc, _undefined_instruction
	ldr	pc, _software_interrupt
	ldr	pc, _prefetch_abort
	ldr	pc, _data_abort
	ldr	pc, _not_used
	ldr	pc, _irq
	ldr	pc, _fiq

.global _undefined_instruction
.global _software_interrupt
.global _prefetch_abort
.global _data_abort
.global _not_used
.global _irq
.global _fiq

_undefined_instruction:	.word undefined_instruction
_software_interrupt:	.word software_interrupt
_prefetch_abort:	.word prefetch_abort
_data_abort:		.word data_abort
_not_used:		.word not_used
_irq:			.word irq
_fiq:			.word fiq

undefined_instruction:
software_interrupt:
prefetch_abort:
data_abort:
not_used:
irq:
fiq:
1:
	bl	1b
.global reset
.global main
reset:
	/* set cpu to SVC32 mode */
	mrs	r0, cpsr
	bic	r0, r0, #0x1f
	orr	r0, r0, #0xd3
	msr	cpsr, r0
	/* enable icache */
	mrc p15, 0, r0, c1, c0, 0
	orr r0, r0, #(1 << 12)
	mcr p15, 0, r0, c1, c0, 0
	/* turn off watch dog */
	ldr	r0, =0x53000000
	mov	r1, #0x0
	str	r1, [r0]
	/* disable IRQs */
	ldr	r0, =0x4A000008
	mov	r1, #0xFFFFFFFF
	str	r1, [r0]
	/* disable SUB IRQs */
	ldr	r0, =0x4A00001C
	ldr	r1, =0x7FFF
	str	r1, [r0]

#define MPLLCON 0x4C000004
#define UPLLCON 0x4C000008
#define CAMDIVN 0x4C000018
#define CLKDIVN	0x4C000014

/* clock config */
#define HDIVN 2
#define PDIVN 1
#if 0
#define M_MDIV	125
#else
#define M_MDIV  92
#endif
#define M_PDIV	1
#define M_SDIV	1
#define DIVN_UPLL 1
#define U_M_MDIV	56
#define U_M_PDIV	2
#define U_M_SDIV	1
	/* FCLK : HCLK : PCLK = 1:4:8 */
	ldr r0, =CLKDIVN
	ldr r1, =(DIVN_UPLL << 3)|(HDIVN<<1)|(PDIVN)
	str r1, [r0]
	ldr r0, =CAMDIVN
	mov r1, #0x0
	str r1, [r0]

	mrc p15, 0, r0, c1, c0, 0
	orr r0, r0, #0xc0000000
	mcr p15, 0, r0, c1, c0, 0

	ldr r0, =UPLLCON
	ldr r1, =(U_M_MDIV << 12)|(U_M_PDIV << 4)|(U_M_SDIV)
	str r1, [r0]
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	ldr r0, =MPLLCON
	ldr r1, =(M_MDIV << 12)|(M_PDIV << 4)|(M_SDIV)
	str r1, [r0]

	ldr sp, =4096
	ldr pc, =main
