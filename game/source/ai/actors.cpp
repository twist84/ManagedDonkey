#include "ai/actors.hpp"

void __cdecl actor_iterator_new(actor_iterator* iterator, bool a2)
{
	INVOKE(0x0142C950, actor_iterator_new, iterator, a2);
}

actor_datum* __cdecl actor_iterator_next(actor_iterator* iterator)
{
	return INVOKE(0x0142C990, actor_iterator_next, iterator);
}


