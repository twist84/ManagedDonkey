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
#include "objects/damage_owner.hpp"

enum e_effect_pass
{
	_effect_pass_opaque = 0,
	_effect_pass_transparents,
	_effect_pass_distortion,
	_effect_pass_first_person,

	k_effect_pass_count
};

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
	byte __data2[0x2];

	dword_flags flags;
	long definition_index;
	long looping_sound_index;

	c_string_id marker_name;

	byte __unknown14;
	byte __unknown15;
	byte __data16[0x2];

	long scale_a_function_reference;
	long scale_b_function_reference;
	s_location location;

	byte __data22[0x2];

	vector3d transitional_velocity;
	long object_index;
	s_damage_owner damage_owner;

	byte __data40[0x6];
	byte_flags __flags46;
	byte_flags __flags47;

	long parent_particle_index;
	long event_index;
	real m_effect_scale_a;
	real m_effect_scale_b;

	byte __data58[0x8];

	long locations[8];
	long lightprobe_index;
	real death_delay;
	dword random_seed;

	long __unknown8C;
	plane3d __plane90;
};
static_assert(sizeof(effect_datum) == 0xA0);

struct event_datum :
	s_datum_header
{
	byte_flags flags;
	byte event_block_index;
	word event_counter; // `counter`?

	byte __data6[0x2];

	long next_event_index; // `next_index`?

	real __unknownC;
	real __unknown10;
};
static_assert(sizeof(event_datum) == 0x14);

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
	byte __data4[0x14];
	real_matrix4x3 matrix;
	byte __data4C[0x10];
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

struct s_game_non_bsp_zone_set;
struct c_scenario_resource_registry;
extern void __cdecl effects_prepare_for_new_zone_set(dword old_structure_bsp_mask, dword new_structure_bsp_mask);
extern void __cdecl effects_initialize_for_new_non_bsp_zone_set(s_game_non_bsp_zone_set const* new_non_bsp_zone_set);
extern void __cdecl effects_dispose_from_old_non_bsp_zone_set(s_game_non_bsp_zone_set const* old_non_bsp_zone_set);
extern bool __cdecl dangerous_effects_near_player();
extern void __cdecl effect_render(long effect_index, long user_index);
extern long __cdecl effect_new_from_point_vector(long effect_index, real_point3d const* position, vector3d const* forward, vector3d const* normal, long match_all_markers, long effect_deterministic, plane3d const* plane, s_cluster_reference* cluster_reference);
extern void __cdecl effects_dispose();
extern void __cdecl effects_dispose_from_old_map();
extern void __cdecl effects_dispose_from_old_structure_bsp(dword old_structure_bsp_mask);
extern void __cdecl effects_frame_advance(real seconds_elapsed);
extern void __cdecl effects_frame_advance_gpu(real seconds_elapsed);
extern void __cdecl effects_initialize();
extern void __cdecl effects_initialize_for_new_map();
extern void __cdecl effects_initialize_for_new_structure_bsp(dword new_structure_bsp_mask);
extern void __cdecl effects_prepare_for_non_bsp_zone_set_switch(s_game_non_bsp_zone_set const* old_non_bsp_zone_set, s_game_non_bsp_zone_set const* new_non_bsp_zone_set, c_scenario_resource_registry* pending_zone_registry);
extern void __cdecl effects_render(long user_index, e_effect_pass pass);
extern void __cdecl effects_submit_cheap_first_person_attachments(long user_index);
extern void __cdecl effects_update();

extern void render_debug_damage_effects();
extern void debug_effects_full();

