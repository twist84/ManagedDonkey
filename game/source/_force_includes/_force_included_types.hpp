#pragma once

typedef char int8;
static_assert(sizeof(int8) == 0x1);

typedef unsigned char uns8;
static_assert(sizeof(uns8) == 0x1);

typedef short int16;
static_assert(sizeof(int16) == 0x2);

typedef unsigned short uns16;
static_assert(sizeof(uns16) == 0x2);

typedef long int32;
static_assert(sizeof(int32) == 0x4);

typedef unsigned long uns32;
static_assert(sizeof(uns32) == 0x4);

typedef long long int64;
static_assert(sizeof(int64) == 0x8);

typedef unsigned long long uns64;
static_assert(sizeof(uns64) == 0x8);

typedef float real32;
static_assert(sizeof(real32) == 0x4);

typedef double real64;
static_assert(sizeof(real64) == 0x8);

typedef unsigned char byte;
static_assert(sizeof(byte) == 0x1);

typedef char utf8;
static_assert(sizeof(utf8) == 0x1);

typedef unsigned long tag;
static_assert(sizeof(tag) == 0x4);

typedef long string_id;
static_assert(sizeof(string_id) == 0x4);

