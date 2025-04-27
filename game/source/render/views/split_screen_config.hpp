#pragma once

#include "cseries/cseries.hpp"

struct s_splitscreen_view_config
{
	real32 m_left;
	real32 m_top;
	real32 m_right;
	real32 m_bottom;
	long m_render_target_resolution;
};
static_assert(sizeof(s_splitscreen_view_config) == 0x14);

struct c_splitscreen_config
{
public:
	static s_splitscreen_view_config const* get_view_bounds(long player_index, long player_count, bool is_widescreen);

private:

	static s_splitscreen_view_config(&m_config_table)[4][4];
};

