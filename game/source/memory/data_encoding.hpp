#pragma once

#include "cseries/cseries.hpp"
#include "memory/byte_swapping.hpp"

struct data_encoding_state
{
	void* buffer;
	int32 offset;
	int32 buffer_size;
	uns8 overflow_flag;
};
COMPILE_ASSERT(sizeof(data_encoding_state) == 0x10);

extern void* data_decode_array(data_encoding_state* state, int32 element_count_size, int32* element_count_reference, int32 maximum_element_count, byte_swap_definition* bs_definition);
extern byte data_decode_byte(data_encoding_state* state);
extern uns64 data_decode_int64(data_encoding_state* state);
extern uns32 data_decode_long(data_encoding_state* state);
extern void* data_decode_memory(data_encoding_state* state, int16 count, int32 code);
extern void data_decode_new(data_encoding_state* state, void* buffer, int32 buffer_size);
extern uns16 data_decode_short(data_encoding_state* state);
extern char* data_decode_string(data_encoding_state* state, int16 maximum_string_length);
extern void* data_decode_structures(data_encoding_state* state, int16 structure_count, byte_swap_definition* bs_definition);
extern byte data_encode_array(data_encoding_state* state, int32 element_count_size, void* source_array, int32 element_count, byte_swap_definition* bs_definition);
extern byte data_encode_integer(data_encoding_state* state, int32 value, int32 maximum_value);
extern byte data_encode_memory(data_encoding_state* state, const void* buffer, int16 count, int32 code);
extern void data_encode_new(data_encoding_state* state, void* buffer, int32 buffer_size);
extern byte data_encode_string(data_encoding_state* state, char* source_string, int16 maximum_string_length);
extern byte data_encode_structures(data_encoding_state* state, const void* source_structures, int16 structure_count, byte_swap_definition* bs_definition);

