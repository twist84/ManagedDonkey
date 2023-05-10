#pragma once

#include "cseries/cseries.hpp"
#include "math/function_definitions.hpp"
#include "tag_files/tag_groups.hpp"

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

extern void __cdecl screen_effect_update(real update_interval);

