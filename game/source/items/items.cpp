#include "items/items.hpp"

#include "cseries/cseries.hpp"

bool __cdecl dangerous_items_near_player(long* out_item_index)
{
	return INVOKE(0x00BAB880, dangerous_items_near_player, out_item_index);
}

//.text:00BAB8E0 ; 
//.text:00BAB8F0 ; void __cdecl item_accelerate(long, vector3d const*)
//.text:00BAB940 ; void __cdecl item_adjust_gravity(long, vector3d*)
//.text:00BABB50 ; void __cdecl item_adjust_scale_and_variant(long, long)
//.text:00BABCB0 ; void __cdecl item_attach_to_marker(long, long, long, long)
//.text:00BABCD0 ; void __cdecl item_attach_to_node(long, long, short)
//.text:00BABCF0 ; void __cdecl item_detach(long)
//.text:00BABD10 ; void __cdecl item_detonate(long)
//.text:00BABE20 ; void __cdecl item_fix_position(long)
//.text:00BAC1B0 ; void __cdecl item_get_position_even_if_in_inventory(long, real_point3d*)
//.text:00BAC210 ; void __cdecl item_hide_inventory(long)
//.text:00BAC270 ; bool __cdecl item_in_hand(long)
//.text:00BAC2B0 ; void __cdecl item_set_inventory_ownership(long, long)
//.text:00BAC510 ; void __cdecl item_maintain_z_up(long)
//.text:00BAC690 ; void __cdecl item_mark_relevance(long)
//.text:00BAC6D0 ; void __cdecl item_move(long)
//.text:00BAC6F0 ; void __cdecl item_multiplayer_at_rest_state_initialize(long)
//.text:00BAC7C0 ; bool __cdecl item_new(long, object_placement_data*, bool*)
//.text:00BAC840 ; void __cdecl item_show_inventory(long, long, long, long)
//.text:00BAC8D0 ; void __cdecl item_show_inventory_unattached(long, long)
//.text:00BAC920 ; void __cdecl item_stow_inventory(long, long, long, long)
//.text:00BAC980 ; bool __cdecl item_stowed(long)
//.text:00BAC9C0 ; void __cdecl item_unstow_inventory(long, long, long, long)
//.text:00BACA40 ; bool __cdecl item_update(long)
//.text:00BACF40 ; void __cdecl item_update_player_inventory(long)

