#include "memory/data_encoding.hpp"

#include "cseries/cseries.hpp"

void __cdecl data_encode_new(data_encoding_state* state, void* buffer, long buffer_size)
{
	ASSERT(buffer);
	ASSERT(buffer_size >= 0);

	csmemset(state, 0, sizeof(data_encoding_state));
	state->buffer = buffer;
	state->buffer_size = buffer_size;
}

unsigned char __cdecl data_encode_memory(data_encoding_state* state, void* data, short count, long code)
{
	ASSERT(state && state->buffer && state->offset >= 0 && state->offset < state->buffer_size);

	long size = 0;
	if ((code + 8) <= 9)
	{
		switch (code)
		{
		case _8byte:
			size = 8 * count;
			break;
		case _4byte:
			size = 4 * count;
			break;
		case _2byte:
			size = 2 * count;
			break;
		case _1byte:
			size = count;
			break;
		default:
			throw; // halt()
			break;
		}
	}

	if (state->offset + size > state->buffer_size || state->overflow_flag)
	{
		state->overflow_flag = 1;
	}
	else
	{
		byte* memory = &static_cast<byte*>(state->buffer)[state->offset];
		if (data)
			csmemcpy(memory, data, size);
		else
			csmemset(memory, 0, size);

		// #TODO: add and implement byte-swap function
		//if (code != _1byte)
		//	_data_encode_memory((unsigned int*)memory, count, code);

		state->offset += size;
	}

	return state->overflow_flag == 0;
}

