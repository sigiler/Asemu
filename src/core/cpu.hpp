
#pragma once

#include "../common/common_types.hpp"
#include <string>

#include "mem.hpp"
#include "ports.hpp"

// move this to static const in registers?
#define NUM_REGISTERS 12

// abusing undefined behaviour
union registers {
	struct {
		u32 a : 24;
		u32 b : 24;
		u32 c : 24;
		u32 x : 24;
		u32 y : 24;
		u32 z : 24;
		// TODO fix endianness
		union {
			struct {
				u32 p : 1;  // privileged flag
				u32 i : 1;  // in interrupt flag
				u32 e : 1;  // in exception flag
				u32 ie : 1;  // interrupts enabled
				u32 ee : 1;  // exceptions enabled
				u32 uu : 3;  // unused bits
				u32 t : 4; // t o, generic flags for compares
				u32 v : 1;  // overflow flag
				u32 n : 1;  // negative flag
				u32 z : 1;  // zero flag
				u32 c : 1;  // carry flag
			} f;
			u32 fs : 24;  // flags and status register
		};
		u32 pr : 24;  // predicate register
		u32 sp : 24;  // stack pointer
		u32 cp : 24;  // code pointer (aka pc, ip)
		u32 ip : 24;  // interrupt pointer
		u32 ep : 24;  // exception pointer
	};

	struct {
		struct {
			u32 r : 24;
		} gpr[6];
		struct {
			u32 r : 24;
		} spr[6];
	};

	struct {
		u32 r : 24;
	} reg[NUM_REGISTERS];
};

//typedef std::function<void (struct cpu*)> instr_func;  // C++ lib is inefficient
//typedef void (*instruction_callback)(struct cpu*);     // C is efficient but ugly syntax
using instruction_callback = void (*) (struct cpu*);     // as efficient as C definition, better syntax

struct instructions {
	instruction_callback* exec_table;
	u8* base_length;
	u8* base_cycles;
};

struct cpu {

	registers regs;
	instructions instrs;
	memory* mem;
	ports* port;

	// properties
	u64 clock_rate;      // number of cycles executed per second
	u64 interrupt_rate;  // number of cycles needed to pass in order to interrupt fire

	// state
	bool running;
	bool ended;
	bool sleep;
	u64 cyclesExecuted;        // number of executed cycles since restart or wrap

	// report unexpected
	void error(std::string description);

	// read write operations
	u8 read_byte(u32 a);
	void write_byte(u32 a, u8 v);
	u32 read_word(u32 a);
	void write_word(u32 a, u32 v);
	void push_word(u32 v);
	u32 pop_word();

	u8 in_byte(u32 p);
	void out_byte(u32 p, u32 v);
	u32 in_word(u32 p);
	void out_word(u32 p, u32 v);

	// addressing modes
	u32 read_reg_mode1(u8 r);
	void write_reg_mode1(u8 r, u32 v);

	// flags check
	u8 conditions(u8 conditions);

	// interrupts
	void interrupt();
	void exception();

	//
	void raiseInterrupt(u8);

	// steps
	void fetch();
	void decode();
	void execute();

	// public part
	cpu();
	~cpu();

	void power();
	void reset();

	void step();
	u64 advance(u64);

	void run();
	void endrun();
	void resume();
	void pause();

	void save(u8* buf);
	void load(u8* buf);

};


// opcode mnemomics

enum OPCODE {
	NOP,
	LDR,
	LDB,
	LDS,
	ADD,
	SUB,
	MUL,
	DIV,
	REM,
	NEG,
	CMP,
	AND,
	OR,
	XOR,
	NOT,
	ROL,
	ROR,
	RCL,
	RCR,
	SHL,
	SHR,
	JMP,
	PUSH,
	POP,
	CALL,
	RET,
	IN,
	OUT,
	SPE1,
	SPE2,
};

// operand types
enum GROUP {
	R_Cw,     // LDR
	R_Cb,     // LDB
	RR_RR_w,  // LDR
	RR_RR_b,  // LDB
	RS_RS,    // LDS
	R_RR_RR,  // ADD, SUB, MUL, DIV, REM, rotations and shifts
	R_RR,     // NEG, NOT
	FF_C24,   // JMP, CALL FF,abs
	FF_C20,   // JMP, CALL FF,rel
	FF_C16,   // JMP, CALL FF,rel
	FF_C12,   // JMP, CALL FF,rel
	FF_C8,    // JMP, CALL FF,rel
	FF_REG,   // JMP, CALL reg
	FF,       // RET
	REGSET,   // PUSH POP

	RR_RR_port, // IN OUT
	IMPLICIT,
	NONE,
	NOOP
};


// registers and addressing modes

enum REGS {
	RGS_A,
	RGS_B,
	RGS_C,
	RGS_X,
	RGS_Y,
	RGS_Z,
	RGS_FS,
	RGS_SP,
	RGS_CP,
	RGS_IP,
};

enum REG_MODE {
	REG_A,
	REG_B,
	REG_C,
	REG_X,
	REG_Y,
	REG_Z,
	REG_A_ind,
	REG_B_ind,
	REG_C_ind,
	REG_X_ind,
	REG_Y_ind,
	REG_Z_ind,
	REG_A_ind_inc,
	REG_A_ind_dec,
	REG_X_ind_inc,
	REG_X_ind_dec,
	REG_A_ind_inc_b,
	REG_A_ind_dec_b,
	REG_X_ind_inc_b,
	REG_X_ind_dec_b,
	REG_A_ind_b,
	REG_B_ind_b,
	REG_C_ind_b,
	REG_X_ind_b,
	REG_Y_ind_b,
	REG_Z_ind_b,
	REG_SP,
	REG_CP,
	REG_FS
};

enum REGSET {
	RS_A = 1 << 0,
	RS_B = 1 << 1,
	RS_C = 1 << 2,
	RS_X = 1 << 3,
	RS_Y = 1 << 4,
	RS_Z = 1 << 5,
	RS_FS = 1 << 6,
	RS_SP = 1 << 7
};

// flags

enum FLAGS {
	Z_MASK = 1 << 3,
	C_MASK = 1 << 2,
	V_MASK = 1 << 1,
	N_MASK = 1 << 0,
};

enum CONDS_FLAGS {
	// conditions
	always = 0x00,
	eq,
	ne,
	gt,
	lt,
	ge,
	le,
	a,
	b,
	ae,
	be,
	random_c,
	// flags
	carry,
	noncarry,
	zero,
	nonzero,
	positive,
	negative,
	overflow,
	nonoverflow,
	// unused
	never,
};

// scrapped parts

struct intexc {
	// almost an interrupt/exception controller, dunno what to do here
	// move to some sort of controller
	// move to ports

	enum InterruptCause {
		Int_poweron, int_timer, int_keys, int_screen, int_speaker, int_rtc,
		Exc_software, Exc_divby0, Exc_overflow, Exc_mem, exc_dbg,
		Int_nmi, int_req,
		arith_overflow, undefined_instr, syscall, external,
	};

	bool intr_enabled;  // master enable interrupt, mirrors flag?
	bool excp_enabled;  // master enable exception, mirrors flag?
	u32 ie_mask;    // determine what can trigger interrupt
	u32 ie_ack;     // acknowledge by writing 1 here
	// unstoppable interrupt?
	// stay pending when masked or miss if masked and not acknowledge?

	u8 cause;  // current interrupt cause
	u32 where; // address where it was caused

	u64 next_interrupt_cycle;  // cycle where next interrupt is scheduled

	// unused code
	//next_interrupt_cycle = cyclesExecuted + interrupt_rate;
	// cyclesExecuted >= next_interrupt_cycle

};
