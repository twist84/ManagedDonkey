#include "config/version.hpp"

#include "cseries/cseries.hpp"

long __cdecl version_get_build_number()
{
	return DECLTHUNK(0x00501420, version_get_build_number);
}
