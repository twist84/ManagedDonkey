#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_sandbox_traits.hpp"
#include "game/game_engine_player_traits.hpp"
#include "saved_games/scenario_map_variant.hpp"

struct c_bitstream;

struct c_game_engine_sandbox_variant :
	public c_game_engine_base_variant
{
public:
	c_game_engine_sandbox_variant* constructor()
	{
		return DECLFUNC(0x00572C00, c_game_engine_sandbox_variant*, __thiscall, c_game_engine_sandbox_variant*)(this);
	}

	void byteswap();

	void set(c_game_engine_sandbox_variant const* variant, bool force);
	//void set(s_game_engine_sandbox_variant_definition const* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	bool get_open_channel_voice() const;
	void set_open_channel_voice(bool open_channel_voice);

	e_sandbox_edit_mode_settings get_edit_mode() const;
	void set_edit_mode(e_sandbox_edit_mode_settings edit_mode);

	short get_respawn_time() const;
	void set_respawn_time(short respawn_time);

	c_player_traits* get_player_traits_writeable();
	c_player_traits const* get_player_traits() const;
	void set_player_traits(c_player_traits const* traits, bool force);

protected:
	c_flags<e_sandbox_variant_flags, byte_flags, k_sandbox_variant_flags> m_variant_flags;
	c_enum<e_sandbox_edit_mode_settings, char, _sandbox_edit_mode_settings_all_players, k_sandbox_edit_mode_settings> m_edit_mode;
	c_enum<e_sandbox_respawn_time, short, _sandbox_respawn_time_instant, k_sandbox_respawn_times> m_respawn_time;
	c_player_traits m_player_traits;
};
static_assert(sizeof(c_game_engine_sandbox_variant) == 0x1F0);

struct c_sandbox_engine :
	c_game_engine
{
};

struct s_sandbox_globals
{
	dword __unknown0;
	byte maximum_count;
	byte minimum_count;
	bool placed_on_map;
	word player_mode_flags;
	c_static_array<long, 16> held_objects;
	c_static_array<real, 16> held_object_distances;
	c_static_array<vector3d, 16> crosshair_points;
	c_static_array<vector3d, 16> crosshair_intersect_normals;
	c_static_array<vector3d, 16> crosshair_redirections;
	c_static_array<long, 16> crosshair_objects;
	c_static_array<byte, 16> __unknown30C;

	c_static_array<long, 64> tracked_objects;
	c_static_array<real_point3d, 64> tracked_object_positions;
	long tracked_object_count;

	byte __unknown720[8];
	bool forge_legal_notice;
	dword __unknown72C;
	s_variant_multiplayer_object_properties_definition object_properties;
};
static_assert(sizeof(s_sandbox_globals) == 0x748);

extern c_game_engine_sandbox_variant*& sandbox_variant;

extern bool __cdecl game_engine_object_is_fixed(long object_index);

