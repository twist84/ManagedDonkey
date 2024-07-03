#include "memory/data_packets.hpp"

#include "cseries/cseries.hpp"
#include "memory/data_encoding.hpp"
#include "memory/data_packet_groups.hpp"


void __cdecl data_packet_verify(data_packet_definition* packet_definition)
{
	// #TODO: implement me!
}

void __cdecl _data_packet_encode(data_packet_definition* packet_definition, data_encoding_state* state, short version, void* a4, short* a5, data_packet_field* fields, short* a6)
{
	// #TODO: implement me!
}

unsigned char __cdecl data_packet_encode(data_packet_definition* packet_definition, short version, void* a3, void* buffer, short* buffer_size, short maximum_buffer_size)
{
	ASSERT(packet_definition);
	ASSERT(buffer && buffer_size);
	ASSERT(maximum_buffer_size >= 0);

	data_packet_verify(packet_definition);

	data_encoding_state state{};
	data_encode_new(&state, buffer, maximum_buffer_size);

	if (version == 0xFFFF)
		version = packet_definition->version;

	if (packet_definition->version > 0)
	{
		byte v9[16]{};
		v9[0] = static_cast<byte>(version);
		data_encode_memory(&state, v9, 1, _1byte);
	}

	_data_packet_encode(packet_definition, &state, version, a3, 0, packet_definition->fields, 0);
	*buffer_size = static_cast<short>(state.offset);

	return state.overflow_flag == 0;
}

