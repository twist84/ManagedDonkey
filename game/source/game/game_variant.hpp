#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_variant.hpp"
#include "game/game_engine_ctf.hpp"
#include "game/game_engine_slayer.hpp"
#include "game/game_engine_oddball.hpp"
#include "game/game_engine_king.hpp"
#include "game/game_engine_sandbox.hpp"
#include "game/game_engine_vip.hpp"
#include "game/game_engine_juggernaut.hpp"
#include "game/game_engine_territories.hpp"
#include "game/game_engine_assault.hpp"
#include "game/game_engine_infection.hpp"

struct c_game_variant
{
	c_enum<e_game_engine_variant, long, k_game_engine_variant_count> m_game_engine_index;

	union
	{
		c_game_engine_base_variant m_base_variant;
		c_game_engine_ctf_variant m_ctf_variant;
		c_game_engine_slayer_variant m_slayer_variant;
		c_game_engine_oddball_variant m_oddball_variant;
		c_game_engine_king_variant m_king_variant;
		c_game_engine_sandbox_variant m_sandbox_variant;
		c_game_engine_vip_variant m_vip_variant;
		c_game_engine_juggernaut_variant m_juggernaut_variant;
		c_game_engine_territories_variant m_territories_variant;
		c_game_engine_assault_variant m_assault_variant;
		c_game_engine_infection_variant m_infection_variant;
		byte variant_data[0x260];
	};
};
static_assert(sizeof(c_game_variant) == 0x264);
