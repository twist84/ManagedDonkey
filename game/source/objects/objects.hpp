#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"
#include "objects/multiplayer_game_objects.hpp"
#include "scenario/scenario_object_definitions.hpp"

enum e_object_type
{
	_object_type_biped = 0,
	_object_type_vehicle,
	_object_type_weapon,
	_object_type_equipment,
	_object_type_arg_device,
	_object_type_terminal,
	_object_type_projectile,
	_object_type_scenery,
	_object_type_machine,
	_object_type_control,
	_object_type_sound_scenery,
	_object_type_crate,
	_object_type_creature,
	_object_type_giant,
	_object_type_effect_scenery,

	k_object_type_count
};

#define UNIT_OBJECTS_MASK (FLAG(_object_type_biped) | FLAG(_object_type_vehicle) | FLAG(_object_type_giant))
static_assert(UNIT_OBJECTS_MASK == 0b0010000000000011);

#define ITEM_OBJECTS_MASK (FLAG(_object_type_weapon) | FLAG(_object_type_equipment))
static_assert(ITEM_OBJECTS_MASK == 0b0000000000001100);

#define DEVICE_OBJECTS_MASK (FLAG(_object_type_arg_device) | FLAG(_object_type_terminal) | FLAG(_object_type_machine) | FLAG(_object_type_control))
static_assert(DEVICE_OBJECTS_MASK == 0b0000001100110000);

#define EDITOR_PLACEABLE_OBJECTS_MASK (FLAG(_object_type_biped) | FLAG(_object_type_vehicle) | FLAG(_object_type_weapon) | FLAG(_object_type_equipment) | FLAG(_object_type_arg_device) | FLAG(_object_type_terminal) | FLAG(_object_type_scenery) | FLAG(_object_type_machine) | FLAG(_object_type_crate) | FLAG(_object_type_creature) | FLAG(_object_type_giant))
static_assert(EDITOR_PLACEABLE_OBJECTS_MASK == 0b0011100110111111);

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

struct object_header_datum : s_datum_header
{
	byte_flags flags;
	c_enum<e_object_type, byte, _object_type_biped, k_object_type_count> object_type;
	short __unknown2;
	short data_size;
	long __unknown8;
	object_datum* datum;
};
static_assert(sizeof(object_header_datum) == 0x10);

struct c_object_identifier
{
	long unique_id; // 'obj#'

	// scenario_structure_bsp_reference
	short origin_bsp_index;

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

struct object_placement_data
{
	long definition_index;
	c_object_identifier object_identifier;
	long model_variant_index;
	long scenario_datum_index;

	byte bsp_placement_policy;
	byte __pad15[3];

	dword_flags flags;
	real_point3d position;
	vector3d forward;
	vector3d up;
	vector3d linear_velocity;
	vector3d angular_velocity;
	real scale;

	long player_index;
	long object_index;
	long team_index;

	byte __data68[0xC];

	dword_flags change_color_flags;
	c_static_array<real_rgb_color, 5> change_colors;

	byte __dataB4[0xA4];

	s_scenario_multiplayer_object_properties multiplayer_object_properties;
};
static_assert(sizeof(object_placement_data) == 0x18C);

struct s_damage_owner;

extern void __cdecl object_delete(long object_index);
extern real_point3d* __cdecl object_get_origin(long object_index, real_point3d* origin);
extern long __cdecl object_get_ultimate_parent(long object_index);
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
extern void __cdecl object_set_position(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location);
extern void __cdecl object_set_position_direct(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location, bool in_editor);
extern void __cdecl object_set_position_in_editor(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location, bool at_rest);
extern void __cdecl object_set_position_in_sandbox_editor(long object_index, real_point3d const* desired_position, vector3d const* desired_forward, vector3d const* desired_up, s_location const* location);
extern void* __cdecl object_try_and_get_and_verify_type(long object_index, dword object_type);
extern void __cdecl object_debug_teleport(long object_index, real_point3d const* position);

