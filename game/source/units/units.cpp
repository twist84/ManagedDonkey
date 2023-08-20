#include "units/units.hpp"

#include "cache/cache_files.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "objects/objects.hpp"
#include "units/unit_definition.hpp"
#include "render/render_debug.hpp"

HOOK_DECLARE(0x00B47080, unit_render_debug);

bool debug_objects_unit_vectors = false;
bool debug_objects_unit_seats = false;
bool debug_objects_unit_mouth_apeture = false;
bool debug_objects_unit_firing = false;
bool debug_objects_unit_acceleration = false;
bool debug_objects_unit_camera = false;

void __cdecl unit_add_equipment_to_inventory(long unit_index, long slot_index, long object_index)
{
	INVOKE(0x00B38AB0, unit_add_equipment_to_inventory, unit_index, slot_index, object_index);
}

bool __cdecl unit_add_grenade_to_inventory(long unit_index, long object_index)
{
	return INVOKE(0x00B38BA0, unit_add_grenade_to_inventory, unit_index, object_index);
}

short __cdecl unit_add_grenade_type_to_inventory(long unit_index, short grenade_type, short grenade_count)
{
	return INVOKE(0x00B38CD0, unit_add_grenade_type_to_inventory, unit_index, grenade_type, grenade_count);
}

void __cdecl unit_add_starting_profile_equipment(long unit_index, short profile_index, bool clear_player, bool create_new)
{
	INVOKE(0x00B39110, unit_add_starting_profile_equipment, unit_index, profile_index, clear_player, create_new);
}

bool __cdecl unit_add_weapon_to_inventory(long unit_index, long object_index, long weapon_addition_method)
{
	return INVOKE(0x00B393D0, unit_add_weapon_to_inventory, unit_index, object_index, weapon_addition_method);
}

void __cdecl unit_get_camera_position(long unit_index, real_point3d* position)
{
	INVOKE(0x00B439D0, unit_get_camera_position, unit_index, position);
}

void __cdecl unit_get_head_position(long unit_index, real_point3d* position)
{
	INVOKE(0x00B441B0, unit_get_head_position, unit_index, position);
}

bool __cdecl unit_has_weapon_definition_index(long unit_index, long weapon_definition_index)
{
	return INVOKE(0x00B450F0, unit_has_weapon_definition_index, unit_index, weapon_definition_index);
}

void __cdecl unit_render_debug(long unit_index)
{
	byte* unit = static_cast<byte*>(object_get_and_verify_type(unit_index, UNIT_OBJECTS_MASK));
	REFERENCE_DECLARE(unit, long, object_definition_index);

	_unit_definition* unit_definition = static_cast<_unit_definition*>(tag_get(UNIT_TAG, object_definition_index));


	if (debug_objects_unit_vectors)
	{

	}

	if (debug_objects_unit_seats)
	{
		for (unit_seat& seat : unit_definition->seats_block)
		{
			object_marker markers[4]{};
			short marker_count = object_get_markers_by_string_id(unit_index, seat.entry_markers_name, markers, NUMBEROF(markers));
			for (short marker_index = 0; marker_index < marker_count; marker_index++)
			{
				object_marker* marker = &markers[marker_index];

				render_debug_string_at_point(&marker->node_matrix.center, string_id_get_string_const(seat.label), global_real_argb_red);
				render_debug_vector(true, &marker->node_matrix.center, &marker->node_matrix.matrix.forward, seat.entry_radius, global_real_argb_red);
				render_debug_sphere(true, &marker->node_matrix.center, seat.entry_radius, global_real_argb_red);
				render_debug_cone_outline(true, &marker->node_matrix.center, &marker->node_matrix.matrix.forward, seat.entry_radius, seat.entry_marker_cone_angle, global_real_argb_red);
			}
		}
	}

	if (debug_objects_unit_mouth_apeture)
	{
		REFERENCE_DECLARE(unit + 0x2C0, real, mouth_aperture);

		real_point3d head_position{};
		c_static_string<512> mouth_apeture_string;

		unit_get_head_position(unit_index, &head_position);
		mouth_apeture_string.print("%.2f", mouth_aperture);

		render_debug_string_at_point(&head_position, mouth_apeture_string.get_string(), global_real_argb_orange);
	}

	if (debug_objects_unit_firing)
	{

	}

	if (debug_objects_unit_acceleration)
	{

	}

	if (debug_objects_unit_camera)
	{
		real_point3d unit_camera_position{};
		unit_get_camera_position(unit_index, &unit_camera_position);
		render_debug_point(true, &unit_camera_position, 0.2f, global_real_argb_cyan);
	}
}

bool units_debug_can_select_unit(long unit_index)
{
	// #TODO implement
	//return true;

	object_header_datum const* header = object_header_get(unit_index);
	void* object = nullptr;
	if (header && TEST_BIT(UNIT_OBJECTS_MASK, header->object_type.get()))
		object = header->datum;

	if (!object)
		return false;

	//void* object = object_get_and_verify_type(unit_index, UNIT_OBJECTS_MASK);
	//void* object = object_try_and_get_and_verify_type(unit_index, UNIT_OBJECTS_MASK);

	//unit_datum* unit = static_cast<unit_datum*>(object);
	byte* unit = static_cast<byte*>(object);

	REFERENCE_DECLARE(unit + 0x198, long, some_index);
	REFERENCE_DECLARE(unit + 0x120, word_flags, damage_flags);
	REFERENCE_DECLARE(unit + 0x4, dword_flags, data_flags);

	if (some_index != NONE || TEST_BIT(damage_flags, 2) || TEST_BIT(data_flags, 26))
		return false;

	return true;
}

long __cdecl units_debug_get_closest_unit(long unit_index)
{
	long closest_unit_index = NONE;
	real closest_distance = 3.4028235e38f;

	c_object_iterator<unit_datum> unit_iterator;
	unit_iterator.begin(UNIT_OBJECTS_MASK, 0);
	while (unit_iterator.next())
	{
		if (unit_iterator.get_index() != unit_index)
		{
			if (units_debug_can_select_unit(unit_iterator.get_index()))
			{
				real distance = 0.0f;
				if (unit_index != NONE)
				{
					real_point3d unit_origin{};
					object_get_origin(unit_index, &unit_origin);
					real_point3d closest_unit_origin{};
					object_get_origin(unit_iterator.get_index(), &closest_unit_origin);
					distance = distance3d(&unit_origin, &closest_unit_origin);
				}

				if (distance < closest_distance)
				{
					closest_unit_index = unit_iterator.get_index();
					closest_distance = distance;
				}
			}
		}
	}

	return closest_unit_index;
}

long units_debug_get_next_unit(long unit_index)
{
	c_object_iterator<unit_datum> unit_iterator;

	long next_unit_index = NONE;

	if (unit_index != NONE)
	{
		unit_iterator.begin(UNIT_OBJECTS_MASK, 0);
		if (unit_iterator.next())
		{
			while (unit_iterator.get_index() != unit_index && unit_iterator.next());
		}
		if (unit_iterator.next())
		{
			while (true)
			{
				if (units_debug_can_select_unit(unit_iterator.get_index()))
					break;

				if (!unit_iterator.next())
					goto LABEL_12;
			}

			next_unit_index = unit_iterator.get_index();
			if (unit_iterator.get_index() != -1)
				return next_unit_index;
		}
	}
LABEL_12:
	unit_iterator.begin(UNIT_OBJECTS_MASK, 0);
	if (!unit_iterator.next())
		return next_unit_index;

	while (true)
	{
		if (units_debug_can_select_unit(unit_iterator.get_index()))
			break;

		if (!unit_iterator.next())
			return next_unit_index;
	}
	return unit_iterator.get_index();
}

