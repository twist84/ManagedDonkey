#include "ai/squads.hpp"

void squad_actor_iterator_new(squad_actor_iterator* iterator, long squad_index, bool a3)
{
	INVOKE(0x0148F590, squad_actor_iterator_new, iterator, squad_index, a3);
}

actor_datum* squad_actor_iterator_next(squad_actor_iterator* iterator)
{
	return INVOKE(0x0148F600, squad_actor_iterator_next, iterator);
}

