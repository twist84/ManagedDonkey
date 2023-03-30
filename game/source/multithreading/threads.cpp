#include "multithreading/threads.hpp"

#include "cseries/cseries.hpp"

bool __cdecl is_async_thread()
{
	return INVOKE(0x0051C620, is_async_thread);
}

