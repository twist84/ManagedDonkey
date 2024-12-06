#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "text/unicode.hpp"

struct s_string_reference
{
	long id;
	long offset;
};
static_assert(sizeof(s_string_reference) == 0x8);

struct c_language_pack
{
	s_string_reference* m_string_references;
	void* m_string_data;
	long m_num_of_strings;
	long m_string_data_size;
	long m_string_reference_cache_offset;
	long m_string_data_cache_offset;
	s_network_http_request_hash m_string_reference_checksum;
	s_network_http_request_hash m_string_data_checksum;
	bool m_data_loaded;
};
static_assert(sizeof(c_language_pack) == 0x44);

extern bool __cdecl string_list_get_normal_string(long definition_index, long id, c_static_wchar_string<256>* buffer);

