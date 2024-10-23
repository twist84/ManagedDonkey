#include "interface/first_person_weapons.hpp"

#include "cseries/cseries.hpp"
#include "memory/thread_local.hpp"

first_person_weapon_data* __cdecl first_person_weapon_get_weapon_data(first_person_weapon* fp_weapon, long weapon_slot)
{
	//INVOKE(0x00A9BF20, first_person_weapon_get_weapon_data, fp_weapon, weapon_slot);

	return &fp_weapon->weapon_slots[weapon_slot];
}

void __cdecl first_person_weapon_perspective_changed(e_output_user_index output_user_index)
{
	INVOKE(0x00A9C550, first_person_weapon_perspective_changed, output_user_index);
}

void __cdecl first_person_weapons_update()
{
	INVOKE(0x00A9ED20, first_person_weapons_update);
}

void __cdecl first_person_weapons_update_camera_estimates()
{
	INVOKE(0x00A9EDF0, first_person_weapons_update_camera_estimates);
}

first_person_weapon* first_person_weapon_get(e_output_user_index output_user_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(first_person_weapons);

	ASSERT(VALID_INDEX(output_user_index, k_number_of_output_users));

	return &first_person_weapons[output_user_index];
}

long first_person_weapon_get_current_state_string(long unit_index, long weapon_slot)
{
	TLS_DATA_GET_VALUE_REFERENCE(first_person_weapons);
	for (e_output_user_index user_index = first_output_user(); user_index != k_output_user_none; user_index = next_output_user(user_index))
	{
		struct first_person_weapon* first_person_weapon = first_person_weapon_get(user_index);
		if (first_person_weapon->unit_index == unit_index)
		{
			//return first_person_weapon_get_weapon_data(first_person_weapon, weapon_slot)->animation_manager.get_state_name();
			return first_person_weapon_get_weapon_data(first_person_weapon, weapon_slot)->current_state_string;
		}
	}

	return _string_id_invalid;
}

long first_person_weapon_get_pending_state_string(long unit_index, long weapon_slot)
{
	for (e_output_user_index user_index = first_output_user(); user_index != k_output_user_none; user_index = next_output_user(user_index))
	{
		struct first_person_weapon* first_person_weapon = first_person_weapon_get(user_index);
		if (first_person_weapon->unit_index == unit_index)
		{
			return first_person_weapon_get_weapon_data(first_person_weapon, weapon_slot)->pending_state_string;
		}
	}

	return _string_id_invalid;
}

