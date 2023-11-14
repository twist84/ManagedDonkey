#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"
#include "objects/damage_new.hpp"
#include "objects/damage_owner.hpp"
#include "objects/lights.hpp"
#include "objects/multiplayer_game_objects.hpp"
#include "objects/object_activation_regions.hpp"
#include "objects/object_definitions.hpp"
#include "objects/object_broadphase.hpp"
#include "objects/object_early_movers.hpp"
#include "objects/object_placement.hpp"
#include "objects/object_recycling.hpp"
#include "objects/object_scheduler.hpp"
#include "objects/object_scripting.hpp"
#include "scenario/scenario_object_definitions.hpp"

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


enum e_object_source
{
	_object_source_structure = 0,
	_object_source_editor,
	_object_source_dynamic,
	_object_source_legacy,
	_object_source_sky,
	_object_source_parent,

	k_object_source_count
};

struct object_datum : s_datum_header
{
	byte __data[0x176];
};
static_assert(sizeof(object_datum) == 0x178);

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

struct object_header_datum : s_datum_header
{
	c_flags<e_object_header_flags, byte, k_object_header_flags> flags;
	c_enum<e_object_type, byte, _object_type_biped, k_object_type_count> object_type;
	short __unknown2;
	short data_size;
	long datum_handle;
	object_datum* datum;
};
static_assert(sizeof(object_header_datum) == 0x10);

struct c_object_identifier
{
	tag m_unique_id; // 'obj#'

	// scenario_structure_bsp_reference
	short m_origin_bsp_index;

	c_enum<e_object_type, char, _object_type_biped, k_object_type_count> m_type;
	c_enum<e_object_source, char, _object_source_structure, k_object_source_count> m_source;
};
static_assert(sizeof(c_object_identifier) == 0x8);

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
struct c_object_iterator : c_object_iterator_base
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
	long definition_index;
	c_object_identifier object_identifier;
	long model_variant_index;
	long scenario_datum_index;
	byte bsp_placement_policy;
	dword_flags flags;
	real_point3d position;
	vector3d forward;
	vector3d up;
	vector3d linear_velocity;
	vector3d translational_velocity;
	real scale;
	long player_index;
	long object_index;
	long team_index;
	s_damage_owner damage_owner;
	dword_flags active_change_colors;
	c_static_array<real_rgb_color, 5> change_colors;
	long model_customization_override_count;
	c_static_array<s_model_customization_region_permutation, 16> model_customization_overrides;
	dword_flags model_customization_flags;
	short destroyed_constraints;
	short loosened_constraints;

	short __unknown140;
	byte __data142[2];

	bool location_set;
	s_location location;
	bool multiplayer_cinematic_object;
	long parent_object_index;      // object_index_from_name_index(scenario_object->object_data.parent_id.parent_object)
	c_string_id parent_marker;     // scenario_object->object_data.parent_id.parent_marker
	c_string_id connection_marker; // scenario_object->object_data.parent_id.connection_marker
	s_scenario_multiplayer_object_properties multiplayer_properties;
};
static_assert(sizeof(object_placement_data) == 0x18C);

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
	byte __data[0x6608];
};
static_assert(sizeof(s_object_globals) == 0x6608);

struct s_object_render_thread_message
{
	long object_index;
	short __unknown4;
	short __unknown6;
};
static_assert(sizeof(s_object_render_thread_message) == 0x8);

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

extern object_header_datum const* __cdecl object_header_get(long object_index);
extern void* __cdecl object_get_and_verify_type(long object_index, dword object_type_mask);
extern e_object_type __cdecl object_get_type(long object_index);
extern long __cdecl cluster_get_first_collideable_object(long* datum_index, s_cluster_reference cluster_reference);
extern long __cdecl cluster_get_first_collideable_object_and_payload(long* datum_index, s_cluster_reference cluster_reference, s_object_cluster_payload const** payload);
extern long __cdecl cluster_get_first_noncollideable_object_and_payload(long* datum_index, s_cluster_reference cluster_reference, s_object_cluster_payload const** payload);
extern long __cdecl cluster_get_next_collideable_object(long* datum_index);
extern long __cdecl cluster_get_next_collideable_object_and_payload(long* datum_index, s_object_cluster_payload const** payload);
extern long __cdecl cluster_get_next_noncollideable_object_and_payload(long* datum_index, s_object_cluster_payload const** payload);
extern bool __cdecl object_load_scenario_placement_matrices(long object_index);
extern void __cdecl object_delete(long object_index);
extern bool __cdecl object_function_get_function_value(long object_index, s_object_function_definition const* function, long object_definition_index, real* out_function_magnitude, bool* deterministic);
extern bool __cdecl object_get_function_value(long object_index, long function_name, long object_definition_index, real* out_function_magnitude);
extern short __cdecl object_get_markers_by_string_id(long object_index, string_id marker_name, object_marker* markers, short maximum_marker_count);
extern real_matrix4x3* __cdecl object_get_node_matrix(long object_index, short node_index);
extern void __cdecl object_get_orientation(long object_index, vector3d* forward, vector3d* up);
extern real_point3d* __cdecl object_get_origin(long object_index, real_point3d* origin);
extern long __cdecl object_get_ultimate_parent(long object_index);
extern void __cdecl object_get_velocities(long object_index, vector3d* linear_velocity, vector3d* angular_velocity);
extern real_matrix4x3* __cdecl object_get_world_matrix(long object_index, real_matrix4x3* matrix);
extern long __cdecl object_new(object_placement_data* placement_data);
extern void __cdecl object_placement_data_new(object_placement_data* placement_data, long object_definition_index, long object_datum_index, s_damage_owner const* damage_owner);
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
extern void __cdecl object_render_debug(long object_index);
extern void __cdecl object_reset(long object_index);
extern void __cdecl object_reset_interpolation(long object_index);
extern void __cdecl object_set_always_active(long object_index, bool always_active);
extern void __cdecl object_set_at_rest(long object_index, bool at_rest);
extern bool __cdecl object_set_base_change_color_by_index(long object_index, long color_index, real_rgb_color const* color);
extern void __cdecl object_set_custom_animations_hold_on_last_frame(bool custom_animations_hold_on_last_frame);
extern void __cdecl object_set_custom_animations_prevent_lipsync_head_movement(bool custom_animations_prevent_lipsync_head_movement);
extern void __cdecl object_set_hidden(long object_index, bool hidden);
extern void __cdecl object_set_in_limbo(long object_index, bool deactivate);
extern void __cdecl object_set_object_index_for_name_index(short name_index, long object_index);
extern bool __cdecl object_set_position_internal(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location, bool compute_node_matrices, bool set_havok_object_position, bool in_editor, bool disconnected);
extern void __cdecl object_set_position(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location);
extern void __cdecl object_set_position_direct(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location, bool in_editor);
extern void __cdecl object_set_position_in_editor(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location, bool at_rest);
extern void __cdecl object_set_position_in_sandbox_editor(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location);
extern void* __cdecl object_try_and_get_and_verify_type(long object_index, dword object_type_mask);
extern void __cdecl object_debug_teleport(long object_index, real_point3d const* position);
extern void __cdecl object_render_debug_internal(long object_index);

