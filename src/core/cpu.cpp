
#include "cpu.hpp"
#include "../utils/logger.hpp"


// table for opcodes base length in bytes
u8 opcode_bytes[256] = {
// _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_A,_B,_C,_D,_E,_F
	1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 1, 1, // 0_
	4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 1_
    2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 2_
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 3_
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 4_
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 5_
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 6_
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 7_
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 8_
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 9_
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // A_
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, // B_
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, // C_
	3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // D_
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // E_
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // F_
};

// table for opcodes base duration in cycles
u8 opcode_cycles[256] = {
// X0,X1,X2,X3,X4,X5,X6,X7,X8,X9,XA,XB,XC,XD,XE,XF
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0X
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 1X
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 2X
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 3X
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 4X
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 5X
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 6X
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 7X
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 8X
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 9X
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // AX
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // BX
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // CX
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // DX
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // EX
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // FX
};

// current opcode context
// move this to cpu?
struct opcode_atrib {
	// fetch context
	u8 byte[4];
	// decode context
	u8 type;
	// execute context
	u8 bytesUsed;
	u8 cyclesUsed;
};
opcode_atrib opcode;

// instructions helpers

int reg_no_inc_dec(int r) {
	// no inc or dec
	if (r <= REG_Z_ind)
		return r;
	else if (r == REG_A_ind_inc_b || r == REG_A_ind_dec_b ||
             r == REG_A_ind_inc || r == REG_A_ind_dec)
		return REG_A_ind;
	else if (r == REG_X_ind_inc_b || r == REG_X_ind_dec_b ||
             r == REG_X_ind_inc || r == REG_X_ind_dec)
		return REG_X_ind;
	else
		return r;
}

int reg_a_or_x(int r) {
	if (r == REG_A_ind_inc_b || r == REG_A_ind_dec_b ||
	    r == REG_A_ind_inc || r == REG_A_ind_dec)
		return REG_A_ind;
	else if (r == REG_X_ind_inc_b || r == REG_X_ind_dec_b ||
	    r == REG_X_ind_inc || r == REG_X_ind_dec)
		return REG_X_ind;
	else
		return r;
}

int reg_a_x_step(int r) {
	int step = 1;
	if (r >= REG_A_ind_inc && r <= REG_X_ind_dec)
		step = 3;
	if (r == REG_A_ind_dec || r == REG_X_ind_dec ||
        r == REG_X_ind_dec_b || r == REG_X_ind_dec_b)
		step = -step;
	return step;
}

// instructions implementations

void instr_nop(cpu* c) {
	// nothing happens
}

void instr_stub(cpu* c) {

}

void instr_uop(cpu* c) {
	// unimplemented opcode
	c->error("unimplemented opcode");
	c->exception();
}

void instr_mov_r_ccc(cpu* c) {
	u32 operand1, operand2;
	operand1 = opcode.byte[0] & 0x0F;
	operand2 = (opcode.byte[1] << 16) | (opcode.byte[2] << 8) | opcode.byte[3];
	c->regs.gpr[operand1].r = operand2;
}

void instr_mov_r_c(cpu* c) {
	u32 operand1, operand2;
	operand1 = opcode.byte[0] & 0x0F;
	operand2 = opcode.byte[1];
	c->regs.gpr[operand1].r = (operand2 & 0xFF) | (c->regs.gpr[operand1].r & 0xFFFF00);
}

void instr_mov_rr_rr_w(cpu* c) {
	u32 operand1, operand2, value;
	operand1 = (opcode.byte[1] >> 4) & 0x0F;
	operand2 = opcode.byte[1] & 0x0F;
	value = c->read_reg_mode1(operand2);
	c->write_reg_mode1(operand1, value);
}

void instr_mov_rr_rr_b(cpu* c) {
	u32 operand1, operand2, value;
	operand1 = (opcode.byte[1] >> 4) & 0x0F;
	operand2 = opcode.byte[1] & 0x0F;
	value = c->read_reg_mode1(operand2) & 0xFF;
	value |= c->read_reg_mode1(reg_no_inc_dec(operand1)) & 0xFFFF00;
	c->write_reg_mode1(operand1, value);
}

void instr_mov_rs_rs(cpu* c) {
	u32  operand1, operand2;
	operand1 = (opcode.byte[1] >> 4) & 0x0F;
	operand2 = opcode.byte[1] & 0x0F;
	c->regs.reg[operand1].r = c->regs.reg[operand2].r;
}

#define STACK_REGS_NUM 8
u32 masks[STACK_REGS_NUM] = {RS_A, RS_B, RS_C, RS_X, RS_Y, RS_Z, RS_FS, RS_SP};

void instr_push(cpu* c) {
	u32 operand1 = opcode.byte[1];
	for (int reg_index = 0; reg_index < STACK_REGS_NUM; reg_index++) {
		if (operand1 & masks[reg_index]) {
			c->push_word(c->regs.reg[reg_index].r);
		}
	}

}

void instr_pop(cpu* c) {
	u32 operand1 = opcode.byte[1];
	for (int reg_index = STACK_REGS_NUM - 1; reg_index >= 0; reg_index--) {
		if (operand1 & masks[reg_index]) {
			c->regs.reg[reg_index].r = c->pop_word();
		}
	}
}

void instr_inw(cpu* c) {
	u32 operand1 = (opcode.byte[1] >> 4) & 0x0F;
	u32 operand2 = opcode.byte[1] & 0x0F;
	c->write_reg_mode1(operand1, c->in_word(operand2));
}

void instr_outw(cpu* c) {
	u32 operand1 = (opcode.byte[1] >> 4) & 0x0F;
	u32 operand2 = opcode.byte[1] & 0x0F;
	c->out_word(c->read_reg_mode1(operand1), c->read_reg_mode1(operand2));
}

void instr_inb(cpu* c) {
	u32 operand1 = (opcode.byte[1] >> 4) & 0x0F;
	u32 operand2 = opcode.byte[1] & 0x0F;
	c->write_reg_mode1(operand1, c->in_byte(operand2));
}

void instr_outb(cpu* c) {
	u32 operand1 = (opcode.byte[1] >> 4) & 0x0F;
	u32 operand2 = opcode.byte[1] & 0x0F;
	c->out_byte(c->read_reg_mode1(operand1), c->read_reg_mode1(operand2));
}

#define setFlag(flag, mask, cond) flag |= (cond) ? (mask) : 0
#define setZF(flag, cond) setFlag(flag, Z_MASK, cond)
#define setCF(flag, cond) setFlag(flag, C_MASK, cond)
#define setNF(flag, cond) setFlag(flag, N_MASK, cond)
#define setVF(flag, cond) setFlag(flag, V_MASK, cond)

// TODO test all this crap, probably some errors

void word_add(u24* r, u24* f, u24 a, u24 b) {
	u24 carry = 0;
	u24 overflow = ~(a ^ b) & (a ^ *r);
	*r = a + b + carry;
	setZF(*f, *r == 0);
	setNF(*f, *r >> 23);
	setVF(*f, (1 << 23) & (overflow));
	setCF(*f, (1 << 23) & (overflow ^ a ^ b ^ *r));
}
void word_adc(u24* r, u24* f, u24 a, u24 b) {
	u24 carry = (*f & C_MASK) ? 1 : 0;
	u24 overflow = ~(a ^ b) & (a ^ *r);
	*r = a + b + carry;
	setZF(*f, *r == 0);
	setNF(*f, *r >> 23);
	setVF(*f, (1 << 23) & (overflow));
	setCF(*f, (1 << 23) & (overflow ^ a ^ b ^ *r));
}
void word_sub(u24* r, u24* f, u24 a, u24 b) {
	word_add(r, f, a, b);
}
void word_sbc(u24* r, u24* f, u24 a, u24 b) {
	word_adc(r, f, a, b);
}
void word_mul(u24* r, u24* f, u24 a, u24 b) {
	*r = a * b;
	// TODO flags
}
void word_div(u24* r, u24* f, u24 a, u24 b) {
	*r = a / b;
	// TODO flags
}
void word_rem(u24* r, u24* f, u24 a, u24 b) {
	*r = a % b;
	// TODO flags
}
void word_and(u24* r, u24* f, u24 a, u24 b) {
	*r = a & b;
	setZF(*f, *r == 0);
	setNF(*f, *r >> 23);
	setVF(*f, 0);
	setCF(*f, 0);
}
void word_or(u24* r, u24* f, u24 a, u24 b) {
	*r = a | b;
	setZF(*f, *r == 0);
	setNF(*f, *r >> 23);
	setVF(*f, 0);
	setCF(*f, 0);
}
void word_xor(u24* r, u24* f, u24 a, u24 b) {
	*r = a ^ b;
	setZF(*f, *r == 0);
	setNF(*f, *r >> 23);
	setVF(*f, 0);
	setCF(*f, 0);
}
void word_shr(u24* r, u24* f, u24 a, u24 b) {
	b = b & 0x0F;
	*r = a >> b;
	setZF(*f, *r == 0);
	setNF(*f, *r >> 23);
	setCF(*f, (a >> (b-1)) & 0x01);
	setVF(*f, 0);

}
void word_shl(u24* r, u24* f, u24 a, u24 b) {
	b = b & 0x0F;
	*r = a << b;
	setZF(*f, *r == 0);
	setNF(*f, *r >> 23);
	setCF(*f, (a << (b-1)) & 0x01);
	setVF(*f, 0);
}
void word_ror(u24* r, u24* f, u24 a, u24 b) {
	b = b & 0x0F;
	*r = ((a >> b) & 0xFFFFFF) | ((a << b) & (0xFFFFFF >> (16 - b)));
	setZF(*f, *r == 0);
	setNF(*f, *r >> 23);
	setCF(*f, 0);
	setVF(*f, 0);
}
void word_rol(u24* r, u24* f, u24 a, u24 b) {
	b = b & 0x0F;
	*r = ((a << b) & 0xFFFFFF) | ((a >> (16-b)) & (0xFFFFFF >> (16 - b)));
	setZF(*f, *r == 0);
	setNF(*f, *r >> 23);
	setCF(*f, 0);
	setVF(*f, 0);
}
void word_rcr(u24* r, u24* f, u24 a, u24 b) {
	u24 c = (*f >> 0) & 0x01;
	b = b & 0x01;
	*r = (a >> b) | c;
	setZF(*f, *r == 0);
	setNF(*f, *r >> 23);
	setCF(*f, (a >> (b-1)) & 0x01);
	setVF(*f, 0);
}
void word_rcl(u24* r, u24* f, u24 a, u24 b) {
	u24 c = (*f << 23) & 0x80;
	b = b & 0x01;
	*r = c | (a << b);
	setZF(*f, *r == 0);
	setNF(*f, *r >> 23);
	setCF(*f, (a << (b-1)) & 0x01);
	setVF(*f, 0);
}

void word_neg(u24* r, u24* f, u24 a) {
	*r = -a;
}
void word_not(u24* r, u24* f, u24 a) {
	*r = ~a;
}
void word_abs(u24* r, u24* f, u24 a) {
	if (*r & 0x7FFFFF)
		*r = -a;
}


typedef void (*alu2_callback)(u24*, u24*, u24, u24);
typedef void (*alu1_callback)(u24*, u24*, u24);

alu2_callback alu2_op[] = {
	&word_add, &word_sub, &word_mul, &word_div, &word_rem,
	&word_and, &word_or, &word_xor,
	&word_shr, &word_shl,
	&word_ror, &word_rol, &word_rcr, &word_rcl,
};

alu1_callback alu1_op[] = {
	&word_neg, &word_not, &word_abs,
};

void instr_alu_r_rr_rr(cpu* c) {
	u32 operation, operand1, operand2, operand3;
	u24 result, a, b, newflags;
	operation = (opcode.byte[1] >> 4) & 0x0F;
	operand1 = opcode.byte[1] & 0x0F;
	operand2 = (opcode.byte[2] >> 4) & 0x0F;
	operand3 = opcode.byte[2] & 0x0F;
	newflags = c->regs.fs;
	a = c->read_reg_mode1(operand2);
	b = c->read_reg_mode1(operand3);
	alu2_op[operation](&result, &newflags, a, b);
	if (operand1 < 6) {
		c->write_reg_mode1(operand1, result);
		c->regs.fs = newflags;
	} else if (operand1 == 6) {
		c->regs.fs = newflags;
	} else if(operand1 == 7) {
		c->write_reg_mode1(operand2, result);
	}
}

void instr_alu_r_rr(cpu* c) {
	u32 operation, operand1, operand2;
	u24 result, a, newflags;
	operation = opcode.byte[0] - 0xAA;
	operand1 = (opcode.byte[1] >> 4) & 0x0F;
	operand2 = opcode.byte[1] & 0x0F;
	a = c->read_reg_mode1(a);
	alu1_op[operation](&result, &newflags, a);
	if (operand1 < 6) {
		c->write_reg_mode1(operand1, result);
		c->regs.fs = newflags;
	} else if (operand1 == 6) {
		c->regs.fs = newflags;
	} else if(operand1 == 7) {
		c->write_reg_mode1(operand2, result);
	}
}

void instr_jc_ff_ccc(cpu* c) {
	u32 operation, operand1, operand2;
	operation = opcode.type < 0xC0 ? JMP : CALL;
	operand1 = opcode.byte[0] & 0x0F;
	operand2 = (opcode.byte[1] << 16) | (opcode.byte[2] << 8) | opcode.byte[3];
	if (c->conditions(operand1)) {
		c->regs.cp += opcode.bytesUsed;
		if (operation == CALL) {
			c->push_word(c->regs.cp);
		}
		c->regs.cp = operand2;
		opcode.bytesUsed = 0;
	}
}

void instr_jc_ff_cc(cpu* c) {
	u32 operation, operand1, operand2;
	operation = opcode.type < 0xC0 ? JMP : CALL;
	operand1 = opcode.byte[1] & 0xF0;
	operand2 = ((opcode.byte[1] & 0x0F) << 8) | opcode.byte[2];
	if (c->conditions(operand1)) {
		c->regs.cp += opcode.bytesUsed;
		if (operation == CALL) {
			c->push_word(c->regs.cp);
		}
		if (operand2 & 0x7FFFFF) {
			c->regs.cp -= 0x1000000 - operand2;
		} else {
			c->regs.cp += operand2;
		}
		opcode.bytesUsed = 0;
	}
}

void instr_jc_ff_r(cpu* c) {
	u32 operation, operand1, operand2;
	operation = opcode.type < 0xC0 ? JMP : CALL;
	operand1 = (opcode.byte[1] >> 4) & 0x0F;
	operand2 = opcode.byte[1] & 0x0F;
	if (c->conditions(operand1)) {
		if (operation == CALL) {
			c->push_word(c->regs.cp);
		}
		c->regs.cp = c->read_reg_mode1(operand2);
		opcode.bytesUsed = 0;
	}
}

void instr_ret_ff(cpu* c) {
	u32 operand1;
	operand1 = (opcode.byte[1] & 0xF0) >> 4;
	if (c->conditions(operand1)) {
		c->regs.cp = c->pop_word();
		opcode.bytesUsed = 0;
	}
}

void instr_fint(cpu* c) {
	c->interrupt();
}

void instr_fexc(cpu* c) {
	c->exception();
}

void instr_rets(cpu* c) {
	c->regs.cp = c->pop_word();
	c->regs.f.i = 0;
	c->regs.f.e = 0;
	opcode.bytesUsed = 0;
}

void instr_dint(cpu* c) {
	c->regs.f.ie = 0;
}

void instr_eint(cpu* c) {
	c->regs.f.ee = 0;
}

void instr_slp(cpu* c) {
	c->sleep = true;
	// alternative implementation: jump back to slp until interrupt is made between
}

// table for instruction set callbacks
instruction_callback isa_table[256] = {
	// X0, X1, X2, X3, X4, X5, X6, X7
	// X8, X9, XA, XB, XC, XD, XE, XF
	instr_nop, instr_mov_rr_rr_w, instr_mov_rr_rr_b, instr_mov_rs_rs, instr_push, instr_pop, instr_inw, instr_outw, // 0X
	instr_inb, instr_outb, instr_alu_r_rr_rr, instr_alu_r_rr_rr, instr_alu_r_rr, instr_alu_r_rr, instr_uop, instr_uop,
	instr_mov_r_ccc, instr_mov_r_ccc, instr_mov_r_ccc, instr_mov_r_ccc, instr_mov_r_ccc, instr_mov_r_ccc, instr_uop, instr_uop, // 1X
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop,
	instr_mov_r_c, instr_mov_r_c, instr_mov_r_c, instr_mov_r_c, instr_mov_r_c, instr_mov_r_c, instr_uop, instr_uop, // 2X
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop,
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, // 3X
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop,
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, // 4X
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop,
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, // 5X
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop,
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, // 6X
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop,
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, // 7X
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop,
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, // 8X
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop,
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, // 9X
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop,
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, // AX
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop,
	instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, // BX
	instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc,
	instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, // CX
	instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc, instr_jc_ff_ccc,
	instr_jc_ff_cc, instr_jc_ff_cc, instr_jc_ff_r, instr_jc_ff_r, instr_uop, instr_uop, instr_uop, instr_uop, // DX
	instr_uop, instr_uop, instr_uop, instr_ret_ff, instr_uop, instr_uop, instr_uop, instr_uop,
	instr_stub, instr_stub, instr_stub, instr_stub, instr_stub, instr_stub, instr_stub, instr_uop, // EX
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop,
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, // FX
	instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_uop, instr_nop,
};


cpu::cpu() {

	// reset everything to defaults
	for (int i = 0; i < NUM_REGISTERS; i++) {
		regs.reg[i].r = 0;
	}

	//    isa_table[0x00] = instr_stub;
	// opcode_bytes[0x00] = 1;
	//opcode_cycles[0x00] = 1;

	instrs.exec_table = isa_table;
	instrs.base_length = opcode_bytes;
	instrs.base_cycles = opcode_cycles;

	mem = nullptr;
	port = nullptr;

	clock_rate = 1000000;  // 1MHz or 2^24 = 1048576 Hz

	interrupt_rate = clock_rate / 60;

	ended = false;
	running = false;
	sleep = false;
	cyclesExecuted = 0;

}

cpu::~cpu() {

}


void cpu::error(std::string description) {
	logger.log(description);
}

u8 cpu::read_byte(u32 a) {
	a = a & 0xFFFFFF;
	return mem->read_byte(a);
}

void cpu::write_byte(u32 a, u8 v) {
	a = a & 0xFFFFFF;
	mem->write_byte(a, v);
}



u32 cpu::read_word(u32 a) {
	u32 value = cpu::read_byte(a);
	value |= cpu::read_byte(a + 1) << 8;
	value |= cpu::read_byte(a + 2) << 16;
	return value;
}

void cpu::write_word(u32 a, u32 v) {
	cpu::write_byte(a, v & 0xFF);
	cpu::write_byte(a + 1, (v >> 8) & 0xFF);
	cpu::write_byte(a + 2, (v >> 16) & 0xFF);
}

void cpu::push_word(u32 value) {
	regs.sp -= 3;
	cpu::write_word(regs.sp, value);
}

u32 cpu::pop_word() {
	u32 value = cpu::read_word(regs.sp);
	regs.sp += 3;
	return value;
}

u8 cpu::in_byte(u32 p) {
	return port->out_byte(p);
}

void cpu::out_byte(u32 p, u32 v) {
	port->in_byte(p, v);
}

u32 cpu::in_word(u32 p) {
	return port->out_word(p);
}

void cpu::out_word(u32 p, u32 v) {
	port->in_word(p, v);
}

void cpu::interrupt() {
	opcode.byte[0] = 0;
	opcode.byte[1] = 0;
	opcode.byte[2] = 0;
	opcode.byte[3] = 0;
	opcode.bytesUsed = 0;
	opcode.cyclesUsed = 1;
	cpu::push_word(regs.cp);
	regs.cp = regs.ip;
	regs.f.i = 1;
	cyclesExecuted += 1;
	cpu::sleep = false;
}

void cpu::exception() {
	opcode.byte[0] = 0;
	opcode.byte[1] = 0;
	opcode.byte[2] = 0;
	opcode.byte[3] = 0;
	opcode.bytesUsed = 0;
	opcode.cyclesUsed = 1;
	cpu::push_word(regs.cp);
	regs.cp = regs.ip;
	regs.f.e = 1;
	cyclesExecuted += 1;
	cpu::sleep = false;
}

u32 cpu::read_reg_mode1(u8 r) {
	if (r >= REG_A && r <= REG_Z)
		return regs.gpr[r].r;
	if (r >= REG_A_ind && r <= REG_Z_ind)
		return cpu::read_word(regs.gpr[r-REG_A_ind].r);
	if (r >= REG_A_ind_inc && r <= REG_X_ind_dec_b) {
		u32 value = cpu::read_word(regs.gpr[reg_a_or_x(r)].r);
		regs.gpr[reg_a_or_x(r)].r += reg_a_x_step(r);
		return value;
	} else {
		cpu::error("bad read");
		return 0;
	}
}

void cpu::write_reg_mode1(u8 r, u32 v) {
	if (r >= REG_A && r <= REG_Z)
		regs.gpr[r].r = v;
	if (r >= REG_A_ind && r <= REG_Z_ind)
		cpu::write_word(regs.gpr[r-REG_A_ind].r, v);
	if (r >= REG_A_ind_inc && r <= REG_X_ind_dec_b) {
		cpu::write_word(regs.gpr[reg_a_or_x(r)].r, v);
		regs.gpr[reg_a_or_x(r)].r += reg_a_x_step(r);
	}
	if (r > REG_X_ind_dec_b)
		cpu::error("bad write");
}

u8 cpu::conditions(u8 conditions) {
	bool zf = regs.f.z;
	bool cf = regs.f.c;
	bool nf = regs.f.n;
	bool vf = regs.f.v;

	switch (conditions) {
		case always:
			return 1;
		case eq:
			return zf;
		case ne:
			return !zf;
		case gt:
			return cf & !zf;
		case lt:
			return !cf & !zf;
		case ge:
			return cf | zf;
		case le:
			return (!cf) | zf;
		case a:
			return !zf && (nf == vf);
		case b:
			return nf != vf;
		case ae:
			return nf == vf;
		case be:
			return zf || (nf != vf);
		case never:
			return 0;
		default:
			cpu::error("invalid condition");
			return 0;
	}
}

void cpu::power() {
	reset();
}

void cpu::reset() {
	// reset registers
	for (int i = 0; i < NUM_REGISTERS; i++) {
		regs.reg[i].r = 0;
	}
	// reset state
	cyclesExecuted = 0;
}

// fetch current executing instruction
void cpu::fetch() {
	u32 cp = regs.cp;
	opcode.byte[0] = cpu::read_byte(cp);
	opcode.byte[1] = cpu::read_byte(cp + 1);
	opcode.byte[2] = cpu::read_byte(cp + 2);
	opcode.byte[3] = cpu::read_byte(cp + 3);
	//u32 value = opcode.byte[0] << 24;
	//value |= opcode.byte[1] << 16;
	//value |= opcode.byte[2] << 8;
	//value |= opcode.byte[3];
}

// decode current executing instruction
void cpu::decode() {
	opcode.type = opcode.byte[0];
}

// execute the opcode after decoding
void cpu::execute() {
	opcode.bytesUsed = instrs.base_length[opcode.type];
	opcode.cyclesUsed = instrs.base_cycles[opcode.type];
	instrs.exec_table[opcode.type](this);
	regs.cp += opcode.bytesUsed;
	cyclesExecuted += opcode.cyclesUsed;
}

void cpu::step() {
	bool interrupt_pending = false;
	bool exception_pending = false;
	if (!sleep && !interrupt_pending && !exception_pending) {
		cpu::fetch();
		cpu::decode();
		cpu::execute();
	} else if (regs.f.ie && interrupt_pending) {
		cpu::interrupt();
	} else if (regs.f.ee && exception_pending) {
		cpu::exception();
	} else if (sleep) {
		opcode.cyclesUsed = 1;
		cyclesExecuted += 1;
	}
}

u64 cpu::advance(u64 amount) {

	u64 cyclesRemaining = amount;
	u64 cyclesExceeded = 0;

	while (cyclesRemaining > 0) {
		cpu::step();
		if (cyclesRemaining > opcode.cyclesUsed) {
			cyclesRemaining -= opcode.cyclesUsed;
		} else {
			cyclesExceeded = opcode.cyclesUsed - cyclesRemaining;
			cyclesRemaining = 0;
		}
	}

	return cyclesExceeded;
}

void cpu::run() {
	ended = false;
	while (!ended) {
		if (running) {
			cpu::step();
		}
		// implement a run with limit and/or sync capabilities?
		//sync_delay();
		// implement with condition to end?
		//ended = end_cond(this);
	}
}

void cpu::endrun() {
	ended = true;
}

void cpu::resume() {
	running = true;
}

void cpu::pause() {
	running = false;
}
