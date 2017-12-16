
#include "common/common.hpp"
#include "utils/logger.hpp"

#include "emu.hpp"
#include "frontend/sdl_app.hpp"

App_SDL app;

// run emulator
void test1(void) {
	//logger.log("start test");

	memory* m = app.e->m;
	// manually assembled code for bootstrapping
	u8 binary[99] = {
		0x10, 0x00, 0x00, 0xF0,    // 0x000000:  ldr a,0x0000FF
		0x11, 0x5C, 0x00, 0x09,    // 0x000004:  ldr b,0x5C0009
		0x12, 0x00, 0x00, 0x01,    // 0x000008:  ldr c,0x000001
		0x13, 0x00, 0x00, 0x00,    // 0x00000C:  ldr x,0x000000
		0x14, 0xBB, 0x00, 0x00,    // 0x000010:  ldr y,0xBB0000
		0x15, 0x5C, 0x00, 0x09,    // 0x000014:  ldr z,0x5C0009
		0x02, 0x70,                // 0x000018:  ldb @b,a
		0x0A, 0x01, 0x12,          // 0x000020:  add b, b, c
		0x01, 0x3A,                // 0x000022:  ldw x,@y
		0x01, 0xB3,                // 0x000024:  ldw @z,x
		0xB0, 0x00, 0x00, 0x18,    // 0x000026:  jmp 0x000018

		0xFF, 0xFF, 0xFF, 0xFF,    // 0x00002A:  ld a,0x000031
		0xFF, 0xFF,                // 0x00002E:  ld ip,a
		0xFF,                      // 0x000031:  ret
	};
	int binary_size = 99;
	for (int i = 0; i < binary_size; i++) {
		m->buffer[i] = binary[i];
	}

	// initial screen
	u8 binary2[99] = {
		0xFF, 0x00, 0x00,
		0x00, 0xFF, 0x00,
		0x00, 0x00, 0xFF,
		0xAA, 0xAA, 0xAA,
	};
	int binary_size2 = 99;
	for (int i = 0; i < binary_size2; i++) {
		m->buffer[0x5C0000 + i] = binary2[i];
	}

	// all ready to go
	app.Run();

	//logger.log("end test");
}

// testing a opcode
void test2() {
	// preparation
	Emu* e = new Emu;
	e->Init();
	cpu *c = e->c;
	memory* m = e->m;
	// load program into memory
	u8 binary[99] = {
		0x10, 0x00, 0x00, 0x01,    // 0x000000:  ldr a,1
		0x11, 0xFF, 0xFF, 0xFF,    // 0x000004:  ldr b,-1
		0x12, 0xFF, 0xFF, 0xFF,    // 0x000008:  ldr c,-1
		0x0A, 0x00, 0x12,          // 0x000020:  add a, b, c
	};
	int binary_size = 99;
	for (int i = 0; i < binary_size; i++) {
		m->buffer[i] = binary[i];
	}
	// execute code
	c->advance(10);
	// test
	if (c->regs.c != 16777215)
		printf("error!\n");
	// see result
	printf("a=%i\n", (u32)c->regs.a);
	printf("b=%i\n", (u32)c->regs.b);
	printf("c=%i\n", (u32)c->regs.c);
	printf("f=%#X\n", (u32)c->regs.fs);
	printf("f.z=%u\n", (u32)c->regs.fs.f.z);
	printf("f.c=%u\n", (u32)c->regs.fs.f.c);
	printf("f.n=%u\n", (u32)c->regs.fs.f.n);
	printf("f.v=%u\n", (u32)c->regs.fs.f.v);
}


int main() {

	test1();

	return EXIT_SUCCESS;
}
