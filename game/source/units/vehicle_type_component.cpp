#include "units/vehicle_type_component.hpp"

#include "units/vehicles.hpp"

s_vehicle_engine* const c_vehicle_type_component::get_engine(long vehicle_index)
{
	switch (vehicle_get_type(vehicle_index))
	{
	case _vehicle_type_human_tank:
	case _vehicle_type_human_jeep:
	case _vehicle_type_chopper:
		return this;
	}

	return nullptr;
}
