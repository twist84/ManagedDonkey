#pragma once

typedef char int8;
COMPILE_ASSERT(sizeof(int8) == 0x1);

typedef unsigned char uns8;
COMPILE_ASSERT(sizeof(uns8) == 0x1);

typedef short int16;
COMPILE_ASSERT(sizeof(int16) == 0x2);

typedef unsigned short uns16;
COMPILE_ASSERT(sizeof(uns16) == 0x2);

typedef long int32;
COMPILE_ASSERT(sizeof(int32) == 0x4);

typedef unsigned long uns32;
COMPILE_ASSERT(sizeof(uns32) == 0x4);

typedef long long int64;
COMPILE_ASSERT(sizeof(int64) == 0x8);

typedef unsigned long long uns64;
COMPILE_ASSERT(sizeof(uns64) == 0x8);

typedef float real32;
COMPILE_ASSERT(sizeof(real32) == 0x4);

typedef double real64;
COMPILE_ASSERT(sizeof(real64) == 0x8);

typedef unsigned char byte;
COMPILE_ASSERT(sizeof(byte) == 0x1);

typedef char utf8;
COMPILE_ASSERT(sizeof(utf8) == 0x1);

typedef unsigned long tag;
COMPILE_ASSERT(sizeof(tag) == 0x4);

typedef long string_id;
COMPILE_ASSERT(sizeof(string_id) == 0x4);

