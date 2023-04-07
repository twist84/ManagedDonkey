#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_sandbox_traits.hpp"
#include "game/game_engine_variant.hpp"
#include "saved_games/scenario_map_variant.hpp"

struct c_game_engine_sandbox_variant : c_game_engine_base_variant
{
public:
	c_game_engine_sandbox_variant* constructor()
	{
		return DECLFUNC(0x00572C00, c_game_engine_sandbox_variant*, __thiscall, c_game_engine_sandbox_variant*)(this);
	}

	void byteswap();

protected:
	c_flags<e_sandbox_variant_flags, byte_flags, k_sandbox_variant_flags> m_variant_flags;
	c_enum<e_sandbox_editing_mode, char, _sandbox_flags_all_players, k_sandbox_editing_mode> m_edit_mode;
	c_enum<e_sandbox_respawn_time, short, _sandbox_respawn_time_instant, k_sandbox_respawn_times> m_respawn_time;
	c_player_traits m_player_traits;
};
static_assert(sizeof(c_game_engine_sandbox_variant) == 0x1F0);

struct c_sandbox_engine : c_game_engine
{
};

struct s_sandbox_globals
{
	dword __unknown0;
	byte maximum_count;
	byte minimum_count;
	bool placed_on_map;
	word player_mode_flags;
	dword held_objects[16];
	real held_object_distances[16];
	vector3d crosshair_points[16];
	vector3d crosshair_intersect_normals[16];
	vector3d crosshair_redirections[16];
	dword crosshair_objects[16];
	byte __unknown30C[16];
	dword __unknown31C[64];
	byte __data41C[768];
	dword __unknown71C;
	byte __unknown720[8];
	bool forge_legal_notice;
	dword __unknown72C;
	s_variant_multiplayer_object_properties_definition object_properties;
};
static_assert(sizeof(s_sandbox_globals) == 0x748);

extern c_game_engine_sandbox_variant*& sandbox_variant;

