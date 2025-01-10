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
#include "objects/damage_owner.hpp"
#include "objects/damage_reporting.hpp"

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
	real_point3d position;
	real_vector3d direction;
	c_string_id name;
};
static_assert(sizeof(s_effect_vector) == 0x1C);

struct effect_datum :
	s_datum_header
{
	dword_flags flags;
	long definition_index;
	long looping_sound_index;
	c_string_id input_marker_name;
	byte explicit_marker_index;
	byte conical_distribution_index;
	union { long scale_a_function_reference; byte scale_a_index; };
	union { long scale_b_function_reference; byte scale_b_index; };
	s_location location;
	real_vector3d velocity;
	long object_index;
	s_damage_owner damage_owner;
	s_damage_reporting_info damage_reporting_info;
	char first_person_player_user_mask;
	char first_person_weapon_user_mask;
	char first_person_weapon_user_index;
	long parent_particle_index;
	long event_datum_head;
	real scale_a;
	real scale_b;
	real_point2d impact_size;
	c_static_array<long, 8> location_datum_indices;
	long lightprobe_index;
	real death_delay;
	dword random_seed;
	long breakable_surface_event_index;
	real_plane3d location_constraint_plane;
};
static_assert(sizeof(effect_datum) == 0xA0);

struct event_datum :
	s_datum_header
{
	byte_flags flags;
	byte event_block_index;
	word event_counter;
	long next_event_index;
	real time;
	real duration;
};
static_assert(sizeof(event_datum) == 0x14);

struct effect_location_datum :
	s_datum_header
{
	short node_designator;
	long intermediate_location_index;
	long next_instance_location_index;
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
	long m_effect_datum_index;
	long m_effect_definition_index;
	long m_event_datum_index;
	byte m_event_block_index;
	byte m_priority;
	word m_event_counter;
	long m_flags;

	union
	{
		// #TODO: add the following union members

		//s_create_particle_system m_create_particle_system;
		//s_add_particle_system_location m_add_particle_system_location;
		//s_initialize_particle_system m_initialize_particle_system;
		//s_create_contrail_system m_create_contrail_system;
		//s_add_contrail_system_location m_add_contrail_system_location;
		//s_initialize_contrail_system m_initialize_contrail_system;
		//s_create_light_volume_system m_create_light_volume_system;
		//s_add_light_volume_system_location m_add_light_volume_system_location;
		//s_initialize_light_volume_system m_initialize_light_volume_system;
		//s_create_beam_system m_create_beam_system;
		//s_add_beam_system_location m_add_beam_system_location;
		//s_initialize_beam_system m_initialize_beam_system;
		//s_create_decal_system m_create_decal_system;
		//s_effect_xsynced_bit m_effect_xsynced_bit;
		//s_location_relocated m_location_relocated;
		//s_effect_restarted m_effect_restarted;
		//s_event_restarted m_event_restarted;
		//s_effect_sample_lightmap m_effect_sample_lightmap;

		byte storage[0x44];
	};
};
static_assert(sizeof(s_effect_message) == 0x5C);

struct s_geometry_sample
{
	real_point3d m_sample_point;
	real m_light_probe_r[36];
	real m_light_probe_g[36];
	real m_light_probe_b[36];
	real_vector3d m_diffuse;
	real_vector3d m_normal;
	real_vector3d m_dominant_light_dir;
	real_rgb_color m_dominant_light_intensity;
	real m_dominant_light_contrast;
	bool m_needs_interpolation;
	real m_chocalate_mountain_scale;
};
static_assert(sizeof(s_geometry_sample) == 0x1F8);

struct s_effect_lightprobe
{
	byte_flags m_flags;
	s_geometry_sample m_geometry_sample;
};
static_assert(sizeof(s_effect_lightprobe) == 0x1FC);

extern bool debug_damage_effects;

struct s_game_non_bsp_zone_set;
struct c_scenario_resource_registry;
extern void __cdecl effects_prepare_for_new_zone_set(dword old_active_structure_bsp_mask, dword new_active_structure_bsp_mask);
extern void __cdecl effects_initialize_for_new_non_bsp_zone_set(s_game_non_bsp_zone_set const* new_non_bsp_zone_set);
extern void __cdecl effects_dispose_from_old_non_bsp_zone_set(s_game_non_bsp_zone_set const* old_non_bsp_zone_set);
extern bool __cdecl dangerous_effects_near_player();
extern void __cdecl effect_render(long effect_index, long user_index);
extern long __cdecl effect_new_from_point_vector(long effect_index, real_point3d const* position, real_vector3d const* forward, real_vector3d const* normal, long match_all_markers, long effect_deterministic, real_plane3d const* plane, s_cluster_reference* cluster_reference);
extern void __cdecl effects_dispose();
extern void __cdecl effects_dispose_from_old_map();
extern void __cdecl effects_dispose_from_old_structure_bsp(dword deactivating_structure_bsp_mask);
extern void __cdecl effects_frame_advance(real seconds_elapsed);
extern void __cdecl effects_frame_advance_gpu(real seconds_elapsed);
extern void __cdecl effects_initialize();
extern void __cdecl effects_initialize_for_new_map();
extern void __cdecl effects_initialize_for_new_structure_bsp(dword activating_structure_bsp_mask);
extern void __cdecl effects_prepare_for_non_bsp_zone_set_switch(s_game_non_bsp_zone_set const* old_non_bsp_zone_set, s_game_non_bsp_zone_set const* new_non_bsp_zone_set, c_scenario_resource_registry* pending_zone_registry);
extern void __cdecl effects_render(long user_index, e_effect_pass pass);
extern void __cdecl effects_submit_cheap_first_person_attachments(long user_index);
extern void __cdecl effects_update();

extern void render_debug_damage_effects();
extern void debug_effects_full();

