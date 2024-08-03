#include "ai/squads.hpp"

#include "ai/actors.hpp"

//.text:0148F000 ; 
//.text:0148F0A0 ; 
//.text:0148F0C0 ; 
//.text:0148F160 ; 
//.text:0148F280 ; 
//.text:0148F2D0 ; bool __cdecl squad_activate(long)
//.text:0148F3C0 ; long __cdecl squad_actor_adopt_squad(long, short)

void squad_actor_iterator_new(squad_actor_iterator* iterator, long squad_index, bool a3)
{
	INVOKE(0x0148F590, squad_actor_iterator_new, iterator, squad_index, a3);
}

actor_datum* squad_actor_iterator_next(squad_actor_iterator* iterator)
{
	return INVOKE(0x0148F600, squad_actor_iterator_next, iterator);
}

//.text:0148F680 ; 
//.text:0148F6D0 ; double __cdecl squad_adoption_distance_to_actor(long, long, short)
//.text:0148F7F0 ; 
//.text:0148F870 ; bool __cdecl squad_area_iterator_new(squad_area_iterator*, short, short, short, bool, e_pureform)
//.text:0148FB30 ; short __cdecl squad_area_iterator_next(squad_area_iterator*)
//.text:0148FDA0 ; 
//.text:0148FDE0 ; void __cdecl squad_attach_actor(long, short, bool)
//.text:0148FF10 ; 
//.text:01490000 ; squad_build_inhibition_vector
//.text:01490090 ; 
//.text:014900E0 ; void __cdecl squad_clear_create_options(squad_create_options*)
//.text:01490100 ; bool __cdecl squad_consider_major_upgrade(short, real)
//.text:01490200 ; void __cdecl squad_create(long, squad_create_options const*)
//.text:01490580 ; squad_create_from_spawn_definitions
//.text:01490900 ; void __cdecl squad_deactivate(long)
//.text:01490980 ; void __cdecl squad_detach_actor(long, bool)
//.text:01490AF0 ; bool __cdecl squad_force_activate(long)
//.text:01490B40 ; void __cdecl squad_force_deactivate(long)
//.text:01490B80 ; 
//.text:01490BA0 ; long __cdecl squad_get_by_name(s_scenario*, char const*)
//.text:01490C10 ; 
//.text:01490E70 ; 
//.text:01490F00 ; 
//.text:01490F60 ; long __cdecl squad_get_leader(short)
//.text:01490FC0 ; 
//.text:01491000 ; style_definition* __cdecl squad_get_style(long)
//.text:01491050 ; s_task* __cdecl squad_get_task(squad_datum const*)
//.text:01491090 ; 
//.text:014910C0 ; 
//.text:01491110 ; 
//.text:01491180 ; 
//.text:014911A0 ; long __cdecl squad_group_get_by_name(s_scenario*, char const*)
//.text:01491210 ; 
//.text:01491250 ; 
//.text:014912A0 ; 
//.text:01491320 ; 
//.text:01491390 ; 
//.text:01491460 ; void __cdecl squad_group_test_activation(long)
//.text:01491530 ; void __cdecl squad_group_update_status(long)
//.text:01491760 ; void __cdecl squad_groups_test_activation()
//.text:014917F0 ; void __cdecl squad_groups_update_status()
//.text:01491880 ; 
//.text:014918D0 ; 
//.text:01491950 ; bool __cdecl squad_is_deaf(long)

void __cdecl squad_iterator_new(squad_iterator* iterator)
{
	INVOKE(0x014919D0, squad_iterator_new, iterator);
}

squad_datum* __cdecl squad_iterator_next(squad_iterator* iterator)
{
	return INVOKE(0x01491A10, squad_iterator_next, iterator);
}

//.text:01491A60 ; void __cdecl squad_load_in_squad(long, long)
//.text:01491CF0 ; void __cdecl squad_new(s_squad_definition const*)
//.text:01491F80 ; 
//.text:01491FB0 ; long __cdecl squad_placement_create_vehicle(long, actor_spawn_definition const*, long*, short*)
//.text:01492170 ; short __cdecl squad_placement_find_seat(long, short, long*, short*)
//.text:014922D0 ; long __cdecl squad_placement_find_vehicle(actor_spawn_definition const*, long, short*)
//.text:014923C0 ; void __cdecl squad_remove_vehicle(long, long)
//.text:01492450 ; 
//.text:014924C0 ; 
//.text:014924E0 ; 
//.text:014925D0 ; 
//.text:01492D10 ; void __cdecl squad_set_blind(long, bool)
//.text:01492D60 ; void __cdecl squad_set_braindead(long, bool)
//.text:01492DB0 ; void __cdecl squad_set_deaf(long, bool)
//.text:01492E00 ; void __cdecl squad_set_vehicle(long, bool)
//.text:01492E50 ; squad_should_use_dynamic_areas?
//.text:01492EC0 ; bool __cdecl squad_task_broken(long)
//.text:01492F30 ; bool __cdecl squad_test_activation_cluster_bit_vector(long, s_game_cluster_bit_vectors const*)
//.text:01492FB0 ; 
//.text:01493010 ; void __cdecl squad_update_disengaged_status(long)
//.text:014930F0 ; squad_update_highest_rank
//.text:01493180 ; squad_update_status

