#include "cache/cache_file_builder_security.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x00601BF0, cache_file_builder_security_clean_header);

void __cdecl cache_file_builder_security_clean_header(s_cache_file_header* out_header)
{
	//INVOKE(0x00601BF0, cache_file_builder_security_clean_header, out_header);

	csmemset(&out_header->__unknown318, 0, sizeof(out_header->__unknown318));
	csmemset(&out_header->hash, 0, sizeof(out_header->hash));
	csmemset(&out_header->rsa_signature, 0, sizeof(out_header->rsa_signature));
	csmemset(&out_header->source_file, 0, sizeof(out_header->source_file));
	csmemset(&out_header->modification_date, 0, sizeof(out_header->modification_date));
	csmemset(&out_header->creation_time, 0, sizeof(out_header->creation_time));
	csmemset(&out_header->shared_file_times, 0, sizeof(out_header->shared_file_times));
	csmemset(&out_header->shared_resource_usage, 0, 0x10);
}

