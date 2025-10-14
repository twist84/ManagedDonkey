#pragma once

#include "cseries/cseries.hpp"
#include "objects/damage.hpp"
#include "objects/damage_owner.hpp"
#include "objects/lights.hpp"
#include "objects/multiplayer_game_objects.hpp"
#include "objects/object_activation_regions.hpp"
#include "objects/object_broadphase.hpp"
#include "objects/object_definitions.hpp"
#include "objects/object_early_movers.hpp"
#include "objects/object_placement.hpp"
#include "objects/object_recycling.hpp"
#include "objects/object_scheduler.hpp"
#include "objects/object_scripting.hpp"
#include "render/render_objects_static_lighting.hpp"

#define OBJECT_GET(TYPE, INDEX) ((TYPE*)object_get_and_verify_type(INDEX, _object_mask_all))

#define UNIT_GET(INDEX) ((struct unit_datum*)object_get_and_verify_type(INDEX, _object_mask_unit))
#define BIPED_GET(INDEX) ((struct biped_datum*)object_get_and_verify_type(INDEX, _object_mask_biped))
#define VEHICLE_GET(INDEX) ((struct vehicle_datum*)object_get_and_verify_type(INDEX, _object_mask_vehicle))
#define GIANT_GET(INDEX) ((struct giant_datum*)object_get_and_verify_type(INDEX, _object_mask_giant))

#define ITEM_GET(INDEX) ((struct item_datum*)object_get_and_verify_type(INDEX, _object_mask_item))
#define WEAPON_GET(INDEX) ((struct weapon_datum*)object_get_and_verify_type(INDEX, _object_mask_weapon))
#define EQUIPMENT_GET(INDEX) ((struct equipment_datum*)object_get_and_verify_type(INDEX, _object_mask_equipment))

#define PROJECTILE_GET(INDEX) ((struct projectile_datum*)object_get_and_verify_type(INDEX, _object_mask_projectile))
#define SCENERY_GET(INDEX) ((struct scenery_datum*)object_get_and_verify_type(INDEX, _object_mask_scenery))
#define SOUND_SCENERY_GET(INDEX) ((struct sound_scenery_datum*)object_get_and_verify_type(INDEX, _object_mask_sound_scenery))
#define CRATE_GET(INDEX) ((struct crate_datum*)object_get_and_verify_type(INDEX, _object_mask_crate))
#define CREATURE_GET(INDEX) ((struct creature_datum*)object_get_and_verify_type(INDEX, _object_mask_creature))
#define EFFECT_SCENERY_GET(INDEX) ((struct effect_scenery_datum*)object_get_and_verify_type(INDEX, _object_mask_effect_scenery))

#define DEVICE_GET(INDEX) ((struct device_datum*)object_get_and_verify_type(INDEX, _object_mask_device))
#define ARG_DEVICE_GET(INDEX) ((struct arg_device_datum*)object_get_and_verify_type(INDEX, _object_mask_arg_device))
#define TERMINAL_GET(INDEX) ((struct terminal_datum*)object_get_and_verify_type(INDEX, _object_mask_terminal))
#define MACHINE_GET(INDEX) ((struct machine_datum*)object_get_and_verify_type(INDEX, _object_mask_machine))
#define CONTROL_GET(INDEX) ((struct control_datum*)object_get_and_verify_type(INDEX, _object_mask_control))

enum e_object_mask
{
	_object_mask_none = 0,
	_object_mask_all = NONE,
	
	_object_mask_unit = 
		FLAG(_object_type_biped) | 
		FLAG(_object_type_vehicle) | 
		FLAG(_object_type_giant),
	_object_mask_biped = FLAG(_object_type_biped),
	_object_mask_vehicle = FLAG(_object_type_vehicle),
	_object_mask_giant = FLAG(_object_type_giant),

	_object_mask_item = 
		FLAG(_object_type_weapon) | 
		FLAG(_object_type_equipment),
	_object_mask_weapon = FLAG(_object_type_weapon),
	_object_mask_equipment = FLAG(_object_type_equipment),

	_object_mask_projectile = FLAG(_object_type_projectile),
	_object_mask_scenery = FLAG(_object_type_scenery),
	_object_mask_crate = FLAG(_object_type_crate),
	_object_mask_creature = FLAG(_object_type_creature),
	_object_mask_sound_scenery = FLAG(_object_type_sound_scenery),
	_object_mask_effect_scenery = FLAG(_object_type_effect_scenery),

	_object_mask_device = 
		FLAG(_object_type_arg_device) | 
		FLAG(_object_type_terminal) | 
		FLAG(_object_type_machine) | 
		FLAG(_object_type_control),
	_object_mask_arg_device = FLAG(_object_type_arg_device),
	_object_mask_terminal = FLAG(_object_type_terminal),
	_object_mask_machine = FLAG(_object_type_machine),
	_object_mask_control = FLAG(_object_type_control),

	_object_mask_mover =
		_object_mask_unit |
		_object_mask_creature,

	_object_mask_editor_placeable_objects = 
		_object_mask_unit | 
		_object_mask_item | 
		_object_mask_arg_device | 
		_object_mask_terminal | 
		_object_mask_scenery | 
		_object_mask_machine | 
		_object_mask_crate | 
		_object_mask_creature,
};
static_assert(0b0010000000000011 == _object_mask_unit);
static_assert(0b0000000000000001 == _object_mask_biped);
static_assert(0b0000000000000010 == _object_mask_vehicle);
static_assert(0b0010000000000000 == _object_mask_giant);
static_assert(0b0000000000001100 == _object_mask_item);
static_assert(0b0000000000000100 == _object_mask_weapon);
static_assert(0b0000000000001000 == _object_mask_equipment);
static_assert(0b0000000001000000 == _object_mask_projectile);
static_assert(0b0000000010000000 == _object_mask_scenery);
static_assert(0b0000010000000000 == _object_mask_sound_scenery);
static_assert(0b0000100000000000 == _object_mask_crate);
static_assert(0b0001000000000000 == _object_mask_creature);
static_assert(0b0100000000000000 == _object_mask_effect_scenery);
static_assert(0b0000001100110000 == _object_mask_device);
static_assert(0b0000000000010000 == _object_mask_arg_device);
static_assert(0b0000000000100000 == _object_mask_terminal);
static_assert(0b0000000100000000 == _object_mask_machine);
static_assert(0b0000001000000000 == _object_mask_control);
static_assert(0b0011100110111111 == _object_mask_editor_placeable_objects);

// Same as Halo 3
enum e_object_data_flags
{
	_object_hidden_bit = 0,
	_object_always_active_bit,
	_object_being_created_bit,
	_object_wholly_under_media_bit,
	_object_has_attached_lights_bit,
	_object_has_attached_looping_sounds_bit,
	_object_has_unattached_lights_bit,
	_object_in_limbo_bit,
	_object_connected_to_map_bit,
	_object_uses_collidable_list_bit,
	_object_mirrored_bit,
	_object_render_model_has_instances_bit,
	_object_static_lighting_recompute_bit,
	_object_static_lighting_raycast_sideways_bit,
	_object_render_time_node_matrices_complete_bit,
	_object_clip_plane_bit,
	_object_shadowless_bit,
	_object_deleted_when_deactivated_bit,
	_object_outside_of_map_bit,
	_object_cinematic_lod_bit,
	_object_cinematic_collision_bit,
	_object_cinematic_visibility_bit,
	_object_static_pathfinding_bit,
	_object_dynamic_pathfinding_disabled_bit,
	_object_uses_cinematic_lighting_bit,
	_object_has_override_bit,
	_object_created_with_parent_bit,
	_object_reconnect_to_map_bit,
	_object_ever_referenced_by_hs_bit,
	_object_orientations_frozen_bit,
	_object_render_only_orientations_needed_bit,
	_object_is_prt_and_lightmapped_bit,

	k_object_data_flags_count
};

struct object_header_block_reference
{
	int16 size;
	uns16 offset;
};
static_assert(sizeof(object_header_block_reference) == 0x4);

struct _object_datum
{
	c_flags<e_object_data_flags, uns32, k_object_data_flags_count> flags;

	// override
	int32 collision_damage_definition_index;

	int32 next_object_index;
	int32 first_child_object_index;
	int32 parent_object_index;
	uns8 parent_node_index;
	uns8 inhibited_flags;
	int16 scenario_datum_index;
	int16 map_variant_index;
	s_location location;
	real_point3d bounding_sphere_center;
	real32 bounding_sphere_radius;
	real_point3d attached_bounds_center;
	real32 attached_bounds_radius;
	real_point3d attached_bounds_sphere_center;
	real32 attached_bounds_sphere_radius;
	int32 first_cluster_reference_index;
	real_point3d position;
	real_vector3d forward;
	real_vector3d up;
	real_vector3d transitional_velocity;
	real_vector3d angular_velocity;
	real32 scale;
	c_object_identifier object_identifier;
	int16 name_index;
	int8 bsp_placement_policy;
	uns8 keyframed_object_collision_damage_ticks;
	int32 havok_component_index;
	int32 local_physics_space_object_index;
	int32 last_motion_time;
	uns32 physics_flags;
	uns8 physics_deactivation_count;
	uns8 physically_attached_node;
	uns8 phantom_power_scale;
	uns8 phantom_speed_scale;
	int16 in_water_ticks;
	int8 environment_interpenetration_ticks;
	uns8 brittle_collision_damage_timer;
	int16 scale_intepolation_ticks;
	int8 variant_index;
	uns8 object_ai_flags;
	uns32 melee_damage_unique_identifier;
	s_damage_owner damage_owner;
	int16 structure_bsp_fake_lightprobe_index;
	bool created_at_rest;
	uns8 clusters_touched_on_connection;
	int32 simulation_object_glue_index;
	int16 owner_team_index;
	uns8 simulation_flags;
	int8 child_variant_index;
	int32 simulation_object_interpolation_time;
	real_point3d simulation_object_interpolation_position;
	int32 first_widget_index;
	uns16 destroyed_constraints;
	uns16 loosened_constraints;
	real32 maximum_body_vitality;
	real32 maximum_shield_vitality;
	real32 shield_charge_delay;
	real32 body_vitality;
	real32 shield_vitality;
	real32 current_body_damage;
	real32 current_shield_damage;
	real32 recent_body_damage;
	real32 recent_shield_damage;
	real32 shield_impact_current_body_damage;
	real32 shield_impact_current_shield_damage;
	int16 shield_stun_ticks;
	int16 body_stun_ticks;
	uns32 damage_flags;
	int8 damaged_explosion_timer;
	int8 body_damage_delay_ticks;
	int8 shield_impact_decay_timer;
	uns8 recycling_flags;
	int32 next_recycling_object_index;
	int32 recycling_time;
	int32 parent_recycling_group;
	int32 next_recycling_group_member;
	int32 next_sync_action_participant_index;
	int32 sync_action_name;
	c_animation_id sync_animation_id;
	int32 ai_sync_action_arranger_index;
	uns16 render_flags;
	object_header_block_reference original_node_orientations;
	object_header_block_reference node_orientations;
	object_header_block_reference node_matrices;
	object_header_block_reference region_information;
	object_header_block_reference attachments;
	object_header_block_reference damage_sections;
	object_header_block_reference change_colors;
	object_header_block_reference animation;
	object_header_block_reference multiplayer;
	byte __pad16E[0x2];

	// (bsp_index << 24) | probe_index & 0xFFFFFF
	int32 air_probe_index; // airprobes block

	// (bsp_index << 24) | typed_probe_index & 0xFFFFFF
	union
	{
		// scenery probes block
		int32 scenery_air_probe_index;

		// device machine probes block
		int32 device_machine_air_probe_index;
	};
};
static_assert(sizeof(_object_datum) == 0x174);

struct object_datum
{
	int32 definition_index;
	_object_datum object;
};
static_assert(sizeof(object_datum) == sizeof(int32) + sizeof(_object_datum));

enum e_object_header_flags
{
	_object_header_active_bit = 0,
	_object_header_awake_bit,
	_object_header_requires_motion_bit,
	_object_header_post_update_bit,
	_object_header_being_deleted_bit,
	_object_header_do_not_update_bit,
	_object_header_connected_to_map_bit,
	_object_header_child_bit,

	k_object_header_flags_count
};

struct object_header_datum :
	s_datum_header
{
	c_flags<e_object_header_flags, uns8, k_object_header_flags_count> flags;
	c_enum<e_object_type, uns8, _object_type_biped, k_object_type_count> object_type;
	s_cluster_reference cluster_reference;
	int16 data_size;
	int32 datum_handle;
	object_datum* datum;
};
static_assert(sizeof(object_header_datum) == 0x10);

class c_object_iterator_base
{
public:
	int32 get_index();

protected:
	void object_iterator_begin_internal(uns32 type_flags, uns32 header_flags, uns32 iteration_match_flags, int32 next_absolute_index);
	bool object_iterator_next_internal();
	bool object_iterator_next_with_match_flags_internal();
	object_datum* get_datum_internal();

private:
	struct s_object_iterator
	{
		c_flags<e_object_type, uns32, k_object_type_count> type_flags;
		uns32 header_mask;
		uns32 iteration_match_flags;
		int32 object_index;
		int32 index;
		uns32 signature; // 0x86868686 is initialized
	};
	static_assert(sizeof(s_object_iterator) == 0x18);

	object_datum* m_object;
	s_object_iterator m_iterator;
};
static_assert(sizeof(c_object_iterator_base) == 0x1C);

template<typename t_object_type>
class c_object_iterator :
	public c_object_iterator_base
{
public:
	void begin(uns32 type_flags, uns8 match_flags)
	{
		object_iterator_begin_internal(type_flags, match_flags, 0, NONE);
	}

	bool next()
	{
		return object_iterator_next_internal();
	}

	t_object_type* get_datum()
	{
		return reinterpret_cast<t_object_type*>(get_datum_internal());
	}
};

struct s_model_customization_region_permutation // s_model_customization_region
{
	int32 region_name;
	int32 permutation_name;
};
static_assert(sizeof(s_model_customization_region_permutation) == 0x8);

struct object_placement_data
{
	object_placement_data();

	int32 definition_index;
	c_object_identifier object_identifier;
	int32 model_variant_index;
	int32 scenario_datum_index;
	uns8 bsp_placement_policy;
	byte pad1[0x3];
	uns32 flags;
	real_point3d position;
	real_vector3d forward;
	real_vector3d up;
	real_vector3d translational_velocity;
	real_vector3d angular_velocity;
	real32 scale;
	int32 owner_player_index;
	int32 owner_object_index;
	int32 owner_team_index;
	s_damage_owner damage_owner;
	c_flags<int32, uns32, 5> change_color_override_mask;
	c_static_array<real_rgb_color, 5> change_color_overrides;
	int32 model_customization_override_count;
	c_static_array<s_model_customization_region_permutation, 16> model_customization_overrides;
	int32 disabled_regions;
	uns16 destroyed_constraints;
	uns16 loosened_constraints;
	int16 ai_state_type;
	int16 ai_state_size;
	int16 ai_state_alignment_bits;
	bool location_set;
	byte __data147[0x1];
	s_location location;
	bool multiplayer_cinematic_object;
	int32 parent_object_index;
	c_string_id parent_marker;
	c_string_id child_marker;
	s_scenario_multiplayer_object_properties multiplayer_properties;
};
static_assert(sizeof(object_placement_data) == 0x18C);
static_assert(0x000 == OFFSETOF(object_placement_data, definition_index));
static_assert(0x004 == OFFSETOF(object_placement_data, object_identifier));
static_assert(0x00C == OFFSETOF(object_placement_data, model_variant_index));
static_assert(0x010 == OFFSETOF(object_placement_data, scenario_datum_index));
static_assert(0x014 == OFFSETOF(object_placement_data, bsp_placement_policy));
static_assert(0x015 == OFFSETOF(object_placement_data, pad1));
static_assert(0x018 == OFFSETOF(object_placement_data, flags));
static_assert(0x01C == OFFSETOF(object_placement_data, position));
static_assert(0x028 == OFFSETOF(object_placement_data, forward));
static_assert(0x034 == OFFSETOF(object_placement_data, up));
static_assert(0x040 == OFFSETOF(object_placement_data, translational_velocity));
static_assert(0x04C == OFFSETOF(object_placement_data, angular_velocity));
static_assert(0x058 == OFFSETOF(object_placement_data, scale));
static_assert(0x05C == OFFSETOF(object_placement_data, owner_player_index));
static_assert(0x060 == OFFSETOF(object_placement_data, owner_object_index));
static_assert(0x064 == OFFSETOF(object_placement_data, owner_team_index));
static_assert(0x068 == OFFSETOF(object_placement_data, damage_owner));
static_assert(0x074 == OFFSETOF(object_placement_data, change_color_override_mask));
static_assert(0x078 == OFFSETOF(object_placement_data, change_color_overrides));
static_assert(0x0B4 == OFFSETOF(object_placement_data, model_customization_override_count));
static_assert(0x0B8 == OFFSETOF(object_placement_data, model_customization_overrides));
static_assert(0x138 == OFFSETOF(object_placement_data, disabled_regions));
static_assert(0x13C == OFFSETOF(object_placement_data, destroyed_constraints));
static_assert(0x13E == OFFSETOF(object_placement_data, loosened_constraints));
static_assert(0x140 == OFFSETOF(object_placement_data, ai_state_type));
static_assert(0x142 == OFFSETOF(object_placement_data, ai_state_size));
static_assert(0x144 == OFFSETOF(object_placement_data, ai_state_alignment_bits));
static_assert(0x146 == OFFSETOF(object_placement_data, location_set));
static_assert(0x147 == OFFSETOF(object_placement_data, __data147));
static_assert(0x148 == OFFSETOF(object_placement_data, location));
static_assert(0x14A == OFFSETOF(object_placement_data, multiplayer_cinematic_object));
static_assert(0x14C == OFFSETOF(object_placement_data, parent_object_index));
static_assert(0x150 == OFFSETOF(object_placement_data, parent_marker));
static_assert(0x154 == OFFSETOF(object_placement_data, child_marker));
static_assert(0x158 == OFFSETOF(object_placement_data, multiplayer_properties));

struct object_marker
{
	int16 node_index;
	real_matrix4x3 node_matrix;
	real_matrix4x3 matrix;
	real32 radius;
};
static_assert(sizeof(object_marker) == 0x70);

struct s_object_cluster_payload
{
	c_enum<e_object_type, uns8, _object_type_biped, k_object_type_count> object_type;
	uns8 object_visibility_cull_flags;
	uns8 object_collision_cull_flags;
	real_point3d bounding_sphere_center;
	real32 bounding_sphere_radius;
};
static_assert(sizeof(s_object_cluster_payload) == 0x14);

struct s_object_render_data
{
	int32 cached_render_state_index;
};
static_assert(sizeof(s_object_render_data) == sizeof(int32));

struct s_object_globals
{
	bool connected_to_structure_bsp;
	bool force_garbage_collection;
	bool garbage_collection_unsafe;
	bool garbage_collection_cannot_run;
	int16 active_garbage_object_count;
	int16 active_garbage_player_bodies;
	int32 first_garbage_object_index;
	int32 last_garbage_warn_time;
	int32 last_garbage_fail_time;
	int32 last_loud_noise_time;
	int32 object_identifier_salt;
	int32 object_update_absolute_index;
	render_lighting cinematic_lighting;
	real32 scripted_object_functions[4];
	bool custom_animations_hold_on_last_frame;
	bool custom_animations_prevent_lipsync_head_movement;
	bool warthog_chaingun_light_disabled;
	c_static_array<int32, k_object_type_count> scenario_object_to_object_index_type_start_index;
	c_static_array<int32, 6368> scenario_object_to_object_index;
};
static_assert(sizeof(s_object_globals) == 0x6608);

struct s_object_render_thread_message
{
	int32 object_index;
	int16 message_type;
	int16 message_param;
};
static_assert(sizeof(s_object_render_thread_message) == 0x8);

struct objects_information
{
	int16 object_count;
	int16 free_object_count;
	int16 active_object_count;
	int16 active_garbage_count;
	real32 used_memory;
	real32 contiguous_free_memory;

	// unsure of these names
	int32 collideable_object_actual_count;
	int32 collideable_object_maximum_count;
	int32 noncollideable_object_actual_count;
	int32 noncollideable_object_maximum_count;
};
static_assert(sizeof(objects_information) == 0x20);

enum
{
	k_maximum_object_override_count = 16
};

struct s_object_override
{
	bool valid;
	byte pad[0x3];
	int32 object_index;
	int32 shader_index;
};
static_assert(sizeof(s_object_override) == 0xC);

struct s_object_override_globals
{
	s_object_override overrides[k_maximum_object_override_count];
};
static_assert(sizeof(s_object_override_globals) == sizeof(s_object_override) * k_maximum_object_override_count);

extern s_object_override_globals object_override_globals;

extern bool debug_objects;
extern bool debug_objects_early_movers;
extern bool debug_objects_bounding_spheres;
extern bool debug_objects_render_models;
extern bool debug_objects_collision_models;
extern bool debug_objects_physics_models;
extern bool debug_objects_sound_spheres;
extern bool debug_objects_indices;
extern bool debug_objects_programmer;
extern bool debug_objects_garbage;
extern bool debug_objects_names;
extern bool debug_objects_names_full;
extern bool debug_objects_active_nodes;
extern bool debug_objects_animation_times;
extern bool debug_objects_functions;
extern bool debug_objects_position_velocity;
extern bool debug_objects_origin;
extern bool debug_objects_root_node;
extern bool debug_objects_root_node_print;
extern bool debug_objects_attached_bounding_spheres;
extern bool debug_objects_dynamic_render_bounding_spheres;
extern bool debug_objects_model_targets;
extern bool debug_objects_profile_times;
extern bool debug_objects_water_physics;
extern bool debug_objects_expensive_physics;
extern bool debug_objects_contact_points;
extern bool debug_objects_constraints;
extern bool debug_objects_vehicle_physics;
extern bool debug_objects_mass;
extern bool debug_objects_pathfinding;
extern bool debug_objects_node_bounds;
extern bool debug_objects_animation;
extern bool debug_objects_skeletons;

struct s_scenario_object;
struct s_game_cluster_bit_vectors;

extern void* __cdecl object_header_block_get(int32 object_index, const object_header_block_reference* reference);
extern void* __cdecl object_header_block_get_with_count(int32 object_index, const object_header_block_reference* reference, unsigned int element_size, int32* element_count);
extern const object_header_datum* __cdecl object_header_get(int32 object_index);
extern object_header_datum* __cdecl object_header_get_mutable(int32 object_index);
extern object_datum* __cdecl object_get(int32 object_index);
extern void* __cdecl object_get_and_verify_type(int32 object_index, uns32 object_type_mask);
extern e_object_type __cdecl object_get_type(int32 object_index);
extern void __cdecl object_get_bounding_sphere(int32 object_index, real_point3d* center, real32* radius);
extern int32 __cdecl cluster_get_first_collideable_object(int32* datum_index, s_cluster_reference cluster_reference);
extern int32 __cdecl cluster_get_first_collideable_object_and_payload(int32* datum_index, s_cluster_reference cluster_reference, const s_object_cluster_payload** payload);
extern int32 __cdecl cluster_get_first_noncollideable_object_and_payload(int32* datum_index, s_cluster_reference cluster_reference, const s_object_cluster_payload** payload);
extern int32 __cdecl cluster_get_next_collideable_object(int32* datum_index);
extern int32 __cdecl cluster_get_next_collideable_object_and_payload(int32* datum_index, const s_object_cluster_payload** payload);
extern int32 __cdecl cluster_get_next_noncollideable_object_and_payload(int32* datum_index, const s_object_cluster_payload** payload);
extern int32 __cdecl find_first_predicted_object_recursive(int32 object_index);
extern void __cdecl garbage_collect_now();
extern void __cdecl garbage_collect_unsafe();
extern bool __cdecl garbage_collection_can_run();
extern void __cdecl hs_object_definition_predict_all(int32 definition_index);
extern void __cdecl hs_object_definition_predict_low(int32 definition_index);
extern int32 __cdecl object_accepted_local_physics_object_get(int32 object_index);
extern void __cdecl object_activate(int32 object_index);
extern void __cdecl object_adjust_garbage_timer(int32 object_index, int32 time);
extern bool __cdecl object_can_be_melee_instant_killed(int32 object_index);
extern bool __cdecl object_can_interpolate(int32 object_index);
extern void __cdecl object_choose_variant(int32 object_index, int32 name);
extern void __cdecl object_cinematic_collision(int32 object_index, bool enable);
extern void __cdecl object_cinematic_lod(int32 object_index, bool enable);
extern void __cdecl object_cinematic_visibility(int32 object_index, bool enable);
extern void __cdecl object_clear_sync_action(int32 object_index);
extern void __cdecl object_connect_lights(int32 object_index, bool disconnect_this_object, bool reconnect_this_object);
extern void __cdecl object_connect_lights_recursive(int32 object_index, bool disconnect_this_object, bool reconnect_this_object, bool a3, bool a4);
extern int32 __cdecl object_count(int32 type_flags, uns8 header_mask);
extern void __cdecl object_create_attachments(int32 object_index);
extern void __cdecl object_create_children(int32 object_index);
extern bool __cdecl object_custom_animations_hold_on_last_frame();
//extern void __cdecl object_deactivate(int32 object_index);
//extern void __cdecl object_deactivate(int32 object_index, bool a2);
extern bool __cdecl object_definition_predict_low(int32 object_definition_index);
extern void __cdecl object_delete(int32 object_index);
extern void __cdecl object_delete_all_multiplayer_cinematic_objects();
extern void __cdecl object_delete_attachments(int32 object_index);
extern void __cdecl object_delete_immediately(int32 object_index);
extern void __cdecl object_delete_recursive(int32 object_index, bool deactivate);
extern void __cdecl object_destroy_instance_group(int32 object_index, int32 instance_group_index);
extern void __cdecl object_detach(int32 object_index);
extern void __cdecl object_detach_from_node(int32 object_index, const real_matrix4x3* node);
extern void __cdecl object_detach_gamestate_entity(int32 object_index, int32 gamestate_index);
extern void __cdecl object_detach_immediate(int32 object_index);
extern void __cdecl object_detach_internal(int32 object_index, bool compute_bounding_sphere);
extern void __cdecl object_detach_predicted_objects(int32 object_index);
extern void __cdecl object_disconnect_from_map(int32 object_index, bool broadphase_remove_object);
extern void __cdecl object_disconnect_from_physics(int32 object_index);
extern bool __cdecl object_find_initial_location(int32 object_index, s_location* location);
extern void __cdecl object_find_structure_bsp_fake_lightprobe_index(int32 structure_bsp_index, int32 object_index);
extern bool __cdecl object_force_inside_bsp(int32 object_index, const real_point3d* position, int32 ignore_object_index);
extern void __cdecl object_freeze_node_orientations(int32 object_index, bool a2);
extern bool __cdecl object_function_get_function_value(int32 object_index, const s_object_function_definition* function, int32 object_definition_index, real32* out_function_magnitude, bool* deterministic);
extern int32 __cdecl object_get_attachment_marker_name(int32 object_index, int16 attachment_index);
extern bool __cdecl object_get_base_change_color_by_index(int32 object_index, int32 change_color_index, real_rgb_color* color);
extern real_point3d* __cdecl object_get_center_of_mass(int32 object_index, real_point3d* center);
extern bool __cdecl object_get_change_color(int32 object_index, int32 change_color_index, real_rgb_color* change_color);
extern void __cdecl object_get_closest_point_and_normal(int32 object_index, const real_point3d* origin, real_point3d* closest_point, real_vector3d* normal);
extern void __cdecl object_get_damage_owner(int32 object_index, s_damage_owner* owner);
extern bool __cdecl object_get_function_value(int32 object_index, int32 function_name, int32 object_definition_index, real32* out_function_magnitude);
extern s_location* __cdecl object_get_location(int32 object_index, s_location* location);
extern int16 __cdecl object_get_markers_by_string_id(int32 object_index, string_id marker_name, object_marker* markers, int16 maximum_marker_count);
extern real_matrix4x3* __cdecl object_get_node_matrix(int32 object_index, int16 node_index);
extern void __cdecl object_get_orientation(int32 object_index, real_vector3d* forward, real_vector3d* up);
extern real_point3d* __cdecl object_get_origin(int32 object_index, real_point3d* origin);
extern int32 __cdecl object_get_root_object(int32 object_index);
extern int32 __cdecl object_get_ultimate_parent(int32 object_index);
extern int32 __cdecl object_get_variant_index(int32 object_index);
extern void __cdecl object_get_velocities(int32 object_index, real_vector3d* linear_velocity, real_vector3d* angular_velocity);
extern real_matrix4x3* __cdecl object_get_world_matrix(int32 object_index, real_matrix4x3* matrix);
extern real_matrix4x3* __cdecl object_get_world_matrix_interpolated(int32 object_index, real_matrix4x3* out_matrix);
extern bool __cdecl object_has_animation_manager(int32 object_index);
extern bool __cdecl object_has_multiplayer_properties(int32 object_index);
extern bool __cdecl object_header_block_allocate(int32 object_index, int16 block_reference_offset, int16 size, int16 alignment_bits);
extern void __cdecl object_header_delete(int32 object_index);
extern int32 __cdecl object_header_new(int16 size);
extern int32 __cdecl object_index_from_name_index(int16 name_index);
extern int32 __cdecl object_index_from_scenario_object_index(e_object_type object_type, int32 object_index);
extern bool __cdecl object_is_at_rest(int32 object_index);
extern bool __cdecl object_is_being_deleted(int32 object_index);
extern bool __cdecl object_is_connected_to_map(int32 object_index);
extern bool __cdecl object_is_hidden(int32 object_index);
extern bool __cdecl object_is_hidden_internal(const object_header_datum* object_header, const object_datum* object);
extern bool __cdecl object_is_hidden_non_recursive(int32 object_index);
extern bool __cdecl object_is_multiplayer_cinematic_object(int32 object_index);
extern bool __cdecl object_is_or_contains_player(int32 object_index);
extern bool __cdecl object_is_running_sync_action(int32 object_index);
extern bool __cdecl object_is_vehicle(int32 object_index);
extern int32 __cdecl object_list_children_by_definition(int32 object_index, int32 definition_index);
extern bool __cdecl object_load_scenario_placement_matrices(int32 object_index);
extern int32 __cdecl object_local_physics_object_get(int32 object_index);
extern int32 __cdecl object_lookup_variant_index_from_name(int32 object_index, int32 name);
extern void __cdecl object_make_noise(int32 object_index, int32 sound_type, int32 ai_sound_volume);
extern void __cdecl object_marker_begin();
extern void __cdecl object_marker_end();
extern void __cdecl object_marker_reopen();
extern void __cdecl object_move(int32 object_index);
extern void __cdecl object_move_position(int32 object_index, const real_point3d* position, const real_vector3d* forward, const real_vector3d* up, const s_location* location);
extern void __cdecl object_name_list_allocate();
extern void __cdecl object_name_list_clear();
extern void __cdecl object_name_list_delete(int32 object_index);
extern int32 __cdecl object_name_list_lookup(int16 name_index);
extern void __cdecl object_name_list_new(int32 name, int16 name_index);
extern bool __cdecl object_needs_rigid_body_update(int32 object_index);
extern int32 __cdecl object_new(object_placement_data* data);
extern int32 __cdecl object_new_by_name(int16 name_index, bool displace_previous_object, bool multiplayer_cinematic_object);
extern bool __cdecl object_node_orientations_frozen(int32 object_index);
extern void __cdecl object_notify_in_local_physics_object(int32 object_index, int32 local_physics_object_index);
extern int32 __cdecl object_override_create(int32 object_index);
extern int32 __cdecl object_override_find(int32 object_index);
extern s_object_override* __cdecl object_override_get(int32 override_index);
extern int32 __cdecl object_override_get_shader(int32 object_index);
extern void __cdecl object_override_set_shader(int32 object_index, int32 shader_index);
extern void __cdecl object_place(int32 object_index, const s_scenario_object* scenario_object);
extern void __cdecl object_placement_data_copy_change_colors(object_placement_data* data, int32 object_index);
extern void __cdecl object_placement_data_new(object_placement_data* data, int32 definition_index, int32 owner_object_index, const s_damage_owner* damage_owner);
extern void __cdecl object_placement_data_set_location(object_placement_data* data, const struct s_location* location);
extern void __cdecl object_postprocess_node_matrices(int32 object_index);
extern void __cdecl object_pre_delete_recursive(int32 object_index, bool a2);
extern bool __cdecl object_predict(int32 object_index, bool low);
extern bool __cdecl object_predict_all(int32 object_index);
extern bool __cdecl object_predict_low(int32 object_index);
extern void __cdecl object_prepare_axis_vectors(int32 object_index, real_vector3d* forward, real_vector3d* up);
extern void __cdecl object_reconnect_to_map(int32 object_index, bool a2, const s_location* location);
extern void __cdecl object_reconnect_to_physics(int32 object_index);
extern void __cdecl object_register_scenario_object(int32 object_index);
extern void __cdecl object_reinitialize_from_placement(int32 object_index, object_placement_data* data, bool at_rest);
extern void __cdecl object_remove_from_list(int32* object_list, int32 object_index);
extern void __cdecl object_remove_pending_messages(int32 object_index);
extern void __cdecl object_render_debug(int32 object_index);
extern void __cdecl object_reset(int32 object_index);
extern void __cdecl object_reset_interpolation(int32 object_index);
extern void __cdecl object_respond_to_physics(int32 object_index);
extern void __cdecl object_resurrect(int32 object_index);
extern void __cdecl object_set_always_active(int32 object_index, bool always_active);
extern void __cdecl object_set_at_rest(int32 object_index, bool at_rest);
extern bool __cdecl object_set_base_change_color_by_index(int32 object_index, int32 color_index, const real_rgb_color* color);
extern void __cdecl object_set_body_vitality(int32 object_index, real32 body_vitality, int16 body_stun_ticks);
extern void __cdecl object_set_custom_animation_speed(int32 object_index, real32 custom_animation_speed);
extern void __cdecl object_set_custom_animations_hold_on_last_frame(bool custom_animations_hold_on_last_frame);
extern void __cdecl object_set_custom_animations_prevent_lipsync_head_movement(bool custom_animations_prevent_lipsync_head_movement);
extern void __cdecl object_set_damage_owner(int32 object_index, const s_damage_owner* damage_owner, bool a3);
extern void __cdecl object_set_desired_velocities(int32 object_index, const real_vector3d* transitional_velocity, const real_vector3d* angular_velocity, bool a4);
extern void __cdecl object_set_garbage(int32 object_index, bool a2, int32 collection_ticks);
extern void __cdecl object_set_hidden(int32 object_index, bool hidden);
extern void __cdecl object_set_in_limbo(int32 object_index, bool deactivate);
extern void __cdecl object_set_infinite_shield_stun(int32 object_index);
extern void __cdecl object_set_initial_change_colors(int32 object_index, c_flags<int8, uns8, 5> active_change_colors, const real_rgb_color* change_colors);
extern void __cdecl object_set_invalid_for_recycling(int32 object_index, bool a2);
extern bool __cdecl object_set_model_state(int32 object_index, int32 a2, int32 a3, bool a4);
extern void __cdecl object_set_model_state_property(int32 object_index, int32 a2, int32 a3, bool a4, bool a5);
extern void __cdecl object_set_model_state_property_per_region(int32 object_index, int32 a2, int32 a3, int16 a4, bool a5);
extern void __cdecl object_set_object_index_for_name_index(int16 name_index, int32 object_index);
extern bool __cdecl object_set_position_internal(int32 object_index, const real_point3d* desired_position, const real_vector3d* desired_forward, const real_vector3d* desired_up, const s_location* location, bool compute_node_matrices, bool set_havok_object_position, bool in_editor, bool disconnected);
extern void __cdecl object_set_region_permutation_direct(int32 object_index, int32 region_name, int32 permutation_name);
extern void __cdecl object_set_requires_motion(int32 object_index);
extern void __cdecl object_set_shadowless(int32 object_index, bool shadowless);
extern void __cdecl object_set_variant_direct(int32 object_index, int32 variant_name);
extern void __cdecl object_set_velocities(int32 object_index, const real_vector3d* linear_velocity, const real_vector3d* angular_velocity);
extern void __cdecl object_set_position(int32 object_index, const real_point3d* desired_position, const real_vector3d* desired_forward, const real_vector3d* desired_up, const s_location* location);
extern void __cdecl object_set_position_direct(int32 object_index, const real_point3d* desired_position, const real_vector3d* desired_forward, const real_vector3d* desired_up, const s_location* location, bool in_editor);
extern void __cdecl object_set_position_in_editor(int32 object_index, const real_point3d* desired_position, const real_vector3d* desired_forward, const real_vector3d* desired_up, const s_location* location, bool at_rest);
extern void __cdecl object_set_position_in_sandbox_editor(int32 object_index, const real_point3d* desired_position, const real_vector3d* desired_forward, const real_vector3d* desired_up, const s_location* location);
extern void* __cdecl object_try_and_get_and_verify_type(int32 object_index, uns32 object_type_mask);
extern bool __cdecl object_unmarked(int32 object_index);
extern void __cdecl object_unregister_scenario_object(int32 object_index);
extern bool __cdecl object_update(int32 object_index);
extern void __cdecl object_update_collision_culling(int32 object_index);
extern void __cdecl object_update_visibility_culling(int32 object_index);
extern bool __cdecl object_visible_to_any_player(int32 object_index);
extern void __cdecl object_wake(int32 object_index);
extern void __cdecl objects_activation(const s_game_cluster_bit_vectors* a1, const s_game_cluster_bit_vectors* a2);
extern bool __cdecl objects_can_connect_to_map();
extern int32 __cdecl objects_compact_memory_pool();
extern void __cdecl objects_detach_from_simulation();
extern void __cdecl objects_dispose();
extern void __cdecl objects_dispose_from_old_map();
extern void __cdecl objects_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
extern void __cdecl objects_enable_warthog_chaingun_light(bool enable_warthog_chaingun_light);
extern void __cdecl objects_garbage_collection();
extern int32 __cdecl objects_get_active_garbage_count();
extern int32 __cdecl objects_get_contiguous_free_memory_size();
extern int32 __cdecl objects_get_first_garbage_object();
extern int32 __cdecl objects_get_free_object_header_count();
extern int32 __cdecl objects_get_next_garbage_object(int32 object_index);
extern void __cdecl objects_handle_deleted_object(int32 object_index);
extern void __cdecl objects_handle_deleted_player(int32 object_index);
extern int32 __cdecl objects_in_sphere(uns32 class_flags, uns32 type_flags, const s_location* location, const real_point3d* center, real32 radius, int32* object_indices, int32 maximum_count);
extern void __cdecl objects_information_get(objects_information* objects_information_out);
extern void __cdecl objects_initialize();
extern void __cdecl objects_initialize_for_new_map();
extern void __cdecl objects_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
extern bool __cdecl objects_is_warthog_chaingun_light_enabled();
extern void __cdecl objects_memory_compact();
extern void __cdecl objects_move();
extern void __cdecl objects_post_update();
extern void __cdecl objects_purge_deleted_objects();
extern void __cdecl objects_render_thread_update();
extern void __cdecl objects_setup_structure_bsp_fake_lightprobes(int32 structure_bsp_index);
extern void __cdecl objects_update();
extern void __cdecl objects_update_header_callback(int32 object_index, uns32 datum_handle);
extern void __cdecl recursive_wake_children_awoken_by_movement(int32 object_index);
extern void __cdecl reset_object_cached_render_state(int32 object_index);
extern void __cdecl reset_object_render_message_queue();
extern bool __cdecl sample_object_lighting_from_probes(int32 object_index);
extern void __cdecl scripted_object_function_set(int32 object_function_index, real32 object_function_value);

extern void object_debug_teleport(int32 object_index, const real_point3d* position);
extern void object_render_debug_internal(int32 object_index);
extern const char* object_describe(int32 object_index);

