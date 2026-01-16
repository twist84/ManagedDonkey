#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_area_set.hpp"
#include "game/game_engine_sandbox_traits.hpp"
#include "game/game_engine_player_traits.hpp"
#include "saved_games/scenario_map_variant.hpp"

class c_bitstream;

class c_game_engine_sandbox_variant :
	public c_game_engine_base_variant
{
public:
	c_game_engine_sandbox_variant* constructor()
	{
		return INVOKE_CLASS_MEMBER(0x00572C00, c_game_engine_sandbox_variant, constructor);
	}

	void byteswap();

	void set(const c_game_engine_sandbox_variant* variant, bool force);
	//void set(const s_game_engine_sandbox_variant_definition* definition, bool force);

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	bool get_open_channel_voice() const;
	void set_open_channel_voice(bool open_channel_voice);

	e_sandbox_edit_mode_settings get_edit_mode() const;
	void set_edit_mode(e_sandbox_edit_mode_settings edit_mode);

	int16 get_respawn_time() const;
	void set_respawn_time(int16 respawn_time);

	c_player_traits* get_player_traits_writeable();
	const c_player_traits* get_player_traits() const;
	void set_player_traits(const c_player_traits* traits, bool force);

protected:
	c_flags<e_sandbox_variant_flags, uns8, k_sandbox_variant_flags> m_variant_flags;
	c_enum<e_sandbox_edit_mode_settings, int8, _sandbox_edit_mode_settings_all_players, k_sandbox_edit_mode_settings> m_edit_mode;
	c_enum<e_sandbox_respawn_time, int16, _sandbox_respawn_time_instant, k_sandbox_respawn_times> m_respawn_time;
	c_player_traits m_player_traits;
};
COMPILE_ASSERT(sizeof(c_game_engine_sandbox_variant) == 0x1F0);

class c_sandbox_engine :
	public c_game_engine
{
};

struct s_core_quota
{
	int32 object_definition_index;
	uns8 maximum_count;
	uns8 minimum_count;
	uns8 placed_on_map;
	byte pad0;
};
COMPILE_ASSERT(sizeof(s_core_quota) == 0x8);

struct s_sandbox_globals
{
	s_core_quota quota;
	int32 player_role_bit_array;
	c_static_array<int32, 16> edited_object_ids;
	c_static_array<real32, 16> cursor_distance_from_camera;
	c_static_array<real_point3d, 16> cursor_position;
	c_static_array<real_vector3d, 16> cursor_normal;
	c_static_array<real_vector3d, 16> cursor_vector;
	c_static_array<int32, 16> object_under_cursor;
	c_static_array<bool, 16> rotating_object;
	c_static_array<int32, 64> tracked_edited_objects;
	c_static_array<real_point3d, 64> tracked_edited_objects_last_soft_ceiling_position;
	int32 number_of_tracked_objects;
	int32 edited_map_variant_index;
	int32 current_object_definition_index;
	bool legal_accepted;
	int32 last_rendered_boundary_object_index;
	s_variant_multiplayer_object_properties_definition edited_object_properties;
};
COMPILE_ASSERT(sizeof(s_sandbox_globals) == 0x748);

extern c_game_engine_sandbox_variant*& sandbox_variant;

extern bool __cdecl game_engine_is_sandbox();
extern bool __cdecl game_engine_object_is_fixed(int32 object_index);

