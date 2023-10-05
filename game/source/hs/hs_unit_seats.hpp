#pragma once

#include "cseries/cseries.hpp"

long const k_maximum_hs_unit_seat_mappings = 0x10000;

struct s_hs_unit_seat_mapping;
bool hs_get_unit_seats_from_substring(long unit_definition_index, char const* seat_substring, c_static_flags<64>* out_unit_seats);
bool hs_unit_seat_mappings_match(s_hs_unit_seat_mapping const& a, s_hs_unit_seat_mapping const& b);
long hs_encode_unit_seat_mapping(long unit_seat_start_index, long unit_seat_mapping_count);

