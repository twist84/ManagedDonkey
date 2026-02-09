#include "ai/squads.hpp"

#include "ai/actors.hpp"
#include "memory/thread_local.hpp"

//.text:0148F000 ; 
//.text:0148F0A0 ; 
//.text:0148F0C0 ; 
//.text:0148F160 ; bool __cdecl starting_location_emitter_is_usable(int16, int16, int16)
//.text:0148F280 ; 
//.text:0148F2D0 ; bool __cdecl squad_activate(int32)
//.text:0148F3C0 ; int32 __cdecl squad_actor_adopt_squad(int32, int16)

void squad_actor_iterator_new(squad_actor_iterator* iterator, int32 squad_index, bool active_only)
{
	INVOKE(0x0148F590, squad_actor_iterator_new, iterator, squad_index, active_only);
}

actor_datum* squad_actor_iterator_next(squad_actor_iterator* iterator)
{
	return INVOKE(0x0148F600, squad_actor_iterator_next, iterator);
}

//.text:0148F680 ; void __cdecl squad_add_vehicle(int32, int32)
//.text:0148F6D0 ; real64 __cdecl squad_adoption_distance_to_actor(int32, int32, int16)
//.text:0148F7F0 ; bool __cdecl squad_ancestor_of_squad_group(int32, int32)
//.text:0148F870 ; bool __cdecl squad_area_iterator_new(squad_area_iterator*, int16, int16, int16, bool, e_pureform)
//.text:0148FB30 ; int16 __cdecl squad_area_iterator_next(squad_area_iterator*)
//.text:0148FDA0 ; bool __cdecl squad_arrived(int32)
//.text:0148FDE0 ; void __cdecl squad_attach_actor(int32, int16, bool)
//.text:0148FF10 ; void __cdecl squad_build_base_spawn_definition(int32, int16, actor_spawn_definition*)
//.text:01490000 ; void __cdecl squad_build_inhibition_vector(c_static_flags<209>*, int16)
//.text:01490090 ; void __cdecl squad_clear_arrived(int32)
//.text:014900E0 ; void __cdecl squad_clear_create_options(squad_create_options*)
//.text:01490100 ; bool __cdecl squad_consider_major_upgrade(int16, real32)
//.text:01490200 ; void __cdecl squad_create(int32, const squad_create_options*)
//.text:01490580 ; int16 __cdecl squad_create_from_spawn_definitions(int32, const squad_create_options*, c_static_stack<actor_spawn_definition, 256>*)

void __cdecl squad_deactivate(int32 squad_index)
{
	INVOKE(0x01490900, squad_deactivate, squad_index);
}

//.text:01490980 ; void __cdecl squad_detach_actor(int32, bool)
//.text:01490AF0 ; bool __cdecl squad_force_activate(int32)
//.text:01490B40 ; void __cdecl squad_force_deactivate(int32)

squad_datum* __cdecl squad_get(int32 squad_index)
{
	squad_datum* squad = DATUM_GET(squad_data, squad_datum, squad_index);
	return squad;
}

//.text:01490B80 ; int32 __cdecl squad_get_by_name(const char*)
//.text:01490BA0 ; int32 __cdecl squad_get_by_name(s_scenario*, const char*)
//.text:01490C10 ; 
//.text:01490E70 ; 
//.text:01490F00 ; 
//.text:01490F60 ; int32 __cdecl squad_get_leader(int16)
//.text:01490FC0 ; 
//.text:01491000 ; style_definition* __cdecl squad_get_style(int32)
//.text:01491050 ; s_task* __cdecl squad_get_task(const squad_datum*)
//.text:01491090 ; void __cdecl squad_group_activate(int32)
//.text:014910C0 ; void __cdecl squad_group_add_group_child(int32, int32)
//.text:01491110 ; void __cdecl squad_group_add_leaf_child(int32, int32)
//.text:01491180 ; int32 __cdecl squad_group_get_by_name(const char*)
//.text:014911A0 ; int32 __cdecl squad_group_get_by_name(s_scenario*, const char*)
//.text:01491210 ; void __cdecl squad_group_iterator_new(squad_group_iterator*)
//.text:01491250 ; squad_group_datum* __cdecl squad_group_iterator_next(squad_group_iterator*)
//.text:014912A0 ; void __cdecl squad_group_new(const squad_group_definition*)
//.text:01491320 ; void __cdecl squad_group_squad_iterator_new(squad_group_squad_iterator*, int32)
//.text:01491390 ; squad_datum* __cdecl squad_group_squad_iterator_next(squad_group_squad_iterator*)
//.text:01491460 ; void __cdecl squad_group_test_activation(int32)
//.text:01491530 ; void __cdecl squad_group_update_status(int32)
//.text:01491760 ; void __cdecl squad_groups_test_activation()
//.text:014917F0 ; void __cdecl squad_groups_update_status()
//.text:01491880 ; bool __cdecl squad_in_vehicles(int32)
//.text:014918D0 ; bool __cdecl squad_is_blind(int32)
//.text:01491950 ; bool __cdecl squad_is_deaf(int32)

void __cdecl squad_iterator_new(squad_iterator* iterator)
{
	INVOKE(0x014919D0, squad_iterator_new, iterator);
}

squad_datum* __cdecl squad_iterator_next(squad_iterator* iterator)
{
	return INVOKE(0x01491A10, squad_iterator_next, iterator);
}

//.text:01491A60 ; void __cdecl squad_load_in_squad(int32, int32)
//.text:01491CF0 ; void __cdecl squad_new(const s_squad_definition*)
//.text:01491F80 ; int32 __cdecl squad_place_actor(int32, const actor_spawn_definition*)
//.text:01491FB0 ; int32 __cdecl squad_placement_create_vehicle(int32, const actor_spawn_definition*, int32*, int16*)
//.text:01492170 ; int16 __cdecl squad_placement_find_seat(int32, int16, int32*, int16*)
//.text:014922D0 ; int32 __cdecl squad_placement_find_vehicle(const actor_spawn_definition*, int32, int16*)
//.text:014923C0 ; void __cdecl squad_remove_vehicle(int32, int32)
//.text:01492450 ; 
//.text:014924C0 ; 
//.text:014924E0 ; 
//.text:014925D0 ; 
//.text:01492D10 ; void __cdecl squad_set_blind(int32, bool)
//.text:01492D60 ; void __cdecl squad_set_braindead(int32, bool)
//.text:01492DB0 ; void __cdecl squad_set_deaf(int32, bool)
//.text:01492E00 ; void __cdecl squad_set_vehicle(int32, bool)
//.text:01492E50 ; squad_should_use_dynamic_areas?
//.text:01492EC0 ; bool __cdecl squad_task_broken(int32)
//.text:01492F30 ; bool __cdecl squad_test_activation_cluster_bit_vector(int32, const s_game_cluster_bit_vectors*)
//.text:01492FB0 ; 
//.text:01493010 ; void __cdecl squad_update_disengaged_status(int32)
//.text:014930F0 ; void __cdecl squad_update_highest_rank(int16)
//.text:01493180 ; void __cdecl squad_update_status(int32)
//.text:01493520 ; bool __cdecl squad_update_style(int32, int16)
//.text:014936C0 ; bool __cdecl squad_using_fronts(const squad_datum*)
//.text:01493750 ; void __cdecl squad_vehicle_change_squad(int32, int32)
//.text:01493810 ; bool __cdecl squad_verify_script_references(int32, bool)
//.text:01493820 ; void __cdecl squadless_attach_actor(int32)
//.text:01493880 ; void __cdecl squadless_detach_actor(int32)
//.text:014938F0 ; void __cdecl squads_build_hierarchy()
//.text:01493B40 ; void __cdecl squads_create_for_new_map()

void __cdecl squads_dispose()
{
	INVOKE(0x01493BE0, squads_dispose);
}

void __cdecl squads_dispose_from_old_map()
{
	INVOKE(0x01493BF0, squads_dispose_from_old_map);
}

//.text:01493C20 ; void __cdecl squads_handle_deleted_unit(int32)

void __cdecl squads_initialize()
{
	INVOKE(0x01493CB0, squads_initialize);
}

void __cdecl squads_initialize_for_new_map()
{
	INVOKE(0x01493D50, squads_initialize_for_new_map);
}

//.text:01493E60 ; void __cdecl squads_test_activation()

void __cdecl squads_update()
{
	INVOKE(0x014941B0, squads_update);
}

void __cdecl squads_update_dirty_status()
{
	INVOKE(0x014942B0, squads_update_dirty_status);
}

