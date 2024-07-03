#pragma once

//#define DATA_PACKET_DEFINITION(_name, _flags, _size, _version, _initialized_flag, ...) \
//data_packet_definition _name =            \
//{                                         \
//	.name = #_name,                       \
//	.flags = _flags,                      \
//	.size = _size,                        \
//	.version = _version,                  \
//	.fields = { __VA_ARGS__ },            \
//	.initialized_flag = _initialized_flag \
//}

#define DATA_PACKET_DEFINITION(_name, _size, _version, ...) \
data_packet_definition _name = \
{                              \
	.name = #_name,            \
	.size = _size,             \
	.version = _version,       \
	.fields = { __VA_ARGS__ }  \
}

#define DATA_PACKET_GROUP_PACKET(_packet_class, _definition) \
{                                  \
	.packet_class = _packet_class, \
	.definition = &_definition     \
}

struct data_packet_field
{
	short type;
	short count;
	short first_version;
	short last_version;
	short size;
};

struct data_packet_definition
{
	const char* name;
	unsigned long flags;
	short size;
	short version;
	data_packet_field* fields;
	unsigned char initialized_flag;
};

struct data_packet_group_packet
{
	short packet_class;
	data_packet_definition* definition;
};

extern void __cdecl data_packet_verify(data_packet_definition*);
extern unsigned char __cdecl data_packet_encode(data_packet_definition* packet_definition, short version, void* a3, void* buffer, short* buffer_size, short maximum_buffer_size);

