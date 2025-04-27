#include "units/vehicle_auto_turret.hpp"

bool c_vehicle_auto_turret::control(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BBFE90, c_vehicle_auto_turret, control, vehicle_index);
}

bool c_vehicle_auto_turret::get_damage_owner(long vehicle_index, s_damage_owner* damage_owner)
{
	return INVOKE_CLASS_MEMBER(0x00BBFF90, c_vehicle_auto_turret, get_damage_owner, vehicle_index, damage_owner);
}

void c_vehicle_auto_turret::handle_deleted_object(long vehicle_index, long deleted_object_index)
{
	INVOKE_CLASS_MEMBER(0x00BBFFC0, c_vehicle_auto_turret, handle_deleted_object, vehicle_index, deleted_object_index);
}

bool c_vehicle_auto_turret::init(long vehicle_index, long trigger_volume, real32 min_range, real32 alt_range, real32 alt_time, long creator_object_index)
{
	return INVOKE_CLASS_MEMBER(0x00BC0010, c_vehicle_auto_turret, init, vehicle_index, trigger_volume, min_range, alt_range, alt_time, creator_object_index);
}

void c_vehicle_auto_turret::reset()
{
	return INVOKE_CLASS_MEMBER(0x00BC01A0, c_vehicle_auto_turret, reset);
}

real32 c_vehicle_auto_turret::score_target(long vehicle_index, long target_unit_index)
{
	return INVOKE_CLASS_MEMBER(0x00BC0210, c_vehicle_auto_turret, score_target, vehicle_index, target_unit_index);
}

void c_vehicle_auto_turret::set_damage_owner(long damage_owner_object_index)
{
	return INVOKE_CLASS_MEMBER(0x00BC0350, c_vehicle_auto_turret, set_damage_owner, damage_owner_object_index);
}

void c_vehicle_auto_turret::track_auto_target(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BC0370, c_vehicle_auto_turret, track_auto_target, vehicle_index);
}

void c_vehicle_auto_turret::update_auto_target(long vehicle_index)
{
	return INVOKE_CLASS_MEMBER(0x00BC0A00, c_vehicle_auto_turret, update_auto_target, vehicle_index);
}

