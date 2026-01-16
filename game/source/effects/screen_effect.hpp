#pragma once

#include "cseries/cseries.hpp"
#include "math/function_definitions.hpp"
#include "tag_files/tag_groups.hpp"

struct s_screen_effect_datum :
	s_datum_header
{
	uns8 position_type;
	uns8 unused;
	int32 definition_index;
	real32 time_accumulator;
	real_point3d world_position;

	union
	{
		struct
		{
			int32 object_index;
			int16 node_index;
			real_point3d node_position;
		} attached_to_object;
	} position;

	// odst?
	real_rectangle2d __rectangle2C;
};
COMPILE_ASSERT(sizeof(s_screen_effect_datum) == 0x3C);

struct screen_effect_scalar_function
{
	// Mapping
	c_function_definition mapping;
};
COMPILE_ASSERT(sizeof(screen_effect_scalar_function) == sizeof(c_function_definition));

struct s_screen_effect_settings
{
	// EFFECTS
	// a selection of effects to choose from
	// in the case of overlapping effects, the maximum will be taken

	// increase or decrease in exposure
	real32 exposure_boost; // stops

	// shifts hue R->G->B
	real32 hue_left; // degrees [0-360]

	// shifts hue B->G->R
	real32 hue_right; // degrees [0-360]

	// increases saturation
	real32 saturation; // [0-1]

	// decreases saturation
	real32 desaturation; // [0-1]

	// contrast increase
	real32 contrast_enhance; // [0-1]

	// gamma increase
	real32 gamma_enhance; // [0-10]

	// gamma increase
	real32 gamma_reduce; // [0-10]

	// this color is multiplied on top
	real_rgb_color color_filter;

	// this color is subtracted
	real_rgb_color color_floor;

	// turns on the global vision mode
	real32 vision_mode; // [0-1]

	// adds noise to the vision mode
	real32 vision_noise; // [0-1]
};
COMPILE_ASSERT(sizeof(s_screen_effect_settings) == 0x40);

struct s_single_screen_effect_definition
{
	c_string_id name;
	uns32 flags;

	// DISTANCE FALLOFF

	// the maximum distance this screen effect will affect
	real32 maximum_distance; // world units
	screen_effect_scalar_function distance_falloff;

	// TIME EVOLUTION
	// controls the lifetime and time falloff of this effect
	// NOTE: not used for scenario global effects

	real32 lifetime;
	screen_effect_scalar_function time_falloff;

	// ANGLE FALLOFF
	// controls the falloff of this effect based on how close you are to looking directly at it
	// NOTE: not used for scenario global effects

	screen_effect_scalar_function angle_falloff;

	s_screen_effect_settings settings;

	// applies this shader to the entire screen
	s_tag_reference shader_effect;
};
COMPILE_ASSERT(sizeof(s_single_screen_effect_definition) == 0x9C);

struct s_area_screen_effect_definition
{
	c_typed_tag_block<s_single_screen_effect_definition> screen_effects;
};
COMPILE_ASSERT(sizeof(s_area_screen_effect_definition) == 0xC);

struct s_screen_effect_shader_sample_result
{
	// $TODO find the actual name of this
	struct s_screen_effect_shader_sample
	{
		int32 definition_index;
		byte __data4[0x10];
		real_rectangle2d rect;
	};
	COMPILE_ASSERT(sizeof(s_screen_effect_shader_sample) == 0x24);

	s_screen_effect_shader_sample __unknown0[8];
	int32 __unknown120;
};
COMPILE_ASSERT(sizeof(s_screen_effect_shader_sample_result) == 0x124);


extern void __cdecl screen_effect_dispose();
extern void __cdecl screen_effect_dispose_from_old_map();
extern void __cdecl screen_effect_initialize();
extern void __cdecl screen_effect_initialize_for_new_map();
extern int32 __cdecl screen_effect_new(int32 definition_index, int32 object_index, int16 node_index, const real_point3d* point, const real_rectangle2d* rectangle);
extern void __cdecl screen_effect_sample(const real_point3d* point, const real_vector3d* vector, s_screen_effect_settings* settings, s_screen_effect_shader_sample_result* result, int32 user_index);
extern void __cdecl screen_effect_update(real32 update_interval);

extern void apply_global_screen_effect();

