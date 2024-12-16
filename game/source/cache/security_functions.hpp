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
static_assert(k_secure_memory_size == 0xA000);

struct s_secure_memory_region
{
	long offset;
	long size;
};
static_assert(sizeof(s_secure_memory_region) == 0x8);

extern s_secure_memory_region(&k_secure_memory_regions)[k_secure_memory_region_count];
extern long& g_secure_memory_size;
extern void*& g_secure_memory;

extern long __cdecl hash_compare_function(void const* in_a, void const* in_b, void const* compare_data);
extern long __cdecl rsa_signature_compare_function(void const* in_a, void const* in_b, void const* compare_data);
extern bool __cdecl security_calculate_hash(void const* buffer, dword buffer_size, bool use_secret_hash_salt, s_network_http_request_hash* hash);
extern void __cdecl security_dispose();
extern void __cdecl security_get_working_memory(e_secure_memory_region region, void** out_working_memory, long* out_working_memory_size);
extern bool __cdecl security_hash_manifest_find_hash(s_network_http_request_hash const* hash, void const* manifest, dword manifest_size);
extern bool __cdecl security_incremental_hash_begin(void* working_memory, long working_memory_size, bool use_secret_hash_salt);
extern void __cdecl security_incremental_hash_finish(void* working_memory, long working_memory_size, s_network_http_request_hash* hash);
extern void __cdecl security_incremental_hash_update(void* working_memory, long working_memory_size, void const* buffer, dword buffer_size);
extern void __cdecl security_initialize();
extern char* __cdecl security_print_hash(s_network_http_request_hash const* hash, char* buffer, long buffer_size);
extern char* __cdecl security_print_rsa_signature(s_rsa_signature const* rsa_signature, char* buffer, long buffer_size);
extern bool __cdecl security_rsa_compute_and_verify_signature(s_network_http_request_hash const* hash, s_rsa_signature* rsa_signature);
extern bool __cdecl security_rsa_manifest_find_signature(s_rsa_signature const* rsa_signature, void const* manifest, dword manifest_size);
extern bool __cdecl security_validate_hash(void const* buffer, dword buffer_size, bool use_secret_hash_salt, s_network_http_request_hash const* hash, s_network_http_request_hash* out_actual_hash);

