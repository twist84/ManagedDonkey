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

unsigned char __cdecl data_encode_memory(data_encoding_state* state, void* a2, short count, long code)
{
	ASSERT(state && state->buffer && state->offset >= 0 && state->offset < state->buffer_size);

	long size = 0;
	if ((code + 8) <= 9)
	{
		switch (code)
		{
		case _1byte:
			size = count;
			break;
		case _2byte:
			size = 2 * count;
			break;
		case _4byte:
			size = 4 * count;
			break;
		case _8byte:
			size = 8 * count;
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
		if (a2)
			csmemcpy(memory, a2, size);
		else
			csmemset(memory, 0, size);

		// #TODO: add and implement byte-swap function
		//if (code != 1)
		//	sub_14049E740((unsigned int*)memory, count, code);

		state->offset += size;
	}

	return state->overflow_flag == 0;
}

