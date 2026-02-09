#include "hs/hs_unit_seats.hpp"

#include "cache/cache_files.hpp"
#include "hs/hs_scenario_definitions.hpp"
#include "units/unit_definition.hpp"

bool hs_get_unit_seats_from_substring(int32 unit_definition_index, const char* seat_substring, c_static_flags<k_maximum_hs_unit_seats>* out_unit_seats)
{
	ASSERT(seat_substring);
	ASSERT(out_unit_seats);
	ASSERT(unit_definition_index != NONE);

	out_unit_seats->clear();

	s_unit_definition* unit_definition = TAG_GET(UNIT_TAG, s_unit_definition, unit_definition_index);

	for (int32 seat_index = 0; seat_index < unit_definition->unit.seats_block.count; seat_index++)
	{
		unit_seat& seat = unit_definition->unit.seats_block[seat_index];
		if (csstrstr(seat.label.get_string(), seat_substring) != 0)
		{
			out_unit_seats->set(seat_index, true);
			return true;
		}
	}

	return false;
}

bool hs_unit_seat_mappings_match(const s_hs_unit_seat_mapping& a, const s_hs_unit_seat_mapping& b)
{
	return csmemcmp(&a, &b, sizeof(s_hs_unit_seat_mapping)) == 0;
}

int32 hs_encode_unit_seat_mapping(int32 unit_seat_start_index, int32 unit_seat_mapping_count)
{
	ASSERT(VALID_INDEX(unit_seat_start_index, k_maximum_hs_unit_seat_mappings));
	ASSERT(IN_RANGE_INCLUSIVE(unit_seat_mapping_count, 0, k_maximum_hs_unit_seat_mappings));
	ASSERT(unit_seat_start_index + unit_seat_mapping_count <= k_maximum_hs_unit_seat_mappings);

	return (unit_seat_mapping_count << 16) | unit_seat_start_index;
}

