#pragma once

#define DATA_PACKET_FIELD(_type, _count, _first_version, _last_version, _size) \
data_packet_field _name =           \
{                                   \
	.type = _type                   \
	.count = _count                 \
	.first_version = _first_version \
	.last_version = _last_version   \
	.size = _size                   \
}

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
	int16 type;
	int16 count;
	int16 first_version;
	int16 last_version;
	int16 size;
};

struct data_packet_definition
{
	const char* name;
	uns32 flags;
	int16 size;
	int16 version;
	data_packet_field* fields;
	unsigned char initialized_flag;
};

struct data_packet_group_packet
{
	int16 packet_class;
	data_packet_definition* definition;
};

extern void __cdecl data_packet_verify(data_packet_definition*);
extern unsigned char __cdecl data_packet_encode(data_packet_definition* packet_definition, int16 version, void* a3, void* buffer, int16* buffer_size, int16 maximum_buffer_size);

