#pragma once

#include "ai/actor_dynamic_firing_position.hpp"
#include "ai/actors.hpp"
#include "ai/ai_dialogue.hpp"
#include "ai/ai_flocks.hpp"
#include "ai/ai_player.hpp"
#include "ai/clump.hpp"
#include "ai/formations.hpp"
#include "ai/joint_behavior.hpp"
#include "ai/objectives.hpp"
#include "ai/props.hpp"
#include "ai/squad_patrol.hpp"
#include "ai/squads.hpp"
#include "ai/swarms.hpp"
#include "cseries/cseries.hpp"

struct ai_globals_type
{
	bool enable;
	bool __unknown1; // render_enable
	byte_flags flags;
	byte __unknown3;
	byte __unknown4;
	bool fast_and_dumb;
	byte __data6[22];
	real unknown1C;
	bool dialogue_enabled;
	byte __data21[31];
	word __unknown40;
	word __unknown42;
	byte __unknown44[0x380];
	byte __data3C4[0x2C4];
};
static_assert(sizeof(ai_globals_type) == 0x688);

extern bool __cdecl ai_enemies_can_see_player(long* out_unit_index);
extern void __cdecl ai_erase(long squad_index, bool delete_immediately);
extern void __cdecl ai_globals_set_ai_active(bool ai_active);
extern void __cdecl ai_handle_bump(long object_index, long bump_object_index, vector3d const* linear_velocity);
extern void __cdecl ai_update();

