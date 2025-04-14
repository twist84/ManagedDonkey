#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct s_scripted_exposure
{
public:
	static void __cdecl dispose();
	static void __cdecl dispose_from_old_map();
	static void __cdecl initialize();
	static void __cdecl initialize_for_new_map();

	dword_flags m_flags;
	real m_light_intensity_scale;
	real m_exposure_bias;
	real m_exposure_blend;
	real m_exposure_stops;
	real g_original_exposure_stops;
	real g_target_exposure_stops;
	real g_original_exposure_blend;
	real g_target_exposure_blend;
	long g_exposure_animation_start_time;
	long g_exposure_animation_end_time;
	long g_exposure_adapt_instantly_frames;
	long g_disable_autoexposure_frames;
};
static_assert(sizeof(s_scripted_exposure) == 0x34);

struct s_ssao_parameter
{
	// SSAO
	// SSAO parameters

	dword_flags m_flags;

	real m_intensity;
	real m_radius;
	real m_sample_z_threshold;
};
static_assert(sizeof(s_ssao_parameter) == 0x10);

struct s_color_grading_parameter
{
	void set_defaults(dword flags, real blend_time);

	// COLOR GRADING
	// Color grading parameters

	dword_flags m_flags;

	real m_blend_time;

	s_tag_block m_curves_editor;
	s_tag_block m_brightness_contrast;
	s_tag_block m_hslv;
	s_tag_block m_colorize_effect;
	s_tag_block m_selective_color;
	s_tag_block m_color_balance;
};
static_assert(sizeof(s_color_grading_parameter) == 0x50);

struct s_lightshafts
{
	// LIGHTSHAFTS
	// Lightshafts parameters

	dword_flags m_flags;

	real m_pitch;           // [0...90]
	real m_heading;         // [0...360]
	real_rgb_color m_tint;
	real m_depth_clamp;
	real m_intensity_clamp; // [0...1]
	real m_falloff_radius;  // [0...2]
	real m_intensity;       // [0...50]
	real m_blur_radius;     // [0...20]
};
static_assert(sizeof(s_lightshafts) == 0x2C);

struct c_camera_fx_settings
{
	void set_defaults(bool use_default_parameters);

	enum e_parameter_flags
	{
		_parameter_use_default_bit = 0,
		_parameter_blend_limit_relative_bit,
		_parameter_auto_bit,
		_parameter_double_sided_star_bit,
		_parameter_fixed_bit,

		k_num_parameter_flags
	};

	struct s_parameter
	{
		word_flags m_flags;
	};

	struct s_real_parameter :
		s_parameter
	{
		word pad;
		real m_target;
		real m_blend_limit;
		real m_blend_speed;
	};

	struct s_real_instant_parameter :
		s_parameter
	{
		word pad;
		real m_target;
	};

	struct s_word_parameter :
		s_parameter
	{
		word m_target;
	};

	struct s_color_parameter :
		s_parameter
	{
		word pad;
		real_rgb_color m_color;
	};

	struct s_real_exposure_parameter :
		s_real_parameter
	{
		real m_target_min;
		real m_target_max;
		real m_auto_exposure_target_brightness;
		real m_auto_exposure_delay;
	};

	s_real_exposure_parameter m_exposure;
	s_real_instant_parameter m_auto_exposure_sensitivity;
	s_real_instant_parameter m_exposure_anti_bloom;
	s_real_parameter m_bloom_point;
	s_real_parameter m_bloom_inherent;
	s_real_parameter m_bloom_intensity;
	s_color_parameter m_bloom_large_color;
	s_color_parameter m_bloom_medium_color;
	s_color_parameter m_bloom_small_color;
	s_real_parameter m_bling_intensity;
	s_real_parameter m_bling_size;
	s_real_parameter m_bling_angle;
	s_word_parameter m_bling_count;
	s_real_parameter m_self_illum_preferred;
	s_real_parameter m_self_illum_scale;
	s_ssao_parameter m_ssao;
	s_color_grading_parameter m_color_grading;
	s_lightshafts m_lightshafts;
};
static_assert(sizeof(c_camera_fx_settings) == 0x170);

// $TODO: figure out the difference
struct c_exposure
{
	long m_render_target_queue_size;
	long m_render_target_queue_index;
	long m_render_target_queue[1];

	real m_prev_exposure;
	real m_exposure;
	real m_exposure_velocity;
	real m_exposure_buffer[60];

	long m_buffer_index;
};
static_assert(sizeof(c_exposure) == 0x10C);

struct c_camera_fx_values
{
public:
	void set(c_camera_fx_settings const* settings);

public:
	static real& g_HDR_target_stops;

//protected:
	c_exposure m_exposure;
	real m_exposure_boost;
	real m_auto_exposure_sensitivity;
	real m_exposure_anti_bloom;
	real m_bloom_point;
	real m_bloom_inherent;
	real m_bloom_intensity;
	real m_bling_intensity;
	real m_bling_size;
	real m_bling_angle;
	word m_bling_count;
	real_rgb_color m_bloom_large_color;
	real_rgb_color m_bloom_medium_color;
	real_rgb_color m_bloom_small_color;
	real m_self_illum_preferred;
	real m_self_illum_scale;
	real m_self_illum_exposure;
	s_ssao_parameter ssao;
	s_color_grading_parameter color_grading;
	s_lightshafts lightshafts;
};
static_assert(sizeof(c_camera_fx_values) == 0x1F0);

extern void __cdecl scripted_exposure_update(real game_seconds_elapsed);

