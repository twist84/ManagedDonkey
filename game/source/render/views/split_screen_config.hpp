#pragma once

#include "cseries/cseries.hpp"

struct s_splitscreen_view_config
{
	real32 m_left;
	real32 m_top;
	real32 m_right;
	real32 m_bottom;
	int32 m_render_target_resolution;
};
COMPILE_ASSERT(sizeof(s_splitscreen_view_config) == 0x14);

class c_splitscreen_config
{
public:
	static const s_splitscreen_view_config* get_view_bounds(int32 player_index, int32 player_count, bool is_widescreen);

private:

	static s_splitscreen_view_config(&m_config_table)[4][4];
};

