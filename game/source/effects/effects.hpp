#pragma once

#include "cseries/cseries.hpp"
#include "effects/beams.hpp"
#include "effects/contrails.hpp"
#include "effects/decals.hpp"
#include "effects/light_volumes.hpp"
#include "effects/particle.hpp"
#include "effects/particle_emitter.hpp"
#include "effects/particle_location.hpp"
#include "effects/particle_system.hpp"
#include "effects/player_effects.hpp"
#include "effects/screen_effect.hpp"
#include "effects/vision_mode.hpp"
#include "memory/data.hpp"

struct s_effect_vector
{
	real_point3d point;
	vector3d direction;
	c_string_id name;
};
static_assert(sizeof(s_effect_vector) == 0x1C);

struct effect_datum :
	s_datum_header
{
	byte __data[0x9E];
};
static_assert(sizeof(effect_datum) == 0xA0);

struct effect_event_datum :
	s_datum_header
{
	byte __data[0x12];
};
static_assert(sizeof(effect_event_datum) == 0x14);

struct effect_location_datum :
	s_datum_header
{
	word_flags flags;
	byte __data4[0x4];
	dword __unknown8;
	real_matrix4x3 matrix;
};
static_assert(sizeof(effect_location_datum) == 0x40);

struct s_effect_counts
{
	byte __data[0x18];
};
static_assert(sizeof(s_effect_counts) == 0x18);

struct effect_geometry_sample_datum :
	s_datum_header
{
	byte __data[0x26];
};
static_assert(sizeof(effect_geometry_sample_datum) == 0x28);

struct s_effect_message
{
	long m_type;
	byte __data[0x58];
};
static_assert(sizeof(s_effect_message) == 0x5C);

struct s_geometry_sample
{
	real_point3d __point0;

	real lightprobe_r[16];
	byte __data4C[0x50];

	real lightprobe_g[16];
	byte __dataDC[0x50];

	real lightprobe_b[16];
	byte __data16C[0x50];

	vector3d __vector1BC;
	vector3d up;
	vector3d __vector1D4;
	real_rgb_color color;
	vector3d __vector1EC;
};
static_assert(sizeof(s_geometry_sample) == 0x1F8);

struct s_effect_lightprobe
{
	byte_flags flags;
	s_geometry_sample geometry_sample;
};
static_assert(sizeof(s_effect_lightprobe) == 0x1FC);

extern bool debug_damage_effects;

extern void render_debug_damage_effects();
extern bool __cdecl dangerous_effects_near_player();

