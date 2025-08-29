#pragma once

#include "cseries/cseries.hpp"
#include "interface/chud/chud_draw.hpp"

struct s_player_navpoint_data
{
	bool last_living_location_valid;
	real_point3d last_living_location;
	uns16 total_time_to_respawn_in_ticks;
	uns16 current_time_to_respawn_in_ticks;

	//int32 dead_unit_index;
	byte __data14[0x4];

	int8 current_navpoint_action;
	int8 current_navpoint_action_timer;
	int8 next_navpoint_action;
};
static_assert(sizeof(s_player_navpoint_data) == 0x1C);

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

public:
	static c_chud_navpoint_manager* __cdecl get(int32 user_index);

private:
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

extern s_player_navpoint_data* __cdecl chud_get_campaign_navpoint_data(int32 absolute_iter_index);
extern real32 __cdecl chud_get_string_width(int32 font_id, wchar_t* s);
extern void __cdecl player_navpoint_data_update(s_player_navpoint_data* player_navpoint_data);

