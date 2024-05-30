#include "networking/online/online_presence_pc.hpp"

#include "cseries/cseries.hpp"

void __cdecl online_rich_presence_dispose()
{
	INVOKE(0x004E46D0, online_rich_presence_dispose);
}

void __cdecl online_rich_presence_initialize()
{
	INVOKE(0x004E46E0, online_rich_presence_initialize);
}

void __cdecl online_rich_presence_update()
{
	INVOKE(0x004E46F0, online_rich_presence_update);
}

