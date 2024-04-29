#include "physics/collision_usage.hpp"

#include "cseries/cseries.hpp"

bool collision_log_render_enable = false;
bool collision_log_detailed = false;
bool collision_log_extended = false;
bool collision_log_totals_only = false;
bool collision_log_time = false;
bool global_collision_log_enable = false;
bool global_collision_log_switch_pending = false;
bool global_collision_log_switch_pending_value = false;
short global_current_collision_user_depth = 0;
short global_collision_period_depth = 0;
short global_current_collision_users[MAXIMUM_COLLISION_USER_STACK_DEPTH]{};

void __cdecl collision_log_end_frame()
{
	INVOKE(0x00641A20, collision_log_end_frame);
}

void __cdecl collision_log_begin_frame()
{
	INVOKE(0x00641A30, collision_log_begin_frame);
}

void collision_log_render()
{
	if (collision_log_render_enable)
	{
		// #TODO: implement
	}
}

char const* const global_collision_function_names[12]
{
	"vector",
	"vector-early-out",
	"vector-structure",
	"vector-water",
	"vector-instances",
	"vector-objects",
	"vector-bounds-object",
	"vector-bounds-instance",
	"vector-intersect-model",
	"vector-intersect-bsp-object",
	"vector-intersect-bsp-instance",
	"vector-intersect-bsp-structure"
};

char const* const global_collision_user_names[28]
{
	/* 0	*/ "????",
	/* 1	*/ "ai-look",
	/* 2	*/ "ai-los",
	/* 3	*/ "ai-comm",
	/* 4	*/ "ai-fire",
	/* 5	*/ "ai-melee",
	/* 6	*/ "ai-path",
	/* 7	*/ "ai-move",
	/* 8	*/ "aim",
	/* 9	*/ "biped",
	/* 10	*/ "melee",
	/* 11	*/ "decal",
	/* 12	*/ "areadmg",
	/* 13	*/ "item",
	/* 14	*/ "obsrv",
	/* 15	*/ "particle",
	/* 16	*/ "pt-phys",
	/* 17	*/ "proj",
	/* 18	*/ "light",
	/* 19	*/ "sound",
	/* 20	*/ "veh",
	/* 21	*/ "ragdoll",
	/* 22	*/ "object",
	/* 23	*/ "ui",
	/* 24	*/ "effct",
	/* 25	*/ "debug",
	/* 26	*/ "unit",
	/* 27	*/ "creat-shad"
};
