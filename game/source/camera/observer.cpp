#include "camera/observer.hpp"

#include "memory/thread_local.hpp"

bool g_debug_observer_render = false;

s_observer* observer_get(long user_index)
{
	ASSERT(user_index >= 0 && user_index < 4);

	if (!get_tls())
		return nullptr;

	TLS_DATA_GET_VALUE_REFERENCE(g_observer_globals);
	return g_observer_globals ? &g_observer_globals->observers[user_index] : nullptr;
}

s_observer_result const* observer_get_camera(long output_user_index)
{
	s_observer* observer = observer_get(output_user_index);
	ASSERT(observer);

	return &observer->result;
}

//e_director_mode __cdecl choose_appropriate_director(long output_user_index)
long __cdecl choose_appropriate_director(long output_user_index)
{
	return INVOKE(0x005916B0, choose_appropriate_director, output_user_index);
}

s_observer_result const* __cdecl observer_try_and_get_camera(long output_user_index)
{
	return INVOKE(0x00613960, observer_try_and_get_camera, output_user_index);
}

