#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"
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

enum e_object_mask
{
	_object_mask_object = NONE,
	_object_mask_biped = FLAG(_object_type_biped),
	_object_mask_vehicle = FLAG(_object_type_vehicle),
	_object_mask_weapon = FLAG(_object_type_weapon),
	_object_mask_equipment = FLAG(_object_type_equipment),
	_object_mask_arg_device = FLAG(_object_type_arg_device),
	_object_mask_terminal = FLAG(_object_type_terminal),
	_object_mask_projectile = FLAG(_object_type_projectile),
	_object_mask_scenery = FLAG(_object_type_scenery),
	_object_mask_machine = FLAG(_object_type_machine),
	_object_mask_control = FLAG(_object_type_control),
	_object_mask_sound_scenery = FLAG(_object_type_sound_scenery),
	_object_mask_crate = FLAG(_object_type_crate),
	_object_mask_creature = FLAG(_object_type_creature),
	_object_mask_giant = FLAG(_object_type_giant),
	_object_mask_effect_scenery = FLAG(_object_type_effect_scenery),

	_object_mask_item = _object_mask_weapon | _object_mask_equipment,
	_object_mask_device = _object_mask_arg_device | _object_mask_terminal | _object_mask_machine | _object_mask_control,
	_object_mask_unit = _object_mask_biped | _object_mask_vehicle | _object_mask_giant,
	_object_mask_editor_placeable_objects = _object_mask_biped | _object_mask_vehicle |  _object_mask_weapon | _object_mask_equipment | _object_mask_arg_device | _object_mask_terminal | _object_mask_scenery | _object_mask_machine | _object_mask_crate | _object_mask_creature | _object_mask_giant,
};
static_assert(0b0000000000000001 == _object_mask_biped);
static_assert(0b0000000000000010 == _object_mask_vehicle);
static_assert(0b0000000000000100 == _object_mask_weapon);
static_assert(0b0000000000001000 == _object_mask_equipment);
static_assert(0b0000000000010000 == _object_mask_arg_device);
static_assert(0b0000000000100000 == _object_mask_terminal);
static_assert(0b0000000001000000 == _object_mask_projectile);
static_assert(0b0000000010000000 == _object_mask_scenery);
static_assert(0b0000000100000000 == _object_mask_machine);
static_assert(0b0000001000000000 == _object_mask_control);
static_assert(0b0000010000000000 == _object_mask_sound_scenery);
static_assert(0b0000100000000000 == _object_mask_crate);
static_assert(0b0001000000000000 == _object_mask_creature);
static_assert(0b0010000000000000 == _object_mask_giant);
static_assert(0b0100000000000000 == _object_mask_effect_scenery);
static_assert(0b0000000000001100 == _object_mask_item);
static_assert(0b0000001100110000 == _object_mask_device);
static_assert(0b0010000000000011 == _object_mask_unit);
static_assert(0b0011100110111111 == _object_mask_editor_placeable_objects);

// Same as Halo 3
enum e_object_data_flags
{
	_object_hidden_bit = 0,
	_object_always_active_bit,
	_object_being_created_bit,

	// semi confirmed
	_object_underwater_bit,

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

	k_object_data_flags
};

struct object_header_block_reference
{
	short size;
	word offset;
};
static_assert(sizeof(object_header_block_reference) == 0x4);

struct _object_datum
{
	c_flags<e_object_data_flags, dword, k_object_data_flags> flags;

	// override
	long collision_damage_definition_index;

	long next_object_index;
	long first_child_object_index;
	long parent_object_index;
	byte parent_node_index;
	byte_flags inhibited_flags;
	short scenario_datum_index;
	short map_variant_index;
	s_location location;
	real_point3d bounding_sphere_center;
	real bounding_sphere_radius;
	real_point3d attached_bounds_center;
	real attached_bounds_radius;
	real_point3d attached_bounds_sphere_center;
	real attached_bounds_sphere_radius;
	long first_cluster_reference_index;
	real_point3d position;
	vector3d forward;
	vector3d up;
	vector3d transitional_velocity;
	vector3d angular_velocity;
	real scale;
	c_object_identifier object_identifier;
	short name_index;
	char bsp_placement_policy;
	byte keyframed_object_collision_damage_ticks;
	long havok_component_index;
	long local_physics_space_object_index;
	long last_motion_time;
	dword_flags physics_flags;
	byte physics_deactivation_count;
	byte physically_attached_node;
	byte phantom_power_scale;
	byte phantom_speed_scale;
	short in_water_ticks;
	char environment_interpenetration_ticks;
	byte brittle_collision_damage_timer;
	short scale_intepolation_ticks;
	char variant_index;
	byte_flags object_ai_flags;
	dword melee_damage_unique_identifier;
	s_damage_owner damage_owner;
	short structure_bsp_fake_lightprobe_index;
	bool created_at_rest;
	byte clusters_touched_on_connection;
	long simulation_object_glue_index;
	short owner_team_index;
	byte_flags simulation_flags;
	char child_variant_index;
	long simulation_object_interpolation_time;
	real_point3d simulation_object_interpolation_position;
	long first_widget_index;
	word destroyed_constraints;
	word loosened_constraints;
	real maximum_body_vitality;
	real maximum_shield_vitality;
	real shield_charge_delay;
	real body_vitality;
	real shield_vitality;
	real current_body_damage;
	real current_shield_damage;
	real recent_body_damage;
	real recent_shield_damage;
	real shield_impact_current_body_damage;
	real shield_impact_current_shield_damage;
	short shield_stun_ticks;
	short body_stun_ticks;
	dword_flags damage_flags;
	char damaged_explosion_timer;
	char body_damage_delay_ticks;
	char shield_impact_decay_timer;
	byte_flags recycling_flags;
	long next_recycling_object_index;
	long recycling_time;
	long parent_recycling_group;
	long next_recycling_group_member;
	long next_sync_action_participant_index;
	long sync_action_name;
	c_animation_id sync_animation_id;
	long ai_sync_action_arranger_index;
	word_flags render_flags;
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
	long air_probe_index; // airprobes block

	// (bsp_index << 24) | typed_probe_index & 0xFFFFFF
	union
	{
		// scenery probes block
		long scenery_air_probe_index;

		// device machine probes block
		long device_machine_air_probe_index;
	};
};
static_assert(sizeof(_object_datum) == 0x174);

struct object_datum
{
	long definition_index;
	_object_datum object;
};
static_assert(sizeof(object_datum) == sizeof(long) +  sizeof(_object_datum));

// Same as Halo 3
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

	k_object_header_flags
};

struct object_header_datum :
	s_datum_header
{
	c_flags<e_object_header_flags, byte, k_object_header_flags> flags;
	c_enum<e_object_type, byte, _object_type_biped, k_object_type_count> object_type;
	short __unknown2;
	short data_size;
	long datum_handle;
	object_datum* datum;
};
static_assert(sizeof(object_header_datum) == 0x10);

struct c_object_iterator_base
{
public:
	long __cdecl get_index();

protected:
	void __cdecl object_iterator_begin_internal(dword_flags type_flags, dword header_mask, dword match_flags, long object_index);
	bool __cdecl object_iterator_next_internal();
	bool __cdecl object_iterator_next_with_match_flags_internal();
	object_datum* __cdecl get_datum_internal();

private:
	object_datum* m_datum;
	c_flags<e_object_type, dword_flags, k_object_type_count> m_type_flags;
	dword m_header_mask;
	dword m_match_flags;
	long m_object_index;
	long m_index;
	dword m_signature; // 0x86868686 is initialized
};

template<typename t_object_type>
struct c_object_iterator :
	c_object_iterator_base
{
	void __cdecl begin(dword type_flags, byte match_flags)
	{
		object_iterator_begin_internal(type_flags, match_flags, 0, NONE);
	}

	bool __cdecl next()
	{
		return object_iterator_next_internal();
	}

	t_object_type* __cdecl get_datum()
	{
		return static_cast<t_object_type*>(get_datum_internal());
	}
};

struct s_model_customization_region_permutation // s_model_customization_region
{
	long region_name;
	long permutation_name;
};
static_assert(sizeof(s_model_customization_region_permutation) == 0x8);

struct object_placement_data
{
	object_placement_data();

	long definition_index;
	c_object_identifier object_identifier;
	long model_variant_index;
	long scenario_datum_index;
	byte bsp_placement_policy;
	byte __data15[0x3];
	dword_flags flags;
	real_point3d position;
	vector3d forward;
	vector3d up;
	vector3d linear_velocity;
	vector3d translational_velocity;
	real scale;
	long owner_player_index;
	long owner_object_index;
	long owner_team_index;
	s_damage_owner damage_owner;
	c_flags<long, dword, 5> active_change_colors;
	c_static_array<real_rgb_color, 5> change_colors;
	long model_customization_override_count;
	c_static_array<s_model_customization_region_permutation, 16> model_customization_overrides;
	dword_flags model_customization_flags;
	short destroyed_constraints;
	short loosened_constraints;

	// creature_new
	short ai_state_type;
	short ai_state_size;
	short ai_state_alignment_bits;

	bool location_set;
	byte __data147[0x1];
	s_location location;

	bool multiplayer_cinematic_object;
	long parent_object_index;      // object_index_from_name_index(scenario_object->object_data.parent_id.parent_object)
	c_string_id parent_marker;     // scenario_object->object_data.parent_id.parent_marker
	c_string_id connection_marker; // scenario_object->object_data.parent_id.connection_marker
	s_scenario_multiplayer_object_properties multiplayer_properties;
};
static_assert(sizeof(object_placement_data) == 0x18C);
static_assert(0x000 == offsetof(object_placement_data, definition_index));
static_assert(0x004 == offsetof(object_placement_data, object_identifier));
static_assert(0x00C == offsetof(object_placement_data, model_variant_index));
static_assert(0x010 == offsetof(object_placement_data, scenario_datum_index));
static_assert(0x014 == offsetof(object_placement_data, bsp_placement_policy));
static_assert(0x015 == offsetof(object_placement_data, __data15));
static_assert(0x018 == offsetof(object_placement_data, flags));
static_assert(0x01C == offsetof(object_placement_data, position));
static_assert(0x028 == offsetof(object_placement_data, forward));
static_assert(0x034 == offsetof(object_placement_data, up));
static_assert(0x040 == offsetof(object_placement_data, linear_velocity));
static_assert(0x04C == offsetof(object_placement_data, translational_velocity));
static_assert(0x058 == offsetof(object_placement_data, scale));
static_assert(0x05C == offsetof(object_placement_data, owner_player_index));
static_assert(0x060 == offsetof(object_placement_data, owner_object_index));
static_assert(0x064 == offsetof(object_placement_data, owner_team_index));
static_assert(0x068 == offsetof(object_placement_data, damage_owner));
static_assert(0x074 == offsetof(object_placement_data, active_change_colors));
static_assert(0x078 == offsetof(object_placement_data, change_colors));
static_assert(0x0B4 == offsetof(object_placement_data, model_customization_override_count));
static_assert(0x0B8 == offsetof(object_placement_data, model_customization_overrides));
static_assert(0x138 == offsetof(object_placement_data, model_customization_flags));
static_assert(0x13C == offsetof(object_placement_data, destroyed_constraints));
static_assert(0x13E == offsetof(object_placement_data, loosened_constraints));
static_assert(0x140 == offsetof(object_placement_data, ai_state_type));
static_assert(0x142 == offsetof(object_placement_data, ai_state_size));
static_assert(0x144 == offsetof(object_placement_data, ai_state_alignment_bits));
static_assert(0x146 == offsetof(object_placement_data, location_set));
static_assert(0x147 == offsetof(object_placement_data, __data147));
static_assert(0x148 == offsetof(object_placement_data, location));
static_assert(0x14A == offsetof(object_placement_data, multiplayer_cinematic_object));
static_assert(0x14C == offsetof(object_placement_data, parent_object_index));
static_assert(0x150 == offsetof(object_placement_data, parent_marker));
static_assert(0x154 == offsetof(object_placement_data, connection_marker));
static_assert(0x158 == offsetof(object_placement_data, multiplayer_properties));

struct object_marker
{
	word __unknown0;
	real_matrix4x3 identity;
	real_matrix4x3 node_matrix;
	real __unknown6C;
};
static_assert(sizeof(object_marker) == 0x70);

struct s_object_cluster_payload
{
	c_enum<e_object_type, byte, _object_type_biped, k_object_type_count> object_type;
	byte_flags object_visibility_cull_flags;
	byte_flags object_collision_cull_flags;
	real_point3d bounding_sphere_center;
	real bounding_sphere_radius;
};
static_assert(sizeof(s_object_cluster_payload) == 0x14);

struct s_object_render_data
{
	byte __data[0x2000];
};
static_assert(sizeof(s_object_render_data) == 0x2000);

struct s_object_globals
{
	byte __unknown0;
	byte __unknown1;
	byte __unknown2;

	bool garbage_collection_cannot_run;
	short active_garbage_object_count;
	short active_garbage_player_bodies;
	long first_garbage_object_index;

	long __unknownC;
	long __unknown10;
	long __unknown14;
	long __unknown18;

	long object_update_absolute_index;
	render_lighting cinematic_lighting;
	real object_function_values[4];

	byte __unknown248;
	byte __unknown249;

	// objects_is_warthog_chaingun_light_enabled
	// - !warthog_chaingun_light_disabled
	// objects_enable_warthog_chaingun_light(bool enable_warthog_chaingun_light)
	// - warthog_chaingun_light_disabled = !enable_warthog_chaingun_light
	bool warthog_chaingun_light_disabled; // #TODO: find the actual name/think of a better name

	byte __unknown24B;

	// not the actual name
	c_static_array<long, k_object_type_count> object_type_scenario_datums_counts;

	// #TODO: find out if this count has a name
	c_static_array<long, 6368> scenario_object_to_object_index;
};
static_assert(sizeof(s_object_globals) == 0x6608);

struct s_object_render_thread_message
{
	long object_index;
	short message_type;
	short __unknown6;
};
static_assert(sizeof(s_object_render_thread_message) == 0x8);

struct objects_information
{
	short __unknown0;
	short __unknown2;
	short __unknown4;
	short __unknown6;
	real __unknown8;
	real __unknownC;
	long __unknown10;
	long __unknown14;
	long __unknown18;
	long __unknown1C;
};
static_assert(sizeof(objects_information) == 0x20);

extern bool debug_objects;
extern bool debug_objects_early_movers;
extern bool debug_objects_sound_spheres;
extern bool debug_objects_indices;
extern bool debug_objects_programmer;
extern bool debug_objects_garbage;
extern bool debug_objects_names;
extern bool debug_objects_full_names;
extern bool debug_objects_active_nodes;
extern bool debug_objects_animation_times;
extern bool debug_objects_functions;
extern bool debug_objects_position_velocity;
extern bool debug_objects_origin;
extern bool debug_objects_root_node;
extern bool debug_objects_bounding_spheres;
extern bool debug_objects_attached_bounding_spheres;
extern bool debug_objects_dynamic_render_bounding_spheres;
extern bool debug_objects_model_targets;
extern bool debug_objects_collision_models;
extern bool debug_objects_profile_times;
extern bool debug_objects_water_physics;
extern bool debug_objects_physics_models;
extern bool debug_objects_contact_points;
extern bool debug_objects_constraints;
extern bool debug_objects_vehicle_physics;
extern bool debug_objects_mass;
extern bool debug_objects_pathfinding;
extern bool debug_objects_node_bounds;
extern bool debug_objects_animation;

struct s_scenario_object;

extern void* __cdecl object_header_block_get(long object_index, object_header_block_reference const* reference);
extern void* __cdecl object_header_block_get_with_count(long object_index, object_header_block_reference const* reference, unsigned int element_size, long* element_count);
extern object_header_datum const* __cdecl object_header_get(long object_index);
extern object_datum* __cdecl object_get(long object_index);
extern void* __cdecl object_get_and_verify_type(long object_index, dword object_type_mask);
extern void __cdecl object_get_bounding_sphere(long object_index, real_point3d* center, real* radius);
extern e_object_type __cdecl object_get_type(long object_index);
extern long __cdecl cluster_get_first_collideable_object(long* datum_index, s_cluster_reference cluster_reference);
extern long __cdecl cluster_get_first_collideable_object_and_payload(long* datum_index, s_cluster_reference cluster_reference, s_object_cluster_payload const** payload);
extern long __cdecl cluster_get_first_noncollideable_object_and_payload(long* datum_index, s_cluster_reference cluster_reference, s_object_cluster_payload const** payload);
extern long __cdecl cluster_get_next_collideable_object(long* datum_index);
extern long __cdecl cluster_get_next_collideable_object_and_payload(long* datum_index, s_object_cluster_payload const** payload);
extern long __cdecl cluster_get_next_noncollideable_object_and_payload(long* datum_index, s_object_cluster_payload const** payload);
extern bool __cdecl object_load_scenario_placement_matrices(long object_index);
extern void __cdecl object_delete(long object_index);
extern void __cdecl object_delete_all_multiplayer_cinematic_objects();
extern void __cdecl object_delete_immediately(long object_index);
extern void __cdecl object_delete_recursive(long object_index, bool deactivate);
extern void __cdecl object_destroy_instance_group(long object_index, long instance_group_index);
extern void __cdecl object_detach(long object_index);
extern void __cdecl object_detach_from_node(long object_index, real_matrix4x3 const* node);
extern void __cdecl object_detach_gamestate_entity(long object_index, long gamestate_index);
extern void __cdecl object_detach_immediate(long object_index);
extern void __cdecl object_detach_internal(long object_index, bool compute_bounding_sphere);
extern void __cdecl object_detach_predicted_objects(long object_index);
extern void __cdecl object_disconnect_from_map(long object_index, bool broadphase_remove_object);
extern void __cdecl object_disconnect_from_physics(long object_index);
extern bool __cdecl object_find_initial_location(long object_index, s_location* location);
extern void __cdecl object_find_structure_bsp_fake_lightprobe_index(long structure_bsp_index, long object_index);
extern bool __cdecl object_force_inside_bsp(long object_index, real_point3d const* position, long ignore_object_index);
extern void __cdecl object_freeze_node_orientations(long object_index, bool a2);
extern bool __cdecl object_function_get_function_value(long object_index, s_object_function_definition const* function, long object_definition_index, real* out_function_magnitude, bool* deterministic);
extern long __cdecl object_get_attachment_marker_name(long object_index, short attachment_index);
extern bool __cdecl object_get_base_change_color_by_index(long object_index, long change_color_index, real_rgb_color* color);
extern real_point3d* __cdecl object_get_center_of_mass(long object_index, real_point3d* center);
extern bool __cdecl object_get_change_color(long object_index, long change_color_index, real_rgb_color* change_color);
extern void __cdecl object_get_closest_point_and_normal(long object_index, real_point3d const* origin, real_point3d* closest_point, vector3d* normal);
extern void __cdecl object_get_damage_owner(long object_index, s_damage_owner* owner);
extern bool __cdecl object_get_function_value(long object_index, long function_name, long object_definition_index, real* out_function_magnitude);
extern short __cdecl object_get_markers_by_string_id(long object_index, string_id marker_name, object_marker* markers, short maximum_marker_count);
extern real_matrix4x3* __cdecl object_get_node_matrix(long object_index, short node_index);
extern void __cdecl object_get_orientation(long object_index, vector3d* forward, vector3d* up);
extern real_point3d* __cdecl object_get_origin(long object_index, real_point3d* origin);
extern long __cdecl object_get_ultimate_parent(long object_index);
extern long __cdecl object_get_variant_index(long object_index);
extern void __cdecl object_get_velocities(long object_index, vector3d* linear_velocity, vector3d* angular_velocity);
extern real_matrix4x3* __cdecl object_get_world_matrix(long object_index, real_matrix4x3* matrix);
extern bool __cdecl object_is_multiplayer_cinematic_object(long object_index);
extern void __cdecl object_marker_begin();
extern void __cdecl object_marker_end();
extern void __cdecl object_marker_reopen();
extern void __cdecl object_move(long object_index);
extern void __cdecl object_move_position(long object_index, real_point3d const* position, vector3d const* forward, vector3d const* up, s_location const* location);
extern void __cdecl object_name_list_allocate();
extern void __cdecl object_name_list_clear();
extern void __cdecl object_name_list_delete(long object_index);
extern long __cdecl object_name_list_lookup(short name_index);
extern void __cdecl object_name_list_new(long name, short name_index);
extern bool __cdecl object_needs_rigid_body_update(long object_index);
extern long __cdecl object_new(object_placement_data* data);
extern bool __cdecl object_node_orientations_frozen(long object_index);
extern void __cdecl object_notify_in_local_physics_object(long object_index, long local_physics_object_index);
extern void __cdecl object_place(long object_index, s_scenario_object const* scenario_object);
extern void __cdecl object_placement_data_copy_change_colors(object_placement_data* data, long object_index);
extern void __cdecl object_placement_data_new(object_placement_data* data, long definition_index, long owner_object_index, s_damage_owner const* damage_owner);
extern void __cdecl object_placement_data_set_location(object_placement_data* data, struct s_location const* location);
extern void __cdecl object_postprocess_node_matrices(long object_index);
extern void __cdecl object_pre_delete_recursive(long object_index, bool a2);
extern bool __cdecl object_predict(long object_index, bool low);
extern bool __cdecl object_predict_all(long object_index);
extern bool __cdecl object_predict_low(long object_index);
extern void __cdecl object_prepare_axis_vectors(long object_index, vector3d* forward, vector3d* up);
extern void __cdecl object_reconnect_to_map(long object_index, bool a2, s_location const* location);
extern void __cdecl object_reconnect_to_physics(long object_index);
extern void __cdecl object_register_scenario_object(long object_index);
extern void __cdecl object_reinitialize_from_placement(long object_index, object_placement_data* data, bool at_rest);
extern void __cdecl object_remove_from_list(long* object_list, long object_index);
extern void __cdecl object_remove_pending_messages(long object_index);
extern void __cdecl object_render_debug(long object_index);
extern void __cdecl object_reset(long object_index);
extern void __cdecl object_reset_interpolation(long object_index);
extern void __cdecl object_respond_to_physics(long object_index);
extern void __cdecl object_resurrect(long object_index);
extern void __cdecl object_set_always_active(long object_index, bool always_active);
extern void __cdecl object_set_at_rest(long object_index, bool at_rest);
extern bool __cdecl object_set_base_change_color_by_index(long object_index, long color_index, real_rgb_color const* color);
extern void __cdecl object_set_body_vitality(long object_index, real body_vitality, short body_stun_ticks);
extern void __cdecl object_set_custom_animation_speed(long object_index, real custom_animation_speed);
extern void __cdecl object_set_custom_animations_hold_on_last_frame(bool custom_animations_hold_on_last_frame);
extern void __cdecl object_set_custom_animations_prevent_lipsync_head_movement(bool custom_animations_prevent_lipsync_head_movement);
extern void __cdecl object_set_damage_owner(long object_index, s_damage_owner const* damage_owner, bool a3);
extern void __cdecl object_set_desired_velocities(long object_index, vector3d const* transitional_velocity, vector3d const* angular_velocity, bool a4);
extern void __cdecl object_set_garbage(long object_index, bool a2, long collection_ticks);
extern void __cdecl object_set_hidden(long object_index, bool hidden);
extern void __cdecl object_set_in_limbo(long object_index, bool deactivate);
extern void __cdecl object_shield_stun_infinite(long object_index);
extern void __cdecl object_set_initial_change_colors(long object_index, c_flags<char, byte, 5> active_change_colors, real_rgb_color const* change_colors);
extern void __cdecl object_set_invalid_for_recycling(long object_index, bool a2);
extern bool __cdecl object_set_model_state(long object_index, long a2, long a3, bool a4);
extern void __cdecl object_set_model_state_property(long object_index, long a2, long a3, bool a4, bool a5);
extern void __cdecl object_set_model_state_property_per_region(long object_index, long a2, long a3, short a4, bool a5);
extern void __cdecl object_set_object_index_for_name_index(short name_index, long object_index);
extern bool __cdecl object_set_position_internal(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location, bool compute_node_matrices, bool set_havok_object_position, bool in_editor, bool disconnected);
extern void __cdecl object_set_position(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location);
extern void __cdecl object_set_position_direct(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location, bool in_editor);
extern void __cdecl object_set_position_in_editor(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location, bool at_rest);
extern void __cdecl object_set_position_in_sandbox_editor(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location);
extern void* __cdecl object_try_and_get_and_verify_type(long object_index, dword object_type_mask);
extern bool __cdecl object_unmarked(long object_index);
extern void __cdecl object_unregister_scenario_object(long object_index);
extern bool __cdecl object_update(long object_index);
extern void __cdecl object_update_collision_culling(long object_index);
extern void __cdecl object_update_visibility_culling(long object_index);
extern bool __cdecl object_visible_to_any_player(long object_index);
extern void __cdecl object_wake(long object_index);
extern bool __cdecl objects_can_connect_to_map();
extern long __cdecl objects_compact_memory_pool();
extern void __cdecl objects_detach_from_simulation();
extern void __cdecl objects_dispose();
extern void __cdecl objects_dispose_from_old_map();
extern void __cdecl objects_dispose_from_old_structure_bsp(dword a1);
extern void __cdecl objects_enable_warthog_chaingun_light(bool enable_warthog_chaingun_light);
extern void __cdecl objects_garbage_collection();
extern long __cdecl objects_get_active_garbage_count();
extern long __cdecl objects_get_contiguous_free_memory_size();
extern long __cdecl objects_get_first_garbage_object();
extern long __cdecl objects_get_free_object_header_count();
extern long __cdecl objects_get_next_garbage_object(long object_index);
extern void __cdecl objects_handle_deleted_object(long object_index);
extern void __cdecl objects_handle_deleted_player(long object_index);
extern void __cdecl objects_information_get(objects_information* objects_information_out);
extern void __cdecl objects_initialize();
extern void __cdecl objects_initialize_for_new_map();
extern void __cdecl objects_initialize_for_new_structure_bsp(dword a1);
extern bool __cdecl objects_is_warthog_chaingun_light_enabled();
extern void __cdecl objects_memory_compact();
extern void __cdecl objects_move();
extern void __cdecl objects_post_update();
extern void __cdecl objects_purge_deleted_objects();
extern void __cdecl objects_render_thread_update();
extern void __cdecl objects_setup_structure_bsp_fake_lightprobes(long structure_bsp_index);
extern void __cdecl objects_update();
extern void __cdecl objects_update_header_callback(long object_index, dword datum_handle);
extern void __cdecl recursive_wake_children_awoken_by_movement(long object_index);
extern void __cdecl reset_object_cached_render_state(long object_index);
extern void __cdecl reset_object_render_message_queue();
extern bool __cdecl sample_object_lighting_from_probes(long object_index);
extern void __cdecl scripted_object_function_set(long object_function_index, real object_function_value);
extern void __cdecl object_debug_teleport(long object_index, real_point3d const* position);
extern void __cdecl object_render_debug_internal(long object_index);

