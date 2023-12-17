#include "cseries/async.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x022B4818, s_async_globals, g_async_globals);

bool __cdecl async_usable()
{
	return INVOKE(0x00508780, async_usable);
}

void __cdecl internal_async_yield_until_done(c_synchronized_long* done, bool with_idle, bool with_spinner, char const* file, long line)
{
	INVOKE(0x00508A20, internal_async_yield_until_done, done, with_idle, with_spinner, file, line);
}

