#pragma once

#include "cseries/cseries.hpp"
#include "saved_games/scenario_map_variant.hpp"

struct s_player_taunt_request_event_data
{
	short absolute_player_index;
	short __unknown2;
};
static_assert(sizeof(s_player_taunt_request_event_data) == 0x4);

struct s_player_respawn_target_request_event_data
{
	short player_index;
	short absolute_player_index;
};
static_assert(sizeof(s_player_respawn_target_request_event_data) == 0x4);

struct s_player_force_base_respawn_event_data
{
	short absolute_player_index;
};
static_assert(sizeof(s_player_force_base_respawn_event_data) == 0x2);

struct s_player_editor_request_event_data
{
	long editor_request_type;
	short editor_player_index;
	union
	{
		long edited_object_id;
		long object_definition_index;
	};
	real_point3d player_placement_point;
	vector3d player_placement_axes_forward;
	vector3d player_placement_axes_up;
	real_point3d placement_point;
	vector3d placement_axes_forward;
	vector3d placement_axes_up;
	real throw_force;
	long changed_gametype;
	byte minimum_count;
	byte maximum_count;
	bool editor_desired_mode;
	s_variant_multiplayer_object_properties_definition placement_properties;
};
static_assert(sizeof(s_player_editor_request_event_data) == 0x78);

