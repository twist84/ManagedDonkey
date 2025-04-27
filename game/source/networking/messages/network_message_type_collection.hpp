#pragma once

#include "memory/bitstream.hpp"

#define k_network_message_maximum_size 0x40000

struct c_network_message_type_collection
{
	using encode_t = void __cdecl(c_bitstream* packet, long message_storage_size, void const* message_storage);
	using decode_t = bool __cdecl(c_bitstream* packet, long message_storage_size, void* message_storage);
	using compare_t = bool __cdecl(long, void*, void*);
	using dispose_t = void __cdecl(long, void*);

	struct s_network_message_type
	{
		bool initialized;
		char const* message_type_name;
		uint32 flags;
		long message_size;
		long message_size_maximum;

		encode_t* encode_function;
		decode_t* decode_function;
		compare_t* compare_function;
		dispose_t* dispose_function;
	};
	static_assert(sizeof(s_network_message_type) == 0x24);

public:
	void __thiscall clear_message_types_();
	bool __thiscall decode_message_(c_bitstream* packet, e_network_message_type* message_type, long* message_storage_size, void* message_storage);
	bool __thiscall decode_message_header_(c_bitstream* packet, e_network_message_type* message_type, long* message_storage_size);
	void __thiscall dispose_message_(e_network_message_type message_type, long message_storage_size, void* message_storage);
	void __thiscall encode_message_(c_bitstream* packet, e_network_message_type message_type, long message_storage_size, void* message_storage);
	void __thiscall encode_message_header_(c_bitstream* packet, e_network_message_type message_type, long message_storage_size);
	char const* __thiscall get_message_type_name_(e_network_message_type message_type);

	void __thiscall register_message_type_(
		e_network_message_type message_type,
		char const* message_type_name,
		long flags,
		long message_size,
		long message_size_maximum,
		encode_t* encode_function,
		decode_t* decode_function,
		compare_t* compare_function,
		dispose_t* dispose_function
	);

	void __cdecl check_message_types() const;
	void __cdecl clear_message_types();
	bool __cdecl decode_message(c_bitstream* packet, e_network_message_type* message_type, long* message_storage_size, void* message_storage) const;
	bool __cdecl decode_message_header(c_bitstream* packet, e_network_message_type* message_type, long* message_storage_size) const;
	void __cdecl dispose_message(e_network_message_type message_type, long message_storage_size, void* message_storage) const;
	void __cdecl encode_message(c_bitstream* packet, e_network_message_type message_type, long message_storage_size, void const* message_storage) const;
	void __cdecl encode_message_header(c_bitstream* packet, e_network_message_type message_type, long message_storage_size) const;
	char const* __cdecl get_message_type_name(e_network_message_type message_type) const;

	void __cdecl register_message_type(
		e_network_message_type message_type,
		char const* message_type_name,
		long flags,
		long message_size,
		long message_size_maximum,
		encode_t* encode_function,
		decode_t* decode_function,
		compare_t* compare_function,
		dispose_t* dispose_function
	);

protected:
	s_network_message_type m_message_types[k_network_message_type_count];
};
static_assert(sizeof(c_network_message_type_collection) == 0x57C
	+ (sizeof(c_network_message_type_collection::s_network_message_type) * k_custom_network_message_count));

