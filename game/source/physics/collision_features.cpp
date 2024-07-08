#include "physics/collision_features.hpp"

#include "render/render_debug.hpp"

bool __cdecl collision_features_test_point(collision_feature_list const* features, real_point3d const* point, collision_plane* plane)
{
	return INVOKE(0x00947EC0, collision_features_test_point, features, point, plane);
}

void render_debug_collision_features(collision_feature_list const* features)
{
	ASSERT(features->count[_collision_feature_sphere] <= MAXIMUM_COLLISION_FEATURES_PER_TEST);
	ASSERT(features->count[_collision_feature_cylinder] <= MAXIMUM_COLLISION_FEATURES_PER_TEST);
	ASSERT(features->count[_collision_feature_prism] <= MAXIMUM_COLLISION_FEATURES_PER_TEST);

	for (short prism_index = 0; prism_index < features->count[_collision_feature_prism]; prism_index++)
		render_debug_collision_prism(&features->prisms[prism_index], global_real_argb_blue);

	for (short cylinder_index = 0; cylinder_index < features->count[_collision_feature_cylinder]; cylinder_index++)
		render_debug_collision_cylinder(&features->cylinders[cylinder_index], global_real_argb_green);

	for (short sphere_index = 0; sphere_index < features->count[_collision_feature_sphere]; sphere_index++)
		render_debug_collision_sphere(&features->spheres[sphere_index], global_real_argb_red);
}

void render_debug_collision_prism(collision_prism const* prism, real_argb_color const* color)
{
	long point_count = prism->point_count;
	if (point_count > MAXIMUM_POINTS_PER_COLLISION_PRISM)
		point_count = MAXIMUM_POINTS_PER_COLLISION_PRISM;

	ASSERT(prism->point_count <= MAXIMUM_POINTS_PER_COLLISION_PRISM);

	real_point3d points[MAXIMUM_POINTS_PER_COLLISION_PRISM]{};
	for (long point_index = 0; point_index < point_count; point_index++)
	{
		project_point2d(&prism->points[point_index], &prism->plane, prism->projection_axis, prism->projection_sign, &points[point_index]);
		point_from_line3d(&points[point_index], &prism->plane.n, prism->height, &points[point_index]);
	}

	for (long point_index = 0; point_index < point_count; point_index++)
		render_debug_line(true, &points[point_index], &points[(point_index + 1) % point_count], color);
}

void render_debug_collision_cylinder(collision_cylinder const* cylinder, real_argb_color const* color)
{
	render_debug_cylinder(true, &cylinder->base, &cylinder->height, cylinder->width, color);
}

void render_debug_collision_sphere(collision_sphere const* sphere, real_argb_color const* color)
{
	render_debug_sphere(true, &sphere->center, sphere->radius, color);
}

