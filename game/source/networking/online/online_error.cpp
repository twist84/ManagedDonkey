#include "networking/online/online_error.hpp"

#include "memory/module.hpp"

HOOK_DECLARE(0x00434580, online_error_get_string);

c_static_string<256> const __cdecl online_error_get_string(dword error)
{
	static c_static_string<256> result;
	HOOK_INVOKE(result =, online_error_get_string, error);
	return result;
}
