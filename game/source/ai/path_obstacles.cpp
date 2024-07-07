#include "ai/path_obstacles.hpp"

#include "physics/collisions.hpp"
#include "render/render_debug.hpp"

//.text:014CA200 ; bool __cdecl obstacle_object_test(long, long, struct s_obstacle_profile*, short, bool, struct obstacles*, struct obstacles*)
//.text:014CB180 ; bool __cdecl obstacle_object_test(long, long, struct s_obstacle_profile*, struct obstacles*, struct obstacles*)
//.text:014CB1A0 ; void __cdecl obstacle_profile_new(struct s_obstacle_profile*, real_point3d const*, real, long, struct special_movement*, vector3d const*, long, long)
//.text:014CB230 ; void __cdecl obstacle_profile_set_actor(long, real_point3d const*, vector3d const*, short, bool, struct s_obstacle_profile*)
//.text:014CB390 ; bool __cdecl obstacles_add_disc(struct obstacles*, long, short, real_point3d const*, real)
//.text:014CB420 ; void __cdecl obstacles_disc_neighborhood(struct obstacles const*, real, short, c_static_flags<256>*)
//.text:014CB560 ; void __cdecl obstacles_disc_tangents(struct obstacles const*, short, real_point2d const*, real, vector2d*, vector2d*, real*)

struct disc const* __cdecl obstacles_get_disc(struct obstacles const* obstacles, short disc_index)
{
	return INVOKE(0x014CB720, obstacles_get_disc, obstacles, disc_index);
}

//.text:014CB740 ; void __cdecl obstacles_get_discs_in_sphere(long, long, short, bool, struct special_movement*, struct obstacles*, struct obstacles*, real_point3d const*, real, vector3d const*, vector3d const*, long, long)

void __cdecl obstacles_new(struct obstacles* obstacles)
{
	INVOKE(0x014CB890, obstacles_new, obstacles);
}

//.text:014CB8B0 ; void __cdecl obstacles_recompute(struct obstacles*, real)
//.text:014CB980 ; short __cdecl obstacles_test_circle(struct obstacles const*, short, real_point2d const*, real)
//.text:014CBA00 ; bool __cdecl obstacles_test_pill(struct obstacles const*, short, real_point2d const*, vector2d const*, real, real, bool, struct obstacles_test_pill_result*)

void render_debug_obstacles(struct obstacles const* obstacles, real radius)
{
	for (short disc_index = 0; disc_index < obstacles->disc_count; disc_index++)
	{
		struct disc const* disc = obstacles_get_disc(obstacles, disc_index);

		s_collision_test_flags flags = {}; // 0x801
		collision_result collision;

		ASSERT(disc->obstacle_index >= 0 && disc->obstacle_index < obstacles->obstacle_count);

		real_point3d point{};
		vector3d vector{};

		flags.collision_flags.set(_collision_test_structure_bit, true);
		flags.collision_flags.set(_collision_test_front_facing_surfaces_bit, true);

		set_real_point3d(&point, disc->center.x, disc->center.y, 0.0f/*disc->__unknown14*/);
		set_real_vector3d(&vector, 0.0f, 0.0f, (-2.0f * (disc->radius + radius)));

		static real_argb_color const _color = { 1.0f, { 1.0f, 1.0f, 0.5f } };
		real_argb_color const* color = &_color;

		if (TEST_BIT(disc->flags, 2))
		{
			color = global_real_argb_red;
		}
		else if (TEST_BIT(disc->flags, 1))
		{
			color = global_real_argb_purple;
		}
		
		if (collision_test_vector(flags, &point, &vector, NONE, NONE, &collision))
		{
			plane3d plane{};
			render_debug_circle(true, &plane, _z, true, &disc->center, disc->radius, color, 0.015625f);
			if (radius > 0.0f)
				render_debug_circle(true, &plane, _z, true, &disc->center, (disc->radius + radius), color, 0.015625f);
		}
		else
		{
			render_debug_sphere(true, &point, radius, color);
			if (radius > 0.0f)
				render_debug_sphere(true, &point, (disc->radius + radius), color);
		}
	}
}

