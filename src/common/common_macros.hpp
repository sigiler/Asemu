
#pragma once

// suppress errors for unused vars
#define UNUSED(x)  (void)(x)

// array size
#define ARRAY_SIZE(x)  (sizeof(x) / sizeof(x[0]))
#define ARRAY_SIZE_2(n)  (sizeof (n) / sizeof (*(n)))

// block for macro
#define MACROBLOCK(m) do { \
  m; \
} while (0)

// concatenate macro
#define _MACRO_CONCAT_AUX(a, b) a##b
#define MACRO_CONCAT(a, b) _MACRO_CONCAT_AUX(a, b)

// assert at compile time
// for C++ use static_assert
#define C_ASSERT(condition) ((void)sizeof(char[1 - 2*!!(condition)]))
#define _ASSERT_CONCAT_(a, b) a##b
#define _ASSERT_CONCAT(a, b) _ASSERT_CONCAT_(a, b)
#define C_ASSERT_2(e) enum { _ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) }

// error checking

// assert

// debug

// log

// preprocessor useful constants
//__FILE__, __LINE__, __DATE__, __TIME__

// optimization related likely, unlikely, force_inline

// abstract platform from compiler

// pack abstract from compiler

// endianness management for compilers

// visibility: lib_export
