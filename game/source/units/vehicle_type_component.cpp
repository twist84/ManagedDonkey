#include "units/vehicle_type_component.hpp"

#include "units/vehicles.hpp"

void c_vehicle_type_component::adjust_gravity(long vehicle_index, vector3d* a2)
{
	DECLFUNC(0x00B8A190, void, __thiscall, c_vehicle_type_component*, long, vector3d*)(this, vehicle_index, a2);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_human_tank:
	//	get_type_human_tank()->adjust_gravity(vehicle_index, a2);
	//	break;
	//case _vehicle_type_human_jeep:
	//	get_type_human_jeep()->adjust_gravity(vehicle_index, a2);
	//	break;
	//case _vehicle_type_human_plane:
	//	get_type_human_plane()->adjust_gravity(vehicle_index, a2);
	//	break;
	//case _vehicle_type_alien_scout:
	//	get_type_alien_scout()->adjust_gravity(vehicle_index, a2);
	//	break;
	//case _vehicle_type_alien_fighter:
	//	get_type_alien_fighter()->adjust_gravity(vehicle_index, a2);
	//	break;
	//case _vehicle_type_turret:
	//	get_type_turret()->adjust_gravity(vehicle_index, a2);
	//	break;
	//case _vehicle_type_mantis:
	//	get_type_mantis()->adjust_gravity(vehicle_index, a2);
	//	break;
	//case _vehicle_type_vtol:
	//	get_type_vtol()->adjust_gravity(vehicle_index, a2);
	//	break;
	//case _vehicle_type_chopper:
	//	get_type_chopper()->adjust_gravity(vehicle_index, a2);
	//	break;
	//case _vehicle_type_guardian:
	//	get_type_guardian()->adjust_gravity(vehicle_index, a2);
	//	break;
	//}
}

bool c_vehicle_type_component::auto_control(long vehicle_index)
{
	return DECLFUNC(0x00B8A290, bool, __thiscall, c_vehicle_type_component*, long)(this, vehicle_index);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_turret:
	//	return get_type_turret()->auto_control(vehicle_index);
	//case _vehicle_type_guardian:
	//	return get_type_guardian()->auto_control(vehicle_index);
	//}
	//return false;
}

bool c_vehicle_type_component::compute_function_value(long vehicle_index, long function_name, real* function_value, bool* function_result)
{
	return DECLFUNC(0x00B8A2D0, bool, __thiscall, c_vehicle_type_component*, long, long, real*, bool*)(this, vehicle_index, function_name, function_value, function_result);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_human_tank:
	//	return get_type_human_tank()->compute_function_value(vehicle_index, function_name, function_value, function_result);
	//case _vehicle_type_human_jeep:
	//	return get_type_human_jeep()->compute_function_value(vehicle_index, function_name, function_value, function_result);
	//case _vehicle_type_human_plane:
	//	return get_type_human_plane()->compute_function_value(vehicle_index, function_name, function_value, function_result);
	//case _vehicle_type_alien_scout:
	//	return get_type_alien_scout()->compute_function_value(vehicle_index, function_name, function_value, function_result);
	//case _vehicle_type_alien_fighter:
	//	return get_type_alien_fighter()->compute_function_value(vehicle_index, function_name, function_value, function_result);
	//case _vehicle_type_turret:
	//	return get_type_turret()->compute_function_value(vehicle_index, function_name, function_value, function_result);
	//case _vehicle_type_mantis:
	//	return get_type_mantis()->compute_function_value(vehicle_index, function_name, function_value, function_result);
	//case _vehicle_type_vtol:
	//	return get_type_vtol()->compute_function_value(vehicle_index, function_name, function_value, function_result);
	//case _vehicle_type_chopper:
	//	return get_type_chopper()->compute_function_value(vehicle_index, function_name, function_value, function_result);
	//case _vehicle_type_guardian:
	//	return get_type_guardian()->compute_function_value(vehicle_index, function_name, function_value, function_result);
	//}
	//return false;
}

void c_vehicle_type_component::create_effects(long vehicle_index)
{
	DECLFUNC(0x00B8A400, s_vehicle_engine* const, __thiscall, c_vehicle_type_component*, long)(this, vehicle_index);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_human_tank:
	//	get_type_human_tank()->create_effects(vehicle_index);
	//	break;
	//case _vehicle_type_human_jeep:
	//	get_type_human_jeep()->create_effects(vehicle_index);
	//	break;
	//case _vehicle_type_human_plane:
	//	get_type_human_plane()->create_effects(vehicle_index);
	//	break;
	//case _vehicle_type_alien_scout:
	//	get_type_alien_scout()->create_effects(vehicle_index);
	//	break;
	//case _vehicle_type_alien_fighter:
	//	get_type_alien_fighter()->create_effects(vehicle_index);
	//	break;
	//case _vehicle_type_turret:
	//	get_type_turret()->create_effects(vehicle_index);
	//	break;
	//case _vehicle_type_mantis:
	//	get_type_mantis()->create_effects(vehicle_index);
	//	break;
	//case _vehicle_type_vtol:
	//	get_type_vtol()->create_effects(vehicle_index);
	//	break;
	//case _vehicle_type_chopper:
	//	get_type_chopper()->create_effects(vehicle_index);
	//	break;
	//case _vehicle_type_guardian:
	//	get_type_guardian()->create_effects(vehicle_index);
	//	break;
	//}
}

void c_vehicle_type_component::deplete_function_variables(long vehicle_index)
{
	DECLFUNC(0x00B8A4E0, s_vehicle_engine* const, __thiscall, c_vehicle_type_component*, long)(this, vehicle_index);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_human_tank:
	//	get_type_human_tank()->deplete_function_variables(vehicle_index);
	//	break;
	//case _vehicle_type_human_jeep:
	//	get_type_human_jeep()->deplete_function_variables(vehicle_index);
	//	break;
	//case _vehicle_type_human_plane:
	//	get_type_human_plane()->deplete_function_variables(vehicle_index);
	//	break;
	//case _vehicle_type_alien_scout:
	//	get_type_alien_scout()->deplete_function_variables(vehicle_index);
	//	break;
	//case _vehicle_type_alien_fighter:
	//	get_type_alien_fighter()->deplete_function_variables(vehicle_index);
	//	break;
	//case _vehicle_type_turret:
	//	get_type_turret()->deplete_function_variables(vehicle_index);
	//	break;
	//case _vehicle_type_mantis:
	//	get_type_mantis()->deplete_function_variables(vehicle_index);
	//	break;
	//case _vehicle_type_vtol:
	//	get_type_vtol()->deplete_function_variables(vehicle_index);
	//	break;
	//case _vehicle_type_chopper:
	//	get_type_chopper()->deplete_function_variables(vehicle_index);
	//	break;
	//case _vehicle_type_guardian:
	//	get_type_guardian()->deplete_function_variables(vehicle_index);
	//	break;
	//}
}

bool c_vehicle_type_component::effected_by_vehicle_ceiling(long vehicle_index)
{
	return DECLFUNC(0x00B8A5B0, bool, __thiscall, c_vehicle_type_component*, long)(this, vehicle_index);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_human_tank:
	//	return get_type_human_tank()->effected_by_vehicle_ceiling(vehicle_index);
	//case _vehicle_type_human_jeep:
	//	return get_type_human_jeep()->effected_by_vehicle_ceiling(vehicle_index);
	//case _vehicle_type_human_plane:
	//	return get_type_human_plane()->effected_by_vehicle_ceiling(vehicle_index);
	//case _vehicle_type_alien_scout:
	//	return get_type_alien_scout()->effected_by_vehicle_ceiling(vehicle_index);
	//case _vehicle_type_alien_fighter:
	//	return get_type_alien_fighter()->effected_by_vehicle_ceiling(vehicle_index);
	//case _vehicle_type_turret:
	//	return get_type_turret()->effected_by_vehicle_ceiling(vehicle_index);
	//case _vehicle_type_mantis:
	//	return get_type_mantis()->effected_by_vehicle_ceiling(vehicle_index);
	//case _vehicle_type_vtol:
	//	return get_type_vtol()->effected_by_vehicle_ceiling(vehicle_index);
	//case _vehicle_type_chopper:
	//	return get_type_chopper()->effected_by_vehicle_ceiling(vehicle_index);
	//case _vehicle_type_guardian:
	//	return get_type_guardian()->effected_by_vehicle_ceiling(vehicle_index);
	//}
	//return false;
}

c_vehicle_auto_turret* c_vehicle_type_component::get_auto_turret(long vehicle_index)
{
	return DECLFUNC(0x00B8A680, c_vehicle_auto_turret*, __thiscall, c_vehicle_type_component*, long)(this, vehicle_index);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_turret:
	//	return get_type_turret()->get_auto_turret(vehicle_index);
	//case _vehicle_type_guardian:
	//	return get_type_guardian()->get_auto_turret(vehicle_index);
	//}
	//return false;
}

bool c_vehicle_type_component::get_auto_turret_damage_owner(long vehicle_index, s_damage_owner* auto_turret_damage_owner)
{
	return DECLFUNC(0x00B8A6E0, bool, __thiscall, c_vehicle_type_component*, long, s_damage_owner*)(this, vehicle_index, auto_turret_damage_owner);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_turret:
	//	return get_type_turret()->get_damage_owner(vehicle_index);
	//case _vehicle_type_guardian:
	//	return get_type_guardian()->get_damage_owner(vehicle_index);
	//}
	//return false;
}

s_vehicle_engine* const c_vehicle_type_component::get_engine(long vehicle_index)
{
	//return DECLFUNC(0x00B8A720, s_vehicle_engine* const, __thiscall, c_vehicle_type_component*, long)(this, vehicle_index);

	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
		return get_type_human_tank()->get_engine(vehicle_index);
	case _vehicle_type_human_jeep:
		return get_type_human_jeep()->get_engine(vehicle_index);
	case _vehicle_type_chopper:
		return get_type_chopper()->get_engine(vehicle_index);
	}
	return NULL;
}

c_vehicle_type_human_tank* c_vehicle_type_component::get_type_human_tank()
{
	//return DECLFUNC(0x00B8A780, c_vehicle_type_human_tank*, __thiscall, c_vehicle_type_component*)(this);

	ASSERT(m_initialization_type == _vehicle_type_human_tank);
	return &m_type_human_tank;
}

c_vehicle_type_human_jeep* c_vehicle_type_component::get_type_human_jeep()
{
	//return DECLFUNC(0x00B8A790, c_vehicle_type_human_jeep*, __thiscall, c_vehicle_type_component*)(this);

	ASSERT(m_initialization_type == _vehicle_type_human_jeep);
	return &m_type_human_jeep;
}

c_vehicle_type_human_plane* c_vehicle_type_component::get_type_human_plane()
{
	//return DECLFUNC(0x00B8A7A0, c_vehicle_type_human_plane*, __thiscall, c_vehicle_type_component*)(this);

	ASSERT(m_initialization_type == _vehicle_type_human_plane);
	return &m_type_human_plane;
}

c_vehicle_type_alien_scout* c_vehicle_type_component::get_type_alien_scout()
{
	//return DECLFUNC(0x00B8A7B0, c_vehicle_type_alien_scout*, __thiscall, c_vehicle_type_component*)(this);

	ASSERT(m_initialization_type == _vehicle_type_alien_scout);
	return &m_type_alien_scout;
}

c_vehicle_type_alien_fighter* c_vehicle_type_component::get_type_alien_fighter()
{
	//return DECLFUNC(0x00B8A7C0, c_vehicle_type_alien_fighter*, __thiscall, c_vehicle_type_component*)(this);

	ASSERT(m_initialization_type == _vehicle_type_alien_fighter);
	return &m_type_alien_fighter;
}

c_vehicle_type_turret* c_vehicle_type_component::get_type_turret()
{
	//return DECLFUNC(0x00B8A7D0, c_vehicle_type_turret*, __thiscall, c_vehicle_type_component*)(this);

	ASSERT(m_initialization_type == _vehicle_type_turret);
	return &m_type_turret;
}

c_vehicle_type_mantis* c_vehicle_type_component::get_type_mantis()
{
	//return DECLFUNC(0x00B8A7E0, c_vehicle_type_mantis*, __thiscall, c_vehicle_type_component*)(this);

	ASSERT(m_initialization_type == _vehicle_type_mantis);
	return &m_type_mantis;
}

c_vehicle_type_vtol* c_vehicle_type_component::get_type_vtol()
{
	//return DECLFUNC(0x00B8A7F0, c_vehicle_type_vtol*, __thiscall, c_vehicle_type_component*)(this);

	ASSERT(m_initialization_type == _vehicle_type_vtol);
	return &m_type_vtol;
}

c_vehicle_type_chopper* c_vehicle_type_component::get_type_chopper()
{
	//return DECLFUNC(0x00B8A800, c_vehicle_type_chopper*, __thiscall, c_vehicle_type_component*)(this);

	ASSERT(m_initialization_type == _vehicle_type_chopper);
	return &m_type_chopper;
}

c_vehicle_type_guardian* c_vehicle_type_component::get_type_guardian()
{
	//return DECLFUNC(0x00B8A810, c_vehicle_type_guardian*, __thiscall, c_vehicle_type_component*)(this);

	ASSERT(m_initialization_type == _vehicle_type_guardian);
	return &m_type_guardian;
}

void c_vehicle_type_component::handle_deleted_object(long vehicle_index, long object_index)
{
	DECLFUNC(0x00B8A820, void, __thiscall, c_vehicle_type_component*, long, long)(this, vehicle_index, object_index);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_turret:
	//	get_type_turret()->handle_deleted_object(vehicle_index, object_index);
	//	break;
	//case _vehicle_type_guardian:
	//	get_type_guardian()->handle_deleted_object(vehicle_index, object_index);
	//	break;
	//}
}

void c_vehicle_type_component::handled_changed_vehicle_type(long vehicle_index)
{
	DECLFUNC(0x00B8A850, void, __thiscall, c_vehicle_type_component*, long)(this, vehicle_index);

	//vehicle_datum* vehicle = (vehicle_datum*)object_get_and_verify_type(vehicle_index, _object_mask_vehicle);
	//if (vehicle_definition_get_default_type(vehicle->object.definition_index) != m_initialization_type)
	//	reset(vehicle_index);
}

bool c_vehicle_type_component::init_auto_turret(long vehicle_index, long trigger_volume, real a3, real a4, real a5, long object_index)
{
	return DECLFUNC(0x00B8A8A0, bool, __thiscall, c_vehicle_type_component*, long, long, real, real, real, long)(
		this, vehicle_index, trigger_volume, a3, a4, a5, object_index);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_turret:
	//	return get_type_turret()->init_auto_turret(vehicle_index, trigger_volume, a3, a4, a5, object_index);
	//case _vehicle_type_guardian:
	//	return get_type_guardian()->init_auto_turret(vehicle_index, trigger_volume, a3, a4, a5, object_index);
	//}
	//return false;
}

bool c_vehicle_type_component::is_e_braking(long vehicle_index)
{
	return DECLFUNC(0x00B8A910, bool, __thiscall, c_vehicle_type_component*, long)(this, vehicle_index);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_human_jeep:
	//	return get_type_human_jeep()->is_e_braking(vehicle_index);
	//case _vehicle_type_chopper:
	//	return get_type_chopper()->is_e_braking(vehicle_index);
	//}
	//return false;
}

bool c_vehicle_type_component::is_running_trick(long vehicle_index)
{
	return DECLFUNC(0x00B8A940, bool, __thiscall, c_vehicle_type_component*, long)(this, vehicle_index);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_alien_fighter:
	//	return get_type_alien_fighter()->is_running_trick(vehicle_index);
	//}
	//return false;
}

bool c_vehicle_type_component::is_stopped(long vehicle_index)
{
	return DECLFUNC(0x00B8A970, bool, __thiscall, c_vehicle_type_component*, long)(this, vehicle_index);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_human_tank:
	//	return get_type_human_tank()->is_stopped(vehicle_index);
	//case _vehicle_type_human_jeep:
	//	return get_type_human_jeep()->is_stopped(vehicle_index);
	//case _vehicle_type_human_plane:
	//	return get_type_human_plane()->is_stopped(vehicle_index);
	//case _vehicle_type_alien_scout:
	//	return get_type_alien_scout()->is_stopped(vehicle_index);
	//case _vehicle_type_alien_fighter:
	//	return get_type_alien_fighter()->is_stopped(vehicle_index);
	//case _vehicle_type_turret:
	//	return get_type_turret()->is_stopped(vehicle_index);
	//case _vehicle_type_mantis:
	//	return get_type_mantis()->is_stopped(vehicle_index);
	//case _vehicle_type_vtol:
	//	return get_type_vtol()->is_stopped(vehicle_index);
	//case _vehicle_type_chopper:
	//	return get_type_chopper()->is_stopped(vehicle_index);
	//case _vehicle_type_guardian:
	//	return get_type_guardian()->is_stopped(vehicle_index);
	//}
	//return false;
}

bool c_vehicle_type_component::kills_riders_at_terminal_velocity(long vehicle_index)
{
	return DECLFUNC(0x00B8AA50, bool, __thiscall, c_vehicle_type_component*, long)(this, vehicle_index);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_human_tank:
	//	return get_type_human_tank()->kills_riders_at_terminal_velocity(vehicle_index);
	//case _vehicle_type_human_jeep:
	//	return get_type_human_jeep()->kills_riders_at_terminal_velocity(vehicle_index);
	//case _vehicle_type_human_plane:
	//	return get_type_human_plane()->kills_riders_at_terminal_velocity(vehicle_index);
	//case _vehicle_type_alien_scout:
	//	return get_type_alien_scout()->kills_riders_at_terminal_velocity(vehicle_index);
	//case _vehicle_type_alien_fighter:
	//	return get_type_alien_fighter()->kills_riders_at_terminal_velocity(vehicle_index);
	//case _vehicle_type_turret:
	//	return get_type_turret()->kills_riders_at_terminal_velocity(vehicle_index);
	//case _vehicle_type_mantis:
	//	return get_type_mantis()->kills_riders_at_terminal_velocity(vehicle_index);
	//case _vehicle_type_vtol:
	//	return get_type_vtol()->kills_riders_at_terminal_velocity(vehicle_index);
	//case _vehicle_type_chopper:
	//	return get_type_chopper()->kills_riders_at_terminal_velocity(vehicle_index);
	//case _vehicle_type_guardian:
	//	return get_type_guardian()->kills_riders_at_terminal_velocity(vehicle_index);
	//}
	//return false;
}

bool c_vehicle_type_component::physics_disabled(long vehicle_index)
{
	return DECLFUNC(0x00B8AB20, bool, __thiscall, c_vehicle_type_component*, long)(this, vehicle_index);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_human_tank:
	//	return get_type_human_tank()->physics_disabled(vehicle_index);
	//case _vehicle_type_human_jeep:
	//	return get_type_human_jeep()->physics_disabled(vehicle_index);
	//case _vehicle_type_human_plane:
	//	return get_type_human_plane()->physics_disabled(vehicle_index);
	//case _vehicle_type_alien_scout:
	//	return get_type_alien_scout()->physics_disabled(vehicle_index);
	//case _vehicle_type_alien_fighter:
	//	return get_type_alien_fighter()->physics_disabled(vehicle_index);
	//case _vehicle_type_turret:
	//	return get_type_turret()->physics_disabled(vehicle_index);
	//case _vehicle_type_mantis:
	//	return get_type_mantis()->physics_disabled(vehicle_index);
	//case _vehicle_type_vtol:
	//	return get_type_vtol()->physics_disabled(vehicle_index);
	//case _vehicle_type_chopper:
	//	return get_type_chopper()->physics_disabled(vehicle_index);
	//case _vehicle_type_guardian:
	//	return get_type_guardian()->physics_disabled(vehicle_index);
	//}
	//return false;
}

void c_vehicle_type_component::process_animation_channels(long vehicle_index, void(__cdecl* callback)(long, render_model_definition const*, c_animation_channel*, real, real, real, void*), void* callback_data, c_animation_channel* channel, bool a5)
{
	DECLFUNC(0x00B8ABF0, void, __thiscall, c_vehicle_type_component*, long, decltype(callback), c_animation_channel*, bool)(this, vehicle_index, callback, channel, a5);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_human_tank:
	//	get_type_human_tank()->process_animation_channels(vehicle_index, callback, channel, a5);
	//	break;
	//case _vehicle_type_human_jeep:
	//	get_type_human_jeep()->process_animation_channels(vehicle_index, callback, channel, a5);
	//	break;
	//case _vehicle_type_human_plane:
	//	get_type_human_plane()->process_animation_channels(vehicle_index, callback, channel, a5);
	//	break;
	//case _vehicle_type_alien_scout:
	//	get_type_alien_scout()->process_animation_channels(vehicle_index, callback, channel, a5);
	//	break;
	//case _vehicle_type_alien_fighter:
	//	get_type_alien_fighter()->process_animation_channels(vehicle_index, callback, channel, a5);
	//	break;
	//case _vehicle_type_turret:
	//	get_type_turret()->process_animation_channels(vehicle_index, callback, channel, a5);
	//	break;
	//case _vehicle_type_mantis:
	//	get_type_mantis()->process_animation_channels(vehicle_index, callback, channel, a5);
	//	break;
	//case _vehicle_type_vtol:
	//	get_type_vtol()->process_animation_channels(vehicle_index, callback, channel, a5);
	//	break;
	//case _vehicle_type_chopper:
	//	get_type_chopper()->process_animation_channels(vehicle_index, callback, channel, a5);
	//	break;
	//case _vehicle_type_guardian:
	//	get_type_guardian()->process_animation_channels(vehicle_index, callback, channel, a5);
	//	break;
	//}
}

void c_vehicle_type_component::reset(long vehicle_index)
{
	DECLFUNC(0x00B8AD30, void, __thiscall, c_vehicle_type_component*, long)(this, vehicle_index);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_human_tank:
	//	get_type_human_tank()->reset(vehicle_index);
	//	break;
	//case _vehicle_type_human_jeep:
	//	get_type_human_jeep()->reset(vehicle_index);
	//	break;
	//case _vehicle_type_human_plane:
	//	get_type_human_plane()->reset(vehicle_index);
	//	break;
	//case _vehicle_type_alien_scout:
	//	get_type_alien_scout()->reset(vehicle_index);
	//	break;
	//case _vehicle_type_alien_fighter:
	//	get_type_alien_fighter()->reset(vehicle_index);
	//	break;
	//case _vehicle_type_turret:
	//	get_type_turret()->reset(vehicle_index);
	//	break;
	//case _vehicle_type_mantis:
	//	get_type_mantis()->reset(vehicle_index);
	//	break;
	//case _vehicle_type_vtol:
	//	get_type_vtol()->reset(vehicle_index);
	//	break;
	//case _vehicle_type_chopper:
	//	get_type_chopper()->reset(vehicle_index);
	//	break;
	//case _vehicle_type_guardian:
	//	get_type_guardian()->reset(vehicle_index);
	//	break;
	//}
}

void c_vehicle_type_component::set_auto_turret_damage_owner(long vehicle_index, long object_index)
{
	DECLFUNC(0x00B8AE20, void, __thiscall, c_vehicle_type_component*, long, long)(this, vehicle_index, object_index);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_turret:
	//	get_type_turret()->set_damage_owner(vehicle_index);
	//	break;
	//case _vehicle_type_guardian:
	//	get_type_guardian()->set_damage_owner(vehicle_index);
	//	break;
	//}
}

bool c_vehicle_type_component::should_override_deactivation(long vehicle_index)
{
	return DECLFUNC(0x00B8AE60, bool, __thiscall, c_vehicle_type_component*, long)(this, vehicle_index);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_human_tank:
	//	return get_type_human_tank()->should_override_deactivation(vehicle_index);
	//case _vehicle_type_human_jeep:
	//	return get_type_human_jeep()->should_override_deactivation(vehicle_index);
	//case _vehicle_type_human_plane:
	//	return get_type_human_plane()->should_override_deactivation(vehicle_index);
	//case _vehicle_type_alien_scout:
	//	return get_type_alien_scout()->should_override_deactivation(vehicle_index);
	//case _vehicle_type_alien_fighter:
	//	return get_type_alien_fighter()->should_override_deactivation(vehicle_index);
	//case _vehicle_type_turret:
	//	return get_type_turret()->should_override_deactivation(vehicle_index);
	//case _vehicle_type_mantis:
	//	return get_type_mantis()->should_override_deactivation(vehicle_index);
	//case _vehicle_type_vtol:
	//	return get_type_vtol()->should_override_deactivation(vehicle_index);
	//case _vehicle_type_chopper:
	//	return get_type_chopper()->should_override_deactivation(vehicle_index);
	//case _vehicle_type_guardian:
	//	return get_type_guardian()->should_override_deactivation(vehicle_index);
	//}
	//return false;
}

//void c_vehicle_type_component::start_trick(long vehicle_index, e_vehicle_trick_type trick_type)
void c_vehicle_type_component::start_trick(long vehicle_index, long trick_type)
{
	DECLFUNC(0x00B8AF30, void, __thiscall, c_vehicle_type_component*, long, long)(this, vehicle_index, trick_type);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_alien_fighter:
	//	get_type_alien_fighter()->start_trick(vehicle_index, trick_type);
	//	break;
	//}
}

void c_vehicle_type_component::update_control(long vehicle_index)
{
	DECLFUNC(0x00B8AF60, void, __thiscall, c_vehicle_type_component*, long)(this, vehicle_index);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_human_tank:
	//	get_type_human_tank()->update_control(vehicle_index);
	//	break;
	//case _vehicle_type_human_jeep:
	//	get_type_human_jeep()->update_control(vehicle_index);
	//	break;
	//case _vehicle_type_human_plane:
	//	get_type_human_plane()->update_control(vehicle_index);
	//	break;
	//case _vehicle_type_alien_scout:
	//	get_type_alien_scout()->update_control(vehicle_index);
	//	break;
	//case _vehicle_type_alien_fighter:
	//	get_type_alien_fighter()->update_control(vehicle_index);
	//	break;
	//case _vehicle_type_turret:
	//	get_type_turret()->update_control(vehicle_index);
	//	break;
	//case _vehicle_type_mantis:
	//	get_type_mantis()->update_control(vehicle_index);
	//	break;
	//case _vehicle_type_vtol:
	//	get_type_vtol()->update_control(vehicle_index);
	//	break;
	//case _vehicle_type_chopper:
	//	get_type_chopper()->update_control(vehicle_index);
	//	break;
	//case _vehicle_type_guardian:
	//	get_type_guardian()->update_control(vehicle_index);
	//	break;
	//}
}

void c_vehicle_type_component::update_physics(long vehicle_index, s_havok_vehicle_physics_instance* instance)
{
	DECLFUNC(0x00B8B030, void, __thiscall, c_vehicle_type_component*, long, s_havok_vehicle_physics_instance*)(this, vehicle_index, instance);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_human_tank:
	//	get_type_human_tank()->update_physics(vehicle_index, instance);
	//	break;
	//case _vehicle_type_human_jeep:
	//	get_type_human_jeep()->update_physics(vehicle_index, instance);
	//	break;
	//case _vehicle_type_human_plane:
	//	get_type_human_plane()->update_physics(vehicle_index, instance);
	//	break;
	//case _vehicle_type_alien_scout:
	//	get_type_alien_scout()->update_physics(vehicle_index, instance);
	//	break;
	//case _vehicle_type_alien_fighter:
	//	get_type_alien_fighter()->update_physics(vehicle_index, instance);
	//	break;
	//case _vehicle_type_turret:
	//	get_type_turret()->update_physics(vehicle_index, instance);
	//	break;
	//case _vehicle_type_mantis:
	//	get_type_mantis()->update_physics(vehicle_index, instance);
	//	break;
	//case _vehicle_type_vtol:
	//	get_type_vtol()->update_physics(vehicle_index, instance);
	//	break;
	//case _vehicle_type_chopper:
	//	get_type_chopper()->update_physics(vehicle_index, instance);
	//	break;
	//case _vehicle_type_guardian:
	//	get_type_guardian()->update_physics(vehicle_index, instance);
	//	break;
	//}
}

bool c_vehicle_type_component::vector_is_upsides_down(long vehicle_index, vector3d const* vector)
{
	return DECLFUNC(0x00B8B1A0, bool, __thiscall, c_vehicle_type_component*, long, vector3d const*)(this, vehicle_index, vector);

	//switch (vehicle_get_type(vehicle_index))
	//{
	//case _vehicle_type_human_tank:
	//	return get_type_human_tank()->vector_is_upsides_down(vehicle_index, vector);
	//case _vehicle_type_human_jeep:
	//	return get_type_human_jeep()->vector_is_upsides_down(vehicle_index, vector);
	//case _vehicle_type_human_plane:
	//	return get_type_human_plane()->vector_is_upsides_down(vehicle_index, vector);
	//case _vehicle_type_alien_scout:
	//	return get_type_alien_scout()->vector_is_upsides_down(vehicle_index, vector);
	//case _vehicle_type_alien_fighter:
	//	return get_type_alien_fighter()->vector_is_upsides_down(vehicle_index, vector);
	//case _vehicle_type_turret:
	//	return get_type_turret()->vector_is_upsides_down(vehicle_index, vector);
	//case _vehicle_type_mantis:
	//	return get_type_mantis()->vector_is_upsides_down(vehicle_index, vector);
	//case _vehicle_type_vtol:
	//	return get_type_vtol()->vector_is_upsides_down(vehicle_index, vector);
	//case _vehicle_type_chopper:
	//	return get_type_chopper()->vector_is_upsides_down(vehicle_index, vector);
	//case _vehicle_type_guardian:
	//	return get_type_guardian()->vector_is_upsides_down(vehicle_index, vector);
	//}
	//return false;
}

