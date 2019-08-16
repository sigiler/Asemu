
#include "cpu.hpp"

#include "../utils/logger.hpp"
#include "../common/common_types.hpp"

// table for opcodes base length in bytes
const u8 opcode_bytes[256] = {
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
const u8 opcode_cycles[256] = {
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

// addressing modes

inline u32 cpu::read_reg_mode1_word(u8 r) {
	switch (r) {
		case REG_A:
		case REG_B:
		case REG_C:
		case REG_X:
		case REG_Y:
		case REG_Z:
			return regs.r[r];
		case REG_A_ind:
		case REG_B_ind:
		case REG_C_ind:
		case REG_X_ind:
		case REG_Y_ind:
		case REG_Z_ind:
			return cpu::read_word(regs.r[r-REG_A_ind]);
		case REG_A_ind_inc:
			regs.r[REG_A] += 3;
			return cpu::read_word(regs.r[REG_A]-3);
		case REG_X_ind_inc:
			regs.r[REG_A] += 3;
			return cpu::read_word(regs.r[REG_X]-3);
		case REG_A_ind_dec:
			regs.r[REG_X] -= 3;
			return cpu::read_word(regs.r[REG_A]+3);
		case REG_X_ind_dec:
			regs.r[REG_X] -= 3;
			return cpu::read_word(regs.r[REG_X]+3);
		default:
			cpu::error("invalid opcode decoding");
			return 0;
	}
}

void cpu::write_reg_mode1_word(const u8 r, const u32 v) {
	switch (r) {
		case REG_A:
		case REG_B:
		case REG_C:
		case REG_X:
		case REG_Y:
		case REG_Z:
			regs.r[r] = v;
			break;
		case REG_A_ind:
		case REG_B_ind:
		case REG_C_ind:
		case REG_X_ind:
		case REG_Y_ind:
		case REG_Z_ind:
			cpu::write_word(regs.r[r-REG_A_ind], v);
			break;
		case REG_A_ind_inc:
			cpu::write_word(regs.r[REG_A], v);
			regs.r[REG_A] += 3;
			break;
		case REG_X_ind_inc:
			cpu::write_word(regs.r[REG_X], v);
			regs.r[REG_A] += 3;
			break;
		case REG_A_ind_dec:
			cpu::write_word(regs.r[REG_A], v);
			regs.r[REG_X] -= 3;
			break;
		case REG_X_ind_dec:
			cpu::write_word(regs.r[REG_X], v);
			regs.r[REG_X] -= 3;
			break;
		default:
			cpu::error("invalid opcode decoding");
			break;
	}
}

u8 cpu::read_reg_mode1_byte(u8 r) {
	switch (r) {
		case REG_A:
		case REG_B:
		case REG_C:
		case REG_X:
		case REG_Y:
		case REG_Z:
			return regs.r[r] & 0xFF;
		case REG_A_ind:
		case REG_B_ind:
		case REG_C_ind:
		case REG_X_ind:
		case REG_Y_ind:
		case REG_Z_ind:
			return cpu::read_byte(regs.r[r-REG_A_ind]);
		case REG_A_ind_inc:
			regs.r[REG_A] += 1;
			return cpu::read_byte(regs.r[REG_A]-1);
		case REG_X_ind_inc:
			regs.r[REG_A] += 1;
			return cpu::read_byte(regs.r[REG_X]-1);
		case REG_A_ind_dec:
			regs.r[REG_X] -= 1;
			return cpu::read_byte(regs.r[REG_A]+1);
		case REG_X_ind_dec:
			regs.r[REG_X] -= 1;
			return  cpu::read_byte(regs.r[REG_X]+1);
		default:
			cpu::error("invalid opcode decoding");
			return 0;
	}
}

void cpu::write_reg_mode1_byte(u8 r, u8 v) {
	switch (r) {
		case REG_A:
		case REG_B:
		case REG_C:
		case REG_X:
		case REG_Y:
		case REG_Z:
			regs.r[r] = (regs.r[r] & 0xFFFF00) | v;
			break;
		case REG_A_ind:
		case REG_B_ind:
		case REG_C_ind:
		case REG_X_ind:
		case REG_Y_ind:
		case REG_Z_ind:
			cpu::write_byte(regs.r[r-REG_A_ind], v);
			break;
		case REG_A_ind_inc:
			cpu::write_byte(regs.r[REG_A], v);
			regs.r[REG_A] += 1;
			break;
		case REG_X_ind_inc:
			cpu::write_byte(regs.r[REG_X], v);
			regs.r[REG_A] += 1;
			break;
		case REG_A_ind_dec:
			cpu::write_byte(regs.r[REG_A], v);
			regs.r[REG_X] -= 1;
			break;
		case REG_X_ind_dec:
			cpu::write_byte(regs.r[REG_X], v);
			regs.r[REG_X] -= 1;
			break;
		default:
			cpu::error("invalid opcode decoding");
			break;
	}
}

u8 cpu::conditions(u8 conditions) {
	bool zf = regs.fs.f.z;
	bool cf = regs.fs.f.c;
	bool nf = regs.fs.f.n;
	bool vf = regs.fs.f.v;
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

// instructions implementations

void instr_stub(cpu* c) {
	UNUSED(c);
}

void instr_nop(cpu* c) {
	// no operation
	c->regs.cp += 1;
	//c->cyclesExecuted += 1;
}

void instr_uop(cpu* c) {
	// warn about this incident
	c->error("unimplemented opcode");
	// unimplemented opcode causes exception
	c->exception();
	c->regs.cp += 1;
	//c->cyclesExecuted += 1;
}

void instr_mov_r_ccc(cpu* c) {
	u32 operand1, operand2;
	operand1 = c->opcode & 0x0F;
	operand2 = c->fetch_word();
	c->regs.r[operand1] = operand2;
	c->regs.cp += 4;
	//c->cyclesExecuted += 1;
}

void instr_mov_r_c(cpu* c) {
	u32 operand1, operand2;
	operand1 = c->opcode & 0x0F;
	operand2 = c->fetch_byte();
	c->write_reg_mode1_byte(operand1, operand2);
	c->regs.cp += 2;
	//c->cyclesExecuted += 1;
}

void instr_mov_rr_rr_w(cpu* c) {
	u32 operand1, operand2, value;
	u8 fetch2 = c->fetch_byte();
	operand1 = (fetch2 >> 4) & 0x0F;
	operand2 = fetch2 & 0x0F;
	value = c->read_reg_mode1_word(operand2);
	c->write_reg_mode1_word(operand1, value);
	c->regs.cp += 2;
	//c->cyclesExecuted += 1;
}

void instr_mov_rr_rr_b(cpu* c) {
	u32 operand1, operand2, value;
	u8 fetch2 = c->fetch_byte();
	operand1 = (fetch2 >> 4) & 0x0F;
	operand2 = fetch2 & 0x0F;
	value = c->read_reg_mode1_byte(operand2);
	c->write_reg_mode1_byte(operand1, value);
	c->regs.cp += 2;
	//c->cyclesExecuted += 1;
}

void instr_mov_rs_rs(cpu* c) {
	u32  operand1, operand2;
	u8 fetch2 = c->fetch_byte();
	operand1 = (fetch2 >> 4) & 0x0F;
	operand2 = fetch2 & 0x0F;
	c->regs.r[operand1] = c->regs.r[operand2];
	c->regs.cp += 2;
	//c->cyclesExecuted += 1;
}

enum { STACK_REGS_NUM = 8 };
const u32 masks[STACK_REGS_NUM] = {RS_A, RS_B, RS_C, RS_X, RS_Y, RS_Z, RS_FS, RS_SP};

void instr_push(cpu* c) {
	u8 fetch2 = c->fetch_byte();
	u32 operand1 = fetch2;
	for (int reg_index = 0; reg_index < STACK_REGS_NUM; reg_index++) {
		if (operand1 & masks[reg_index]) {
			c->push_word(c->regs.r[reg_index]);
		}
	}
	c->regs.cp += 2;
	//c->cyclesExecuted += 1;
}

void instr_pop(cpu* c) {
	u8 fetch2 = c->fetch_byte();
	u32 operand1 = fetch2;
	for (int reg_index = STACK_REGS_NUM - 1; reg_index >= 0; reg_index--) {
		if (operand1 & masks[reg_index]) {
			c->regs.r[reg_index] = c->pop_word();
		}
	}
	c->regs.cp += 2;
	//c->cyclesExecuted += 1;
}

void instr_inw(cpu* c) {
	u8 fetch2 = c->fetch_byte();
	u32 operand1 = (fetch2 >> 4) & 0x0F;
	u32 operand2 = fetch2 & 0x0F;
	c->write_reg_mode1_word(operand1, c->in_word(operand2));
	c->regs.cp += 2;
	//c->cyclesExecuted += 1;
}

void instr_outw(cpu* c) {
	u8 fetch2 = c->fetch_byte();
	u32 operand1 = (fetch2 >> 4) & 0x0F;
	u32 operand2 = fetch2 & 0x0F;
	c->out_word(c->read_reg_mode1_word(operand1), c->read_reg_mode1_word(operand2));
	c->regs.cp += 2;
	//c->cyclesExecuted += 1;
}

void instr_inb(cpu* c) {
	u8 fetch2 = c->fetch_byte();
	u32 operand1 = (fetch2 >> 4) & 0x0F;
	u32 operand2 = fetch2 & 0x0F;
	c->write_reg_mode1_byte(operand1, c->in_byte(operand2));
	c->regs.cp += 2;
	//c->cyclesExecuted += 1;
}

void instr_outb(cpu* c) {
	u8 fetch2 = c->fetch_byte();
	u32 operand1 = (fetch2 >> 4) & 0x0F;
	u32 operand2 = fetch2 & 0x0F;
	c->out_byte(c->read_reg_mode1_byte(operand1), c->read_reg_mode1_byte(operand2));
	c->regs.cp += 2;
	//c->cyclesExecuted += 1;
}


// arithmetic operations

using f24 = RegFS;
using alu2_callback = void (*)(u24*, f24*, u24, u24);
using alu1_callback = void (*)(u24*, f24*, u24);

void word_add(u24* r, f24* f, u24 a, u24 b) {
	*r = a + b;
	(*f).f.z = *r == 0;
	(*f).f.n = *r >> 23;
	u24 overflow = ~(a ^ b) & (a ^ *r);
	(*f).f.v = ((1 << 23) & (overflow)) ? 1 : 0;
	(*f).f.c = ((1 << 23) & (overflow ^ a ^ b ^ *r)) ? 1 : 0;
}
void word_adc(u24* r, f24* f, u24 a, u24 b) {
	*r = a + b + (*f).f.c;
	(*f).f.z = *r == 0;
	(*f).f.n = *r >> 23;
	u24 overflow = ~(a ^ b) & (a ^ *r);
	(*f).f.v = ((1 << 23) & (overflow)) ? 1 : 0;
	(*f).f.c = ((1 << 23) & (overflow ^ a ^ b ^ *r)) ? 1 : 0;
}
void word_sub(u24* r, f24* f, u24 a, u24 b) {
	u24 carry = 0;
	u24 overflow = ~(a ^ b) & (a ^ *r);
	*r = a - b - carry;
	(*f).f.z = *r == 0;
	(*f).f.n = *r >> 23;
	(*f).f.v = ((1 << 23) & (overflow)) ? 1 : 0;
	(*f).f.c = ((1 << 23) & (overflow ^ a ^ b ^ *r)) ? 1 : 0;
}
void word_sbc(u24* r, f24* f, u24 a, u24 b) {
	u24 carry = (*f).f.c ? 1 : 0;
	u24 overflow = ~(a ^ b) & (a ^ *r);
	*r = a - b - carry;
	(*f).f.z = *r == 0;
	(*f).f.c = ((1 << 23) & (overflow ^ a ^ b ^ *r)) ? 1 : 0;
	(*f).f.n = *r >> 23;
	(*f).f.v = ((1 << 23) & (overflow)) ? 1 : 0;

}
void word_mul(u24* r, f24* f, u24 a, u24 b) {
	*r = a * b;
	u64 rr = a * b;
	(*f).f.z = *r == 0;
	(*f).f.c = rr > 0xFFFFFF;
	(*f).f.n = *r >> 23;
	(*f).f.v =0;
}
void word_div(u24* r, f24* f, u24 a, u24 b) {
	*r = a / b;
	(*f).f.z = *r == 0;
	(*f).f.c = 0;
	(*f).f.n = *r >> 23;
	(*f).f.v = 0;
}
void word_rem(u24* r, f24* f, u24 a, u24 b) {
	*r = a % b;
	(*f).f.z = *r == 0;
	(*f).f.c = 0;
	(*f).f.n = *r >> 23;
	(*f).f.v = 0;
}
void word_and(u24* r, f24* f, u24 a, u24 b) {
	*r = a & b;
	(*f).f.z = *r == 0;
	(*f).f.n = *r >> 23;
	(*f).f.v = 0;
	(*f).f.c = 0;
}
void word_or(u24* r, f24* f, u24 a, u24 b) {
	*r = a | b;
	(*f).f.z = *r == 0;
	(*f).f.n = *r >> 23;
	(*f).f.v = 0;
	(*f).f.c = 0;
}
void word_xor(u24* r, f24* f, u24 a, u24 b) {
	*r = a ^ b;
	(*f).f.z = *r == 0;
	(*f).f.n = *r >> 23;
	(*f).f.v = 0;
	(*f).f.c = 0;
}
void word_shr(u24* r, f24* f, u24 a, u24 b) {
	b = b & 0x0F;
	*r = a >> b;
	(*f).f.z = *r == 0;
	(*f).f.n = *r >> 23;
	(*f).f.c = (a >> (b-1)) & 0x01;
	(*f).f.v = 0;

}
void word_shl(u24* r, f24* f, u24 a, u24 b) {
	b = b & 0x0F;
	*r = a << b;
	(*f).f.z = *r == 0;
	(*f).f.n = *r >> 23;
	(*f).f.c = (a << (b-1)) & 0x01;
	(*f).f.v = 0;
}
void word_ror(u24* r, f24* f, u24 a, u24 b) {
	b = b & 0x0F;
	*r = ((a >> b) & 0xFFFFFF) | ((a << b) & (0xFFFFFF >> (16 - b)));
	(*f).f.z = *r == 0;
	(*f).f.n = *r >> 23;
	(*f).f.c = 0;
	(*f).f.v = 0;
}
void word_rol(u24* r, f24* f, u24 a, u24 b) {
	b = b & 0x0F;
	*r = ((a << b) & 0xFFFFFF) | ((a >> (16-b)) & (0xFFFFFF >> (16 - b)));
	(*f).f.z = *r == 0;
	(*f).f.n = *r >> 23;
	(*f).f.c = 0;
	(*f).f.v = 0;
}
void word_rcr(u24* r, f24* f, u24 a, u24 b) {
	u24 c = (*f).f.c ? 1 : 0;
	b = b & 0x01;
	*r = (a >> b) | c;
	(*f).f.z = *r == 0;
	(*f).f.n = *r >> 23;
	(*f).f.c = (a >> (b-1)) & 0x01;
	(*f).f.v = 0;
}
void word_rcl(u24* r, f24* f, u24 a, u24 b) {
	u24 c = (*f).f.c ? 0x80 : 0;
	b = b & 0x01;
	*r = c | (a << b);
	(*f).f.z = *r == 0;
	(*f).f.n = *r >> 23;
	(*f).f.c = (a << (b-1)) & 0x01;
	(*f).f.v = 0;
}

void word_neg(u24* r, f24* f, u24 a) {
	*r = -a;
	(*f).f.z = *r == 0;
	(*f).f.c = 0;
	(*f).f.n = *r >> 23;
	(*f).f.v = 0;
}
void word_not(u24* r, f24* f, u24 a) {
	*r = ~a;
	(*f).f.z = *r == 0;
	(*f).f.c = 0;
	(*f).f.n = *r >> 23;
	(*f).f.v = 0;
}
void word_abs(u24* r, f24* f, u24 a) {
	if (*r & 0x7FFFFF)
		*r = -a;
	(*f).f.z = *r == 0;
	(*f).f.c = 0;
	(*f).f.n = 0;
	(*f).f.v = 0;
}

const alu2_callback alu2_op[] = {
	&word_add, &word_sub, &word_mul, &word_div, &word_rem,
	&word_and, &word_or, &word_xor,
	&word_shr, &word_shl,
	&word_ror, &word_rol, &word_rcr, &word_rcl,
};

const alu1_callback alu1_op[] = {
	&word_neg, &word_not, &word_abs,
};

void instr_alu_r_rr_rr(cpu* c) {
	u32 operation, operand1, operand2, operand3;
	u24 result, a, b;
	f24 newflags;
	u8 fetch2 = c->fetch_byte();
	operation = (fetch2 >> 4) & 0x0F;
	operand1 = fetch2 & 0x0F;
	operand2 = (c->fetch_next_byte() >> 4) & 0x0F;
	operand3 = c->fetch_next_byte() & 0x0F;
	newflags = c->regs.fs;
	a = c->read_reg_mode1_word(operand2);
	b = c->read_reg_mode1_word(operand3);
	alu2_op[operation](&result, &newflags, a, b);
	if (operand1 < 6) {
		c->write_reg_mode1_word(operand1, result);
		c->regs.fs = newflags;
	} else if (operand1 == 6) {
		c->regs.fs = newflags;
	} else if(operand1 == 7) {
		c->write_reg_mode1_word(operand2, result);
	}
	c->regs.cp += 3;
	//c->cyclesExecuted += 1;
}

void instr_alu_r_rr(cpu* c) {
	u32 operation, operand1, operand2;
	u24 result, a;
	f24 newflags;
	u8 fetch2 = c->fetch_byte();
	operation = c->opcode - 0xAA;
	operand1 = (fetch2 >> 4) & 0x0F;
	operand2 = fetch2 & 0x0F;
	a = c->read_reg_mode1_word(a);
	alu1_op[operation](&result, &newflags, a);
	if (operand1 < 6) {
		c->write_reg_mode1_word(operand1, result);
		c->regs.fs = newflags;
	} else if (operand1 == 6) {
		c->regs.fs = newflags;
	} else if(operand1 == 7) {
		c->write_reg_mode1_word(operand2, result);
	}
	c->regs.cp += 2;
	//c->cyclesExecuted += 1;
}

void instr_jc_ff_ccc(cpu* c) {
	u32 operation, operand1, operand2;
	operation = c->opcode < 0xC0 ? JMP : CALL;
	operand1 = c->opcode & 0x0F;
	operand2 = c->fetch_word();
	c->regs.cp += 4;
	if (c->conditions(operand1)) {
		if (operation == CALL) {
			c->push_word(c->regs.cp);
		}
		c->regs.cp = operand2;
	}
	//c->cyclesExecuted += 1;
}

void instr_jc_ff_cc(cpu* c) {
	u32 operation, operand1, operand2;
	operation = c->opcode < 0xC0 ? JMP : CALL;
	u8 fetch2 = c->fetch_byte();
	operand1 = fetch2 & 0xF0;
	operand2 = ((fetch2 & 0x0F) << 8) | c->fetch_next_byte();
	c->regs.cp += 3;
	if (c->conditions(operand1)) {
		if (operation == CALL) {
			c->push_word(c->regs.cp);
		}
		if (operand2 & 0x7FFFFF) {
			c->regs.cp -= 0x1000000 - operand2;
		} else {
			c->regs.cp += operand2;
		}
	}
	//c->cyclesExecuted += 1;
}

void instr_jc_ff_r(cpu* c) {
	u32 operation, operand1, operand2;
	operation = c->opcode < 0xC0 ? JMP : CALL;
	u8 fetch2 = c->fetch_byte();
	operand1 = (fetch2 >> 4) & 0x0F;
	operand2 = fetch2 & 0x0F;
	c->regs.cp += 2;
	if (c->conditions(operand1)) {
		if (operation == CALL) {
			c->push_word(c->regs.cp);
		}
		c->regs.cp = c->read_reg_mode1_word(operand2);
	}
	//c->cyclesExecuted += 1;
}

void instr_ret_ff(cpu* c) {
	u8 fetch2 = c->fetch_byte();
	u8 operand1 = (fetch2 & 0xF0) >> 4;
	c->regs.cp += 1;
	if (c->conditions(operand1)) {
		c->regs.cp = c->pop_word();
	}
	//c->cyclesExecuted += 1;
}

void instr_fint(cpu* c) {
	c->interrupt();
	c->regs.cp += 1;
	//c->cyclesExecuted += 1;
}

void instr_fexc(cpu* c) {
	c->exception();
	c->regs.cp += 1;
	//c->cyclesExecuted += 1;
}

void instr_rets(cpu* c) {
	c->regs.cp = c->pop_word();
	c->regs.fs.s.i = 0;
	c->regs.fs.s.e = 0;
	c->regs.cp += 1;
	//c->cyclesExecuted += 1;
}

void instr_dint(cpu* c) {
	c->regs.fs.s.ie = 0;
	c->regs.cp += 1;
	//c->cyclesExecuted += 1;
}

void instr_eint(cpu* c) {
	c->regs.fs.s.ee = 0;
	c->regs.cp += 1;
	//c->cyclesExecuted += 1;
}

void instr_slp(cpu* c) {
	// to implement by looping to itself until interrupt or detect mode in execution
	//c->mode = 0;
	c->regs.cp += 1;
	//c->cyclesExecuted += 1;
}

// table for instruction set callbacks
const instruction isa_table[256] = {
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
	for (u8 i = 0; i < regs.number; i++) {
		regs.r[i] = 0;
	}
	instr_set = isa_table;

	mem = nullptr;
	prt = nullptr;

	clock_rate = 16 * 1024 * 1024;  // 2^24 = 16777216 Hz

	mode = 0;
	opcode = 0;
	cyclesExecuted = 0;
	cycleNextEvent = 0;

}

cpu::~cpu() {
	mem = nullptr;
	prt = nullptr;
}

// used to log about unusual circumstances
void cpu::error(std::string description) {
	UNUSED(description);
	//logger.log(description);
}

inline u8 cpu::read_byte(u32 a) {
	return mem->buffer[a];
	// modular slower way
	//return mem->read_byte(a);
}

inline void cpu::write_byte(u32 a, u8 v) {
	mem->buffer[a] = v;
	// modular slower way
	//mem->write_byte(a, v);
}

inline u32 cpu::read_word(u32 a) {
	u32* ptr_word = reinterpret_cast<u32*>(&mem->buffer[a]);
	return *ptr_word & 0xFFFFFF;
	//u32 value = mem->buffer[a];
	//value |= mem->buffer[a+1] << 8;
	//value |= mem->buffer[a+2] << 16;
	//return value;
}

inline void cpu::write_word(u32 a, u32 v) {
	mem->buffer[a] = v & 0xFF;
	mem->buffer[a+1] = (v >> 8) & 0xFF;
	mem->buffer[a+2] = (v >> 16) & 0xFF;
}

inline void cpu::pre_fetch_byte() {
	// actually fetches byte at cp
	opcode = mem->buffer[regs.cp];
}

inline u8 cpu::fetch_byte() {
	// actually fetches next byte
	return mem->buffer[regs.cp+1];
}

inline u8 cpu::fetch_next_byte() {
	// actually fetches next next byte
	return mem->buffer[regs.cp+2];
}

inline u16 cpu::fetch_pair() {
	// actually fetches next byte pair
	u32 addr = regs.cp;
	return (mem->buffer[addr+2] << 8) | mem->buffer[addr+1];
}

inline u32 cpu::fetch_word() {
	// actually fetches next word
	u32 addr = regs.cp;
	u32 value = mem->buffer[addr+3];
	value |= mem->buffer[addr+2] << 8;
	value |= mem->buffer[addr+1] << 16;
	return value;
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
	return prt->out_byte(p);
}

void cpu::out_byte(u32 p, u32 v) {
	prt->in_byte(p, v);
}

u32 cpu::in_word(u32 p) {
	return prt->out_word(p);
}

void cpu::out_word(u32 p, u32 v) {
	prt->in_word(p, v);
}

void cpu::interrupt() {
	cpu::push_word(regs.cp);
	regs.cp = regs.ip;
	regs.fs.s.i = 1;
}

void cpu::exception() {
	cpu::push_word(regs.cp);
	regs.cp = regs.ep;
	regs.fs.s.e = 1;
}

// interface

void cpu::power() {
	reset();
}

void cpu::reset() {
	// reset registers
	for (u8 i = 0; i < regs.number; i++) {
		regs.r[i] = 0;
	}
	// reset state
	mode = 0;
	cyclesExecuted = 0;
}

void cpu::step() {
	// fetch
	opcode = mem->buffer[regs.cp];
	// decode
	instruction instr = instr_set[opcode];
	// execute
	instr(this);
}

u64 cpu::advance(u64 amount) {

	s64 cyclesRemaining = amount;

	while (cyclesRemaining > 0) {
		cpu::step();
		cyclesRemaining -= 1;
	}

	return -cyclesRemaining;
}

void cpu::run() {
	ended = false;
	while (!ended) {
		if (running) {
			cpu::step();
		}
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

void cpu::save(std::vector<u8> buf) {
	u32 cpu_size = 3*regs.number;
	buf.reserve(cpu_size);
	u32 r;
	for (u8 i = 0; i < regs.number; i++) {
		r = regs.r[i];
		buf[3*i+0] = r & 0xFF;
		buf[3*i+1] = (r >> 8) & 0xFF;
		buf[3*i+2] = (r >> 16) & 0xFF;
	}
}

void cpu::load(std::vector<u8> buf) {
	u32 cpu_size = 3*regs.number;
	buf.reserve(cpu_size);
	u32 r;
	for (u8 i = 0; i < regs.number; i++) {
		r = 0;
		r |= buf[3*i+0];
		r |= buf[3*i+1] << 8;
		r |= buf[3*i+1] << 16;
		regs.r[i] = r;
	}
}
