#include "units/vehicle_engine.hpp"

real __cdecl vehicle_engine_get_rpm_function_scale(s_vehicle_engine const* engine)
{
	return INVOKE(0x00BEC8E0, vehicle_engine_get_rpm_function_scale, engine);
}

