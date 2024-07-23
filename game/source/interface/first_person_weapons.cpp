#include "interface/first_person_weapons.hpp"

#include "cseries/cseries.hpp"

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

