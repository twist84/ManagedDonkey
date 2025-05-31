#include "camera/camera_scripting.hpp"

//.text:0072BFE0 ; real32 __cdecl calculate_field_of_view(real32)
//.text:0072C040 ; void __cdecl camera_render_main_camera_to_texture_camera(bool)
//.text:0072C060 ; void __cdecl cinematic_camera_set_easing_in(real32, real32, real32, real32, real32, real32)
//.text:0072C170 ; void __cdecl cinematic_camera_set_easing_out(real32)
//.text:0072C260 ; void __cdecl cinematic_camera_update_texture_camera(const s_camera_frame_dynamic*)
//.text:0072C380 ; void __cdecl cinematic_set_active_camera(int32)
//.text:0072C3D0 ; public: real32 __cdecl c_camera_pan_navigator::evaluate(int32)
//.text:0072C510 ; public: virtual e_director_perspective __cdecl c_scripted_camera::get_perspective() const
//.text:0072C570 ; public: virtual e_camera_mode __cdecl c_scripted_camera::get_type() const
//.text:0072C5C0 ; void __cdecl scripted_camera_cinematic_stop()
//.text:0072C600 ; void __cdecl scripted_camera_enable(bool)

void __cdecl scripted_camera_game_tick()
{
	INVOKE(0x0072C630, scripted_camera_game_tick);
}

//.text:0072CAA0 ; s_scripted_camera_animation* __cdecl scripted_camera_get_animation()
//.text:0072CAC0 ; bool __cdecl scripted_camera_get_cinematic_data(int32*, int32*, int32*)
//.text:0072CB20 ; s_scripted_camera_constant_pan* __cdecl scripted_camera_get_constant_pan()
//.text:0072CB40 ; s_scripted_camera_control_point* __cdecl scripted_camera_get_control_point()
//.text:0072CB60 ; bool __cdecl scripted_camera_get_deterministic_animated_camera_matrix(real_matrix4x3*, real32*)
//.text:0072CE60 ; void __cdecl scripted_camera_get_deterministic_cinematic_camera_matrix(const s_camera_frame_dynamic*, real_matrix4x3*)
//.text:0072CF20 ; s_cluster_reference __cdecl scripted_camera_get_deterministic_cluster_reference()
//.text:0072D010 ; real32 __cdecl scripted_camera_get_field_of_view()
//.text:0072D090 ; int32 __cdecl scripted_camera_get_first_person_camera_object()
//.text:0072D0E0 ; void __cdecl scripted_camera_get_relative_object_space(int32, real_matrix4x3*)
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
//.text:0072D390 ; int32 __cdecl scripted_camera_object_relative_to()
//.text:0072D3B0 ; void __cdecl scripted_camera_pan(int16, int16, int16, int16, real32, int16, real32)
//.text:0072D540 ; void __cdecl scripted_camera_predict_resources_at_frame(int32, int32, int32, int16, int32)
//.text:0072D550 ; void __cdecl scripted_camera_predict_resources_at_point(int16)

// Halo Online specific
//.text:0072D560 ; void __cdecl scripted_camera_prematch_set_camera_director()
//.text:0072D580 ; bool __cdecl scripted_camera_prematch_set_camera_point()

//.text:0072D610 ; void __cdecl scripted_camera_set(int16, int16, int32)
//.text:0072D660 ; void __cdecl scripted_camera_set_absolute(int16, int16)
//.text:0072D6B0 ; void __cdecl scripted_camera_set_animation(int32, int32)
//.text:0072D6E0 ; void __cdecl scripted_camera_set_animation_relative(int32, int32, int32, int16)
//.text:0072D710 ; void __cdecl scripted_camera_set_animation_relative_with_speed(int32, int32, int32, int16, real32)
//.text:0072D740 ; void __cdecl scripted_camera_set_animation_relative_with_speed_loop(int32, int32, int32, int16, real32, bool)
//.text:0072D770 ; void __cdecl scripted_camera_set_animation_relative_with_speed_loop_offset(int32, int32, int32, int16, real32, bool, real32)
//.text:0072D8F0 ; void __cdecl scripted_camera_set_animation_with_speed(int32, int32, real32)
//.text:0072D920 ; void __cdecl scripted_camera_set_cinematic()
//.text:0072DA10 ; void __cdecl scripted_camera_set_cinematic_scene(int32, int32, int16)
//.text:0072DAF0 ; void __cdecl scripted_camera_set_cinematic_shot_user_input_constraints(int32, const rectangle2d&, real32)
//.text:0072DC10 ; void __cdecl scripted_camera_set_field_of_view(real32, int16)
//.text:0072DD30 ; void __cdecl scripted_camera_set_first_person(int32)
//.text:0072DD70 ; void __cdecl scripted_camera_set_internal(e_scenario_camera_type, const real_point3d*, const real_euler_angles3d*, real32, int16, int32)
//.text:0072DFB0 ; void __cdecl scripted_camera_set_pan(int16, int16)
//.text:0072E120 ; int16 __cdecl scripted_camera_time_in_hs_ticks()
//.text:0072E170 ; void __cdecl scripted_camera_update(void*, const s_camera_control*, s_observer_command*)
//.text:0072EFA0 ; public: bool __cdecl c_camera_pan_navigator::set_parameters(real32, real32, real32, real32, real32)
//.text:0072F0E0 ; void __cdecl transform_into_flag_space(real_matrix4x3*, int32)
//.text:0072F150 ; public: virtual void __cdecl c_scripted_camera::update(int32, real32, s_observer_command*)
//.text:0072F170 ; public: __cdecl c_static_camera::c_static_camera(int32)
//.text:0072F1D0 ; public: __cdecl c_static_camera::c_static_camera()
//.text:0072F1F0 ; public: virtual e_director_perspective __cdecl c_static_camera::get_perspective() const
//.text:0072F200 ; c_static_camera::
//.text:0072F210 ; public: virtual e_camera_mode __cdecl c_static_camera::get_type() const
//.text:0072F220 ; public: virtual void __cdecl c_static_camera::set_forward(const real_vector3d*)
//.text:0072F240 ; public: virtual void __cdecl c_static_camera::set_position(const real_point3d*)
//.text:0072F260 ; public: virtual void __cdecl c_static_camera::update(int32, real32, s_observer_command*)

