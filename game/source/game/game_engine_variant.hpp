#pragma once
#pragma pack(push, 4)

#include "cseries/cseries.hpp"
#include "game/game_engine_traits.hpp"
#include "memory/bitstream.hpp"
#include "saved_games/saved_game_files.hpp"

enum e_game_engine_type
{
	_game_engine_base_variant = 0,
	_game_engine_ctf_variant,
	_game_engine_slayer_variant,
	_game_engine_oddball_variant,
	_game_engine_king_variant,
	_game_engine_sandbox_variant,
	_game_engine_vip_variant,
	_game_engine_juggernaut_variant,
	_game_engine_territories_variant,
	_game_engine_assault_variant,
	_game_engine_infection_variant,

	k_game_engine_type_count,
	k_game_engine_type_default = _game_engine_base_variant
};

struct c_game_engine_base_variant
{
	virtual long get_game_engine_name_string_id();
	virtual long get_game_engine_default_description_string_id();
	virtual void initialize();
	virtual void validate();
	virtual void encode(c_bitstream*);
	virtual void decode(c_bitstream*);
	virtual bool can_add_to_recent_list();
	virtual long get_score_to_win_round();
	virtual long get_score_unknown();                                  // halo online specific
	virtual bool can_be_cast_to(e_game_engine_type, void const**);
	virtual void custom_team_score_stats(long, long, long);

	dword m_checksum;
	char m_name[32];
	s_saved_game_item_metadata m_metadata;
	c_game_engine_miscellaneous_options m_miscellaneous_options;
	c_game_engine_respawn_options m_respawn_options;
	c_game_engine_social_options m_social_options;
	c_game_engine_map_override_options m_map_override_options;
	word_flags m_flags;
	short m_team_scoring_method;
};
constexpr size_t k_game_engine_base_variant_size = sizeof(c_game_engine_base_variant);
static_assert(k_game_engine_base_variant_size == 0x1D0);

extern const char* game_engine_variant_get_name(long game_engine_variant);

struct c_game_variant;
extern c_game_variant* __cdecl build_default_game_variant(c_game_variant* game_variant, e_game_engine_type game_engine_index);
extern bool __cdecl game_engine_tag_defined_variant_get_built_in_variant(e_game_engine_type game_engine_index, long variant_index, c_game_variant* game_variant);

#pragma pack(pop)