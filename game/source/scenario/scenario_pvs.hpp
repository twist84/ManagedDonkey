#pragma once

#include "cseries/cseries.hpp"

struct s_scenario_pvs_row
{
	uint32 bit_vector[16];
	int32 bit_vector_counts[16];
};
static_assert(sizeof(s_scenario_pvs_row) == 0x80);

struct s_game_cluster_bit_vectors;
extern void __cdecl scenario_zone_set_pvs_get_row(int32 scenario_index, s_scenario_pvs_row* pvs_row, int32 zone_set_index, s_cluster_reference cluster_reference, bool a5);
extern bool __cdecl scenario_zone_set_pvs_row_test(int32 scenario_index, s_scenario_pvs_row const* pvs_row, s_cluster_reference cluster_reference);
extern void __cdecl scenario_zone_set_pvs_write_row(s_game_cluster_bit_vectors* v0, s_scenario_pvs_row const* v1);

