#include "game/game_engine_variant.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "game/game_options.hpp"
#include "memory/byte_swapping.hpp"

char const* k_game_engine_variant_names[k_game_engine_type_count] =
{
	"base",
	"ctf",
	"slayer",
	"oddball",
	"king",
	"sandbox",
	"vip",
	"juggernaut",
	"territories",
	"assault",
	"infection"
};

const char* game_engine_variant_get_name(long game_engine_index)
{
	if (game_engine_index < _game_engine_base_variant || game_engine_index >= k_game_engine_type_count)
		return "<invalid 'game_engine_index'>";

	return k_game_engine_variant_names[game_engine_index];
}

void c_game_engine_base_variant::byteswap()
{
	m_metadata.byteswap();
	m_respawn_options.m_respawn_player_traits.byteswap();
	bswap_word_inplace(m_social_options.m_flags);
	bswap_word_inplace(m_social_options.m_team_changing);
	bswap_dword_inplace(m_map_override_options.m_flags);
	m_map_override_options.m_base_player_traits.byteswap();
	bswap_word_inplace(m_map_override_options.m_weapon_set_absolute_index);
	bswap_word_inplace(m_map_override_options.m_vehicle_set_absolute_index);
	m_map_override_options.m_red_powerup_traits.byteswap();
	m_map_override_options.m_blue_powerup_traits.byteswap();
	m_map_override_options.m_yellow_powerup_traits.byteswap();
	bswap_word_inplace(m_flags);
	bswap_word_inplace(m_team_scoring_method);
}

c_game_variant* __cdecl build_default_game_variant(c_game_variant* game_variant, e_game_engine_type game_engine_index)
{
	return INVOKE(0x005721C0, build_default_game_variant, game_variant, game_engine_index);
}

bool __cdecl game_engine_tag_defined_variant_get_built_in_variant(e_game_engine_type game_engine_index, long variant_index, c_game_variant* game_variant)
{
	return INVOKE(0x00572270, game_engine_tag_defined_variant_get_built_in_variant, game_engine_index, variant_index, game_variant);
}

bool __cdecl game_engine_variant_is_valid(c_game_variant* game_variant)
{
	return INVOKE(0x005728B0, game_engine_variant_is_valid, game_variant);
}

