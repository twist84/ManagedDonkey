#include "config/version.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"

long __cdecl version_get_build_number()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00501420, version_get_build_number);
}
