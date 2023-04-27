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
}

long units_debug_get_next_unit(long unit_index)
{
	dword_flags object_type_flags = FLAG(_object_type_biped) | FLAG(_object_type_vehicle) | FLAG(_object_type_giant);

	long next_unit_index = NONE;

	if (unit_index != NONE)
	{
		static c_object_iterator<unit_datum> unit_iterator;
		static bool fui = true;
		if (fui)
		{
			fui = false;
			unit_iterator.begin(object_type_flags, 0);
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
		unit_iterator.begin(object_type_flags, 0);

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

void __cdecl debug_rotate_all_units()
{
	if (game_in_progress() && !game_is_ui_shell())
	{
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
}

