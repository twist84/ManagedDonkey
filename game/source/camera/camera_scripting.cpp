#include "camera/camera_scripting.hpp"

//.text:0072BFE0 ; real32 __cdecl calculate_field_of_view(real32)
//.text:0072C040 ; void __cdecl camera_render_main_camera_to_texture_camera(bool)
//.text:0072C060 ; void __cdecl cinematic_camera_set_easing_in(real32, real32, real32, real32, real32, real32)
//.text:0072C170 ; void __cdecl cinematic_camera_set_easing_out(real32)
//.text:0072C260 ; void __cdecl cinematic_camera_update_texture_camera(s_camera_frame_dynamic const*)
//.text:0072C380 ; void __cdecl cinematic_set_active_camera(long)
//.text:0072C3D0 ; public: real32 __cdecl c_camera_pan_navigator::evaluate(long)
//.text:0072C510 ; public: virtual e_director_perspective __cdecl c_scripted_camera::get_perspective() const
//.text:0072C570 ; public: virtual e_camera_mode __cdecl c_scripted_camera::get_type() const
//.text:0072C5C0 ; void __cdecl scripted_camera_cinematic_stop()
//.text:0072C600 ; void __cdecl scripted_camera_enable(bool)

void __cdecl scripted_camera_game_tick()
{
	INVOKE(0x0072C630, scripted_camera_game_tick);
}

//.text:0072CAA0 ; s_scripted_camera_animation* __cdecl scripted_camera_get_animation()
//.text:0072CAC0 ; bool __cdecl scripted_camera_get_cinematic_data(long*, long*, long*)
//.text:0072CB20 ; s_scripted_camera_constant_pan* __cdecl scripted_camera_get_constant_pan()
//.text:0072CB40 ; s_scripted_camera_control_point* __cdecl scripted_camera_get_control_point()
//.text:0072CB60 ; bool __cdecl scripted_camera_get_deterministic_animated_camera_matrix(real_matrix4x3*, real32*)
//.text:0072CE60 ; void __cdecl scripted_camera_get_deterministic_cinematic_camera_matrix(s_camera_frame_dynamic const*, real_matrix4x3*)
//.text:0072CF20 ; s_cluster_reference __cdecl scripted_camera_get_deterministic_cluster_reference()
//.text:0072D010 ; real32 __cdecl scripted_camera_get_field_of_view()
//.text:0072D090 ; long __cdecl scripted_camera_get_first_person_camera_object()
//.text:0072D0E0 ; void __cdecl scripted_camera_get_relative_object_space(long, real_matrix4x3*)
//.text:0072D130 ; void __cdecl scripted_camera_handle_tag_reload()

void __cdecl scripted_camera_initialize()
{
	INVOKE(0x0072D200, scripted_camera_initialize);
}

//.text:0072D260 ; void __cdecl scripted_camera_initialize_for_animation()
//.text:0072D290 ; void __cdecl scripted_camera_initialize_for_cinematic_camera()
//.text:0072D2D0 ; void __cdecl scripted_camera_initialize_for_constant_pan()
//.text:0072D300 ; void __cdecl scripted_camera_initialize_for_control_point()
//.text:0072D330 ; void __cdecl scripted_camera_initialize_for_first_person_camera()
//.text:0072D360 ; void __cdecl scripted_camera_initialize_for_new_map()
//.text:0072D390 ; long __cdecl scripted_camera_object_relative_to()
//.text:0072D3B0 ; void __cdecl scripted_camera_pan(short, short, short, short, real32, short, real32)
//.text:0072D540 ; void __cdecl scripted_camera_predict_resources_at_frame(long, long, long, short, long)
//.text:0072D550 ; void __cdecl scripted_camera_predict_resources_at_point(short)

// Halo Online specific
//.text:0072D560 ; void __cdecl scripted_camera_prematch_set_camera_director()
//.text:0072D580 ; bool __cdecl scripted_camera_prematch_set_camera_point()

//.text:0072D610 ; void __cdecl scripted_camera_set(short, short, long)
//.text:0072D660 ; void __cdecl scripted_camera_set_absolute(short, short)
//.text:0072D6B0 ; void __cdecl scripted_camera_set_animation(long, long)
//.text:0072D6E0 ; void __cdecl scripted_camera_set_animation_relative(long, long, long, short)
//.text:0072D710 ; void __cdecl scripted_camera_set_animation_relative_with_speed(long, long, long, short, real32)
//.text:0072D740 ; void __cdecl scripted_camera_set_animation_relative_with_speed_loop(long, long, long, short, real32, bool)
//.text:0072D770 ; void __cdecl scripted_camera_set_animation_relative_with_speed_loop_offset(long, long, long, short, real32, bool, real32)
//.text:0072D8F0 ; void __cdecl scripted_camera_set_animation_with_speed(long, long, real32)
//.text:0072D920 ; void __cdecl scripted_camera_set_cinematic()
//.text:0072DA10 ; void __cdecl scripted_camera_set_cinematic_scene(long, long, short)
//.text:0072DAF0 ; void __cdecl scripted_camera_set_cinematic_shot_user_input_constraints(long, rectangle2d const&, real32)
//.text:0072DC10 ; void __cdecl scripted_camera_set_field_of_view(real32, short)
//.text:0072DD30 ; void __cdecl scripted_camera_set_first_person(long)
//.text:0072DD70 ; void __cdecl scripted_camera_set_internal(e_scenario_camera_type, real_point3d const*, real_euler_angles3d const*, real32, short, long)
//.text:0072DFB0 ; void __cdecl scripted_camera_set_pan(short, short)
//.text:0072E120 ; short __cdecl scripted_camera_time_in_hs_ticks()
//.text:0072E170 ; void __cdecl scripted_camera_update(void*, s_camera_control const*, s_observer_command*)
//.text:0072EFA0 ; public: bool __cdecl c_camera_pan_navigator::set_parameters(real32, real32, real32, real32, real32)
//.text:0072F0E0 ; void __cdecl transform_into_flag_space(real_matrix4x3*, long)
//.text:0072F150 ; public: virtual void __cdecl c_scripted_camera::update(long, real32, s_observer_command*)
//.text:0072F170 ; public: __cdecl c_static_camera::c_static_camera(long)
//.text:0072F1D0 ; public: __cdecl c_static_camera::c_static_camera()
//.text:0072F1F0 ; public: virtual e_director_perspective __cdecl c_static_camera::get_perspective() const
//.text:0072F200 ; c_static_camera::
//.text:0072F210 ; public: virtual e_camera_mode __cdecl c_static_camera::get_type() const
//.text:0072F220 ; public: virtual void __cdecl c_static_camera::set_forward(real_vector3d const*)
//.text:0072F240 ; public: virtual void __cdecl c_static_camera::set_position(real_point3d const*)
//.text:0072F260 ; public: virtual void __cdecl c_static_camera::update(long, real32, s_observer_command*)

