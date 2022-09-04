#pragma once

#include "memory/bitstream.hpp"

enum e_network_message_type
{
	k_network_message_type_count = 39
};

struct c_network_message_type
{
	bool initialized;
	char const* message_type_name;
	long flags;
	long message_size;
	long message_size_maximum;
	void (*encode_function)(c_bitstream*, long, void const*);
	bool (*decode_function)(c_bitstream*, long, void*);
	bool (*compare_function)(long, void*, void*);
	void (*dispose_function)(long, void*);
};
static_assert(sizeof(c_network_message_type) == 0x24);

struct c_network_message_type_collection
{
	c_network_message_type m_message_types[k_network_message_type_count];
};
static_assert(sizeof(c_network_message_type_collection) == 0x57C);