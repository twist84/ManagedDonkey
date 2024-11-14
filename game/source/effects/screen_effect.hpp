#pragma once

#include "cseries/cseries.hpp"
#include "math/function_definitions.hpp"
#include "memory/data.hpp"
#include "tag_files/tag_groups.hpp"

struct s_screen_effect_datum :
	s_datum_header
{
	byte __unknown2;

	byte_flags flags;
	long definition_index;
	real seconds_active;
	real_point3d transformed_position;
	long attached_object_index;
	short node_index;

	byte __data1E[0x2];

	real_point3d position;

	real_rectangle2d __rectangle2C;
};
static_assert(sizeof(s_screen_effect_datum) == 0x3C);

struct screen_effect_scalar_function
{
	// Mapping
	mapping_function mapping;
};
static_assert(sizeof(screen_effect_scalar_function) == sizeof(mapping_function));

struct s_single_screen_effect_definition
{
	c_string_id name;
	dword_flags flags;

	// DISTANCE FALLOFF

	// the maximum distance this screen effect will affect
	real maximum_distance; // world units
	struct screen_effect_scalar_function distance_falloff;

	// TIME EVOLUTION
	// controls the lifetime and time falloff of this effect
	// NOTE: not used for scenario global effects

	real lifetime;
	struct screen_effect_scalar_function time_falloff;

	// ANGLE FALLOFF
	// controls the falloff of this effect based on how close you are to looking directly at it
	// NOTE: not used for scenario global effects

	struct screen_effect_scalar_function angle_falloff;

	// EFFECTS
	// a selection of effects to choose from
	// in the case of overlapping effects, the maximum will be taken

	// increase or decrease in exposure
	real exposure_boost; // stops

	// shifts hue R->G->B
	real hue_left; // degrees [0-360]

	// shifts hue B->G->R
	real hue_right; // degrees [0-360]

	// increases saturation
	real saturation; // [0-1]

	// decreases saturation
	real desaturation; // [0-1]

	// contrast increase
	real contrast_enhance; // [0-1]

	// gamma increase
	real gamma_enhance; // [0-10]

	// gamma increase
	real gamma_reduce; // [0-10]

	// this color is multiplied on top
	real_rgb_color color_filter;

	// this color is subtracted
	real_rgb_color color_floor;

	// turns on the global vision mode
	real vision_mode; // [0-1]

	// adds noise to the vision mode
	real vision_noise; // [0-1]

	// applies this shader to the entire screen
	s_tag_reference shader_effect;
};
static_assert(sizeof(s_single_screen_effect_definition) == 0x9C);

struct s_area_screen_effect_definition
{
	c_typed_tag_block<s_single_screen_effect_definition> screen_effects;
};
static_assert(sizeof(s_area_screen_effect_definition) == 0xC);

struct s_screen_effect_settings
{
	real __unknown0;
	real __unknown4;
	real __unknown8;
	real __unknownC;
	real __unknown10;
	real __unknown14;
	real __unknown18;
	real __unknown1C;
	real_linear_rgb_color __unknown20;
	real_linear_rgb_color __unknown2C;
	real __unknown38;
	real __unknown3C;
};

struct s_screen_effect_shader_sample_result
{
	// #TODO: find the actual name of this
	struct s_screen_effect_shader_sample
	{
		long definition_index;
		byte __data4[0x10];
		real_rectangle2d rect;
	};
	static_assert(sizeof(s_screen_effect_shader_sample) == 0x24);

	s_screen_effect_shader_sample __unknown0[8];
	long __unknown120;
};
static_assert(sizeof(s_screen_effect_shader_sample_result) == 0x124);

enum e_output_user_index;

extern void __cdecl screen_effect_dispose();
extern void __cdecl screen_effect_dispose_from_old_map();
extern void __cdecl screen_effect_initialize();
extern void __cdecl screen_effect_initialize_for_new_map();
extern long __cdecl screen_effect_new(long definition_index, long object_index, short node_index, real_point3d const* point, real_rectangle2d const* rectangle);
extern void __cdecl screen_effect_sample(real_point3d const* point, vector3d const* vector, s_screen_effect_settings* settings, s_screen_effect_shader_sample_result* result, e_output_user_index output_user_index);
extern void __cdecl screen_effect_update(real update_interval);

extern void apply_global_screen_effect();

