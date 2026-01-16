#pragma once

#include "cseries/cseries.hpp"
#include "cseries/language.hpp"
#include "memory/secure_signature.hpp"
#include "tag_files/tag_groups.hpp"
#include "text/unicode.hpp"

struct s_string_reference
{
	int32 id;
	int32 offset;
};
COMPILE_ASSERT(sizeof(s_string_reference) == 0x8);

class c_language_pack
{
public:
	s_string_reference* m_string_references;
	void* m_string_data;
	int32 m_num_of_strings;
	int32 m_string_data_size;
	int32 m_string_reference_cache_offset;
	int32 m_string_data_cache_offset;
	s_network_http_request_hash m_string_reference_checksum;
	s_network_http_request_hash m_string_data_checksum;
	bool m_data_loaded;
};
COMPILE_ASSERT(sizeof(c_language_pack) == 0x44);

struct s_language_pack_offsets
{
	int16 start_index;
	int16 string_count;
};
COMPILE_ASSERT(sizeof(s_language_pack_offsets) == 0x4);

struct s_multilingual_unicode_string_reference
{
	int32 id;
	char string[k_tag_string_length];
	int32 offset[k_language_count];
};
COMPILE_ASSERT(sizeof(s_multilingual_unicode_string_reference) == 0x54);

struct s_multilingual_unicode_string_list_group_header
{
	c_typed_tag_block<s_multilingual_unicode_string_reference> string_references;
	s_tag_data text_data;
	s_language_pack_offsets language_pack_offsets[k_language_count];
};
COMPILE_ASSERT(sizeof(s_multilingual_unicode_string_list_group_header) == 0x50);

extern bool __cdecl string_list_get_interface_string(int32 tag_index, int32 string, c_static_wchar_string<1024>* buffer);
extern bool __cdecl string_list_get_normal_string(int32 definition_index, int32 id, c_static_wchar_string<256>* buffer);

