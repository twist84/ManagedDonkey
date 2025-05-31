#include "render/camera_fx_settings.hpp"

REFERENCE_DECLARE(0x019134C0, real32, c_camera_fx_values::g_HDR_target_stops);

void __cdecl s_scripted_exposure::dispose()
{
	INVOKE(0x00A2A7E0, dispose);
}

void __cdecl s_scripted_exposure::dispose_from_old_map()
{
	INVOKE(0x00A2A7F0, dispose_from_old_map);
}

void __cdecl s_scripted_exposure::initialize()
{
	INVOKE(0x00A2AA90, initialize);
}

void __cdecl s_scripted_exposure::initialize_for_new_map()
{
	INVOKE(0x00A2AAF0, initialize_for_new_map);
}

void __cdecl scripted_exposure_update(real32 game_seconds_elapsed)
{
	INVOKE(0x00A2AF70, scripted_exposure_update, game_seconds_elapsed);
}

void c_camera_fx_settings::set_defaults(bool use_default_parameters)
{
	INVOKE_CLASS_MEMBER(0x00A2B070, c_camera_fx_settings, set_defaults, use_default_parameters);

	//m_exposure.m_target = 0.0f;
	//m_exposure.m_blend_limit = 0.05f;
	//m_exposure.m_blend_speed = 0.05f;
	//m_exposure.m_flags = use_default_parameters | 6;
	//m_exposure.m_target_min = -6.0f;
	//m_exposure.m_target_max = 6.0f;
	//m_exposure.m_auto_exposure_delay = 0.5f;
	//
	//m_auto_exposure_sensitivity.m_target = 0.0f;
	//m_auto_exposure_sensitivity.m_flags = use_default_parameters;
	//
	//m_exposure_anti_bloom.m_target = 1.0f;
	//m_exposure_anti_bloom.m_flags = use_default_parameters;
	//
	//m_bloom_point.m_target = 1.5;
	//m_bloom_point.m_blend_limit = 0.1f;
	//m_bloom_point.m_blend_speed = 0.15f;
	//m_bloom_point.m_flags = use_default_parameters;
	//
	//m_bloom_inherent.m_target = 0.06f;
	//m_bloom_inherent.m_blend_limit = 0.01f;
	//m_bloom_inherent.m_blend_speed = 0.15f;
	//m_bloom_inherent.m_flags = use_default_parameters;
	//
	//m_bloom_intensity.m_target = 1.0f;
	//m_bloom_intensity.m_blend_limit = 0.05f;
	//m_bloom_intensity.m_blend_speed = 0.25f;
	//m_bloom_intensity.m_flags = use_default_parameters;
	//
	//m_bloom_large_color.m_color = { 1.0f, 1.0f, 1.0f };
	//m_bloom_large_color.m_flags = use_default_parameters;
	//
	//m_bloom_medium_color.m_color = { 1.0f, 1.0f, 1.0f };
	//m_bloom_medium_color.m_flags = use_default_parameters;
	//
	//m_bloom_small_color.m_color = { 1.0f, 1.0f, 1.0f };
	//m_bloom_small_color.m_flags = use_default_parameters;
	//
	//m_bling_intensity.m_target = 1.0f;
	//m_bling_intensity.m_blend_limit = 0.05f;
	//m_bling_intensity.m_blend_speed = 0.25f;
	//m_bling_intensity.m_flags = use_default_parameters;
	//
	//m_bling_size.m_target = 250.0f;
	//m_bling_size.m_blend_limit = 2.0f;
	//m_bling_size.m_blend_speed = 0.25f;
	//m_bling_size.m_flags = use_default_parameters;
	//
	//m_bling_angle.m_target = 45.0f;
	//m_bling_angle.m_blend_limit = 10.0f;
	//m_bling_angle.m_blend_speed = 0.1f;
	//m_bling_angle.m_flags = use_default_parameters;
	//
	//m_bling_count.m_flags = use_default_parameters;
	//m_bling_count.m_target = 3;
	//
	//m_self_illum_preferred.m_target = 0.0f;
	//m_self_illum_preferred.m_blend_limit = 0.05f;
	//m_self_illum_preferred.m_blend_speed = 0.2f;
	//m_self_illum_preferred.m_flags = use_default_parameters;
	//m_self_illum_scale.m_target = 1.0f;
	//m_self_illum_scale.m_blend_limit = 0.05f;
	//m_self_illum_scale.m_blend_speed = 0.1f;
	//m_self_illum_scale.m_flags = use_default_parameters;
	//
	//m_ssao.m_flags = FLAG(0) | FLAG(1);
	//m_ssao.m_intensity = 1.0f;
	//m_ssao.m_radius = 0.75f;
	//m_ssao.m_sample_z_threshold = 0.5f;
	//
	//m_color_grading.set_defaults(FLAG(0), 0.5f);
	//
	//m_lightshafts.m_flags = FLAG(0);
	//m_lightshafts.m_pitch = 45.0f;
	//m_lightshafts.m_heading = 0.0f;
	//m_lightshafts.m_tint.red = 1.0f;
	//m_lightshafts.m_tint.green = 1.0f;
	//m_lightshafts.m_tint.blue = 1.0f;
	//m_lightshafts.m_depth_clamp = 5000.0f;
	//m_lightshafts.m_intensity_clamp = 0.0f;
	//m_lightshafts.m_falloff_radius = 0.5f;
	//m_lightshafts.m_intensity = 2.0f;
	//m_lightshafts.m_blur_radius = 1.0f;
}

void c_camera_fx_values::set(const c_camera_fx_settings* settings)
{
	INVOKE_CLASS_MEMBER(0x00A3E920, c_camera_fx_values, set, settings);

	//m_exposure.set_exposure(settings->m_exposure.m_target);
	//m_exposure.reset_render_target_queue();
	//m_auto_exposure_sensitivity = settings->m_auto_exposure_sensitivity.m_target;
	//m_exposure_anti_bloom = settings->m_exposure_anti_bloom.m_target;
	//m_bloom_point = settings->m_bloom_point.m_target;
	//m_bloom_inherent = settings->m_bloom_inherent.m_target;
	//m_bloom_intensity = settings->m_bloom_intensity.m_target;
	//m_bloom_large_color = settings->m_bloom_large_color.m_color;
	//m_bloom_medium_color = settings->m_bloom_medium_color.m_color;
	//m_bloom_small_color = settings->m_bloom_small_color.m_color;
	//m_bling_intensity = settings->m_bling_intensity.m_target;
	//m_bling_angle = settings->m_bling_angle.m_target;
	//m_bling_count = settings->m_bling_count.m_target;
}

void s_color_grading_parameter::set_defaults(uns32 flags, real32 blend_time)
{
	INVOKE_CLASS_MEMBER(0x00A6FDB0, s_color_grading_parameter, set_defaults, flags, blend_time);

	//m_flags = flags;
	//m_blend_time = blend_time;
}

