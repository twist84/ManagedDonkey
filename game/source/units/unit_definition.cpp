#include "units/unit_definition.hpp"

void s_unit_camera_track::update_reference_names()
{
	track.get_name();
}

void s_unit_boarding_melee::update_reference_names()
{
	boarding_melee_damage.get_name();
	boarding_melee_response.get_name();
	eviction_melee_damage.get_name();
	eviction_melee_response.get_name();
	landing_melee_damage.get_name();
	flurry_melee_damage.get_name();
	obstacle_smash_damage.get_name();
	assassination_primary_damage.get_name();
	assassination_ragdoll_damage.get_name();
}

void s_unit_boost::update_reference_names()
{
	boost_collision_damage.get_name();
}

void _unit_definition::update_reference_names()
{
	hologram_unit_reference.get_name();
	integrated_light_toggle.get_name();
	assasination_start_damage_response.get_name();
	assassination_weapon.get_name();
	spawned_turret_character.get_name();
	melee_damage.get_name();
	emp_disabled_effect.get_name();
	exit_and_detach_damage.get_name();
	exit_and_detach_weapon.get_name();

	UPDATE_REFERENCE_NAMES(new_hud_interfaces);
	UPDATE_REFERENCE_NAMES(dialogue_variants);
	UPDATE_REFERENCE_NAMES(weapons);
	UPDATE_REFERENCE_NAMES(target_trackingk);
	UPDATE_REFERENCE_NAMES(seats_block);
}

void unit_hud_reference::update_reference_names()
{
	chud_interface.get_name();
}

void dialogue_variant_definition::update_reference_names()
{
	dialogue.get_name();
}

void unit_initial_weapon::update_reference_names()
{
	weapon.get_name();
}

void s_target_tracking_parameters::update_reference_names()
{
	tracking_sound.get_name();
	locked_sound.get_name();
}

void unit_seat::update_reference_names()
{
	built_in_gunner.get_name();

	UPDATE_REFERENCE_NAMES(unit_hud_interface);
}

