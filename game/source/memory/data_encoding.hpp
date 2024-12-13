#pragma once

enum e_byte_swap_code_constant
{
	_1byte = 1,
	_2byte = -2,
	_4byte = -4,
	_8byte = -8,
	_begin_bs_array = -100,
	_end_bs_array = -101,
	_extern_bs_definition = -102,
	BYTE_SWAP_DEFINITION_SIGNATURE = 'bysw',
};

struct data_encoding_state
{
	void* buffer;
	long offset;
	long buffer_size;
	unsigned char overflow_flag;
};

extern void __cdecl data_encode_new(data_encoding_state* state, void* buffer, long buffer_size);
extern unsigned char __cdecl data_encode_memory(data_encoding_state* state, void* data, short count, long code);
//extern unsigned char __cdecl data_encode_integer(data_encoding_state* state, long data, long maximum_value);
//extern unsigned char __cdecl data_encode_structures(data_encoding_state* state, void* data, short, struct byte_swap_definition*);
//extern unsigned char __cdecl data_encode_array(data_encoding_state* state, long, void* data, long, struct byte_swap_definition*);
//extern unsigned char __cdecl data_encode_string(data_encoding_state* state, char* s, short);

