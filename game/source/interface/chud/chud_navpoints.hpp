#pragma once

#include "cseries/cseries.hpp"
#include "interface/chud/chud_draw.hpp"

struct s_chud_navpoint
{
	int32 navpoint_identifier;
	int32 arrow_type;
	int32 icon_type;
	int32 color_type;
	int32 flags;
	real32 alpha;
	wchar_t navpoint_text[6];
	real_point3d position_worldspace;
};
static_assert(sizeof(s_chud_navpoint) == 0x30);

class __declspec(align(4)) c_chud_navpoint_manager
{
public:
	struct s_managed_navpoint
	{
		s_chud_navpoint navpoint;
		bool valid;
		int32 last_valid_tick;
		real32 scale;
		c_chud_point_tracker point_tracker;
		int32 flash_type;
		int32 flash_timer;
		real32 fade_alpha;
	};
	static_assert(sizeof(s_managed_navpoint) == 0x74);

	s_managed_navpoint m_managed_navpoints[20];
	int32 m_named_player_index;
	wchar_t m_named_player_name[17];
	real32 m_name_theta;
	real32 m_named_player_name_width;
	int32 m_named_player_managed_navpoint_index;
	real32 m_distanced_theta;
	wchar_t m_distanced_text[10];
	real32 m_distanced_text_width;
	real32 m_distanced_managed_navpoint_index;
	int16 m_scripted_navpoints[8];
	real32 m_scripted_navpoint_vertical_offsets[8];
	int32 m_scripted_navpoint_count;
};
static_assert(sizeof(c_chud_navpoint_manager) == 0x998);

extern real32 __cdecl chud_get_string_width(int32 font_id, wchar_t* s);

