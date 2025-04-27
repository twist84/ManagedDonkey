#pragma once

#include "cseries/cseries.hpp"
#include "memory/byte_swapping.hpp"

struct data_encoding_state
{
	void* buffer;
	int32 offset;
	int32 buffer_size;
	uint8 overflow_flag;
};
static_assert(sizeof(data_encoding_state) == 0x10);

extern void* data_decode_array(data_encoding_state* state, int32 element_count_size, int32* element_count_reference, int32 maximum_element_count, byte_swap_definition* bs_definition);
extern uint8 data_decode_byte(data_encoding_state* state);
extern uint64 data_decode_int64(data_encoding_state* state);
extern uint32 data_decode_long(data_encoding_state* state);
extern void* data_decode_memory(data_encoding_state* state, int16 count, int32 code);
extern void data_decode_new(data_encoding_state* state, void* buffer, int32 buffer_size);
extern uint16 data_decode_short(data_encoding_state* state);
extern char* data_decode_string(data_encoding_state* state, int16 maximum_string_length);
extern void* data_decode_structures(data_encoding_state* state, int16 structure_count, byte_swap_definition* bs_definition);
extern uint8 data_encode_array(data_encoding_state* state, int32 element_count_size, void* source_array, int32 element_count, byte_swap_definition* bs_definition);
extern uint8 data_encode_integer(data_encoding_state* state, int32 value, int32 maximum_value);
extern uint8 data_encode_memory(data_encoding_state* state, void const* buffer, int16 count, int32 code);
extern void data_encode_new(data_encoding_state* state, void* buffer, int32 buffer_size);
extern uint8 data_encode_string(data_encoding_state* state, char* source_string, int16 maximum_string_length);
extern uint8 data_encode_structures(data_encoding_state* state, void const* source_structures, int16 structure_count, byte_swap_definition* bs_definition);

