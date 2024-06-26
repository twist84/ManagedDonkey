#include "units/vehicle_type_component.hpp"

#include "units/vehicles.hpp"

s_vehicle_engine* const c_vehicle_type_component::get_engine(long vehicle_index)
{
	s_vehicle_engine* engine = NULL;
	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
		engine = get_type_human_tank()->get_engine(vehicle_index);
		break;
	case _vehicle_type_human_jeep:
		engine = get_type_human_jeep()->get_engine(vehicle_index);
		break;
	case _vehicle_type_chopper:
		engine = get_type_chopper()->get_engine(vehicle_index);
		break;
	}

	return engine;
}

c_vehicle_type_human_tank* c_vehicle_type_component::get_type_human_tank()
{
	ASSERT(m_initialization_type == _vehicle_type_human_tank);
	return &m_type_human_tank;
}

c_vehicle_type_human_jeep* c_vehicle_type_component::get_type_human_jeep()
{
	ASSERT(m_initialization_type == _vehicle_type_human_jeep);
	return &m_type_human_jeep;
}

c_vehicle_type_human_plane* c_vehicle_type_component::get_type_human_plane()
{
	ASSERT(m_initialization_type == _vehicle_type_human_plane);
	return &m_type_human_plane;
}

c_vehicle_type_alien_scout* c_vehicle_type_component::get_type_alien_scout()
{
	ASSERT(m_initialization_type == _vehicle_type_alien_scout);
	return &m_type_alien_scout;
}

c_vehicle_type_alien_fighter* c_vehicle_type_component::get_type_alien_fighter()
{
	ASSERT(m_initialization_type == _vehicle_type_alien_fighter);
	return &m_type_alien_fighter;
}

c_vehicle_type_turret* c_vehicle_type_component::get_type_turret()
{
	ASSERT(m_initialization_type == _vehicle_type_turret);
	return &m_type_turret;
}

c_vehicle_type_mantis* c_vehicle_type_component::get_type_mantis()
{
	ASSERT(m_initialization_type == _vehicle_type_mantis);
	return &m_type_mantis;
}

c_vehicle_type_vtol* c_vehicle_type_component::get_type_vtol()
{
	ASSERT(m_initialization_type == _vehicle_type_vtol);
	return &m_type_vtol;
}

c_vehicle_type_chopper* c_vehicle_type_component::get_type_chopper()
{
	ASSERT(m_initialization_type == _vehicle_type_chopper);
	return &m_type_chopper;
}

c_vehicle_type_guardian* c_vehicle_type_component::get_type_guardian()
{
	ASSERT(m_initialization_type == _vehicle_type_guardian);
	return &m_type_guardian;
}


