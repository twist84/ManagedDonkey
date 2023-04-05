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
	m_miscellaneous_options.byteswap();
	m_respawn_options.byteswap();
	m_social_options.byteswap();
	m_map_override_options.byteswap();
	bswap_word_inplace(m_flags);
	bswap_word_inplace(m_team_scoring_method);
}

char const* c_game_engine_base_variant::get_name() const
{
	return m_name;
}

void c_game_engine_base_variant::set_name(char const* name)
{
	csstrnzcpy(m_name, name, 32);
}

char const* c_game_engine_base_variant::get_description() const
{
	return m_metadata.description;
}

void c_game_engine_base_variant::set_description(char const* description)
{
	csstrnzcpy(m_metadata.description, description, 128);
}

c_game_engine_miscellaneous_options* c_game_engine_base_variant::get_miscellaneous_options_writeable()
{
	return &m_miscellaneous_options;
}

c_game_engine_miscellaneous_options const* c_game_engine_base_variant::get_miscellaneous_options() const
{
	return &m_miscellaneous_options;
}

c_game_engine_respawn_options* c_game_engine_base_variant::get_respawn_options_writeable()
{
	return &m_respawn_options;
}

c_game_engine_respawn_options const* c_game_engine_base_variant::get_respawn_options() const
{
	return &m_respawn_options;
}

c_game_engine_social_options* c_game_engine_base_variant::get_social_options_writeable()
{
	return &m_social_options;
}

c_game_engine_social_options const* c_game_engine_base_variant::get_social_options() const
{
	return &m_social_options;
}

c_game_engine_map_override_options* c_game_engine_base_variant::get_map_override_options_writeable()
{
	return &m_map_override_options;
}

c_game_engine_map_override_options const* c_game_engine_base_variant::get_map_override_options() const
{
	return &m_map_override_options;
}

bool c_game_engine_base_variant::get_built_in() const
{
	return TEST_BIT(m_flags, 0);
}

void c_game_engine_base_variant::set_built_in(bool built_in)
{
	if (built_in)
		m_flags |= FLAG(0);
	else
		m_flags &= ~FLAG(0);
}

short c_game_engine_base_variant::get_team_scoring_method() const
{
	return m_team_scoring_method;
}

void c_game_engine_base_variant::set_team_scoring_method(short team_scoring_method)
{
	m_team_scoring_method = team_scoring_method;
}

c_game_variant* __cdecl build_default_game_variant(c_game_variant* game_variant, e_game_engine_type game_engine_index)
{
	return INVOKE(0x005721C0, build_default_game_variant, game_variant, game_engine_index);
}

bool __cdecl game_engine_tag_defined_variant_get_built_in_variant(e_game_engine_type game_engine_index, long variant_index, c_game_variant* game_variant)
{
	return INVOKE(0x00572270, game_engine_tag_defined_variant_get_built_in_variant, game_engine_index, variant_index, game_variant);
}

void __cdecl game_engine_variant_describe_invalidity(c_game_variant const* game_variant)
{
	INVOKE(0x00572830, game_engine_variant_describe_invalidity, game_variant);
}

bool __cdecl game_engine_variant_is_valid(c_game_variant const* game_variant)
{
	return INVOKE(0x005728B0, game_engine_variant_is_valid, game_variant);
}

bool game_engine_variant_validate(c_game_variant* game_variant)
{
	return INVOKE(0x00572920, game_engine_variant_validate, game_variant);
}

