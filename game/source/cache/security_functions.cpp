#include "cache/security_functions.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE_ARRAY(0x01650460, s_secure_memory_region, k_secure_memory_regions, k_secure_memory_region_count);
REFERENCE_DECLARE(0x022B7FA4, int32, g_secure_memory_size);
REFERENCE_DECLARE(0x022B7FA8, void*, g_secure_memory);

HOOK_DECLARE(0x00508DB0, security_get_working_memory);
HOOK_DECLARE(0x00508F80, security_rsa_compute_and_verify_signature);

bool const override_scenario_load_security_rsa_compute_and_verify_signature = true;

int32 __cdecl hash_compare_function(void const* in_a, void const* in_b, void const* compare_data)
{
	return INVOKE(0x00508C90, hash_compare_function, in_a, in_b, compare_data);
}

int32 __cdecl rsa_signature_compare_function(void const* in_a, void const* in_b, void const* compare_data)
{
	return INVOKE(0x00508CE0, hash_compare_function, in_a, in_b, compare_data);
}

bool __cdecl security_calculate_hash(void const* buffer, uns32 buffer_size, bool use_secret_hash_salt, s_network_http_request_hash* hash)
{
	bool result = INVOKE(0x00508D30, security_calculate_hash, buffer, buffer_size, use_secret_hash_salt, hash);

	return result;
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

	if (g_secure_memory_size < k_secure_memory_size)
	{
		event(_event_critical, "security: failed to get working memory");
		*out_working_memory = NULL;
		*out_working_memory_size = 0;
	}
	else
	{
		*out_working_memory = offset_pointer(g_secure_memory, k_secure_memory_regions[region].offset);
		*out_working_memory_size = k_secure_memory_regions[region].size;
	}
}

bool __cdecl security_hash_manifest_find_hash(s_network_http_request_hash const* hash, void const* manifest, uns32 manifest_size)
{
	return INVOKE(0x00508E00, security_hash_manifest_find_hash, hash, manifest, manifest_size);
}

bool __cdecl security_incremental_hash_begin(void* working_memory, int32 working_memory_size, bool use_secret_hash_salt)
{
	return INVOKE(0x00508E50, security_incremental_hash_begin, working_memory, working_memory_size, use_secret_hash_salt);
}

void __cdecl security_incremental_hash_finish(void* working_memory, int32 working_memory_size, s_network_http_request_hash* hash)
{
	INVOKE(0x00508E80, security_incremental_hash_finish, working_memory, working_memory_size, hash);
}

void __cdecl security_incremental_hash_update(void* working_memory, int32 working_memory_size, void const* buffer, uns32 buffer_size)
{
	INVOKE(0x00508EA0, security_incremental_hash_update, working_memory, working_memory_size, buffer, buffer_size);
}

void __cdecl security_initialize()
{
	//INVOKE(0x00508EC0, security_initialize);

	g_secure_memory = system_malloc(k_secure_memory_size);
	g_secure_memory_size = k_secure_memory_size;
}

char* __cdecl security_print_hash(s_network_http_request_hash const* hash, char* buffer, int32 buffer_size)
{
	return INVOKE(0x00508EE0, security_print_hash, hash, buffer, buffer_size);
}

char* __cdecl security_print_rsa_signature(s_rsa_signature const* rsa_signature, char* buffer, int32 buffer_size)
{
	return INVOKE(0x00508F30, security_print_rsa_signature, rsa_signature, buffer, buffer_size);
}

bool __cdecl security_rsa_compute_and_verify_signature(s_network_http_request_hash const* hash, s_rsa_signature* rsa_signature)
{
	//return INVOKE(0x00508F80, security_rsa_compute_and_verify_signature, hash, rsa_signature);

	bool result = false;
	HOOK_INVOKE(result =, security_rsa_compute_and_verify_signature, hash, rsa_signature);

	if (!result && override_scenario_load_security_rsa_compute_and_verify_signature)
		return true;

	return result;
}

bool __cdecl security_rsa_manifest_find_signature(s_rsa_signature const* rsa_signature, void const* manifest, uns32 manifest_size)
{
	return INVOKE(0x00508FE0, security_rsa_manifest_find_signature, rsa_signature, manifest, manifest_size);
}

bool __cdecl security_validate_hash(void const* buffer, uns32 buffer_size, bool use_secret_hash_salt, s_network_http_request_hash const* hash, s_network_http_request_hash* out_actual_hash)
{
	bool result = INVOKE(0x00509020, security_validate_hash, buffer, buffer_size, use_secret_hash_salt, hash, out_actual_hash);

	return result;
}

