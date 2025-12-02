#pragma once

#include "cseries/cseries.hpp"
#include "effects/beams.hpp"
#include "effects/contrails.hpp"
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

class c_scenario_resource_registry;

struct effect_vector_field;
struct s_game_non_bsp_zone_set;

enum e_effect_deterministic
{
	_effect_not_deterministic = 0,
	_effect_deterministic,
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
	uns32 flags;
	int32 definition_index;
	int32 looping_sound_index;
	c_string_id input_marker_name;
	uns8 explicit_marker_index;
	uns8 conical_distribution_index;
	union { int32 scale_a_function_reference; uns8 scale_a_index; };
	union { int32 scale_b_function_reference; uns8 scale_b_index; };
	s_location location;
	real_vector3d velocity;
	int32 object_index;
	s_damage_owner damage_owner;
	s_damage_reporting_info damage_reporting_info;
	int8 first_person_player_user_mask;
	int8 first_person_weapon_user_mask;
	int8 first_person_weapon_user_index;
	int32 parent_particle_index;
	int32 event_datum_head;
	real32 scale_a;
	real32 scale_b;
	real_point2d impact_size;
	c_static_array<int32, 8> location_datum_indices;
	int32 lightprobe_index;
	real32 death_delay;
	uns32 random_seed;
	int32 breakable_surface_event_index;
	real_plane3d location_constraint_plane;
};
static_assert(sizeof(effect_datum) == 0xA0);

struct event_datum :
	s_datum_header
{
	uns8 flags;
	uns8 event_block_index;
	uns16 event_counter;
	int32 next_event_index;
	real32 time;
	real32 duration;
};
static_assert(sizeof(event_datum) == 0x14);

struct effect_location_datum :
	s_datum_header
{
	int16 node_designator;
	int32 intermediate_location_index;
	int32 next_instance_location_index;
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
	int32 m_type;
	int32 m_effect_datum_index;
	int32 m_effect_definition_index;
	int32 m_event_datum_index;
	uns8 m_event_block_index;
	uns8 m_priority;
	uns16 m_event_counter;
	int32 m_flags;

	union
	{
		// $TODO add the following union members

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
	real32 m_light_probe_r[36];
	real32 m_light_probe_g[36];
	real32 m_light_probe_b[36];
	real_vector3d m_diffuse;
	real_vector3d m_normal;
	real_vector3d m_dominant_light_dir;
	real_rgb_color m_dominant_light_intensity;
	real32 m_dominant_light_contrast;
	bool m_needs_interpolation;
	real32 m_chocalate_mountain_scale;
};
static_assert(sizeof(s_geometry_sample) == 0x1F8);

struct s_effect_lightprobe
{
	uns8 m_flags;
	s_geometry_sample m_geometry_sample;
};
static_assert(sizeof(s_effect_lightprobe) == 0x1FC);

extern bool debug_damage_effects;

extern void __cdecl effects_prepare_for_new_zone_set(uns32 old_active_structure_bsp_mask, uns32 new_active_structure_bsp_mask);
extern void __cdecl effects_initialize_for_new_non_bsp_zone_set(const s_game_non_bsp_zone_set* new_non_bsp_zone_set);
extern void __cdecl effects_dispose_from_old_non_bsp_zone_set(const s_game_non_bsp_zone_set* old_non_bsp_zone_set);
extern bool __cdecl dangerous_effects_near_player();
extern void __cdecl effect_render(int32 effect_index, int32 user_index);
extern int32 __cdecl effect_new_from_object(c_tag_index definition_index, const s_damage_owner* damage_owner, const s_damage_reporting_info& damage_reporting_info, int32 object_index, real32 scale_a, real32 scale_b, const real_rgb_color* color, const effect_vector_field* impulse_field, e_effect_deterministic deterministic);
extern int32 __cdecl effect_new_from_point_vector(int32 effect_index, const real_point3d* position, const real_vector3d* forward, const real_vector3d* normal, int32 match_all_markers, int32 effect_deterministic, const real_plane3d* plane, s_cluster_reference* cluster_reference);
extern void __cdecl effects_dispose();
extern void __cdecl effects_dispose_from_old_map();
extern void __cdecl effects_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
extern void __cdecl effects_frame_advance(real32 seconds_elapsed);
extern void __cdecl effects_frame_advance_gpu(real32 seconds_elapsed);
extern void __cdecl effects_initialize();
extern void __cdecl effects_initialize_for_new_map();
extern void __cdecl effects_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
extern void __cdecl effects_prepare_for_non_bsp_zone_set_switch(const s_game_non_bsp_zone_set* old_non_bsp_zone_set, const s_game_non_bsp_zone_set* new_non_bsp_zone_set, c_scenario_resource_registry* pending_zone_registry);
extern void __cdecl effects_render(int32 user_index, e_effect_pass pass);
extern void __cdecl effects_submit_cheap_first_person_attachments(int32 user_index);
extern void __cdecl effects_update();

extern void render_debug_damage_effects();
extern void debug_effects_full();

