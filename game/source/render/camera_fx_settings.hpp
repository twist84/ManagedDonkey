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

	byte __data[0x34];
};
static_assert(sizeof(s_scripted_exposure) == 0x34);

struct s_ssao_parameter
{
	// SSAO
	// SSAO parameters

	dword_flags flags;

	real intensity;
	real radius;
	real sample_z_threshold;
};
static_assert(sizeof(s_ssao_parameter) == 0x10);

struct s_color_grading_parameter
{
	// COLOR GRADING
	// Color grading parameters

	dword_flags flags;

	real blend_time;

	s_tag_block curves_editor;
	s_tag_block brightness_contrast;
	s_tag_block hue_saturation_lightness_vibrance;
	s_tag_block colorize_effect;
	s_tag_block selective_color;
	s_tag_block color_balance;
};
static_assert(sizeof(s_color_grading_parameter) == 0x50);

struct s_lightshafts
{
	// LIGHTSHAFTS
	// Lightshafts parameters

	dword_flags flags;

	real pitch;          // [0...90]
	real heading;        // [0...360]
	real_rgb_color tint;
	real depth_clamp;
	real intensity_clamp; // [0...1]
	real falloff_radius;  // [0...2]
	real intensity;       // [0...50]
	real blur_radius;     // [0...20]
};
static_assert(sizeof(s_lightshafts) == 0x2C);

struct c_camera_fx_settings
{
	byte __data[0x164];
	
	//struct exposure;
	//struct auto_exposure_sensitivity;
	//struct auto_exposure_anti_bloom;
	//struct bloom_point;
	//struct bloom_inherent;
	//struct bloom_intensity;
	//struct bloom_large_color;
	//struct bloom_medium_color;
	//struct bloom_small_color;
	//struct bling_intensity;
	//struct bling_size;
	//struct bling_angle;
	//struct bling_count;
	//struct self_illum_preferred;
	//struct self_illum_scale;
	s_ssao_parameter ssao;
	s_color_grading_parameter color_grading;
	s_lightshafts lightshafts;
};
static_assert(sizeof(c_camera_fx_settings) == 0x1F0);

struct c_camera_fx_values
{
public:
	static real& g_HDR_target_stops;

//protected:
	byte __data[0x1F0];
};
static_assert(sizeof(c_camera_fx_values) == 0x1F0);

extern void __cdecl scripted_exposure_update(real game_seconds_elapsed);

