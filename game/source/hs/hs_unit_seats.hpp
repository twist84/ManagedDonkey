#pragma once

#include "cseries/cseries.hpp"
#include "hs/hs_scenario_definitions.hpp"

struct s_hs_unit_seat_mapping;
bool hs_get_unit_seats_from_substring(int32 unit_definition_index, const char* seat_substring, c_static_flags<k_maximum_hs_unit_seats>* out_unit_seats);
bool hs_unit_seat_mappings_match(const s_hs_unit_seat_mapping& a, const s_hs_unit_seat_mapping& b);
int32 hs_encode_unit_seat_mapping(int32 unit_seat_start_index, int32 unit_seat_mapping_count);

