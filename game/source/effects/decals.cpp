#include "effects/decals.hpp"

#include "game/game.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CALL(0x006947FE, sub_6948C0);

bool disable_sub_6948C0 = true;

void __cdecl c_decal_system::prepare_for_new_zone_set(dword old_structure_bsp_mask, dword new_structure_bsp_mask)
{
	INVOKE(0x005302D0, prepare_for_new_zone_set, old_structure_bsp_mask, new_structure_bsp_mask);
}

void __cdecl c_decal_system::initialize_for_new_non_bsp_zone_set(s_game_non_bsp_zone_set const* new_non_bsp_zone_set)
{
	INVOKE(0x005302E0, initialize_for_new_non_bsp_zone_set, new_non_bsp_zone_set);
}

void __cdecl c_decal_system::dispose_from_old_non_bsp_zone_set(s_game_non_bsp_zone_set const* old_non_bsp_zone_set)
{
	INVOKE(0x005302F0, dispose_from_old_non_bsp_zone_set, old_non_bsp_zone_set);
}

void __cdecl c_decal_system::change_pvs(s_game_cluster_bit_vectors const* a1, s_game_cluster_bit_vectors const* a2, bool a3)
{
	INVOKE(0x0068FBB0, change_pvs, a1, a2, a3);
}

void __cdecl c_decal_system::dispose()
{
	INVOKE(0x00691B00, dispose);
}

void __cdecl c_decal_system::dispose_from_old_map()
{
	INVOKE(0x00691CD0, dispose_from_old_map);
}

void __cdecl c_decal_system::dispose_from_old_structure_bsp(dword old_structure_bsp_mask)
{
	INVOKE(0x00691D70, dispose_from_old_structure_bsp, old_structure_bsp_mask);
}

void __cdecl c_decal_system::initialize()
{
	INVOKE(0x006931F0, initialize);
}

void __cdecl c_decal_system::initialize_for_new_map()
{
	INVOKE(0x006936C0, initialize_for_new_map);
}

void __cdecl c_decal_system::initialize_for_new_structure_bsp(dword new_structure_bsp_mask)
{
	INVOKE(0x00693740, initialize_for_new_structure_bsp, new_structure_bsp_mask);
}

void __cdecl c_decal_system::prepare_for_non_bsp_zone_set_switch(s_game_non_bsp_zone_set const* old_non_bsp_zone_set, s_game_non_bsp_zone_set const* new_non_bsp_zone_set, c_scenario_resource_registry* pending_zone_registry)
{
	INVOKE(0x006941B0, prepare_for_non_bsp_zone_set_switch, old_non_bsp_zone_set, new_non_bsp_zone_set, pending_zone_registry);
}

void __cdecl sub_6948C0(long a1)
{
	if (disable_sub_6948C0)
		return;

	INVOKE(0x006948C0, sub_6948C0, a1);
}


