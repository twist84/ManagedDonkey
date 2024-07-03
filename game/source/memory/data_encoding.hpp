#pragma once

#define _1byte  1
#define _2byte -2
#define _4byte -4
#define _8byte -8

struct data_encoding_state
{
	void* buffer;
	long offset;
	long buffer_size;
	unsigned char overflow_flag;
};

extern void __cdecl data_encode_new(data_encoding_state* state, void* buffer, long buffer_size);
extern unsigned char __cdecl data_encode_memory(data_encoding_state* state, void* a2, short count, long code);

