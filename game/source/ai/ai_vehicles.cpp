#include "ai/ai_vehicles.hpp"

#include "cseries/cseries.hpp"

long __cdecl ai_vehicle_get_reservation(long vehicle_index, long seat_index, long team)
{
	return INVOKE(0x01454C00, ai_vehicle_get_reservation, vehicle_index, seat_index, team);
}

