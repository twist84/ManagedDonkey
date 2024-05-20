#include "ai/actors.hpp"

#include "memory/thread_local.hpp"

void __cdecl actor_iterator_new(actor_iterator* iterator, bool a2)
{
	//INVOKE(0x0142C950, actor_iterator_new, iterator, a2);

	TLS_DATA_GET_VALUE_REFERENCE(actor_data);
	TLS_DATA_GET_VALUE_REFERENCE(ai_globals);
	
	if (ai_globals->__unknown1)
	{
		iterator->begin(*actor_data);
		iterator->__unknown10 = a2;
	}
}

actor_datum* __cdecl actor_iterator_next(actor_iterator* iterator)
{
	//return INVOKE(0x0142C990, actor_iterator_next, iterator);
	
	TLS_DATA_GET_VALUE_REFERENCE(ai_globals);

	actor_datum* actor = NULL;
	if (ai_globals->__unknown1)
	{
		while (iterator->next())
		{
			if (!iterator->__unknown10 || actor_is_active(iterator->m_datum))
			{
				actor = iterator->m_datum;
				break;
			}
		}
		iterator->index = iterator->get_index();
	}

	ASSERT(((iterator->index == NONE) && (actor == NULL)) || ((iterator->index != NONE) && (actor != NULL)));

	return actor;
}

bool actor_datum_available_to_current_thread()
{
	TLS_DATA_GET_VALUE_REFERENCE(actor_data);
	return *actor_data != NULL;
}

bool actor_is_active(actor_datum const* actor)
{
	return actor->meta.active;
}

