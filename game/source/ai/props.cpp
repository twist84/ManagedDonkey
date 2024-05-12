#include "ai/props.hpp"

void __cdecl actor_prop_ref_iterator_new(actor_prop_ref_iterator* iterator, long actor_index)
{
	INVOKE(0x014AAEC0, actor_prop_ref_iterator_new, iterator, actor_index);
}

prop_ref_datum* __cdecl actor_prop_ref_iterator_next(actor_prop_ref_iterator* iterator)
{
	return INVOKE(0x014AAF00, actor_prop_ref_iterator_next, iterator);
}

