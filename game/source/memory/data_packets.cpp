#include "memory/data_packets.hpp"

#include "cseries/cseries.hpp"
#include "memory/data_encoding.hpp"
#include "memory/data_packet_groups.hpp"

void __cdecl _data_packet_verify(data_packet_definition* packet_definition, int16* byte_count_reference, struct data_packet_field* first_field, int16* field_count_reference)
{
	// $IMPLEMENT
}

void __cdecl data_packet_verify(data_packet_definition* packet_definition)
{
	ASSERT(packet_definition);
	ASSERT(packet_definition->size >= 0);
	ASSERT(packet_definition->version >= 0);
	ASSERT(packet_definition->name && packet_definition->fields);
	
	if (!packet_definition->initialized_flag)
	{
		int16 packet_size = 0;
		int16 field_count = 0;
		_data_packet_verify(packet_definition, &packet_size, packet_definition->fields, &field_count);

		VASSERT(packet_size == packet_definition->size, c_string_builder("packet '%s' fields added up to #%d bytes but should have been #%d bytes.",
			packet_definition->name,
			packet_size,
			packet_definition->size).get_string());
	
		packet_definition->initialized_flag = 1;
	}
}

void __cdecl _data_packet_encode(data_packet_definition* packet_definition, data_encoding_state* encode_state, int16 version, void* original_buffer, int16* byte_count_reference, data_packet_field* first_field, int16* field_count_reference)
{
	// $IMPLEMENT
}

unsigned char __cdecl data_packet_encode(data_packet_definition* packet_definition, int16 version, void* data, void* buffer, int16* buffer_size, int16 maximum_buffer_size)
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
		t_value_type<byte> data = { .value = static_cast<byte>(version) };
		data_encode_memory(&state, data.bytes, sizeof(data.bytes), _1byte);
	}

	_data_packet_encode(packet_definition, &state, version, data, nullptr, packet_definition->fields, nullptr);
	*buffer_size = static_cast<int16>(state.offset);

	return state.overflow_flag == 0;
}

