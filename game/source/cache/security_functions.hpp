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

extern bool __cdecl security_validate_hash(void const* buffer, dword buffer_size, bool calculate, s_network_http_request_hash const* hash, s_network_http_request_hash* out_hash);
extern bool __cdecl security_calculate_hash(void const* buffer, dword buffer_size, bool calculate, s_network_http_request_hash* hash);
extern bool __cdecl security_rsa_compute_and_verify_signature(s_network_http_request_hash const* hash, s_rsa_signature* rsa_signature);
extern void __cdecl security_get_working_memory(long region, void** out_working_memory, long* out_working_memory_size);