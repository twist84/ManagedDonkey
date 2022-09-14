#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"

extern bool __cdecl security_validate_hash(void const* buffer, dword buffer_size, bool calculate, s_network_http_request_hash const* hash, s_network_http_request_hash* out_hash);
extern bool __cdecl security_calculate_hash(void const* buffer, dword buffer_size, bool calculate, s_network_http_request_hash* hash);
extern bool __cdecl security_rsa_compute_and_verify_signature(s_network_http_request_hash const* hash, s_rsa_signature* rsa_signature);
extern void __cdecl security_get_working_memory(long region, void** out_working_memory, long* out_working_memory_size);