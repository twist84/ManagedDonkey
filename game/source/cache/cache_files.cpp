#include "cache/cache_files.hpp"

#include "cache/cache_file_builder_security.hpp"
#include "cache/security_functions.hpp"
#include "cseries/cseries_windows.hpp"
#include "memory/module.hpp"

#include <string.h>

s_cache_file_globals& g_cache_file_globals = *reinterpret_cast<s_cache_file_globals*>(0x022AAFE8);

struct s_cache_file_global_tags_definition
{
	struct
	{
		long count;
		struct
		{
			tag group_tag;
			long name_offset;
			long name_length;
			long index;
		}*address;
		void* definition;
	} references;
};

long __cdecl cache_file_get_global_tag_index(tag group_tag)
{
	//s_cache_file_global_tags_definition* global_tags = tag_get<s_cache_file_global_tags_definition>('cfgt', 0);
	//if (!global_tags)
	//	return -1;
	//for (long i = 0; i < global_tags->references.count; i++)
	//	if (group_tag == global_tags->references.address[i].group_tag)
	//		return global_tags->references.address[i].index;
	//return -1;

	return DECLTHUNK(0x005017E0, cache_file_get_global_tag_index, group_tag);
}

const bool override_cache_file_header_security_validate_hash = true;
const bool override_cache_file_header_security_rsa_compute_and_verify_signature = true;

bool __cdecl cache_files_verify_header_rsa_signature(s_cache_file_header* header)
{
	if (header->header_signature != 'head' || header->footer_signature != 'foot')
	{
		display_debug_string("cache_files:header is invalid, header/footer signatures are bad");
		return false;
	}

	s_cache_file_header clean_header{};
	memmove(&clean_header, header, sizeof(s_cache_file_header));
	cache_file_builder_security_clean_header(&clean_header);

	s_network_http_request_hash hash{};
	if (!security_validate_hash(&clean_header, sizeof(s_cache_file_header), true, &header->hash, &hash))
	{
		if (!override_cache_file_header_security_validate_hash)
		{
			display_debug_string("cache_files:header failed hash verification - possible cheating?");
			return false;
		}

		char* hash_string = nullptr;
		type_as_byte_string(&hash, &hash_string);
		display_debug_string("cache_files:header: failed hash verification - copying new validated values, %s", hash_string);
		delete[] hash_string;

		memcpy(&header->hash, &hash, sizeof(s_network_http_request_hash));
	}

	security_calculate_hash(&header->hash, sizeof(s_network_http_request_hash), true, &hash);

	s_rsa_signature rsa_signature{};
	memcpy(&rsa_signature, &header->rsa_signature, sizeof(s_rsa_signature));

	bool result = security_rsa_compute_and_verify_signature(&hash, &rsa_signature);
	if (!result)
	{
		if (!override_cache_file_header_security_rsa_compute_and_verify_signature)
		{
			display_debug_string("cache_files:header failed RSA signature verification - possible cheating?");
			return false;
		}
	}
	return true;

	//return DECLTHUNK(0x00502210, cache_files_verify_header_rsa_signature, header);
}