#include "scenario/scenario_pvs.hpp"

//.text:005F0E40 ; 
//.text:005F0F00 ; void __cdecl scenario_pvs_calculate_visible_seams(long, long, s_cluster_reference, bool, c_static_flags<64>*)

void __cdecl scenario_zone_set_pvs_get_row(long scenario_index, s_scenario_pvs_row* pvs_row, long zone_set_index, s_cluster_reference cluster_reference, bool a5)
{
	INVOKE(0x005F1050, scenario_zone_set_pvs_get_row, scenario_index, pvs_row, zone_set_index, cluster_reference, a5);
}

//.text:005F1190 ; 
//.text:005F11E0 ; 

bool __cdecl scenario_zone_set_pvs_row_test(long scenario_index, s_scenario_pvs_row const* pvs_row, s_cluster_reference cluster_reference)
{
	return INVOKE(0x005F1240, scenario_zone_set_pvs_row_test, scenario_index, pvs_row, cluster_reference);
}

void __cdecl scenario_zone_set_pvs_write_row(s_game_cluster_bit_vectors* v0, s_scenario_pvs_row const* v1)
{
	INVOKE(0x005F1280, scenario_zone_set_pvs_write_row, v0, v1);
}

