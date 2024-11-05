#include "render/render_objects_static_lighting.hpp"

REFERENCE_DECLARE(0x050FB668, s_render_object_first_person_globals, render_object_globals);

//.text:00A42000 ; 
//.text:00A42020 ; 
//.text:00A42030 ; 
//.text:00A42060 ; 
//.text:00A420A0 ; 
//.text:00A420D0 ; 
//.text:00A42110 ; 
//.text:00A42140 ; 
//.text:00A421E0 ; 
//.text:00A425E0 ; 
//.text:00A439A0 ; bool __cdecl is_dynamic_object(e_object_type)

render_lighting const* object_get_cached_render_lighting(long object_index)
{
	return INVOKE(0x00A43A40, object_get_cached_render_lighting, object_index);
}

//.text:00A43AC0 ; long __cdecl object_get_cached_render_state(long)
//.text:00A43D10 ; 
//.text:00A43D70 ; 
//.text:00A445A0 ; render_lighting* __cdecl object_update_cached_render_lighting(long, real)
//.text:00A44740 ; 
//.text:00A44750 ; long __cdecl render_model_allocate_and_fill_skinning_matrices(long, byte*, byte, real_matrix4x3 const*, long*, long)
//.text:00A447B0 ; 
//.text:00A447C0 ; 
//.text:00A44840 ; void __cdecl render_object_calculate_alphas(long, real_point3d const*, real, long, bool, bool, real*)
//.text:00A44BE0 ; bool __cdecl render_object_first_person_globals_valid(void)
//.text:00A44C10 ; 
//.text:00A44C70 ; void __cdecl render_object_get_render_model_from_index(long, bool, long*, real_matrix4x3 const**, long*)
//.text:00A44D30 ; s_shader_extern_info* __cdecl render_object_get_render_state_info(long)
//.text:00A44D80 ; char* __cdecl render_object_get_render_state_last_level_of_detail(long)
//.text:00A44DD0 ; char* __cdecl render_object_get_render_state_last_permutations(long, long)
//.text:00A44E30 ; bool __cdecl render_object_get_render_state_last_sample_position(long, real_point3d*)
//.text:00A44E90 ; bool __cdecl render_object_has_lightmap_shadow(long)
//.text:00A44F20 ; void __cdecl render_object_invalidate_cached_render_state(long)
//.text:00A44F70 ; void __cdecl render_object_invalidate_render_state(long)
//.text:00A44FD0 ; bool __cdecl render_object_is_first_person_or_child_weapon(e_output_user_index, long)
//.text:00A45060 ; 

bool __cdecl render_object_should_be_visible(long object_index)
{
	return INVOKE(0x00A450F0, render_object_should_be_visible, object_index);
}

//.text:00A45160 ; void __cdecl render_object_update_change_colors(long, bool)
//.text:00A45240 ; 
//.text:00A45250 ; 
//.text:00A45270 ; 

void __cdecl render_objects_initialize()
{
	INVOKE(0x00A452B0, render_objects_initialize);
}

void __cdecl render_objects_initialize_for_new_map()
{
	INVOKE(0x00A452F0, render_objects_initialize_for_new_map);
}

//void __cdecl render_objects_prepare_for_window(e_output_user_index output_user_index)
void __cdecl render_objects_prepare_for_window(long output_user_index)
{
	INVOKE(0x00A45310, render_objects_prepare_for_window, output_user_index);
}

//.text:00A45410 ; void __cdecl render_objects_reset_render_state(long)
//.text:00A45470 ; 
//.text:00A454B0 ; 
//.text:00A454D0 ; 

