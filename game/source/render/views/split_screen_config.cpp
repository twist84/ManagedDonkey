#include "render/views/split_screen_config.hpp"

s_splitscreen_view_config(&c_splitscreen_config::m_config_table)[4][4] = *reinterpret_cast<s_splitscreen_view_config(*)[4][4]>(0x01917DA8);

//c_splitscreen_config::m_config_table[0][0] = { { 0.0f, 0.0f, 1.0f, 1.0f }, 0 };
//c_splitscreen_config::m_config_table[0][1] = { { 0.0f, 0.0f, 0.0f, 0.0f }, 0 };
//c_splitscreen_config::m_config_table[0][2] = { { 0.0f, 0.0f, 0.0f, 0.0f }, 0 };
//c_splitscreen_config::m_config_table[0][3] = { { 0.0f, 0.0f, 0.0f, 0.0f }, 0 };
//c_splitscreen_config::m_config_table[1][0] = { { 0.0f, 0.0f, 1.0f, 0.5f }, 1 };
//c_splitscreen_config::m_config_table[1][1] = { { 0.0f, 0.5f, 1.0f, 1.0f }, 1 };
//c_splitscreen_config::m_config_table[1][2] = { { 0.0f, 0.0f, 0.0f, 0.0f }, 0 };
//c_splitscreen_config::m_config_table[1][3] = { { 0.0f, 0.0f, 0.0f, 0.0f }, 0 };
//c_splitscreen_config::m_config_table[2][0] = { { 0.0f, 0.0f, 1.0f, 0.5f }, 1 };
//c_splitscreen_config::m_config_table[2][1] = { { 0.0f, 0.5f, 0.5f, 1.0f }, 3 };
//c_splitscreen_config::m_config_table[2][2] = { { 0.5f, 0.5f, 1.0f, 1.0f }, 3 };
//c_splitscreen_config::m_config_table[2][3] = { { 0.0f, 0.0f, 0.0f, 0.0f }, 0 };
//c_splitscreen_config::m_config_table[3][0] = { { 0.0f, 0.0f, 0.5f, 0.5f }, 3 };
//c_splitscreen_config::m_config_table[3][1] = { { 0.5f, 0.0f, 1.0f, 0.5f }, 3 };
//c_splitscreen_config::m_config_table[3][2] = { { 0.0f, 0.5f, 0.5f, 1.0f }, 3 };
//c_splitscreen_config::m_config_table[3][3] = { { 0.5f, 0.5f, 1.0f, 1.0f }, 3 };

const s_splitscreen_view_config* c_splitscreen_config::get_view_bounds(int32 player_index, int32 player_count, bool is_widescreen)
{
	return INVOKE(0x00604270, get_view_bounds, player_index, player_count, is_widescreen);
}
