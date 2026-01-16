#pragma once

#include "cseries/cseries.hpp"
#include "cseries/location.hpp"

struct s_scenario_pvs_row
{
	c_static_array<uns32*, 16> bit_vectors;
	c_static_array<int32, 16> bit_vector_counts;
};
COMPILE_ASSERT(sizeof(s_scenario_pvs_row) == 0x80);

class c_pvs_cluster_iterator
{
public:
	c_pvs_cluster_iterator(int32 scenario_index, int32 zone_set_index, const s_cluster_reference& cluster_reference);
	bool next();
	s_cluster_reference get_cluster_reference() const;
	uns32 get_bsp_mask() const;

private:
	bool go_to_next_cluster();
	bool valid_cluster_reference() const;
	bool visible_cluster_reference() const;

private:
	s_scenario_pvs_row m_pvs;
	int32 m_current_bsp_index;
	int32 m_current_cluster_index;
};
COMPILE_ASSERT(sizeof(c_pvs_cluster_iterator) == 0x88);

struct s_game_cluster_bit_vectors;
extern void __cdecl scenario_zone_set_pvs_get_row(int32 scenario_definition_index, s_scenario_pvs_row* row, int32 zone_set_index, s_cluster_reference cluster_reference, bool doors_closed);
extern void __cdecl scenario_zone_set_pvs_row_fill(int32 scenario_definition_index, s_scenario_pvs_row* row, bool value);
extern void __cdecl scenario_zone_set_pvs_row_set(int32 scenario_definition_index, s_scenario_pvs_row* row, s_cluster_reference cluster_reference, bool value);
extern bool __cdecl scenario_zone_set_pvs_row_test(int32 scenario_definition_index, const s_scenario_pvs_row* row, s_cluster_reference cluster_reference);
extern void __cdecl scenario_zone_set_pvs_write_row(s_game_cluster_bit_vectors* pvs, const s_scenario_pvs_row* row);

