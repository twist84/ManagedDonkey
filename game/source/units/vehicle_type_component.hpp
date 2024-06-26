#pragma once

#include "cseries/cseries.hpp"
#include "units/vehicle_definitions.hpp"
#include "units/vehicle_engine.hpp"
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

struct c_vehicle_type_component
{
public:
	s_vehicle_engine* const get_engine(long vehicle_index);
	c_vehicle_type_human_tank* get_type_human_tank();
	c_vehicle_type_human_jeep* get_type_human_jeep();
	c_vehicle_type_human_plane* get_type_human_plane();
	c_vehicle_type_alien_scout* get_type_alien_scout();
	c_vehicle_type_alien_fighter* get_type_alien_fighter();
	c_vehicle_type_turret* get_type_turret();
	c_vehicle_type_mantis* get_type_mantis();
	c_vehicle_type_vtol* get_type_vtol();
	c_vehicle_type_chopper* get_type_chopper();
	c_vehicle_type_guardian* get_type_guardian();

protected:
	union
	{
		c_vehicle_type_human_tank m_type_human_tank;
		c_vehicle_type_human_jeep m_type_human_jeep;
		c_vehicle_type_human_plane m_type_human_plane;
		c_vehicle_type_alien_scout m_type_alien_scout;
		c_vehicle_type_alien_fighter m_type_alien_fighter;
		c_vehicle_type_turret m_type_turret;
		c_vehicle_type_mantis m_type_mantis;
		c_vehicle_type_vtol m_type_vtol;
		c_vehicle_type_chopper m_type_chopper;
		c_vehicle_type_guardian m_type_guardian;

		byte m_type_storage[0x60];
	};

	c_enum<e_vehicle_type, char, _vehicle_type_human_tank, k_vehicle_type_count> m_initialization_type;

	byte __data61[0x43];
};
static_assert(sizeof(c_vehicle_type_component) == 0xA4);

