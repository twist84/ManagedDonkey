#pragma once

#include "cseries/cseries.hpp"

struct s_scripted_exposure
{
	byte __data[0x34];
};
static_assert(sizeof(s_scripted_exposure) == 0x34);

struct c_camera_fx_settings
{
	byte __data[0x170];
	
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
	//struct ssao;
	//struct color_grading;
	//struct lightshafts;
};
static_assert(sizeof(c_camera_fx_settings) == 0x170);

struct c_camera_fx_values
{
	c_camera_fx_settings m_settings;
	byte __data[0x80];
};
static_assert(sizeof(c_camera_fx_values) == 0x1F0);

extern void __cdecl scripted_exposure_update(real game_seconds_elapsed);

