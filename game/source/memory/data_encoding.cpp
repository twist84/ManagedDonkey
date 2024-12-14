#include "memory/data_encoding.hpp"

void* data_decode_array(data_encoding_state* state, long element_count_size, long* element_count_reference, long maximum_element_count, byte_swap_definition* bs_definition)
{
	ASSERT(state && state->buffer && state->offset >= 0 && state->offset < state->buffer_size);
	ASSERT(element_count_reference);
	ASSERT(maximum_element_count > 0);
	ASSERT(bs_definition);

	long element_count = 0;
	switch (element_count_size)
	{
	case _8byte:
		element_count = data_decode_int64(state);
		break;
	case _4byte:
		element_count = data_decode_long(state);
		break;
	case _2byte:
		element_count = data_decode_short(state);
		break;
	case _1byte:
		element_count = data_decode_byte(state);
		break;
	default:
		throw; // halt()
		break;
	}

	if (state->overflow_flag || element_count < 0 || element_count > maximum_element_count)
		return NULL;

	*element_count_reference = element_count;
	return data_decode_structures(state, element_count, bs_definition);
}

byte data_decode_byte(data_encoding_state* state)
{
	ASSERT(state && state->buffer && state->offset >= 0 && state->offset <= state->buffer_size);

	long offset = state->offset;
	if (offset + sizeof(byte) > state->buffer_size || state->overflow_flag)
	{
		state->overflow_flag = 1;
		return 0;
	}

	byte* memory = (byte*)state->buffer + offset;
	state->offset += sizeof(byte);

	if (!memory)
		return 0;

	return *memory;
}

qword data_decode_int64(data_encoding_state* state)
{
	ASSERT(state && state->buffer && state->offset >= 0 && state->offset <= state->buffer_size);

	long offset = state->offset;
	if (offset + sizeof(qword) > state->buffer_size || state->overflow_flag)
	{
		state->overflow_flag = 1;
		return 0;
	}

	byte* memory = (byte*)state->buffer + offset;
	byte_swap_memory(memory, 1, _8byte);
	state->offset += sizeof(qword);

	if (!memory)
		return 0;

	return *memory;
}

dword data_decode_long(data_encoding_state* state)
{
	ASSERT(state && state->buffer && state->offset >= 0 && state->offset <= state->buffer_size);

	long offset = state->offset;
	if (offset + sizeof(dword) > state->buffer_size || state->overflow_flag)
	{
		state->overflow_flag = 1;
		return 0;
	}

	byte* memory = (byte*)state->buffer + offset;
	byte_swap_memory(memory, 1, _4byte);
	state->offset += sizeof(dword);

	if (!memory)
		return 0;

	return *memory;
}

void* data_decode_memory(data_encoding_state* state, short count, long code)
{
	ASSERT(state && state->buffer && state->offset >= 0 && state->offset <= state->buffer_size);
	ASSERT(count >= 0);

	long size = 0;
	switch (code)
	{
	case _8byte:
		size = count * sizeof(qword);
		break;
	case _4byte:
		size = count * sizeof(dword);
		break;
	case _2byte:
		size = count * sizeof(word);
		break;
	case _1byte:
		size = count * sizeof(byte);
		break;
	default:
		throw; // halt()
		break;
	}

	long offset = state->offset;
	if (offset + size > state->buffer_size || state->overflow_flag)
	{
		state->overflow_flag = 1;
		return 0;
	}

	byte* memory = (byte*)state->buffer + offset;

	if (code != _1byte)
		byte_swap_memory(memory, count, code);

	state->offset += size;

	return memory;
}

void data_decode_new(data_encoding_state* state, void* buffer, long buffer_size)
{
	ASSERT(buffer);
	ASSERT(buffer_size >= 0);

	csmemset(state, 0, sizeof(data_encoding_state));
	state->buffer = buffer;
	state->buffer_size = buffer_size;
}

word data_decode_short(data_encoding_state* state)
{
	ASSERT(state && state->buffer && state->offset >= 0 && state->offset <= state->buffer_size);

	long offset = state->offset;
	if (offset + sizeof(word) > state->buffer_size || state->overflow_flag)
	{
		state->overflow_flag = 1;
		return 0;
	}

	byte* memory = (byte*)state->buffer + offset;
	byte_swap_memory(memory, 1, _2byte);
	state->offset += sizeof(word);

	if (!memory)
		return 0;

	return *memory;
}

char* data_decode_string(data_encoding_state* state, short maximum_string_length)
{
	throw "unimplemented";
}

void* data_decode_structures(data_encoding_state* state, short structure_count, byte_swap_definition* bs_definition)
{
	ASSERT(state && state->buffer && state->offset >= 0 && state->offset <= state->buffer_size);
	ASSERT(structure_count >= 0);
	ASSERT(bs_definition);

	short structure_size = structure_count * (short)bs_definition->size;
	if (structure_size + state->offset > state->buffer_size || state->overflow_flag)
	{
		state->overflow_flag = 1;
		return NULL;
	}

	byte* memory = (byte*)state->buffer + state->offset;
	if (structure_size)
	{
		byte_swap_data(bs_definition, memory, structure_count);
		state->offset += structure_size;
	}

	return memory;
}

byte data_encode_array(data_encoding_state* state, long element_count_size, void* source_array, long element_count, byte_swap_definition* bs_definition)
{
	throw "unimplemented";
}

byte data_encode_integer(data_encoding_state* state, long value, long maximum_value)
{
	throw "unimplemented";
}

byte data_encode_memory(data_encoding_state* state, void const* buffer, short count, long code)
{
	ASSERT(state && state->buffer && state->offset >= 0 && state->offset < state->buffer_size);

	long size = 0;
	switch (code)
	{
	case _8byte:
		size = count * sizeof(qword);
		break;
	case _4byte:
		size = count * sizeof(dword);
		break;
	case _2byte:
		size = count * sizeof(word);
		break;
	case _1byte:
		size = count * sizeof(byte);
		break;
	default:
		throw; // halt()
		break;
	}

	if (state->offset + size > state->buffer_size || state->overflow_flag)
	{
		state->overflow_flag = 1;
	}
	else
	{
		byte* memory = (byte*)state->buffer + state->offset;
		if (buffer)
			csmemcpy(memory, buffer, size);
		else
			csmemset(memory, 0, size);

		if (code != _1byte)
			byte_swap_memory(memory, count, code);

		state->offset += size;
	}

	return state->overflow_flag == 0;
}

void data_encode_new(data_encoding_state* state, void* buffer, long buffer_size)
{
	ASSERT(buffer);
	ASSERT(buffer_size >= 0);

	csmemset(state, 0, sizeof(data_encoding_state));
	state->buffer = buffer;
	state->buffer_size = buffer_size;
}

byte data_encode_string(data_encoding_state* state, char* source_string, short maximum_string_length)
{
	dword string_length = csstrnlen(source_string, maximum_string_length);
	ASSERT(state->offset + string_length + 1 <= state->buffer_size);

	if (state->offset + string_length + 1 <= state->buffer_size || state->overflow_flag)
	{
		state->overflow_flag = 1;
	}
	else
	{
		char* definition = (char*)state->buffer + state->offset;
		csstrnzcpy(definition, source_string, string_length + 1);
		state->offset += string_length + 1;
	}

	return state->overflow_flag == 0;
}

byte data_encode_structures(data_encoding_state* state, void const* source_structures, short structure_count, byte_swap_definition* bs_definition)
{
	ASSERT(state && state->buffer && state->offset >= 0 && state->offset < state->buffer_size);
	ASSERT(source_structures);
	ASSERT(bs_definition);

	short structure_size = structure_count * (short)bs_definition->size;
	if (structure_size > 0)
	{
		if (structure_size + state->offset > state->buffer_size || state->overflow_flag)
		{
			state->overflow_flag = 1;
		}
		else
		{
			void* definition_structures = (byte*)state->buffer + state->offset;
			csmemcpy(definition_structures, source_structures, structure_size);
			byte_swap_data(bs_definition, definition_structures, structure_count);
			state->offset += structure_size;
		}
	}

	return state->overflow_flag == 0;
}

