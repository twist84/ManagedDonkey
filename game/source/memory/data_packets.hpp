#pragma once

#define DATA_PACKET_DEFINITION(_name, _size, _version, ...) \
data_packet_definition _name = \
{                              \
	.name = #_name,            \
	.size = _size,             \
	.version = _version,       \
	.fields = { __VA_ARGS__ }  \
}

#define DATA_PACKET(_packet_class, _definition) \
{                                  \
	.packet_class = _packet_class, \
	.definition = &_definition     \
}

struct data_packet_field
{
	short type;
	short count;
	short __unknown4;
	short __unknown6;
	short __unknown8;
};

struct data_packet_definition
{
	const char* name;
	unsigned char __data[0x4];
	short size;
	short version;
	data_packet_field* fields;
	bool __unknown;
};

struct data_packet
{
	short packet_class;
	data_packet_definition* definition;
};

extern void __cdecl data_packet_verify(data_packet_definition*);
extern unsigned char __cdecl data_packet_encode(data_packet_definition* packet_definition, short version, void* a3, void* buffer, short* buffer_size, short maximum_buffer_size);

