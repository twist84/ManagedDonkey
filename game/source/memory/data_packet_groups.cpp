#include "memory/data_packet_groups.hpp"

#include "cseries/cseries.hpp"
#include "memory/data_packets.hpp"


char const* data_packet_groups_error;

unsigned char data_packet_group_append_packet_header(data_packet_group_definition* group_definition, void* encoded_packet, short* encoded_packet_size, short packet_type)
{
	byte* encoded_packet_type = static_cast<byte*>(encoded_packet) + *encoded_packet_size;

	ASSERT(encoded_packet && encoded_packet_size);
	ASSERT(*encoded_packet_size >= 0);
	ASSERT(packet_type >= 0 && packet_type < group_definition->packet_type_count);

	char const* error = nullptr;
	if (*encoded_packet_size + 1 >= group_definition->maximum_encoded_packet_size)
	{
		error = "couldn't append header to encoded packet";
	}
	else
	{
		*encoded_packet_type = static_cast<byte>(packet_type);
		++*encoded_packet_size;
	}
	data_packet_groups_error = error;

	return error == nullptr;
}

unsigned char __cdecl data_packet_group_encode_packet(data_packet_group_definition* group_definition, void* a2, void* encoded_packet, short* encoded_packet_size, short packet_type, short version)
{
	ASSERT(group_definition);
	ASSERT(packet_type >= 0 && packet_type < group_definition->packet_type_count);
	ASSERT(encoded_packet && encoded_packet_size);

	data_packet_group_packet* packet = &group_definition->packets[packet_type];
	ASSERT(packet->definition);

	char const* error = nullptr;
	if (data_packet_encode(packet->definition, version, a2, encoded_packet, encoded_packet_size, static_cast<short>(group_definition->maximum_encoded_packet_size)))
	{
		if (!data_packet_group_append_packet_header(group_definition, encoded_packet, encoded_packet_size, packet_type))
			error = data_packet_groups_error;
	}
	else
	{
		error = "couldn't encode packet";
	}
	data_packet_groups_error = error;

	return error == nullptr;
}

