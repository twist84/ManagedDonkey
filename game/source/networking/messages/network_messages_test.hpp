#pragma once

struct s_network_message_test
{
	bool in_sequence;
	int32 id;
};
static_assert(sizeof(s_network_message_test) == 0x8);

struct c_bitstream;

class c_network_message_test
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

struct c_network_message_type_collection;
extern void __cdecl network_message_types_register_test(c_network_message_type_collection* message_collection);

