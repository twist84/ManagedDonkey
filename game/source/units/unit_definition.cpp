#include "units/unit_definition.hpp"

void s_unit_camera_track::update_reference_names()
{
	UPDATE_REFERENCE_NAME(track);
}

void s_unit_boarding_melee::update_reference_names()
{
	UPDATE_REFERENCE_NAME(boarding_melee_damage);
	UPDATE_REFERENCE_NAME(boarding_melee_response);
	UPDATE_REFERENCE_NAME(eviction_melee_damage);
	UPDATE_REFERENCE_NAME(eviction_melee_response);
	UPDATE_REFERENCE_NAME(landing_melee_damage);
	UPDATE_REFERENCE_NAME(flurry_melee_damage);
	UPDATE_REFERENCE_NAME(obstacle_smash_damage);
	UPDATE_REFERENCE_NAME(assassination_primary_damage);
	UPDATE_REFERENCE_NAME(assassination_ragdoll_damage);
}

void s_unit_boost::update_reference_names()
{
	UPDATE_REFERENCE_NAME(boost_collision_damage);
}

void _unit_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(hologram_unit_reference);
	UPDATE_REFERENCE_NAME(integrated_light_toggle);
	UPDATE_REFERENCE_NAME(assasination_start_damage_response);
	UPDATE_REFERENCE_NAME(assassination_weapon);
	UPDATE_REFERENCE_NAME(spawned_turret_character);
	UPDATE_REFERENCE_NAME(melee_damage);
	UPDATE_REFERENCE_NAME(emp_disabled_effect);
	UPDATE_REFERENCE_NAME(exit_and_detach_damage);
	UPDATE_REFERENCE_NAME(exit_and_detach_weapon);

	UPDATE_BLOCK_REFERENCE_NAMES(new_hud_interfaces);
	UPDATE_BLOCK_REFERENCE_NAMES(dialogue_variants);
	UPDATE_BLOCK_REFERENCE_NAMES(weapons);
	UPDATE_BLOCK_REFERENCE_NAMES(target_trackingk);
	UPDATE_BLOCK_REFERENCE_NAMES(seats_block);
}

void unit_hud_reference::update_reference_names()
{
	UPDATE_REFERENCE_NAME(chud_interface);
}

void dialogue_variant_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(dialogue);
}

void unit_initial_weapon::update_reference_names()
{
	UPDATE_REFERENCE_NAME(weapon);
}

void s_target_tracking_parameters::update_reference_names()
{
	UPDATE_REFERENCE_NAME(tracking_sound);
	UPDATE_REFERENCE_NAME(locked_sound);
}

void unit_seat::update_reference_names()
{
	UPDATE_REFERENCE_NAME(built_in_gunner);

	UPDATE_BLOCK_REFERENCE_NAMES(unit_hud_interface);
}

void unit_definition::update_reference_names()
{
	UPDATE_STRUCT_REFERENCE_NAMES(unit);
}

