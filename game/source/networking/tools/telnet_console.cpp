#include "networking/tools/telnet_console.hpp"

#include "cseries/cseries.hpp"

void __cdecl telnet_console_initialize()
{
	INVOKE(0x005301A0, telnet_console_initialize);
}

void __cdecl telnet_console_dispose()
{
	INVOKE(0x005301B0, telnet_console_dispose);
}

