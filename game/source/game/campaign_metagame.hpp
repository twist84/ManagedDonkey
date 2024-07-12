#pragma once

#include "cseries/cseries.hpp"
#include "game/campaign_metagame_definitions.hpp"
#include "game/players.hpp"

struct s_campaign_metagame_runtime_globals
{
	enum e_metagame_flags
	{
		_initialized_bit = 1
	};

	byte __data0[0x1A14C];

	dword_flags flags;

	byte __data[0x8];
};
static_assert(sizeof(s_campaign_metagame_runtime_globals) == 0x1A158);

struct s_campaign_metagame_runtime_sizeof_10C
{
	s_player_identifier identifier;
	long campaign_deaths;
	long campaign_betrayals;

	byte __data10[4];

	long points_lost;
	byte buckets[k_campaign_metagame_bucket_type_count][k_campaign_metagame_bucket_class_count];

	byte __unknown108[4];
};
static_assert(sizeof(s_campaign_metagame_runtime_sizeof_10C) == 0x10C);

struct s_campaign_metagame_runtime_globals_secondary
{
	long game_ticks;
	bool initialized;
	byte __pad5[0x3];

	s_campaign_metagame_runtime_sizeof_10C __unknown8[4];
	real_rectangle3d __unknown438[16];

	long __unknown5B8; // __unknown5BC_index
	byte __unknown5BC[500][0xE];
};
static_assert(sizeof(s_campaign_metagame_runtime_globals_secondary) == 0x2114);

extern bool& chud_debug_metagame;
extern s_campaign_metagame_runtime_globals_secondary& g_campaign_metagame_runtime_globals_secondary;

extern void __cdecl campaign_metagame_update();
extern bool __cdecl sub_60B080();

