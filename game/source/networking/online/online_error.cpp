#include "networking/online/online_error.hpp"

#include "memory/module.hpp"
#include "networking/transport/transport_endpoint_set_winsock.hpp"

#include <windows.h>

HOOK_DECLARE(0x00434580, online_error_get_string);

struct s_online_error_entry
{
	dword error;
	char const* error_string;
};

extern s_online_error_entry k_windows_error_table[1515];
extern s_online_error_entry k_com_error_table[925];
extern s_online_error_entry k_online_error_table[167];

void add_error(char const* error, c_static_string<256>* string)
{
	if (!string->is_empty())
		string->append(",");

	string->append(error);
}

c_static_string<256> const __cdecl online_error_get_string(dword error)
{
	//INVOKE(0x00434580, online_error_get_string, error);

	c_static_string<256> string;
	string.clear();

	//for (long i = 0; i < 1515; i++)
	//{
	//	if (k_windows_error_table[i].error == error)
	//		add_error(k_windows_error_table[i].error_string, &string);
	//}
	//
	//for (long i = 0; i < 925; i++)
	//{
	//	if (k_com_error_table[i].error == error)
	//		add_error(k_com_error_table[i].error_string, &string);
	//}
	//
	//for (long i = 0; i < 167; i++)
	//{
	//	if (k_online_error_table[i].error == error)
	//		add_error(k_online_error_table[i].error_string, &string);
	//}
	//
	//if (winsock_error_to_string_no_default(error))
	//{
	//	add_error(winsock_error_to_string_no_default(error), &string);
	//}

	if (string.is_empty())
	{
		string.print("0x%08X (unknown)", error);
	}
	else
	{
		string.print("0x%08X=%s", error, string.get_string());
	}

	return string;
}

#define MAKE_ERROR(_ERROR) { (dword)_ERROR, #_ERROR }

s_online_error_entry k_windows_error_table[1515]
{
	MAKE_ERROR(ERROR_INVALID_FUNCTION),

	// ...

	MAKE_ERROR(ERROR_IO_INCOMPLETE),
	MAKE_ERROR(ERROR_IO_PENDING),
	MAKE_ERROR(ERROR_CANCELLED),
	MAKE_ERROR(ERROR_FUNCTION_FAILED),

	// ...
};

s_online_error_entry k_com_error_table[925]
{
	MAKE_ERROR(E_OUTOFMEMORY),

	// ...
};

s_online_error_entry k_online_error_table[167]
{
	//MAKE_ERROR(XONLINE_E_NO_SESSION),

	// ...
};

