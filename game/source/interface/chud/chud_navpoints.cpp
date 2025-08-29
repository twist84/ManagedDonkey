#include "interface/chud/chud_navpoints.hpp"

#include "interface/chud/chud.hpp"

s_player_navpoint_data* __cdecl chud_get_campaign_navpoint_data(int32 absolute_iter_index)
{
	return INVOKE(0x00A89670, chud_get_campaign_navpoint_data, absolute_iter_index);

	//ASSERT(absolute_iter_index >= 0 && absolute_iter_index < c_chud_persistent_global_data::k_max_campaign_players);
	//return &c_chud_manager::x_persistent_global_data->m_campaign_player_navpoint_data[absolute_iter_index];
}

real32 __cdecl chud_get_string_width(int32 font_id, wchar_t* s)
{
	return INVOKE(0x00AAF0A0, chud_get_string_width, font_id, s);
}

void __cdecl player_navpoint_data_update(s_player_navpoint_data* player_navpoint_data)
{
	INVOKE(0x00AAF650, player_navpoint_data_update, player_navpoint_data);
}

