#include "networking/tools/xbox_connection_manager.hpp"

#include "cseries/cseries.hpp"

void __cdecl xbox_connection_dispose()
{
	INVOKE(0x014E2AB0, xbox_connection_dispose);
}

void __cdecl xbox_connection_initialize()
{
	INVOKE(0x014E2AC0, xbox_connection_initialize);
}

