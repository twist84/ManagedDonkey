#pragma once

#include "memory/bitstream.hpp"

class c_network_session_parameter_type_collection
{
public:
	using encode_t = void __cdecl(c_bitstream*, const void*, int32);
	using decode_t = bool __cdecl(c_bitstream*, void*, int32);

	struct s_network_session_parameter_type
	{
		bool initialized;
		const char* session_parameter_name;

		encode_t* update_encode;
		decode_t* update_decode;
		int32 update_size;

		encode_t* change_request_encode;
		decode_t* change_request_decode;
		int32 change_request_size;
	};

public:
	void check_session_parameter_types() const;
	void clear_session_parameter_types();
	void register_session_parameter_type(
		e_network_session_parameter_type session_parameter_type,
		const char* session_parameter_name, 
		encode_t* update_encode,
		decode_t* update_decode,
		int32 update_size, 
		encode_t* change_request_encode,
		decode_t* change_request_decode,
		int32 change_request_size
	);

protected:
	s_network_session_parameter_type m_session_parameter_types[k_network_session_parameter_type_count];
};
static_assert(sizeof(c_network_session_parameter_type_collection) == 0x620);

