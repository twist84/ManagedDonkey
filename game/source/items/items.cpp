#include "items/items.hpp"

#include "cseries/cseries.hpp"

bool __cdecl dangerous_items_near_player(long* out_item_index)
{
	return INVOKE(0x00BAB880, dangerous_items_near_player, out_item_index);
}

