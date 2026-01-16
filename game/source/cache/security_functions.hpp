#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"

enum e_secure_memory_region
{
	_secure_memory_region_cache_file = 0,
	_secure_memory_region_network_storage_manifest,

	k_secure_memory_region_count,

	k_secure_memory_cache_file_size = 0x3C00,
	k_secure_memory_network_storage_manifest_size = 0x6400,
	k_secure_memory_size = k_secure_memory_cache_file_size + k_secure_memory_network_storage_manifest_size
};
COMPILE_ASSERT(k_secure_memory_size == 0xA000);

struct s_secure_memory_region
{
	int32 offset;
	int32 size;
};
COMPILE_ASSERT(sizeof(s_secure_memory_region) == 0x8);

extern s_secure_memory_region(&k_secure_memory_regions)[k_secure_memory_region_count];
extern int32& g_secure_memory_size;
extern void*& g_secure_memory;

extern int32 __cdecl hash_compare_function(const void* in_a, const void* in_b, const void* compare_data);
extern int32 __cdecl rsa_signature_compare_function(const void* in_a, const void* in_b, const void* compare_data);
extern bool __cdecl security_calculate_hash(const void* buffer, uns32 buffer_size, bool use_secret_hash_salt, s_network_http_request_hash* hash);
extern void __cdecl security_dispose();
extern void __cdecl security_get_working_memory(e_secure_memory_region region, void** out_working_memory, int32* out_working_memory_size);
extern bool __cdecl security_hash_manifest_find_hash(const s_network_http_request_hash* hash, const void* manifest, uns32 manifest_size);
extern bool __cdecl security_incremental_hash_begin(void* working_memory, int32 working_memory_size, bool use_secret_hash_salt);
extern void __cdecl security_incremental_hash_finish(void* working_memory, int32 working_memory_size, s_network_http_request_hash* hash);
extern void __cdecl security_incremental_hash_update(void* working_memory, int32 working_memory_size, const void* buffer, uns32 buffer_size);
extern void __cdecl security_initialize();
extern char* __cdecl security_print_hash(const s_network_http_request_hash* hash, char* buffer, int32 buffer_length);
extern char* __cdecl security_print_rsa_signature(const s_rsa_signature* rsa_signature, char* buffer, int32 buffer_length);
extern bool __cdecl security_rsa_compute_and_verify_signature(const s_network_http_request_hash* hash, s_rsa_signature* rsa_signature);
extern bool __cdecl security_rsa_manifest_find_signature(const s_rsa_signature* rsa_signature, const void* manifest, uns32 manifest_size);
extern bool __cdecl security_validate_hash(const void* buffer, uns32 buffer_size, bool use_secret_hash_salt, const s_network_http_request_hash* hash, s_network_http_request_hash* out_actual_hash);

