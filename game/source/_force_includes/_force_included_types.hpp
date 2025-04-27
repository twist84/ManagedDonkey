#pragma once

typedef unsigned long tag;
static_assert(sizeof(tag) == 0x4);

typedef long string_id;
static_assert(sizeof(string_id) == 0x4);

typedef char int8;
static_assert(sizeof(int8) == 0x1);

typedef unsigned char uint8;
static_assert(sizeof(uint8) == 0x1);

typedef short int16;
static_assert(sizeof(int16) == 0x2);

typedef unsigned short uint16;
static_assert(sizeof(uint16) == 0x2);

typedef long int32;
static_assert(sizeof(int32) == 0x4);

typedef unsigned long uint32;
static_assert(sizeof(uint32) == 0x4);

typedef long long int64;
static_assert(sizeof(int64) == 0x8);

typedef unsigned long long uint64;
static_assert(sizeof(uint64) == 0x8);

typedef float real32;
static_assert(sizeof(real32) == 0x4);

typedef double real64;
static_assert(sizeof(real64) == 0x8);

typedef char utf8;

