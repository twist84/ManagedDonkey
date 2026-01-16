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

	uns32 flags;

	byte __data[0x8];
};
COMPILE_ASSERT(sizeof(s_campaign_metagame_runtime_globals) == 0x1A158);

struct s_campaign_metagame_runtime_sizeof_10C
{
	s_player_identifier identifier;
	int32 campaign_deaths;
	int32 campaign_betrayals;

	byte __data10[0x4];

	int32 points_lost;
	uns8 buckets[k_campaign_metagame_bucket_type_count][k_campaign_metagame_bucket_class_count];

	byte __unknown108[4];
};
COMPILE_ASSERT(sizeof(s_campaign_metagame_runtime_sizeof_10C) == 0x10C);

struct s_campaign_metagame_runtime_globals_secondary
{
	int32 game_ticks;
	bool initialized;
	byte __pad5[0x3];

	s_campaign_metagame_runtime_sizeof_10C __unknown8[4];
	real_rectangle3d __unknown438[16];

	int32 __unknown5B8; // __unknown5BC_index
	byte __unknown5BC[500][0xE];
};
COMPILE_ASSERT(sizeof(s_campaign_metagame_runtime_globals_secondary) == 0x2114);

extern bool& chud_debug_metagame;
extern s_campaign_metagame_runtime_globals_secondary& g_campaign_metagame_runtime_globals_secondary;

extern bool debug_skulls;

extern void __cdecl campaign_metagame_dispose();
extern void __cdecl campaign_metagame_dispose_from_old_map();
extern void __cdecl campaign_metagame_initialize();
extern void __cdecl campaign_metagame_initialize_for_new_map();
extern void __cdecl campaign_metagame_update();
extern bool __cdecl campaign_skull_is_active(e_campaign_skulls_primary primary_skull);
extern void __cdecl metagame_postgame_dismiss_ui();
extern bool __cdecl metagame_postgame_in_progress();

extern void render_debug_campaign_metagame();

