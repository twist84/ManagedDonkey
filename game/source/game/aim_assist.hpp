#pragma once

#include "cseries/cseries.hpp"
#include "game/aim_target.hpp"

struct s_aim_assist_targeting_result
{
	c_aim_target_object aim_target;
	long target_player;
	real primary_autoaim_level;
	real secondary_autoaim_level;
	vector3d lead_vector;
	dword_flags flags;
};
static_assert(sizeof(s_aim_assist_targeting_result) == 0x28);

struct s_aim_assist_query
{
	byte __data0[0x1];
	bool weapon_melee_only;
	byte __data2[0x2];

	real weapon_autoaim_angle4;
	real weapon_autoaim_range8;
	real weapon_autoaim_falloff_rangeC;
	real weapon_autoaim_near_falloff_range10;

	real weapon_magnetism_angle;
	real weapon_magnetism_range;
	real weapon_magnetism_falloff_range;
	real weapon_magnetism_near_falloff_range;

	union
	{
		real weapon_autoaim_angle;
		real weapon_deviation_angle;
	};

	byte_flags __flags28;
	byte __data29[0x3];

	real weapon_autoaim_angle2C;
	real weapon_autoaim_range30;
	real weapon_autoaim_falloff_range34;
	real weapon_autoaim_near_falloff_range38;

	real weapon_autoaim_range3C;

	long __unknown40;
	real_point3d __unknown44;

	real projectile_velocity;
	real projectile_autoaim_leading_max_lead_time;
};
static_assert(sizeof(s_aim_assist_query) == 0x58);

extern bool __cdecl aim_assist_build_query_parameters(long unit_index, byte flags, short magnification_level, s_aim_assist_query* out_query);

extern void aim_assist_debug_render();

