#include "ai/actors.hpp"

#include "memory/thread_local.hpp"

void __cdecl actor_iterator_new(actor_iterator* iterator, bool a2)
{
	INVOKE(0x0142C950, actor_iterator_new, iterator, a2);
}

actor_datum* __cdecl actor_iterator_next(actor_iterator* iterator)
{
	return INVOKE(0x0142C990, actor_iterator_next, iterator);
}

bool actor_datum_available_to_current_thread()
{
	TLS_DATA_GET_VALUE_REFERENCE(actor_data);
	return *actor_data != NULL;
}

