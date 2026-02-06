#include "units/vehicle_type_component.hpp"

#include "memory/module.hpp"
#include "motor/vehicle_motor_program.hpp"
#include "units/vehicles.hpp"
#include "units/vehicle_type_alien_fighter.hpp"
#include "units/vehicle_type_alien_scout.hpp"
#include "units/vehicle_type_chopper.hpp"
#include "units/vehicle_type_guardian.hpp"
#include "units/vehicle_type_human_jeep.hpp"
#include "units/vehicle_type_human_plane.hpp"
#include "units/vehicle_type_human_tank.hpp"
#include "units/vehicle_type_mantis.hpp"
#include "units/vehicle_type_turret.hpp"
#include "units/vehicle_type_vtol.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B8A190, c_vehicle_type_component, adjust_gravity);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A290, c_vehicle_type_component, auto_control);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A2D0, c_vehicle_type_component, compute_function_value);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A400, c_vehicle_type_component, create_effects);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A4E0, c_vehicle_type_component, deplete_function_variables);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A5B0, c_vehicle_type_component, effected_by_vehicle_ceiling);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A680, c_vehicle_type_component, get_auto_turret);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A6E0, c_vehicle_type_component, get_auto_turret_damage_owner);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A720, c_vehicle_type_component, get_engine);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A780, c_vehicle_type_component, get_type_human_tank);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A790, c_vehicle_type_component, get_type_human_jeep);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A7A0, c_vehicle_type_component, get_type_human_plane);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A7B0, c_vehicle_type_component, get_type_alien_scout);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A7C0, c_vehicle_type_component, get_type_alien_fighter);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A7D0, c_vehicle_type_component, get_type_turret);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A7E0, c_vehicle_type_component, get_type_mantis);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A7F0, c_vehicle_type_component, get_type_guardian);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A800, c_vehicle_type_component, get_type_vtol);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A810, c_vehicle_type_component, get_type_chopper);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A820, c_vehicle_type_component, handle_deleted_object);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A850, c_vehicle_type_component, handled_changed_vehicle_type);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A8A0, c_vehicle_type_component, init_auto_turret);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A910, c_vehicle_type_component, is_e_braking);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A940, c_vehicle_type_component, is_running_trick);
HOOK_DECLARE_CLASS_MEMBER(0x00B8A970, c_vehicle_type_component, is_stopped);
HOOK_DECLARE_CLASS_MEMBER(0x00B8AA50, c_vehicle_type_component, kills_riders_at_terminal_velocity);
HOOK_DECLARE_CLASS_MEMBER(0x00B8AB20, c_vehicle_type_component, physics_disabled);
HOOK_DECLARE_CLASS_MEMBER(0x00B8ABF0, c_vehicle_type_component, process_animation_channels);
HOOK_DECLARE_CLASS_MEMBER(0x00B8AD30, c_vehicle_type_component, reset);
HOOK_DECLARE_CLASS_MEMBER(0x00B8AE20, c_vehicle_type_component, set_auto_turret_damage_owner);
HOOK_DECLARE_CLASS_MEMBER(0x00B8AE60, c_vehicle_type_component, should_override_deactivation);
HOOK_DECLARE_CLASS_MEMBER(0x00B8AF30, c_vehicle_type_component, start_trick);
HOOK_DECLARE_CLASS_MEMBER(0x00B8AF60, c_vehicle_type_component, update_control);
HOOK_DECLARE_CLASS_MEMBER(0x00B8B030, c_vehicle_type_component, update_physics);
HOOK_DECLARE_CLASS_MEMBER(0x00B8B1A0, c_vehicle_type_component, vector_is_upsides_down);

void c_vehicle_type_component::adjust_gravity(int32 vehicle_index, real_vector3d* gravity_acceleration)
{
	//INVOKE_CLASS_MEMBER(0x00B8A190, c_vehicle_type_component, adjust_gravity, vehicle_index, gravity_acceleration);

	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
	{
		get_type_human_tank()->adjust_gravity(vehicle_index, gravity_acceleration);
	}
	break;
	case _vehicle_type_human_jeep:
	{
		get_type_human_jeep()->adjust_gravity(vehicle_index, gravity_acceleration);
	}
	break;
	case _vehicle_type_human_plane:
	{
		get_type_human_plane()->adjust_gravity(vehicle_index, gravity_acceleration);
	}
	break;
	case _vehicle_type_alien_scout:
	{
		get_type_alien_scout()->adjust_gravity(vehicle_index, gravity_acceleration);
	}
	break;
	case _vehicle_type_alien_fighter:
	{
		get_type_alien_fighter()->adjust_gravity(vehicle_index, gravity_acceleration);
	}
	break;
	case _vehicle_type_turret:
	{
		get_type_turret()->adjust_gravity(vehicle_index, gravity_acceleration);
	}
	break;
	case _vehicle_type_mantis:
	{
		get_type_mantis()->adjust_gravity(vehicle_index, gravity_acceleration);
	}
	break;
	case _vehicle_type_vtol:
	{
		get_type_vtol()->adjust_gravity(vehicle_index, gravity_acceleration);
	}
	break;
	case _vehicle_type_chopper:
	{
		get_type_chopper()->adjust_gravity(vehicle_index, gravity_acceleration);
	}
	break;
	case _vehicle_type_guardian:
	{
		get_type_guardian()->adjust_gravity(vehicle_index, gravity_acceleration);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}
}

bool c_vehicle_type_component::auto_control(int32 vehicle_index)
{
	//return INVOKE_CLASS_MEMBER(0x00B8A290, c_vehicle_type_component, auto_control, vehicle_index);

	bool result = false;
	if (c_vehicle_auto_turret* auto_turret = get_auto_turret(vehicle_index))
	{
		result = auto_turret->control(vehicle_index);
	}
	return result;
}

bool c_vehicle_type_component::compute_function_value(int32 vehicle_index, int32 function, real32* magnitude, bool* force_active)
{
	//return INVOKE_CLASS_MEMBER(0x00B8A2D0, c_vehicle_type_component, compute_function_value, vehicle_index, function, magnitude, force_active);

	bool result = false;

	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
	{
		result = get_type_human_tank()->compute_function_value(vehicle_index, function, magnitude, force_active);
	}
	break;
	case _vehicle_type_human_jeep:
	{
		result = get_type_human_jeep()->compute_function_value(vehicle_index, function, magnitude, force_active);
	}
	break;
	case _vehicle_type_human_plane:
	{
		result = get_type_human_plane()->compute_function_value(vehicle_index, function, magnitude, force_active);
	}
	break;
	case _vehicle_type_alien_scout:
	{
		result = get_type_alien_scout()->compute_function_value(vehicle_index, function, magnitude, force_active);
	}
	break;
	case _vehicle_type_alien_fighter:
	{
		result = get_type_alien_fighter()->compute_function_value(vehicle_index, function, magnitude, force_active);
	}
	break;
	case _vehicle_type_turret:
	{
		result = get_type_turret()->compute_function_value(vehicle_index, function, magnitude, force_active);
	}
	break;
	case _vehicle_type_mantis:
	{
		result = get_type_mantis()->compute_function_value(vehicle_index, function, magnitude, force_active);
	}
	break;
	case _vehicle_type_vtol:
	{
		result = get_type_vtol()->compute_function_value(vehicle_index, function, magnitude, force_active);
	}
	break;
	case _vehicle_type_chopper:
	{
		result = get_type_chopper()->compute_function_value(vehicle_index, function, magnitude, force_active);
	}
	break;
	case _vehicle_type_guardian:
	{
		result = get_type_guardian()->compute_function_value(vehicle_index, function, magnitude, force_active);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}

	return result;
}

void c_vehicle_type_component::create_effects(int32 vehicle_index)
{
	//INVOKE_CLASS_MEMBER(0x00B8A400, c_vehicle_type_component, create_effects, vehicle_index);

	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
	{
		get_type_human_tank()->create_effects(vehicle_index);
	}
	break;
	case _vehicle_type_human_jeep:
	{
		get_type_human_jeep()->create_effects(vehicle_index);
	}
	break;
	case _vehicle_type_human_plane:
	{
		get_type_human_plane()->create_effects(vehicle_index);
	}
	break;
	case _vehicle_type_alien_scout:
	{
		get_type_alien_scout()->create_effects(vehicle_index);
	}
	break;
	case _vehicle_type_alien_fighter:
	{
		get_type_alien_fighter()->create_effects(vehicle_index);
	}
	break;
	case _vehicle_type_turret:
	{
		get_type_turret()->create_effects(vehicle_index);
	}
	break;
	case _vehicle_type_mantis:
	{
		get_type_mantis()->create_effects(vehicle_index);
	}
	break;
	case _vehicle_type_vtol:
	{
		get_type_vtol()->create_effects(vehicle_index);
	}
	break;
	case _vehicle_type_chopper:
	{
		get_type_chopper()->create_effects(vehicle_index);
	}
	break;
	case _vehicle_type_guardian:
	{
		get_type_guardian()->create_effects(vehicle_index);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}
}

void c_vehicle_type_component::deplete_function_variables(int32 vehicle_index)
{
	//INVOKE_CLASS_MEMBER(0x00B8A4E0, c_vehicle_type_component, deplete_function_variables, vehicle_index);

	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
	{
		get_type_human_tank()->deplete_function_variables(vehicle_index);
	}
	break;
	case _vehicle_type_human_jeep:
	{
		get_type_human_jeep()->deplete_function_variables(vehicle_index);
	}
	break;
	case _vehicle_type_human_plane:
	{
		get_type_human_plane()->deplete_function_variables(vehicle_index);
	}
	break;
	case _vehicle_type_alien_scout:
	{
		get_type_alien_scout()->deplete_function_variables(vehicle_index);
	}
	break;
	case _vehicle_type_alien_fighter:
	{
		get_type_alien_fighter()->deplete_function_variables(vehicle_index);
	}
	break;
	case _vehicle_type_turret:
	{
		get_type_turret()->deplete_function_variables(vehicle_index);
	}
	break;
	case _vehicle_type_mantis:
	{
		get_type_mantis()->deplete_function_variables(vehicle_index);
	}
	break;
	case _vehicle_type_vtol:
	{
		get_type_vtol()->deplete_function_variables(vehicle_index);
	}
	break;
	case _vehicle_type_chopper:
	{
		get_type_chopper()->deplete_function_variables(vehicle_index);
	}
	break;
	case _vehicle_type_guardian:
	{
		get_type_guardian()->deplete_function_variables(vehicle_index);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}
}

bool c_vehicle_type_component::effected_by_vehicle_ceiling(int32 vehicle_index)
{
	//return INVOKE_CLASS_MEMBER(0x00B8A5B0, c_vehicle_type_component, effected_by_vehicle_ceiling, vehicle_index);

	bool result = false;

	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
	{
		result = get_type_human_tank()->effected_by_vehicle_ceiling(vehicle_index);
	}
	break;
	case _vehicle_type_human_jeep:
	{
		result = get_type_human_jeep()->effected_by_vehicle_ceiling(vehicle_index);
	}
	break;
	case _vehicle_type_human_plane:
	{
		result = get_type_human_plane()->effected_by_vehicle_ceiling(vehicle_index);
	}
	break;
	case _vehicle_type_alien_scout:
	{
		result = get_type_alien_scout()->effected_by_vehicle_ceiling(vehicle_index);
	}
	break;
	case _vehicle_type_alien_fighter:
	{
		result = get_type_alien_fighter()->effected_by_vehicle_ceiling(vehicle_index);
	}
	break;
	case _vehicle_type_turret:
	{
		result = get_type_turret()->effected_by_vehicle_ceiling(vehicle_index);
	}
	break;
	case _vehicle_type_mantis:
	{
		result = get_type_mantis()->effected_by_vehicle_ceiling(vehicle_index);
	}
	break;
	case _vehicle_type_vtol:
	{
		result = get_type_vtol()->effected_by_vehicle_ceiling(vehicle_index);
	}
	break;
	case _vehicle_type_chopper:
	{
		result = get_type_chopper()->effected_by_vehicle_ceiling(vehicle_index);
	}
	break;
	case _vehicle_type_guardian:
	{
		result = get_type_guardian()->effected_by_vehicle_ceiling(vehicle_index);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}

	return result;
}

c_vehicle_auto_turret* c_vehicle_type_guardian::get_auto_turret()
{
	//return INVOKE_CLASS_MEMBER(0x00B8A6C0, c_vehicle_type_guardian, get_auto_turret);

	return &m_auto_turret;
}

c_vehicle_auto_turret* c_vehicle_type_turret::get_auto_turret()
{
	//return INVOKE_CLASS_MEMBER(0x00B8A6D0, c_vehicle_type_turret, get_auto_turret);

	return &m_auto_turret;
}

c_vehicle_auto_turret* c_vehicle_type_component::get_auto_turret(int32 vehicle_index)
{
	//return INVOKE_CLASS_MEMBER(0x00B8A680, c_vehicle_type_component, get_auto_turret, vehicle_index);

	c_vehicle_auto_turret* auto_turret = nullptr;

	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_turret:
	{
		auto_turret = get_type_turret()->get_auto_turret();
	}
	break;
	case _vehicle_type_guardian:
	{
		auto_turret = get_type_guardian()->get_auto_turret();
	}
	break;
	}

	return auto_turret;
}

bool c_vehicle_type_component::get_auto_turret_damage_owner(int32 vehicle_index, s_damage_owner* damage_owner)
{
	//return INVOKE_CLASS_MEMBER(0x00B8A6E0, c_vehicle_type_component, get_auto_turret_damage_owner, vehicle_index, damage_owner);

	bool result = false;
	if (c_vehicle_auto_turret* auto_turret = get_auto_turret(vehicle_index))
	{
		result = auto_turret->get_damage_owner(vehicle_index, damage_owner);
	}

	return result;
}

const s_vehicle_engine* c_vehicle_type_component::get_engine(int32 vehicle_index)
{
	//return INVOKE_CLASS_MEMBER(0x00B8A720, c_vehicle_type_component, get_engine, vehicle_index);

	const s_vehicle_engine* engine = nullptr;

	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
	{
		engine = get_type_human_tank()->get_engine(vehicle_index);
	}
	break;
	case _vehicle_type_human_jeep:
	{
		engine = get_type_human_jeep()->get_engine(vehicle_index);
	}
	break;
	case _vehicle_type_chopper:
	{
		engine = get_type_chopper()->get_engine(vehicle_index);
	}
	break;
	}

	return engine;
}

c_vehicle_type_human_tank* c_vehicle_type_component::get_type_human_tank()
{
	//return INVOKE_CLASS_MEMBER(0x00B8A780, c_vehicle_type_component, get_type_human_tank);

	ASSERT(m_initialization_type == _vehicle_type_human_tank);
	return (c_vehicle_type_human_tank*)&m_vehicle_type_buffer;
}

c_vehicle_type_human_jeep* c_vehicle_type_component::get_type_human_jeep()
{
	//return INVOKE_CLASS_MEMBER(0x00B8A790, c_vehicle_type_component, get_type_human_jeep);

	ASSERT(m_initialization_type == _vehicle_type_human_jeep);
	return (c_vehicle_type_human_jeep*)&m_vehicle_type_buffer;
}

c_vehicle_type_human_plane* c_vehicle_type_component::get_type_human_plane()
{
	//return INVOKE_CLASS_MEMBER(0x00B8A7A0, c_vehicle_type_component, get_type_human_plane);

	ASSERT(m_initialization_type == _vehicle_type_human_plane);
	return (c_vehicle_type_human_plane*)&m_vehicle_type_buffer;
}

c_vehicle_type_alien_scout* c_vehicle_type_component::get_type_alien_scout()
{
	//return INVOKE_CLASS_MEMBER(0x00B8A7B0, c_vehicle_type_component, get_type_alien_scout);

	ASSERT(m_initialization_type == _vehicle_type_alien_scout);
	return (c_vehicle_type_alien_scout*)&m_vehicle_type_buffer;
}

c_vehicle_type_alien_fighter* c_vehicle_type_component::get_type_alien_fighter()
{
	//return INVOKE_CLASS_MEMBER(0x00B8A7C0, c_vehicle_type_component, get_type_alien_fighter);

	ASSERT(m_initialization_type == _vehicle_type_alien_fighter);
	return (c_vehicle_type_alien_fighter*)&m_vehicle_type_buffer;
}

c_vehicle_type_turret* c_vehicle_type_component::get_type_turret()
{
	//return INVOKE_CLASS_MEMBER(0x00B8A7D0, c_vehicle_type_component, get_type_turret);

	ASSERT(m_initialization_type == _vehicle_type_turret);
	return (c_vehicle_type_turret*)&m_vehicle_type_buffer;
}

c_vehicle_type_mantis* c_vehicle_type_component::get_type_mantis()
{
	//return INVOKE_CLASS_MEMBER(0x00B8A7E0, c_vehicle_type_component, get_type_mantis);

	ASSERT(m_initialization_type == _vehicle_type_mantis);
	return (c_vehicle_type_mantis*)&m_vehicle_type_buffer;
}

c_vehicle_type_guardian* c_vehicle_type_component::get_type_guardian()
{
	//return INVOKE_CLASS_MEMBER(0x00B8A7F0, c_vehicle_type_component, get_type_guardian);

	ASSERT(m_initialization_type == _vehicle_type_guardian);
	return (c_vehicle_type_guardian*)&m_vehicle_type_buffer;
}

c_vehicle_type_vtol* c_vehicle_type_component::get_type_vtol()
{
	//return INVOKE_CLASS_MEMBER(0x00B8A800, c_vehicle_type_component, get_type_vtol);

	ASSERT(m_initialization_type == _vehicle_type_vtol);
	return (c_vehicle_type_vtol*)&m_vehicle_type_buffer;
}

c_vehicle_type_chopper* c_vehicle_type_component::get_type_chopper()
{
	//return INVOKE_CLASS_MEMBER(0x00B8A810, c_vehicle_type_component, get_type_chopper);

	ASSERT(m_initialization_type == _vehicle_type_chopper);
	return (c_vehicle_type_chopper*)&m_vehicle_type_buffer;
}

void c_vehicle_type_component::handle_deleted_object(int32 vehicle_index, int32 object_index)
{
	//INVOKE_CLASS_MEMBER(0x00B8A820, c_vehicle_type_component, handle_deleted_object, vehicle_index, object_index);

	if (vehicle_get_type(vehicle_index) == _vehicle_type_turret)
	{
		get_type_turret()->handle_deleted_object(vehicle_index, object_index);
	}
}

void c_vehicle_type_component::handled_changed_vehicle_type(int32 vehicle_index)
{
	//INVOKE_CLASS_MEMBER(0x00B8A850, c_vehicle_type_component, handled_changed_vehicle_type, vehicle_index);

	vehicle_datum* vehicle = VEHICLE_GET(vehicle_index);
	if (vehicle_definition_get_default_type(vehicle->definition_index) != m_initialization_type)
	{
		c_vehicle_type_component::reset(vehicle_index);
	}
}

bool c_vehicle_type_component::init_auto_turret(int32 vehicle_index, int32 trigger_volume, real32 min_range, real32 alt_range, real32 alt_time, int32 object_index)
{
	//return INVOKE_CLASS_MEMBER(0x00B8A8A0, c_vehicle_type_component, init_auto_turret, vehicle_index, trigger_volume, min_range, alt_range, alt_time, object_index);

	bool result = false;
	if (c_vehicle_auto_turret* auto_turret = c_vehicle_type_component::get_auto_turret(vehicle_index))
	{
		result = auto_turret->init(vehicle_index, trigger_volume, min_range, alt_range, alt_time, object_index);
	}

	return result;
}

bool c_vehicle_type_component::is_e_braking(int32 vehicle_index)
{
	//return INVOKE_CLASS_MEMBER(0x00B8A910, c_vehicle_type_component, is_e_braking, vehicle_index);

	bool result = false;
	if (vehicle_get_type(vehicle_index) == _vehicle_type_human_jeep)
	{
		result = get_type_human_jeep()->is_e_braking(vehicle_index);
	}
	return result;
}

bool c_vehicle_type_component::is_running_trick(int32 vehicle_index)
{
	//return INVOKE_CLASS_MEMBER(0x00B8A940, c_vehicle_type_component, is_running_trick, vehicle_index);

	bool result = false;
	if (vehicle_get_type(vehicle_index) == _vehicle_type_alien_fighter)
	{
		result = get_type_alien_fighter()->is_running_trick(vehicle_index);
	}
	return result;
}

bool c_vehicle_type_component::is_stopped(int32 vehicle_index)
{
	//return INVOKE_CLASS_MEMBER(0x00B8A970, c_vehicle_type_component, is_stopped, vehicle_index);

	bool result = false;

	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
	{
		result = get_type_human_tank()->is_stopped(vehicle_index);
	}
	break;
	case _vehicle_type_human_jeep:
	{
		result = get_type_human_jeep()->is_stopped(vehicle_index);
	}
	break;
	case _vehicle_type_human_plane:
	{
		result = get_type_human_plane()->is_stopped(vehicle_index);
	}
	break;
	case _vehicle_type_alien_scout:
	{
		result = get_type_alien_scout()->is_stopped(vehicle_index);
	}
	break;
	case _vehicle_type_alien_fighter:
	{
		result = get_type_alien_fighter()->is_stopped(vehicle_index);
	}
	break;
	case _vehicle_type_turret:
	{
		result = get_type_turret()->is_stopped(vehicle_index);
	}
	break;
	case _vehicle_type_mantis:
	{
		result = get_type_mantis()->is_stopped(vehicle_index);
	}
	break;
	case _vehicle_type_vtol:
	{
		result = get_type_vtol()->is_stopped(vehicle_index);
	}
	break;
	case _vehicle_type_chopper:
	{
		result = get_type_chopper()->is_stopped(vehicle_index);
	}
	break;
	case _vehicle_type_guardian:
	{
		result = get_type_guardian()->is_stopped(vehicle_index);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}

	return result;
}

bool c_vehicle_type_component::kills_riders_at_terminal_velocity(int32 vehicle_index)
{
	//return INVOKE_CLASS_MEMBER(0x00B8AA50, c_vehicle_type_component, kills_riders_at_terminal_velocity, vehicle_index);

	bool result = false;

	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
	{
		result = get_type_human_tank()->kills_riders_at_terminal_velocity(vehicle_index);
	}
	break;
	case _vehicle_type_human_jeep:
	{
		result = get_type_human_jeep()->kills_riders_at_terminal_velocity(vehicle_index);
	}
	break;
	case _vehicle_type_human_plane:
	{
		result = get_type_human_plane()->kills_riders_at_terminal_velocity(vehicle_index);
	}
	break;
	case _vehicle_type_alien_scout:
	{
		result = get_type_alien_scout()->kills_riders_at_terminal_velocity(vehicle_index);
	}
	break;
	case _vehicle_type_alien_fighter:
	{
		result = get_type_alien_fighter()->kills_riders_at_terminal_velocity(vehicle_index);
	}
	break;
	case _vehicle_type_turret:
	{
		result = get_type_turret()->kills_riders_at_terminal_velocity(vehicle_index);
	}
	break;
	case _vehicle_type_mantis:
	{
		result = get_type_mantis()->kills_riders_at_terminal_velocity(vehicle_index);
	}
	break;
	case _vehicle_type_vtol:
	{
		result = get_type_vtol()->kills_riders_at_terminal_velocity(vehicle_index);
	}
	break;
	case _vehicle_type_chopper:
	{
		result = get_type_chopper()->kills_riders_at_terminal_velocity(vehicle_index);
	}
	break;
	case _vehicle_type_guardian:
	{
		result = get_type_guardian()->kills_riders_at_terminal_velocity(vehicle_index);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}

	return result;
}

bool c_vehicle_type_component::physics_disabled(int32 vehicle_index)
{
	//return INVOKE_CLASS_MEMBER(0x00B8AB20, c_vehicle_type_component, physics_disabled, vehicle_index);

	bool result = false;

	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
	{
		result = get_type_human_tank()->physics_disabled(vehicle_index);
	}
	break;
	case _vehicle_type_human_jeep:
	{
		result = get_type_human_jeep()->physics_disabled(vehicle_index);
	}
	break;
	case _vehicle_type_human_plane:
	{
		result = get_type_human_plane()->physics_disabled(vehicle_index);
	}
	break;
	case _vehicle_type_alien_scout:
	{
		result = get_type_alien_scout()->physics_disabled(vehicle_index);
	}
	break;
	case _vehicle_type_alien_fighter:
	{
		result = get_type_alien_fighter()->physics_disabled(vehicle_index);
	}
	break;
	case _vehicle_type_turret:
	{
		result = get_type_turret()->physics_disabled(vehicle_index);
	}
	break;
	case _vehicle_type_mantis:
	{
		result = get_type_mantis()->physics_disabled(vehicle_index);
	}
	break;
	case _vehicle_type_vtol:
	{
		result = get_type_vtol()->physics_disabled(vehicle_index);
	}
	break;
	case _vehicle_type_chopper:
	{
		result = get_type_chopper()->physics_disabled(vehicle_index);
	}
	break;
	case _vehicle_type_guardian:
	{
		result = get_type_guardian()->physics_disabled(vehicle_index);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}

	return result;
}

void c_vehicle_type_component::process_animation_channels(int32 vehicle_index, void(__cdecl* callback)(int32, const render_model_definition*, c_animation_channel*, real32, real32, real32, void*), void* user_data, c_animation_channel* channel, bool find_animations)
{
	//INVOKE_CLASS_MEMBER(0x00B8ABF0, c_vehicle_type_component, process_animation_channels, vehicle_index, callback, user_data, channel, find_animations);

	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
	{
		get_type_human_tank()->process_animation_channels(vehicle_index, callback, user_data, channel, find_animations);
	}
	break;
	case _vehicle_type_human_jeep:
	{
		get_type_human_jeep()->process_animation_channels(vehicle_index, callback, user_data, channel, find_animations);
	}
	break;
	case _vehicle_type_human_plane:
	{
		get_type_human_plane()->process_animation_channels(vehicle_index, callback, user_data, channel, find_animations);
	}
	break;
	case _vehicle_type_alien_scout:
	{
		get_type_alien_scout()->process_animation_channels(vehicle_index, callback, user_data, channel, find_animations);
	}
	break;
	case _vehicle_type_alien_fighter:
	{
		get_type_alien_fighter()->process_animation_channels(vehicle_index, callback, user_data, channel, find_animations);
	}
	break;
	case _vehicle_type_turret:
	{
		get_type_turret()->process_animation_channels(vehicle_index, callback, user_data, channel, find_animations);
	}
	break;
	case _vehicle_type_mantis:
	{
		get_type_mantis()->process_animation_channels(vehicle_index, callback, user_data, channel, find_animations);
	}
	break;
	case _vehicle_type_vtol:
	{
		get_type_vtol()->process_animation_channels(vehicle_index, callback, user_data, channel, find_animations);
	}
	break;
	case _vehicle_type_chopper:
	{
		get_type_chopper()->process_animation_channels(vehicle_index, callback, user_data, channel, find_animations);
	}
	break;
	case _vehicle_type_guardian:
	{
		get_type_guardian()->process_animation_channels(vehicle_index, callback, user_data, channel, find_animations);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}
}

void c_vehicle_type_component::reset(int32 vehicle_index)
{
	//INVOKE_CLASS_MEMBER(0x00B8AD30, c_vehicle_type_component, reset, vehicle_index);

	m_initialization_type = vehicle_get_type(vehicle_index);
	csmemset(m_vehicle_type_buffer, 0, sizeof(m_vehicle_type_buffer));

	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
	{
		get_type_human_tank()->reset(vehicle_index);
	}
	break;
	case _vehicle_type_human_jeep:
	{
		get_type_human_jeep()->reset(vehicle_index);
	}
	break;
	case _vehicle_type_human_plane:
	{
		get_type_human_plane()->reset(vehicle_index);
	}
	break;
	case _vehicle_type_alien_scout:
	{
		get_type_alien_scout()->reset(vehicle_index);
	}
	break;
	case _vehicle_type_alien_fighter:
	{
		get_type_alien_fighter()->reset(vehicle_index);
	}
	break;
	case _vehicle_type_turret:
	{
		get_type_turret()->reset(vehicle_index);
	}
	break;
	case _vehicle_type_mantis:
	{
		get_type_mantis()->reset(vehicle_index);
	}
	break;
	case _vehicle_type_vtol:
	{
		get_type_vtol()->reset(vehicle_index);
	}
	break;
	case _vehicle_type_chopper:
	{
		get_type_chopper()->reset(vehicle_index);
	}
	break;
	case _vehicle_type_guardian:
	{
		get_type_guardian()->reset(vehicle_index);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}
}

void c_vehicle_type_component::set_auto_turret_damage_owner(int32 vehicle_index, int32 damage_owner_index)
{
	//INVOKE_CLASS_MEMBER(0x00B8AE20, c_vehicle_type_component, set_auto_turret_damage_owner, vehicle_index, damage_owner_index);

	if (c_vehicle_auto_turret* auto_turret = get_auto_turret(vehicle_index))
	{
		auto_turret->set_damage_owner(damage_owner_index);
	}
}

bool c_vehicle_type_component::should_override_deactivation(int32 vehicle_index)
{
	//return INVOKE_CLASS_MEMBER(0x00B8AE60, c_vehicle_type_component, should_override_deactivation, vehicle_index);

	bool result = false;

	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
	{
		result = get_type_human_tank()->should_override_deactivation(vehicle_index);
	}
	break;
	case _vehicle_type_human_jeep:
	{
		result = get_type_human_jeep()->should_override_deactivation(vehicle_index);
	}
	break;
	case _vehicle_type_human_plane:
	{
		result = get_type_human_plane()->should_override_deactivation(vehicle_index);
	}
	break;
	case _vehicle_type_alien_scout:
	{
		result = get_type_alien_scout()->should_override_deactivation(vehicle_index);
	}
	break;
	case _vehicle_type_alien_fighter:
	{
		result = get_type_alien_fighter()->should_override_deactivation(vehicle_index);
	}
	break;
	case _vehicle_type_turret:
	{
		result = get_type_turret()->should_override_deactivation(vehicle_index);
	}
	break;
	case _vehicle_type_mantis:
	{
		result = get_type_mantis()->should_override_deactivation(vehicle_index);
	}
	break;
	case _vehicle_type_vtol:
	{
		result = get_type_vtol()->should_override_deactivation(vehicle_index);
	}
	break;
	case _vehicle_type_chopper:
	{
		result = get_type_chopper()->should_override_deactivation(vehicle_index);
	}
	break;
	case _vehicle_type_guardian:
	{
		result = get_type_guardian()->should_override_deactivation(vehicle_index);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}

	return result;
}

void c_vehicle_type_component::start_trick(int32 vehicle_index, e_vehicle_trick_type trick_type)
{
	//INVOKE_CLASS_MEMBER(0x00B8AF30, c_vehicle_type_component, start_trick, vehicle_index, trick_type);

	if (vehicle_get_type(vehicle_index) == _vehicle_type_alien_fighter)
	{
		get_type_alien_fighter()->start_trick(vehicle_index, trick_type);
	}
}

void c_vehicle_type_component::update_control(int32 vehicle_index)
{
	//return INVOKE_CLASS_MEMBER(0x00B8AF60, c_vehicle_type_component, update_control, vehicle_index);

	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
	{
		get_type_human_tank()->update_control(vehicle_index);
	}
	break;
	case _vehicle_type_human_jeep:
	{
		get_type_human_jeep()->update_control(vehicle_index);
	}
	break;
	case _vehicle_type_human_plane:
	{
		get_type_human_plane()->update_control(vehicle_index);
	}
	break;
	case _vehicle_type_alien_scout:
	{
		get_type_alien_scout()->update_control(vehicle_index);
	}
	break;
	case _vehicle_type_alien_fighter:
	{
		get_type_alien_fighter()->update_control(vehicle_index);
	}
	break;
	case _vehicle_type_turret:
	{
		get_type_turret()->update_control(vehicle_index);
	}
	break;
	case _vehicle_type_mantis:
	{
		get_type_mantis()->update_control(vehicle_index);
	}
	break;
	case _vehicle_type_vtol:
	{
		get_type_vtol()->update_control(vehicle_index);
	}
	break;
	case _vehicle_type_chopper:
	{
		get_type_chopper()->update_control(vehicle_index);
	}
	break;
	case _vehicle_type_guardian:
	{
		get_type_guardian()->update_control(vehicle_index);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}
}

void c_vehicle_type_component::update_physics(int32 vehicle_index, s_havok_vehicle_physics_instance* instance)
{
	//return INVOKE_CLASS_MEMBER(0x00B8B030, c_vehicle_type_component, update_physics, vehicle_index, instance);

	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
	{
		get_type_human_tank()->update_physics(vehicle_index, instance);
	}
	break;
	case _vehicle_type_human_jeep:
	{
		get_type_human_jeep()->update_physics(vehicle_index, instance);
	}
	break;
	case _vehicle_type_human_plane:
	{
		get_type_human_plane()->update_physics(vehicle_index, instance);
	}
	break;
	case _vehicle_type_alien_scout:
	{
		get_type_alien_scout()->update_physics(vehicle_index, instance);
	}
	break;
	case _vehicle_type_alien_fighter:
	{
		get_type_alien_fighter()->update_physics(vehicle_index, instance);
	}
	break;
	case _vehicle_type_turret:
	{
		get_type_turret()->update_physics(vehicle_index, instance);
	}
	break;
	case _vehicle_type_mantis:
	{
		get_type_mantis()->update_physics(vehicle_index, instance);
	}
	break;
	case _vehicle_type_vtol:
	{
		get_type_vtol()->update_physics(vehicle_index, instance);
	}
	break;
	case _vehicle_type_chopper:
	{
		get_type_chopper()->update_physics(vehicle_index, instance);
	}
	break;
	case _vehicle_type_guardian:
	{
		get_type_guardian()->update_physics(vehicle_index, instance);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}

	vehicle_try_to_join_local_physics(vehicle_index, instance);
}

bool c_vehicle_type_component::vector_is_upsides_down(int32 vehicle_index, const real_vector3d* vector)
{
	//return INVOKE_CLASS_MEMBER(0x00B8B1A0, c_vehicle_type_component, vector_is_upsides_down, vehicle_index, vector);

	bool result = false;

	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
	{
		result = get_type_human_tank()->vector_is_upsides_down(vehicle_index, vector);
	}
	break;
	case _vehicle_type_human_jeep:
	{
		result = get_type_human_jeep()->vector_is_upsides_down(vehicle_index, vector);
	}
	break;
	case _vehicle_type_human_plane:
	{
		result = get_type_human_plane()->vector_is_upsides_down(vehicle_index, vector);
	}
	break;
	case _vehicle_type_alien_scout:
	{
		result = get_type_alien_scout()->vector_is_upsides_down(vehicle_index, vector);
	}
	break;
	case _vehicle_type_alien_fighter:
	{
		result = get_type_alien_fighter()->vector_is_upsides_down(vehicle_index, vector);
	}
	break;
	case _vehicle_type_turret:
	{
		result = get_type_turret()->vector_is_upsides_down(vehicle_index, vector);
	}
	break;
	case _vehicle_type_mantis:
	{
		result = get_type_mantis()->vector_is_upsides_down(vehicle_index, vector);
	}
	break;
	case _vehicle_type_vtol:
	{
		result = get_type_vtol()->vector_is_upsides_down(vehicle_index, vector);
	}
	break;
	case _vehicle_type_chopper:
	{
		result = get_type_chopper()->vector_is_upsides_down(vehicle_index, vector);
	}
	break;
	case _vehicle_type_guardian:
	{
		result = get_type_guardian()->vector_is_upsides_down(vehicle_index, vector);
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}

	return result;
}

