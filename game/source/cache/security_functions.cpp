#include "cache/security_functions.hpp"

#include "cseries/console.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x00508F80, security_rsa_compute_and_verify_signature);

bool __cdecl security_validate_hash(void const* buffer, dword buffer_size, bool calculate, s_network_http_request_hash const* hash, s_network_http_request_hash* out_hash)
{
	FUNCTION_BEGIN(true);

	bool result = INVOKE(0x00509020, security_validate_hash, buffer, buffer_size, calculate, hash, out_hash);

	return result;
}

bool __cdecl security_calculate_hash(void const* buffer, dword buffer_size, bool calculate, s_network_http_request_hash* hash)
{
	FUNCTION_BEGIN(true);

	bool result = INVOKE(0x00508D30, security_calculate_hash, buffer, buffer_size, calculate, hash);

	return result;
}

const bool override_scenario_load_security_rsa_compute_and_verify_signature = true;

bool __cdecl security_rsa_compute_and_verify_signature(s_network_http_request_hash const* hash, s_rsa_signature* rsa_signature)
{
	FUNCTION_BEGIN(true);

	bool result = false;
	HOOK_INVOKE(result =, security_rsa_compute_and_verify_signature, hash, rsa_signature);

	if (!result && override_scenario_load_security_rsa_compute_and_verify_signature)
		return true;

	return result;
}

//void security_get_working_memory(enum e_secure_memory_region, void**, long*);
void __cdecl security_get_working_memory(long region, void** out_working_memory, long* out_working_memory_size)
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00508DB0, security_get_working_memory, region, out_working_memory, out_working_memory_size);
}
