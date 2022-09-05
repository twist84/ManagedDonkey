#include "cache/security_functions.hpp"


bool __cdecl security_validate_hash(void const* buffer, unsigned long buffer_size, bool calculate, s_network_http_request_hash const* hash, s_network_http_request_hash* out_hash)
{
	bool result = DECLTHUNK(0x00509020, security_validate_hash, buffer, buffer_size, calculate, hash, out_hash);

	return result;
}

bool __cdecl security_calculate_hash(void const* buffer, unsigned long buffer_size, bool calculate, s_network_http_request_hash* hash)
{
	bool result = DECLTHUNK(0x00508D30, security_calculate_hash, buffer, buffer_size, calculate, hash);

	return result;
}

bool __cdecl security_rsa_compute_and_verify_signature(s_network_http_request_hash const* hash, s_rsa_signature* rsa_signature)
{
	bool result = DECLTHUNK(0x00508F80, security_rsa_compute_and_verify_signature, hash, rsa_signature);

	return result;
}