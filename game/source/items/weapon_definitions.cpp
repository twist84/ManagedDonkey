#include "items\weapon_definitions.hpp"

void melee_damage_parameters_struct::update_reference_names()
{
	UPDATE_REFERENCE_NAME(first_hit_damage);
	UPDATE_REFERENCE_NAME(first_hit_response);
	UPDATE_REFERENCE_NAME(second_hit_damage);
	UPDATE_REFERENCE_NAME(second_hit_response);
	UPDATE_REFERENCE_NAME(third_hit_damage);
	UPDATE_REFERENCE_NAME(third_hit_response);
	UPDATE_REFERENCE_NAME(lunge_melee_damage);
	UPDATE_REFERENCE_NAME(lunge_melee_response);
	UPDATE_REFERENCE_NAME(empty_melee_damage);
	UPDATE_REFERENCE_NAME(empty_melee_response);
	UPDATE_REFERENCE_NAME(clang_melee_damage);
	UPDATE_REFERENCE_NAME(clang_melee_response);
}

void weapon_interface_definition_new::update_reference_names()
{
	UPDATE_REFERENCE_NAME(chud_interface);
}

void _weapon_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(ready_effect);
	UPDATE_REFERENCE_NAME(ready_damage_effect);
	UPDATE_REFERENCE_NAME(overheated);
	UPDATE_REFERENCE_NAME(overheated_damage_effect);
	UPDATE_REFERENCE_NAME(detonation);
	UPDATE_REFERENCE_NAME(detonation_damage_effect);
	UPDATE_REFERENCE_NAME(player_melee_damage);
	UPDATE_REFERENCE_NAME(player_melee_response);
	UPDATE_REFERENCE_NAME(clash_effect);
	UPDATE_REFERENCE_NAME(weapon_power_on_effect);
	UPDATE_REFERENCE_NAME(weapon_power_off_effect);
	UPDATE_REFERENCE_NAME(pickup_sound);
	UPDATE_REFERENCE_NAME(zoom_in_sound);
	UPDATE_REFERENCE_NAME(zoom_out_sound);
	UPDATE_REFERENCE_NAME(deployed_vehicle);
	UPDATE_REFERENCE_NAME(tossed_weapon);
	UPDATE_REFERENCE_NAME(age_effect);
	UPDATE_REFERENCE_NAME(age_weapon);
	UPDATE_REFERENCE_NAME(age_material_effects);

	UPDATE_STRUCT_REFERENCE_NAMES(melee_damage_parameters);
}

void weapon_definition::update_reference_names()
{
	UPDATE_STRUCT_REFERENCE_NAMES(weapon);
}

