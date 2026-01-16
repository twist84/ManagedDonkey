#pragma once

#include "memory/bitstream.hpp"

#define k_network_message_maximum_size 0x40000

class c_network_message_type_collection
{
public:
	using encode_t = void __cdecl(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	using decode_t = bool __cdecl(c_bitstream* packet, int32 message_storage_size, void* message_storage);
	using compare_t = bool __cdecl(int32, void*, void*);
	using dispose_t = void __cdecl(int32, void*);

	struct s_network_message_type
	{
		bool initialized;
		const char* message_type_name;
		uns32 flags;
		int32 message_size;
		int32 message_size_maximum;

		encode_t* encode_function;
		decode_t* decode_function;
		compare_t* compare_function;
		dispose_t* dispose_function;
	};
	COMPILE_ASSERT(sizeof(s_network_message_type) == 0x24);

public:
	void __thiscall clear_message_types_();
	bool __thiscall decode_message_(c_bitstream* packet, e_network_message_type* message_type, int32* message_storage_size, void* message_storage);
	bool __thiscall decode_message_header_(c_bitstream* packet, e_network_message_type* message_type, int32* message_storage_size);
	void __thiscall dispose_message_(e_network_message_type message_type, int32 message_storage_size, void* message_storage);
	void __thiscall encode_message_(c_bitstream* packet, e_network_message_type message_type, int32 message_storage_size, void* message_storage);
	void __thiscall encode_message_header_(c_bitstream* packet, e_network_message_type message_type, int32 message_storage_size);
	const char* __thiscall get_message_type_name_(e_network_message_type message_type);

	void __thiscall register_message_type_(
		e_network_message_type message_type,
		const char* message_type_name,
		int32 flags,
		int32 message_size,
		int32 message_size_maximum,
		encode_t* encode_function,
		decode_t* decode_function,
		compare_t* compare_function,
		dispose_t* dispose_function
	);

	void __cdecl check_message_types() const;
	void __cdecl clear_message_types();
	bool __cdecl decode_message(c_bitstream* packet, e_network_message_type* message_type, int32* message_storage_size, void* message_storage) const;
	bool __cdecl decode_message_header(c_bitstream* packet, e_network_message_type* message_type, int32* message_storage_size) const;
	void __cdecl dispose_message(e_network_message_type message_type, int32 message_storage_size, void* message_storage) const;
	void __cdecl encode_message(c_bitstream* packet, e_network_message_type message_type, int32 message_storage_size, const void* message_storage) const;
	void __cdecl encode_message_header(c_bitstream* packet, e_network_message_type message_type, int32 message_storage_size) const;
	const char* __cdecl get_message_type_name(e_network_message_type message_type) const;

	void __cdecl register_message_type(
		e_network_message_type message_type,
		const char* message_type_name,
		int32 flags,
		int32 message_size,
		int32 message_size_maximum,
		encode_t* encode_function,
		decode_t* decode_function,
		compare_t* compare_function,
		dispose_t* dispose_function
	);

protected:
	s_network_message_type m_message_types[k_network_message_type_count];
};
COMPILE_ASSERT(sizeof(c_network_message_type_collection) == 0x57C
	+ (sizeof(c_network_message_type_collection::s_network_message_type) * k_custom_network_message_count));

