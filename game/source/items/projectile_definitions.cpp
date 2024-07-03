#include "items/projectile_definitions.hpp"

void _projectile_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(detonation_started);
	UPDATE_REFERENCE_NAME(detonation_effect_airborne);
	UPDATE_REFERENCE_NAME(detonation_effect_ground);
	UPDATE_REFERENCE_NAME(detonation_damage);
	UPDATE_REFERENCE_NAME(attached_detonation_damage);
	UPDATE_REFERENCE_NAME(super_detonation);
	UPDATE_REFERENCE_NAME(super_detonation_damage);
	UPDATE_REFERENCE_NAME(detonation_sound);
	UPDATE_REFERENCE_NAME(super_attached_detonation_damage);
	UPDATE_REFERENCE_NAME(flyby_sound);
	UPDATE_REFERENCE_NAME(impact_effect);
	UPDATE_REFERENCE_NAME(object_impact_effect);
	UPDATE_REFERENCE_NAME(impact_damage);
	UPDATE_REFERENCE_NAME(boarding_detonation_damage);
	UPDATE_REFERENCE_NAME(boarding_attached_detonation_damage);
}

void projectile_definition::update_reference_names()
{
	UPDATE_STRUCT_REFERENCE_NAMES(projectile);
}

