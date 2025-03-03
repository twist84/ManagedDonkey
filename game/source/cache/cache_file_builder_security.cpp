#include "cache/cache_file_builder_security.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x00601BF0, cache_file_builder_security_clean_header);

void __cdecl cache_file_builder_security_clean_header(s_cache_file_header* out_header)
{
	//INVOKE(0x00601BF0, cache_file_builder_security_clean_header, out_header);

	csmemset(&out_header->signature_marker, 0, sizeof(out_header->signature_marker));
	csmemset(out_header->content_hashes, 0, sizeof(out_header->content_hashes));
	csmemset(&out_header->rsa_signature, 0, sizeof(out_header->rsa_signature));
	csmemset(&out_header->path, 0, sizeof(out_header->path));
	csmemset(&out_header->slot_modification_date, 0, sizeof(out_header->slot_modification_date));
	csmemset(&out_header->creation_date, 0, sizeof(out_header->creation_date));
	csmemset(&out_header->shared_creation_date, 0, sizeof(out_header->shared_creation_date));
	csmemset(&out_header->shared_resource_usage.shared_layout_identifier, 0, sizeof(out_header->shared_resource_usage.shared_layout_identifier));
}

