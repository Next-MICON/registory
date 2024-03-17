#include "custom_ops.S"

.global irq
.global main

///////////////////////////////////////

.section .data

// Memory for IRQ Handler
irq_regs:
	.fill 32,4

	.fill 128,4
irq_stack:

///////////////////////////////////////

.section .text

// Reset
reset_vec:
	//picorv32_waitirq_insn(zero)
	//picorv32_maskirq_insn(zero, zero)
	j start

// Interrupt Handler
.balign 16 // picorv32.PROGADDR_IRQ
irq_vec:

	// 0. Context at this point
	//      pc = 0x0500_0010
	//      q0 = interrupt return address
	//      q1 = irq bits

	// 1. Stash registors
	picorv32_setq_insn(q2, x1) // q2 <= x1 (ra)
	picorv32_setq_insn(q3, x2) // q3 <= x2 (sp)

	lui  x1,     %hi(irq_regs)
	addi x1, x1, %lo(irq_regs)

    picorv32_getq_insn(x2, q0)
	sw x2,   0*4(x1) // irq_regs + 00 <= interrut return address

	picorv32_getq_insn(x2, q2)
	sw x2,   1*4(x1) // irq_regs + 04 <= original x1 (ra)

	picorv32_getq_insn(x2, q3)
	sw x2,   2*4(x1) // irq_regs + 08 <= original x2 (sp)

	sw x3,   3*4(x1) // irq_regs + 0C <=  x3
	sw x4,   4*4(x1) // irq_regs + 10 <=  x4
	sw x5,   5*4(x1) // irq_regs + 14 <=  x5
	sw x6,   6*4(x1) // irq_regs + 18 <=  x6
	sw x7,   7*4(x1) // irq_regs + 1C <=  x7
	sw x8,   8*4(x1) // irq_regs + 20 <=  x8
	sw x9,   9*4(x1) // irq_regs + 24 <=  x9
	sw x10, 10*4(x1) // irq_regs + 28 <= x10
	sw x11, 11*4(x1) // irq_regs + 2C <= x11
	sw x12, 12*4(x1) // irq_regs + 30 <= x12
	sw x13, 13*4(x1) // irq_regs + 34 <= x13
	sw x14, 14*4(x1) // irq_regs + 38 <= x14
	sw x15, 15*4(x1) // irq_regs + 3C <= x15
	sw x16, 16*4(x1) // irq_regs + 40 <= x16
	sw x17, 17*4(x1) // irq_regs + 44 <= x17
	sw x18, 18*4(x1) // irq_regs + 48 <= x18
	sw x19, 19*4(x1) // irq_regs + 4C <= x19
	sw x20, 20*4(x1) // irq_regs + 50 <= x20
	sw x21, 21*4(x1) // irq_regs + 54 <= x21
	sw x22, 22*4(x1) // irq_regs + 58 <= x22
	sw x23, 23*4(x1) // irq_regs + 5C <= x23
	sw x24, 24*4(x1) // irq_regs + 60 <= x24
	sw x25, 25*4(x1) // irq_regs + 64 <= x25
	sw x26, 26*4(x1) // irq_regs + 68 <= x26
	sw x27, 27*4(x1) // irq_regs + 6C <= x27
	sw x28, 28*4(x1) // irq_regs + 70 <= x28
	sw x29, 29*4(x1) // irq_regs + 74 <= x29
	sw x30, 30*4(x1) // irq_regs + 78 <= x30
	sw x31, 31*4(x1) // irq_regs + 7C <= x31

	// 2. Call irq function
	//   arg(a0) = pointer to saved context
	//   arg(a1) = interrupt bitmask
	//   ret(a0) = pointer to new context

	// irq function use irq_stack
	lui  sp,     %hi(irq_stack)
	addi sp, sp, %lo(irq_stack)

	// set arg0
	lui  a0,     %hi(irq_regs)
	addi a0, a0, %lo(irq_regs)

	// set arg1
	picorv32_getq_insn(a1, q1)

	jal ra, irq

	// 3. Restore context

	mv x1, a0 // pointer to context
	
	lw x2,   0*4(x1)
	picorv32_setq_insn(q0, x2)

	lw x2,   1*4(x1)
	picorv32_setq_insn(q2, x2)

	lw x2,   2*4(x1)
	picorv32_setq_insn(q3, x2)

	lw x3,   3*4(x1)
	lw x4,   4*4(x1)
	lw x5,   5*4(x1)
	lw x6,   6*4(x1)
	lw x7,   7*4(x1)
	lw x8,   8*4(x1)
	lw x9,   9*4(x1)
	lw x10, 10*4(x1)
	lw x11, 11*4(x1)
	lw x12, 12*4(x1)
	lw x13, 13*4(x1)
	lw x14, 14*4(x1)
	lw x15, 15*4(x1)
	lw x16, 16*4(x1)
	lw x17, 17*4(x1)
	lw x18, 18*4(x1)
	lw x19, 19*4(x1)
	lw x20, 20*4(x1)
	lw x21, 21*4(x1)
	lw x22, 22*4(x1)
	lw x23, 23*4(x1)
	lw x24, 24*4(x1)
	lw x25, 25*4(x1)
	lw x26, 26*4(x1)
	lw x27, 27*4(x1)
	lw x28, 28*4(x1)
	lw x29, 29*4(x1)
	lw x30, 30*4(x1)
	lw x31, 31*4(x1)

	picorv32_getq_insn(x1, q2)
	picorv32_getq_insn(x2, q3)

	//4. Return from interruption
	picorv32_retirq_insn()

// Main program
start:
	// 1. Initialize all registers to 0
	addi x1, zero, 0
	// x2 = sp is initialize to picorv32.STACKADDR
	addi x3, zero, 0
	addi x4, zero, 0
	addi x5, zero, 0
	addi x6, zero, 0
	addi x7, zero, 0
	addi x8, zero, 0
	addi x9, zero, 0
	addi x10, zero, 0
	addi x11, zero, 0
	addi x12, zero, 0
	addi x13, zero, 0
	addi x14, zero, 0
	addi x15, zero, 0
	addi x16, zero, 0
	addi x17, zero, 0
	addi x18, zero, 0
	addi x19, zero, 0
	addi x20, zero, 0
	addi x21, zero, 0
	addi x22, zero, 0
	addi x23, zero, 0
	addi x24, zero, 0
	addi x25, zero, 0
	addi x26, zero, 0
	addi x27, zero, 0
	addi x28, zero, 0
	addi x29, zero, 0
	addi x30, zero, 0
	addi x31, zero, 0

	// 2. Initialize BSS Section to zero
	init_bss:
		la a0, _bss_start
		la a1, _bss_end
	init_bss_loop:
		beq a0, a1, init_bss_done
		sw zero, 0(a0)
		add a0, a0, 4
		j init_bss_loop
	init_bss_done:

	// 3. Copy Data Section
	init_data:
		la a0, _data_start
		la a1, _data_end
		la a2, _init_data_start
	init_data_loop:
		bge a0, a1, init_data_done
		lw a3, 0(a2)
		sw a3, 0(a0)
		addi a2, a2, 4
		addi a0, a0, 4
		j init_data_loop
	init_data_done:

	// 4. Run static initializer
	init_static:
		la a0, _init_array_start
		la a1, _init_array_end
		addi sp,sp,-4
	init_static_loop:
		beq a0, a1, init_static_done
		lw  a3, 0(a0)
		add a0, a0, 4
		sw  a0, 0(sp)
		jalr a3
		lw a0, 0(sp)
		j init_static_loop
	init_static_done:
		addi sp, sp, 4

	// 5. Main function
	call main

	// TODO: Shutdown

loop:
	j loop
