#pragma once

#include "cseries/cseries.hpp"
#include "game/aim_target.hpp"

struct s_aim_assist_targeting_result
{
	c_aim_target_object aim_target;
	int32 target_player;
	real32 primary_autoaim_level;
	real32 secondary_autoaim_level;
	real_vector3d lead_vector;
	uns32 flags;
};
COMPILE_ASSERT(sizeof(s_aim_assist_targeting_result) == 0x28);

struct s_aim_assist_query
{
	byte __data0[0x1];
	bool weapon_melee_only;
	byte __data2[0x2];

	real32 weapon_autoaim_angle4;
	real32 weapon_autoaim_range8;
	real32 weapon_autoaim_falloff_rangeC;
	real32 weapon_autoaim_near_falloff_range10;

	real32 weapon_magnetism_angle;
	real32 weapon_magnetism_range;
	real32 weapon_magnetism_falloff_range;
	real32 weapon_magnetism_near_falloff_range;

	union
	{
		real32 weapon_autoaim_angle;
		real32 weapon_deviation_angle;
	};

	uns8 __flags28;
	byte __data29[0x3];

	real32 weapon_autoaim_angle2C;
	real32 weapon_autoaim_range30;
	real32 weapon_autoaim_falloff_range34;
	real32 weapon_autoaim_near_falloff_range38;

	real32 weapon_autoaim_range3C;

	int32 __unknown40;
	real_point3d __unknown44;

	real32 projectile_velocity;
	real32 projectile_autoaim_leading_max_lead_time;
};
COMPILE_ASSERT(sizeof(s_aim_assist_query) == 0x58);

extern bool __cdecl aim_assist_build_query_parameters(int32 unit_index, uns8 flags, int16 magnification_level, s_aim_assist_query* out_query);

extern void aim_assist_debug_render();

