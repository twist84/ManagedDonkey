#include "networking/online/online.hpp"

#include "cseries/console.hpp"
#include "memory/module.hpp"

#include <winsock.h>

HOOK_DECLARE(0x00442BF0, online_update);

s_online_user& g_online_user = *reinterpret_cast<s_online_user*>(0x019AB728);

void __cdecl online_update()
{
	FUNCTION_BEGIN(false);

	char name[264];
	gethostname(name, 256);
	wsprintfW(g_online_user.player_name, L"%S", name);
}
