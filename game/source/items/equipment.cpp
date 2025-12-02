#include "items/equipment.hpp"

//.text:00B86440 ; void __cdecl sub_B86440(equipment_datum*, int32, void*(__cdecl*)(void*), void*(__cdecl*)(void*))
//.text:00B865A0 ; void __cdecl sub_B865A0(equipment_datum*, int32, void*(__cdecl*)(void*), void*(__cdecl*)(void*))
//.text:00B86700 ; void __cdecl sub_B86700(equipment_datum*, int32)
//.text:00B86BC0 ; void __cdecl sub_B86BC0(int32)
//.text:00B86C20 ; bool __cdecl equipment_activate(int32, int32, bool)
//.text:00B87DA0 ; real64 __cdecl equipment_active_fraction(int32)
//.text:00B87E60 ; 
//.text:00B87EA0 ; 
//.text:00B87EC0 ; bool __cdecl equipment_begin_activation_animation(int32)
//.text:00B87FD0 ; bool __cdecl equipment_begin_animation_state(int32, int32, int32, uns32)
//.text:00B88040 ; void __cdecl equipment_calculate_noise_maker_blip(int32, uns32*, int32, real_point2d*)
//.text:00B88160 ; int32 __cdecl equipment_calculate_noise_maker_blip_count(int32)
//.text:00B881A0 ; void __cdecl equipment_calculate_spawn_location(int32, real32, real32, real32, int16, real_point3d*, real_vector3d*)
//.text:00B882F0 ; bool __cdecl equipment_can_be_thrown(int32)
//.text:00B88360 ; bool __cdecl equipment_compute_function_value(int32, int32, int32, real32*, bool*, bool*)
//.text:00B88470 ; void __cdecl equipment_delete_recursive(int32, int32)

void __cdecl equipment_definition_handle_pickup(int32 player_index, int32 equipment_definition_index)
{
	INVOKE(0x00B887B0, equipment_definition_handle_pickup, player_index, equipment_definition_index);
}

//.text:00B88860 ; bool __cdecl equipment_desires_3rd_person_camera(int32)
//.text:00B888E0 ; void __cdecl sub_B888E0(int32)
//.text:00B889F0 ; real64 __cdecl sub_B889F0(int32)
//.text:00B88B10 ; real64 __cdecl equipment_get_invincible_fraction(int32)
//.text:00B88B70 ; bool __cdecl sub_B88B70(int32, real32*)
//.text:00B88C00 ; int32 __cdecl sub_B88C00(int32, int32)
//.text:00B88C40 ; void __cdecl equipment_handle_pickup(int32, int32)
//.text:00B88C80 ; equipment_has_remaining_use_duration?
//.text:00B88CF0 ; bool __cdecl equipment_is_active_noise_maker(int32)
//.text:00B88D90 ; 
//.text:00B88DA0 ; bool __cdecl equipment_new(int32, object_placement_data*, bool*)
//.text:00B88F70 ; bool __cdecl sub_B88F70(int32, int32*)
//.text:00B89010 ; bool __cdecl equipment_override_damage_material_type(int32, c_global_material_type*)
//.text:00B89090 ; bool __cdecl sub_B89090(int32, int32*)
//.text:00B89130 ; void __cdecl equipment_place(int32, s_scenario_equipment*)
//.text:00B89190 ; int32 __cdecl equipment_remaining_charges(int32)
//.text:00B891F0 ; void __cdecl sub_B891F0(int32, int32)
//.text:00B89550 ; bool __cdecl equipment_update(int32)
//.text:00B89E90 ; 
//.text:00B89EA0 ; 
//.text:00B89EB0 ; s_unknown_unit_struct_sizeof_14* __cdecl sub_B89EC0(void*)
//.text:00B89EC0 ; s_unknown_unit_struct_sizeof_14* __cdecl sub_B89EC0(void*)
//.text:00B89ED0 ; void __cdecl sub_B89ED0(int32)
//.text:00B89F30 ; 
//.text:00B89F60 ; 
//.text:00B89F80 ; void __cdecl sub_B89F80(int32)
//.text:00B8A040 ; bool __cdecl sub_B8A040(int32, int32)

