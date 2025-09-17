#include "cache/security_functions.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE_ARRAY(0x01650460, s_secure_memory_region, k_secure_memory_regions, k_secure_memory_region_count);
REFERENCE_DECLARE(0x022B7FA4, int32, g_secure_memory_size);
REFERENCE_DECLARE(0x022B7FA8, void*, g_secure_memory);

HOOK_DECLARE(0x00508C90, hash_compare_function);
HOOK_DECLARE(0x00508CE0, rsa_signature_compare_function);
HOOK_DECLARE(0x00508D30, security_calculate_hash);
HOOK_DECLARE(0x00508D80, security_dispose);
HOOK_DECLARE(0x00508DB0, security_get_working_memory);
HOOK_DECLARE(0x00508E00, security_hash_manifest_find_hash);
HOOK_DECLARE(0x00508E50, security_incremental_hash_begin);
HOOK_DECLARE(0x00508E80, security_incremental_hash_finish);
HOOK_DECLARE(0x00508EA0, security_incremental_hash_update);
HOOK_DECLARE(0x00508EC0, security_initialize);
HOOK_DECLARE(0x00508EE0, security_print_hash);
HOOK_DECLARE(0x00508F30, security_print_rsa_signature);
HOOK_DECLARE(0x00508F80, security_rsa_compute_and_verify_signature);
HOOK_DECLARE(0x00508FE0, security_rsa_manifest_find_signature);
HOOK_DECLARE(0x00509020, security_validate_hash);

int32 __cdecl hash_compare_function(const void* in_a, const void* in_b, const void* compare_data)
{
	//return INVOKE(0x00508C90, hash_compare_function, in_a, in_b, compare_data);

	return csmemcmp(in_a, in_b, sizeof(s_network_http_request_hash));
}

int32 __cdecl rsa_signature_compare_function(const void* in_a, const void* in_b, const void* compare_data)
{
	//return INVOKE(0x00508CE0, rsa_signature_compare_function, in_a, in_b, compare_data);

	return csmemcmp(in_a, in_b, sizeof(s_rsa_signature));
}

bool __cdecl security_calculate_hash(const void* buffer, uns32 buffer_size, bool use_secret_hash_salt, s_network_http_request_hash* hash)
{
	//return INVOKE(0x00508D30, security_calculate_hash, buffer, buffer_size, use_secret_hash_salt, hash);

	ASSERT(hash);

	*hash = {};

	return true;
}

void __cdecl security_dispose()
{
	//INVOKE(0x00508D80, security_dispose);

	if (g_secure_memory)
	{
		system_free(g_secure_memory);
		g_secure_memory = NULL;
	}
	g_secure_memory_size = 0;
}

void __cdecl security_get_working_memory(e_secure_memory_region region, void** out_working_memory, int32* out_working_memory_size)
{
	//INVOKE(0x00508DB0, security_get_working_memory, region, out_working_memory, out_working_memory_size);

	ASSERT(VALID_INDEX(region, k_secure_memory_region_count));
	ASSERT(out_working_memory);
	ASSERT(out_working_memory_size);
	ASSERT(k_secure_memory_regions[region].size > 0);
	ASSERT(k_secure_memory_regions[region].offset >= 0 && k_secure_memory_regions[region].offset + k_secure_memory_regions[region].size <= k_secure_memory_size);
	ASSERT(g_secure_memory);

	void* working_memory = NULL;
	int32 working_memory_size = 0;

	if (g_secure_memory_size >= k_secure_memory_size)
	{
		working_memory = offset_pointer(g_secure_memory, k_secure_memory_regions[region].offset);
		working_memory_size = k_secure_memory_regions[region].size;
	}
	else
	{
		event(_event_critical, "security: failed to get working memory");
	}

	*out_working_memory = working_memory;
	*out_working_memory_size = working_memory_size;
}

bool __cdecl security_hash_manifest_find_hash(const s_network_http_request_hash* hash, const void* manifest, uns32 manifest_size)
{
	//return INVOKE(0x00508E00, security_hash_manifest_find_hash, hash, manifest, manifest_size);

	ASSERT(hash);

#if 1
	return true;
#else
	bool found = false;
	if (manifest_size)
	{
		ASSERT(manifest);

		int32 manifest_count = manifest_size / sizeof(s_network_http_request_hash);
		if (manifest_size == sizeof(s_network_http_request_hash) * manifest_count)
		{
			int32 manifest_index = bsearch_elements(hash, manifest, manifest_count, sizeof(s_network_http_request_hash), hash_compare_function, NULL);
			found = manifest_index != NONE;
		}
	}
	else
	{
		event(_event_error, "security: hash manifest size %d is not a multiple of hash size %d",
			manifest_size,
			sizeof(s_network_http_request_hash));
	}
	return found;
#endif
}

bool __cdecl security_incremental_hash_begin(void* working_memory, int32 working_memory_size, bool use_secret_hash_salt)
{
	//return INVOKE(0x00508E50, security_incremental_hash_begin, working_memory, working_memory_size, use_secret_hash_salt);

	ASSERT(working_memory);

	csmemset(working_memory, 0, working_memory_size);
	return true;
}

void __cdecl security_incremental_hash_finish(void* working_memory, int32 working_memory_size, s_network_http_request_hash* hash)
{
	//INVOKE(0x00508E80, security_incremental_hash_finish, working_memory, working_memory_size, hash);

	ASSERT(hash);

	*hash = {};
}

void __cdecl security_incremental_hash_update(void* working_memory, int32 working_memory_size, const void* buffer, uns32 buffer_size)
{
	//INVOKE(0x00508EA0, security_incremental_hash_update, working_memory, working_memory_size, buffer, buffer_size);
}

void __cdecl security_initialize()
{
	//INVOKE(0x00508EC0, security_initialize);

	g_secure_memory = system_malloc(k_secure_memory_size);
	g_secure_memory_size = k_secure_memory_size;
}

char* __cdecl security_print_hash(const s_network_http_request_hash* hash, char* buffer, int32 buffer_length)
{
	//return INVOKE(0x00508EE0, security_print_hash, hash, buffer, buffer_length);

	csstrnzcpy(buffer, "", buffer_length);
	for (int32 byte_index = 0; byte_index < sizeof(s_network_http_request_hash); byte_index++)
	{
		csnzappendf(buffer, buffer_length, "%02X", hash->hash[byte_index]);
	}
	return buffer;
}

char* __cdecl security_print_rsa_signature(const s_rsa_signature* rsa_signature, char* buffer, int32 buffer_length)
{
	//return INVOKE(0x00508F30, security_print_rsa_signature, rsa_signature, buffer, buffer_length);

	csstrnzcpy(buffer, "", buffer_length);
	for (int32 byte_index = 0; byte_index < sizeof(s_rsa_signature); byte_index++)
	{
		csnzappendf(buffer, buffer_length, "%02X", rsa_signature->signature[byte_index]);
	}
	return buffer;
}

bool __cdecl security_rsa_compute_and_verify_signature(const s_network_http_request_hash* hash, s_rsa_signature* rsa_signature)
{
	//return INVOKE(0x00508F80, security_rsa_compute_and_verify_signature, hash, rsa_signature);

	ASSERT(rsa_signature);

	return true;
}

bool __cdecl security_rsa_manifest_find_signature(const s_rsa_signature* rsa_signature, const void* manifest, uns32 manifest_size)
{
	//return INVOKE(0x00508FE0, security_rsa_manifest_find_signature, rsa_signature, manifest, manifest_size);

	ASSERT(rsa_signature);

#if 1
	return true;
#else
	bool found = false;
	if (manifest_size)
	{
		ASSERT(manifest);

		int32 manifest_count = manifest_size >> 8;
		int32 manifest_index = bsearch_elements(rsa_signature, manifest, manifest_count, sizeof(s_rsa_signature), rsa_signature_compare_function, NULL);
		found = manifest_index != NONE;
	}
	else
	{
		event(_event_error, "security: rsa signature manifest size %d is not a multiple of signature size %d",
			manifest_size,
			sizeof(s_rsa_signature));
	}
	return found;
#endif
}

bool __cdecl security_validate_hash(const void* buffer, uns32 buffer_size, bool use_secret_hash_salt, const s_network_http_request_hash* hash, s_network_http_request_hash* out_actual_hash)
{
	//return INVOKE(0x00509020, security_validate_hash, buffer, buffer_size, use_secret_hash_salt, hash, out_actual_hash);

	ASSERT(buffer);
	ASSERT(hash);

#if 1
	return true;
#else
	s_network_http_request_hash calculated_hash{};

	bool success = csmemcmp(&calculated_hash, hash, sizeof(s_network_http_request_hash)) == 0;

	if (out_actual_hash)
	{
		*out_actual_hash = {};
	}

	return success;
#endif
}

