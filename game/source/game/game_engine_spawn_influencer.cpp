#include "game/game_engine_spawn_influencer.hpp"

void s_weapon_spawn_influence::update_reference_names()
{
	weapon.get_name();
}

void s_vehicle_spawn_influence::update_reference_names()
{
	vehicle.get_name();
}

void s_projectile_spawn_influence::update_reference_names()
{
	projectile.get_name();
}

void s_equipment_spawn_influence::update_reference_names()
{
	equipment.get_name();
}

