#include "game/game_engine_spawn_influencer.hpp"

void s_weapon_spawn_influence::update_reference_names()
{
	UPDATE_REFERENCE_NAME(weapon);
}

void s_vehicle_spawn_influence::update_reference_names()
{
	UPDATE_REFERENCE_NAME(vehicle);
}

void s_projectile_spawn_influence::update_reference_names()
{
	UPDATE_REFERENCE_NAME(projectile);
}

void s_equipment_spawn_influence::update_reference_names()
{
	UPDATE_REFERENCE_NAME(equipment);
}

