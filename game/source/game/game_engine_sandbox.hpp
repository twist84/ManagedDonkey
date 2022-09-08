#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_sandbox_traits.hpp"
#include "game/game_engine_variant.hpp"

struct c_game_engine_sandbox_variant : c_game_engine_base_variant
{
	c_flags<e_sandbox_variant_flags, byte_flags, k_sandbox_variant_flags> m_variant_flags;
	c_enum<e_sandbox_editing_mode, char, k_sandbox_editing_mode> m_edit_mode;
	c_enum<e_sandbox_respawn_time, short, k_sandbox_respawn_times> m_respawn_time;
	c_player_traits m_all_player_traits;

	byte unused[0x70];
};
static_assert(sizeof(c_game_engine_sandbox_variant) == 0x260);

struct c_sandbox_engine : c_game_engine
{
};