#include "render/render_visibility.hpp"

#include "cseries/cseries.hpp"
#include "visibility/visibility_collection.hpp"

void __cdecl render_invisible_objects_iterate(void(*function)(int32))
{
	ASSERT(function);
	ASSERT(c_visible_items::get_cluster_starting_index() == 0);

	INVOKE(0x00A7BC50, render_invisible_objects_iterate, function);
}

bool __cdecl render_sphere_visible(const real_point3d* center, real32 radius)
{
	return INVOKE(0x00A7BDA0, render_sphere_visible, center, radius);
}

void __cdecl render_visibility_build_projection(const render_camera* camera, const render_projection* projection, s_cluster_reference camera_cluster_reference, struct visibility_projection* visibility_projection)
{
	INVOKE(0x00A7BDD0, render_visibility_build_projection, camera, projection, camera_cluster_reference, visibility_projection);
}

int32 __cdecl render_visible_objects_iterate(void(*function)(int32))
{
    ASSERT(function);
    ASSERT(c_visible_items::get_root_objects_starting_index() == 0);

    return INVOKE(0x00A7C130, render_visible_objects_iterate, function);
}

void visibility_profile_display(char* buffer, int32 buffer_size)
{
	csnzappendf(buffer, buffer_size, "|nObjects:                  (bip/veh/weap/other)     skies: %d|n",
		0); // $TODO c_render_information::sky_count

	csnzappendf(buffer, buffer_size, "     total %3d     visible: %2d/%2d/%2d/%2d|n",
		0,  // $TODO c_render_information::total_object_count[0]
		0,  // $TODO c_render_information::object_types[0][0]
		0,  // $TODO c_render_information::object_types[1][0]
		0,  // $TODO c_render_information::object_types[2][0]
		0); // $TODO c_render_information::object_types[3][0]

	csnzappendf(buffer, buffer_size, "     total %3d     shadow:  %2d/%2d/%2d/%2d|n",
		0,  // $TODO c_render_information::total_object_count[2]
		0,  // $TODO c_render_information::object_types[0][2]
		0,  // $TODO c_render_information::object_types[1][2]
		0,  // $TODO c_render_information::object_types[2][2]
		0); // $TODO c_render_information::object_types[3][2]

	csnzappendf(buffer, buffer_size, "     total %3d     light:   %2d/%2d/%2d/%2d|n",
		0,  // $TODO c_render_information::total_object_count[1]
		0,  // $TODO c_render_information::object_types[0][1]
		0,  // $TODO c_render_information::object_types[1][1]
		0,  // $TODO c_render_information::object_types[2][1]
		0); // $TODO c_render_information::object_types[3][1]

	csnzappendf(buffer, buffer_size, "               indices total: %4dk     alb: %4dk, sl: %4dk, li: %4dk sha: %4dk tran: %4dk|n",
		0,  /* $TODO
		(c_render_information::indices_counters[3][1]
		+c_render_information::indices_counters[3][2]
		+c_render_information::indices_counters[3][3]
		+c_render_information::indices_counters[3][0]
		+c_render_information::indices_counters[3][4]) / 1024 */
		0,  // $TODO c_render_information::indices_counters[3][0] / 1024
		0,  // $TODO c_render_information::indices_counters[3][1] / 1024
		0,  // $TODO c_render_information::indices_counters[3][2] / 1024
		0,  // $TODO c_render_information::indices_counters[3][3] / 1024
		0); // $TODO c_render_information::indices_counters[3][4] / 1024

	csnzappendf(buffer, buffer_size, "Clusters:     Visible: %3d Shadow: %3d Dyn Light: %3d Indices total: %4dk     alb: %4dk, sl: %4dk, li: %4dk sha: %4dk tran: %4dk|n",
		0,  // $TODO c_render_information::cluster_count[0],
		0,  // $TODO c_render_information::cluster_count[2],
		0,  // $TODO c_render_information::cluster_count[1],
		0,  /* $TODO
		(c_render_information::indices_counters[0][1]
		+c_render_information::indices_counters[0][2]
		+c_render_information::indices_counters[0][3]
		+c_render_information::indices_counters[0][0]
		+c_render_information::indices_counters[0][4]) / 1024 */
		0,  // $TODO c_render_information::indices_counters[0][0] / 1024
		0,  // $TODO c_render_information::indices_counters[0][1] / 1024
		0,  // $TODO c_render_information::indices_counters[0][2] / 1024
		0,  // $TODO c_render_information::indices_counters[0][3] / 1024
		0); // $TODO c_render_information::indices_counters[0][4] / 1024

	csnzappendf(buffer, buffer_size, "Instances:    Visible: %3d Shadow: %3d Dyn Light: %3d Indices total: %4dk     alb: %4dk, sl: %4dk, li: %4dk sha: %4dk tran: %4dk|n",
		0,  // $TODO c_render_information::instance_count[0]
		0,  // $TODO c_render_information::instance_count[2]
		0,  // $TODO c_render_information::instance_count[1]
		0,  /* $TODO (c_render_information::indices_counters[1][1]
		+c_render_information::indices_counters[1][2]
		+c_render_information::indices_counters[1][3]
		+c_render_information::indices_counters[1][0]
		+c_render_information::indices_counters[1][4]) / 1024 */
		0,  // $TODO c_render_information::indices_counters[1][0] / 1024
		0,  // $TODO c_render_information::indices_counters[1][1] / 1024
		0,  // $TODO c_render_information::indices_counters[1][2] / 1024
		0,  // $TODO c_render_information::indices_counters[1][3] / 1024
		0); // $TODO c_render_information::indices_counters[1][4] / 1024

	csnzappendf(buffer, buffer_size, "CPU Lights: %3d   GPU Lights %3d  Lightmap Shadows: %3d|n",
		0,  // $TODO c_render_information::cpu_dynamic_light_count
		0,  // $TODO c_render_information::gpu_dynamic_light_count
		0); // $TODO c_render_information::dynamic_shadow_count
}

