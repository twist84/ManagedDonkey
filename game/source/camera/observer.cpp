#include "camera/observer.hpp"

#include "memory/thread_local.hpp"

s_observer* observer_get(long user_index)
{
	s_thread_local_storage* tls = get_tls();
	if (!tls || !tls->g_observer_globals)
		return nullptr;

	return &tls->g_observer_globals->observers[user_index];
}