#include "units/units.hpp"

#include "game/game.hpp"
#include "memory/thread_local.hpp"
#include "objects/objects.hpp"

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

bool __cdecl unit_has_weapon_definition_index(long unit_index, long weapon_definition_index)
{
    return INVOKE(0x00B450F0, unit_has_weapon_definition_index, unit_index, weapon_definition_index);
}

bool units_debug_can_select_unit(long unit_index)
{
	// #TODO implement
	return true;

	TLS_REFERENCE(object_header_data);
	object_header_datum* header = (object_header_datum*)datum_try_and_get(object_header_data, unit_index);
	void* object = nullptr;
	if (header && TEST_BIT(UNIT_DATUM_MASK, header->object_type.get()))
		object = header->datum;

	if (!object)
		return false;

	//void* object = object_get_and_verify_type(unit_index, UNIT_DATUM_MASK);
	//void* object = object_try_and_get_and_verify_type(unit_index, UNIT_DATUM_MASK);

	//unit_datum* unit = static_cast<unit_datum*>(object);
	byte* unit = static_cast<byte*>(object);

	REFERENCE_DECLARE(unit + 0x198, long, some_index);
	REFERENCE_DECLARE(unit + 0x120, word_flags, damage_flags);
	REFERENCE_DECLARE(unit + 0x4, dword_flags, data_flags);

	if (some_index != NONE || TEST_BIT(damage_flags, 2) || TEST_BIT(data_flags, 26))
		return false;

	return true;
}

// #TODO: find a home
vector3d* __cdecl vector_from_points3d(real_point3d const* a, real_point3d const* b, vector3d* out_vector)
{
	out_vector->n[0] = b->n[0] - a->n[0];
	out_vector->n[1] = b->n[1] - a->n[1];
	out_vector->n[2] = b->n[2] - a->n[2];

	return out_vector;
}

// #TODO: find a home
real magnitude_squared3d(vector3d const* vector)
{
	return real(vector->n[0] * vector->n[0]) + real(vector->n[1] * vector->n[1]) + real(vector->n[2] * vector->n[2]);
}

// #TODO: find a home
real distance_squared3d(real_point3d const* a, real_point3d const* b)
{
	vector3d temp{};
	return magnitude_squared3d(vector_from_points3d(a, b, &temp));
}

// #TODO: find a home
real square_root(real value)
{
	return sqrtf(value); // sqrt
}

// #TODO: find a home
real distance3d(real_point3d const* a, real_point3d const* b)
{
	return square_root(distance_squared3d(a, b));
}

long __cdecl units_debug_get_closest_unit(long unit_index)
{
	long closest_unit_index = NONE;
	real closest_distance = 3.4028235e38f;

	c_object_iterator<unit_datum> unit_iterator;
	unit_iterator.begin(UNIT_DATUM_MASK, 0);
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
	long next_unit_index = NONE;

	if (unit_index != NONE)
	{
		static c_object_iterator<unit_datum> unit_iterator;
		static bool fui = true;
		if (fui)
		{
			fui = false;
			unit_iterator.begin(UNIT_DATUM_MASK, 0);
		}
		do
		{
			if (unit_iterator.next())
				break;

		} while (unit_iterator.get_index() != unit_index);

		while (unit_iterator.next())
		{
			if (/*unit_iterator.get_index() != unit_index &&*/ units_debug_can_select_unit(unit_iterator.get_index()))
			{
				next_unit_index = unit_iterator.get_index();
				break;
			}
		}
	}

	if (next_unit_index == NONE)
	{
		c_object_iterator<unit_datum> unit_iterator{};
		unit_iterator.begin(UNIT_DATUM_MASK, 0);

		while (unit_iterator.next())
		{
			if (units_debug_can_select_unit(unit_iterator.get_index()))
			{
				next_unit_index = unit_iterator.get_index();
				break;
			}
		}
	}

	return next_unit_index;
}

void __cdecl debug_rotate_units_callback(bool enable)
{
	if (!enable)
		return;

	if (!game_in_progress() || game_is_ui_shell())
		return;

	TLS_REFERENCE(player_data);

	long active_user = players_first_active_user();
	if (active_user != NONE)
	{
		long player_index = player_index_from_user_index(active_user);
		player_datum* player = (player_datum*)datum_try_and_get(player_data, player_index);
		long unit_index = player->unit_index;
		if (unit_index != NONE)
		{
			long next_unit = units_debug_get_closest_unit(unit_index);
			if (next_unit != NONE)
				player_set_unit_index(player_index, next_unit);
		}
	}
}

void __cdecl debug_rotate_all_units_callback(bool enable)
{
	if (!enable)
		return;

	if (!game_in_progress() || game_is_ui_shell())
		return;

	TLS_REFERENCE(player_data);

	long active_user = players_first_active_user();
	if (active_user != NONE)
	{
		long player_index = player_index_from_user_index(active_user);
		player_datum* player = (player_datum*)datum_try_and_get(player_data, player_index);
		long unit_index = player->unit_index;
		if (unit_index != NONE)
		{
			long next_unit = units_debug_get_next_unit(unit_index);
			if (next_unit != NONE)
				player_set_unit_index(player_index, next_unit);
		}
	}
}

