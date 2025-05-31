#include "render/depth_of_field.hpp"

#include "memory/thread_local.hpp"

#include <cmath>

//.text:00A3EA20 ; void __cdecl __tls_set_g_depth_of_field_game_globals_allocator(void*)
//.text:00A3EA40 ; 
//.text:00A3EA80 ; 

void __cdecl s_depth_of_field::dispose()
{
	//INVOKE(0x00A3EAB0, s_depth_of_field::dispose);
}

void __cdecl s_depth_of_field::dispose_from_old_map()
{
	//INVOKE(0x00A3EAC0, s_depth_of_field::dispose_from_old_map);
}

//.text:00A3EAD0 ; 

s_depth_of_field* __cdecl s_depth_of_field::get()
{
	//return INVOKE(0x00A3EB00, s_depth_of_field::get);

	return depth_of_field_game_globals;
}

bool s_depth_of_field::get_current_depth_of_field_parameters(const s_observer_depth_of_field* observer_depth_of_field, real32* focus_distance, real32* aperture, real32* focus_half_width, real32* depth_of_field_blur)
{
	return INVOKE_CLASS_MEMBER(0x00A3EB20, s_depth_of_field, get_current_depth_of_field_parameters, observer_depth_of_field, focus_distance, aperture, focus_half_width, depth_of_field_blur);
}

real32 __cdecl s_depth_of_field::get_depth_of_field_blur_sigma(const s_observer_depth_of_field* observer_depth_of_field)
{
	return INVOKE(0x00A3EC30, s_depth_of_field::get_depth_of_field_blur_sigma, observer_depth_of_field);

	//real32 actual_blur = depth_of_field_game_globals->actual_blur;
	//if (observer_depth_of_field && TEST_BIT(observer_depth_of_field->flags, s_observer_depth_of_field::_active_bit))
	//{
	//	actual_blur = observer_depth_of_field->blur_amount;
	//	actual_blur *= 10.124f;
	//}
	//actual_blur *= actual_blur - 2.5f;
	//return actual_blur;
}

bool __cdecl s_depth_of_field::get_enabled()
{
	//return INVOKE(0x00A3ECB0, s_depth_of_field::get_enabled);

	return depth_of_field_game_globals->enabled;
}

void __cdecl s_depth_of_field::initialize()
{
	//INVOKE(0x00A3ECD0, s_depth_of_field::initialize);

	depth_of_field_game_globals = (s_depth_of_field*)g_depth_of_field_game_globals_allocator.allocate(sizeof(s_depth_of_field), "DOF globals");
	depth_of_field_game_globals->set_default_values();
}

void __cdecl s_depth_of_field::initialize_for_new_map()
{
	//INVOKE(0x00A3ED30, s_depth_of_field::initialize_for_new_map);

	depth_of_field_game_globals->enabled = false;
}

void __cdecl render_animate_depth_of_field(real32 near_distance, real32 far_distance, real32 depth, real32 ticks)
{
	INVOKE(0x00A3ED50, render_animate_depth_of_field, near_distance, far_distance, depth, ticks);

	//depth_of_field_game_globals->enabled = true;
	//depth_of_field_game_globals->animate = true;
	//depth_of_field_game_globals->focus_distance_near = near_distance;
	//depth_of_field_game_globals->focus_distance_far = far_distance;
	//depth_of_field_game_globals->aperture = 1.0f / (depth > 0.001f ? depth : 0.001f);
	//depth_of_field_game_globals->animation_start_time = game_time_get();
	//depth_of_field_game_globals->animation_end_time = depth_of_field_game_globals->animation_start_time + (int32)ceil(ticks);
	//depth_of_field_game_globals->animation_last_alpha = 0.0f;
}

void __cdecl render_animate_depth_of_field_blur(real32 blur, real32 ticks)
{
	INVOKE(0x00A3EE30, render_animate_depth_of_field_blur, blur, ticks);

	//depth_of_field_game_globals->blur_start = depth_of_field_game_globals->actual_blur;
	//depth_of_field_game_globals->blur_target = blur;
	//depth_of_field_game_globals->blur_start_time = game_time_get();
	//depth_of_field_game_globals->blur_end_time = depth_of_field_game_globals->blur_start_time + (int32)ceil(ticks);
}

void __cdecl render_enable_depth_of_field(bool enable)
{
	INVOKE(0x00A3EEB0, render_enable_depth_of_field, enable);

	//depth_of_field_game_globals->enabled = enable;
	//depth_of_field_game_globals->animate = false;
}

void __cdecl render_set_depth_of_field(real32 depth)
{
	INVOKE(0x00A3EEE0, render_set_depth_of_field, depth);

	//depth_of_field_game_globals->aperture = 1.0f / (depth > 0.001f ? depth : 0.001f);
}

//.text:00A3EF30 ; 

void s_depth_of_field::set_default_values()
{
	INVOKE_CLASS_MEMBER(0x00A3EF70, s_depth_of_field, set_default_values);

	//enabled = false;
	//animate = false;
	//speed = 0.95f;
	//focus_distance_near = 2.0f;
	//focus_distance_far = 2.0f;
	//aperture = 0.7f;
	//blur_start = 10.124f;
	//blur_target = 10.124f;
	//blur_start_time = 0;
	//blur_end_time = 0;
	//animation_start_time = 0;
	//animation_end_time = 0;
	//animation_last_alpha = 0.0f;
	//actual_blur = 0.0f;
	//actual_focus_distance_near = -5.0f;
	//actual_focus_distance_far = -5.0f;
	//actual_aperture = 8.0f;
}

void __cdecl s_depth_of_field::update()
{
	//INVOKE(0x00A3EFE0, s_depth_of_field::update);

	depth_of_field_game_globals->update_internal();
}

void s_depth_of_field::update_internal()
{
	INVOKE_CLASS_MEMBER(0x00A3F000, s_depth_of_field, update_internal);
}

