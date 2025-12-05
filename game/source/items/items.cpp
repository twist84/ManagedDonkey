#include "items/items.hpp"

#include "cseries/cseries.hpp"

bool __cdecl dangerous_items_near_player(int32* out_item_index)
{
	return INVOKE(0x00BAB880, dangerous_items_near_player, out_item_index);
}

//.text:00BAB8E0 ; 
//.text:00BAB8F0 ; void __cdecl item_accelerate(int32, const real_vector3d*)
//.text:00BAB940 ; void __cdecl item_adjust_gravity(int32, real_vector3d*)
//.text:00BABB50 ; void __cdecl item_adjust_scale_and_variant(int32, int32)
//.text:00BABCB0 ; void __cdecl item_attach_to_marker(int32, int32, int32, int32)
//.text:00BABCD0 ; void __cdecl item_attach_to_node(int32, int32, int16)
//.text:00BABCF0 ; void __cdecl item_detach(int32)
//.text:00BABD10 ; void __cdecl item_detonate(int32)
//.text:00BABE20 ; void __cdecl item_fix_position(int32)
//.text:00BAC1B0 ; void __cdecl item_get_position_even_if_in_inventory(int32, real_point3d*)

void __cdecl item_hide_inventory(int32 item_index)
{
	INVOKE(0x00BAC210, item_hide_inventory, item_index);
}

//.text:00BAC270 ; bool __cdecl item_in_hand(int32)

void __cdecl item_in_unit_inventory(int32 item_index, int32 owner_unit_index)
{
	INVOKE(0x00BAC2B0, item_in_unit_inventory, item_index, owner_unit_index);
}

//.text:00BAC510 ; void __cdecl item_maintain_z_up(int32)
//.text:00BAC690 ; void __cdecl item_mark_relevance(int32)
//.text:00BAC6D0 ; void __cdecl item_move(int32)
//.text:00BAC6F0 ; void __cdecl item_multiplayer_at_rest_state_initialize(int32)
//.text:00BAC7C0 ; bool __cdecl item_new(int32, object_placement_data*, bool*)
//.text:00BAC840 ; void __cdecl item_show_inventory(int32, int32, int32, int32)
//.text:00BAC8D0 ; void __cdecl item_show_inventory_unattached(int32, int32)
//.text:00BAC920 ; void __cdecl item_stow_inventory(int32, int32, int32, int32)
//.text:00BAC980 ; bool __cdecl item_stowed(int32)
//.text:00BAC9C0 ; void __cdecl item_unstow_inventory(int32, int32, int32, int32)
//.text:00BACA40 ; bool __cdecl item_update(int32)
//.text:00BACF40 ; void __cdecl item_update_player_inventory(int32)

