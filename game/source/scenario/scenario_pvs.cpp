#include "scenario/scenario_pvs.hpp"

#include "scenario/scenario.hpp"

//.text:005F0E40 ; const uns8* __cdecl scenario_pvs_audio_cluster_neighbors_get(s_cluster_reference, int32*)
//.text:005F0F00 ; void __cdecl scenario_pvs_calculate_visible_seams(int32, int32, s_cluster_reference, bool, c_static_flags<64>*)

void __cdecl scenario_zone_set_pvs_get_row(int32 scenario_definition_index, s_scenario_pvs_row* row, int32 zone_set_index, s_cluster_reference cluster_reference, bool doors_closed)
{
	INVOKE(0x005F1050, scenario_zone_set_pvs_get_row, scenario_definition_index, row, zone_set_index, cluster_reference, doors_closed);
}

void __cdecl scenario_zone_set_pvs_row_fill(int32 scenario_definition_index, s_scenario_pvs_row* row, bool value)
{
	INVOKE(0x005F1190, scenario_zone_set_pvs_row_fill, scenario_definition_index, row, value);
}

void __cdecl scenario_zone_set_pvs_row_set(int32 scenario_definition_index, s_scenario_pvs_row* row, s_cluster_reference cluster_reference, bool value)
{
	INVOKE(0x005F11E0, scenario_zone_set_pvs_row_set, scenario_definition_index, row, cluster_reference, value);
}

bool __cdecl scenario_zone_set_pvs_row_test(int32 scenario_definition_index, const s_scenario_pvs_row* row, s_cluster_reference cluster_reference)
{
	return INVOKE(0x005F1240, scenario_zone_set_pvs_row_test, scenario_definition_index, row, cluster_reference);
}

void __cdecl scenario_zone_set_pvs_write_row(s_game_cluster_bit_vectors* pvs, const s_scenario_pvs_row* row)
{
	INVOKE(0x005F1280, scenario_zone_set_pvs_write_row, pvs, row);
}

c_pvs_cluster_iterator::c_pvs_cluster_iterator(int32 scenario_index, int32 zone_set_index, const s_cluster_reference& cluster_reference) :
	m_pvs(),
	m_current_bsp_index(),
	m_current_cluster_index()
{
	//DECLFUNC(0x00604F50, void, __thiscall, c_pvs_cluster_iterator*, int32, int32, const s_cluster_reference&)(this,
	//	scenario_index, zone_set_index, cluster_reference);

	scenario_zone_set_pvs_get_row(scenario_index, &m_pvs, zone_set_index, cluster_reference, false);
}

uns32 c_pvs_cluster_iterator::get_bsp_mask() const
{
	return INVOKE_CLASS_MEMBER(0x006059E0, c_pvs_cluster_iterator, get_bsp_mask);

	//uns32 bsp_mask = 0;
	//for (int32 bit_vector_index = 0; bit_vector_index < m_pvs.bit_vector_counts.get_count(); bit_vector_index++)
	//{
	//	if (m_pvs.bit_vector_counts[bit_vector_index] > 0)
	//	{
	//		bsp_mask |= FLAG(bit_vector_index);
	//	}
	//}
	//return bsp_mask;
}

s_cluster_reference c_pvs_cluster_iterator::get_cluster_reference() const
{
	return INVOKE_CLASS_MEMBER(0x00605A20, c_pvs_cluster_iterator, get_cluster_reference);

	//s_cluster_reference cluster_reference{};
	//cluster_reference_set(&cluster_reference, m_current_bsp_index, m_current_cluster_index);
	//return cluster_reference;
}

bool c_pvs_cluster_iterator::go_to_next_cluster()
{
	return INVOKE_CLASS_MEMBER(0x00605B10, c_pvs_cluster_iterator, go_to_next_cluster);
}

bool c_pvs_cluster_iterator::next()
{
	return INVOKE_CLASS_MEMBER(0x00605E50, c_pvs_cluster_iterator, next);

	//while (go_to_next_cluster() && !visible_cluster_reference());
	//return visible_cluster_reference();
}

bool c_pvs_cluster_iterator::valid_cluster_reference() const
{
	return INVOKE_CLASS_MEMBER(0x006061B0, c_pvs_cluster_iterator, valid_cluster_reference);
}

bool c_pvs_cluster_iterator::visible_cluster_reference() const
{
	return INVOKE_CLASS_MEMBER(0x006061D0, c_pvs_cluster_iterator, visible_cluster_reference);
}

