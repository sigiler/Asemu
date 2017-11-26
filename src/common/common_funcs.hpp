
#pragma once

// Endianness

#include <stdint.h>

uint32_t swapEndianness32bits(uint32_t value) {
	uint32_t result = 0;
    result |= (value & 0x000000FF) << 24;
    result |= (value & 0x0000FF00) << 8;
    result |= (value & 0x00FF0000) >> 8;
    result |= (value & 0xFF000000) >> 24;
    return result;
}


// RNG

#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

unsigned long mix(unsigned long a, unsigned long b, unsigned long c) {
    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}

void random_seed() {
	unsigned long seed = mix(clock(), time(NULL), getpid());
	srand(seed);
}

int random_int(int min, int max) {
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

float random_float(float a, float b) {
    return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}
