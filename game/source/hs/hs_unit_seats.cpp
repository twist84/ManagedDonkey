#include "hs/hs_unit_seats.hpp"

#include "cache/cache_files.hpp"
#include "hs/hs_scenario_definitions.hpp"
#include "units/unit_definition.hpp"

bool hs_get_unit_seats_from_substring(long unit_definition_index, char const* seat_substring, c_static_flags<64>* out_unit_seats)
{
	ASSERT(seat_substring);
	ASSERT(out_unit_seats);
	ASSERT(unit_definition_index != NONE);

	out_unit_seats->clear();

	struct unit_definition* unit_definition = (struct unit_definition*)tag_get(UNIT_TAG, unit_definition_index);

	for (long seat_index = 0; seat_index < unit_definition->unit.seats_block.count(); seat_index++)
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

bool hs_unit_seat_mappings_match(s_hs_unit_seat_mapping const& a, s_hs_unit_seat_mapping const& b)
{
	return csmemcmp(&a, &b, sizeof(s_hs_unit_seat_mapping)) == 0;
}

long hs_encode_unit_seat_mapping(long unit_seat_start_index, long unit_seat_mapping_count)
{
	ASSERT(VALID_INDEX(unit_seat_start_index, k_maximum_hs_unit_seat_mappings));
	ASSERT(IN_RANGE_INCLUSIVE(unit_seat_mapping_count, 0, k_maximum_hs_unit_seat_mappings));
	ASSERT(unit_seat_start_index + unit_seat_mapping_count <= k_maximum_hs_unit_seat_mappings);

	// Xenon
	//return unit_seat_start_index | (unit_seat_mapping_count << 16);

	// PC
	return (unit_seat_mapping_count << 16) | unit_seat_start_index;
}

