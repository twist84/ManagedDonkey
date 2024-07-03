#pragma once

#define DATA_PACKET_GROUP_DEFINITION(_name, _packet_type_count, _packet_class_count, _maximum_decoded_packet_size, _maximum_encoded_packet_size, _packets) \
data_packet_group_definition _name =                             \
{                                                                \
	.name = #_name,                                              \
	.packet_type_count = _packet_type_count,                     \
	.packet_class_count = _packet_class_count,                   \
	.maximum_decoded_packet_size = _maximum_decoded_packet_size, \
	.maximum_encoded_packet_size = _maximum_encoded_packet_size, \
	.packets = _packets                                          \
}

struct data_packet_group_packet;
struct data_packet_group_definition
{
	char const* name;
	short packet_type_count;
	short packet_class_count;
	long maximum_decoded_packet_size;
	long maximum_encoded_packet_size;
	data_packet_group_packet* packets;
};

struct packet_header
{
	char type;
};

extern unsigned char __cdecl data_packet_group_encode_packet(data_packet_group_definition* group_definition, void* a2, void* encoded_packet, short* encoded_packet_size, short packet_type, short version);

