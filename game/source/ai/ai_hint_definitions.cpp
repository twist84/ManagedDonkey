#include "ai/ai_hint_definitions.hpp"

#include "ai/ai_debug.hpp"
#include "ai/ai_reference_frame.hpp"
#include "ai/sector.hpp"
#include "ai/sector_definitions.hpp"
#include "ai/sector_geometry.hpp"
#include "cache/cache_files.hpp"
#include "objects/objects.hpp"
#include "render/render_debug.hpp"
#include "scenario/scenario.hpp"

const char* g_hoist_height_names[k_climb_hint_hoist_count]
{
	"step",
	"crouch",
	"stand",
};

const char* g_jump_height_names[k_jump_height_count]
{
	"down",
	"step",
	"crouch",
	"stand",
	"storey",
	"tower",
};

//.text:0148A2C0 ; bool __cdecl ai_hint_door_traversable(const pathfinding_data*, sector_link*, const pathfinding_hint_data*)
//.text:0148A370 ; bool __cdecl ai_hint_find_closest_point(object_marker*, const real_point3d*, real32, real_point3d*)
//.text:0148A490 ; bool __cdecl ai_hint_find_hoist_marker(int32, int32, real32, object_marker*, real_point3d*)
//.text:0148A660 ; bool __cdecl ai_hint_find_marker_point(int32, int32, int16, real32, object_marker*, real_point3d*)
//.text:0148A6E0 ; bool __cdecl ai_hint_find_mount_marker(int32, int32, real32, object_marker*, real_point3d*)
//.text:0148A8A0 ; bool __cdecl ai_hint_find_pill_entry_point(int32, int32, int32, int32, int32, const real_point3d*, const real_vector3d*, real_point3d*, c_ai_point3d*, real_vector3d*, c_sector_ref*)
//.text:0148A9C0 ; bool __cdecl ai_hint_find_root_entry_matrix(int32, int32, int32, const real_point3d*, const real_vector3d*, real_matrix4x3*)
//.text:0148AA20 ; bool __cdecl ai_hint_find_root_entry_point(int32, int32, int32, const real_point3d*, const real_vector3d*, real_point3d*, c_ai_point3d*, real_vector3d*, c_sector_ref*)
//.text:0148AB50 ; bool __cdecl ai_hint_find_vault_marker(int32, int32, real32, object_marker*, real_point3d*)
//.text:0148AE40 ; e_action_hoist __cdecl ai_hint_get_best_hoist_height(int32, real32)
//.text:0148AEF0 ; int16 __cdecl ai_hint_get_best_hoist_height(real32, int32)
//.text:0148AFC0 ; bool __cdecl ai_hint_get_hoist_height(int16, real32*, real32*)
//.text:0148B030 ; int16 __cdecl ai_hint_get_hoist_markers(int32, object_marker*, int16, int16)
//.text:0148B0B0 ; bool __cdecl ai_hint_get_jump_destination_normal(pathfinding_data*, hint_jump_data*, real_vector3d*)

bool __cdecl ai_hint_get_jump_destination_rail(int16 structure_index, hint_jump_data* jump, const real_point3d* anchor, real_point3d* point_ref0, real_point3d* point_ref1, c_sector_ref* destination_sector_ref, int16* destination_reference_frame)
{
	return INVOKE(0x0148B1C0, ai_hint_get_jump_destination_rail, structure_index, jump, anchor, point_ref0, point_ref1, destination_sector_ref, destination_reference_frame);
}

//.text:0148B350 ; bool __cdecl ai_hint_get_jump_maneuver_point(pathfinding_data*, pathfinding_hint_data*, const c_ai_point3d*, c_sector_ref, real32, c_ai_point3d*, c_sector_ref*, c_ai_point3d*, c_sector_ref*)
//.text:0148B610 ; real32 __cdecl ai_hint_get_jump_velocity(int16)
//.text:0148B670 ; bool __cdecl ai_hint_get_maneuver_point(int32, real32, int32, const c_ai_point3d*, c_sector_ref, c_ai_point3d*, c_sector_ref*, c_ai_point3d*, c_sector_ref*)
//.text:0148B9E0 ; real32 __cdecl ai_hint_get_max_jump_down_height(int16)
//.text:0148BA40 ; int16 __cdecl ai_hint_get_mount_markers(int32, object_marker*, int16, int16)
//.text:0148BAF0 ; bool __cdecl ai_hint_get_vault_height(int16, real32*, real32*)
//.text:0148BB60 ; int16 __cdecl ai_hint_get_vault_markers(int32, object_marker*, int16, int16)
//.text:0148BBD0 ; bool __cdecl ai_hint_hoist_range_valid(const real_point3d*, const real_point3d*, const special_movement*)
//.text:0148BC90 ; bool __cdecl ai_hint_is_down(const real_point3d*, const real_point3d*, const real_point3d*, const real_point3d*)
//.text:0148BCD0 ; bool __cdecl ai_hint_marker_valid(object_marker*)

bool __cdecl ai_hint_test_jump(const real_point3d* point0, const real_point3d* point1, const real_point3d* point2, const real_point3d* point3, int16 jump_height)
{
	return INVOKE(0x0148BD20, ai_hint_test_jump, point0, point1, point2, point3, jump_height);
}

//.text:0148BE80 ; bool __cdecl ai_hint_vault_range_valid(const real_point3d*, const real_point3d*, const special_movement*)
//.text:0148BF30 ; bool __cdecl ai_link_hint_traversable(const struct pathfinding_data*, int32)

bool ai_point_on_structure(const c_ai_point3d* point, int16 structure_index, bool* ambiguous)
{
	bool result = false;
	if (point->bsp_index() == structure_index)
	{
		result = true;
	}
	else if (point->bsp_index() == NONE)
	{
		*ambiguous = true;
		result = true;
	}
	return result;
}

void ai_render_hints()
{
	const scenario* scenario = global_scenario_get();
	int16 bsp_mask = (int16)global_structure_bsp_active_mask_get();
	for (int16 structure_index = 0; structure_index < (int16)scenario->structure_bsp_references.count; structure_index++)
	{
		if (TEST_BIT(bsp_mask, structure_index))
		{
			const pathfinding_data* pf_data = pathfinding_data_get(structure_index);
			if (pf_data)
			{
				for (int32 hint_index = 0; hint_index < pf_data->hints.count; hint_index++)
				{
					pathfinding_hint_data* hint = TAG_BLOCK_GET_ELEMENT(&pf_data->hints, hint_index, pathfinding_hint_data);
					switch (hint->type)
					{
					case _hint_type_jump:
					{
						render_jump_hint(structure_index, hint_index);
					}
					break;
					case _hint_type_hoist:
					{
						render_hoist_hint(pf_data, hint_index);
					}
					break;
					case _hint_type_wall_jump:
					{
						render_jump_hint(structure_index, hint_index);
					}
					break;
					}
				}

				for (int32 link_index = 0; link_index < pf_data->sector_links.count; link_index++)
				{
					sector_link* link = TAG_BLOCK_GET_ELEMENT(&pf_data->sector_links, link_index, sector_link);
					if (TEST_BIT(link->flags, _sector_link_leanable_bit))
					{
						real_point3d* p0 = &TAG_BLOCK_GET_ELEMENT_SAFE(&pf_data->sector_vertices, link->index, sector_vertex)->point;
						real_point3d* p1 = &TAG_BLOCK_GET_ELEMENT_SAFE(&pf_data->sector_vertices, link->index2, sector_vertex)->point;
						render_debug_line(true, p0, p1, global_real_argb_green);

						real_vector3d link_vector{};
						vector_from_points3d(p0, p1, &link_vector);
						real32 length = normalize3d(&link_vector);
						if (length > 0.0f)
						{
							real_point3d center{};
							if (TEST_BIT(link->flags, _sector_link_start_corner_bit))
							{
								point_from_line3d(p0, &link_vector, 0.05f, &center);
								render_debug_sphere(true, &center, 0.05f, global_real_argb_green);
							}
							if (TEST_BIT(link->flags, _sector_link_end_corner_bit))
							{
								point_from_line3d(p0, &link_vector, length - 0.05f, &center);
								render_debug_sphere(true, &center, 0.05f, global_real_argb_green);
							}
						}
					}
				}
			}
		}
	}
}

void ai_render_object_hints(bool active_only)
{
	//{
	//	c_object_iterator<object_datum> iterator;
	//	uns32 flags;
	//}
}

//void ai_render_object_properties()
//{
//}

void ai_render_user_hints()
{
	const user_hint_data* hints = TAG_BLOCK_GET_ELEMENT_SAFE(&global_scenario_get()->ai_user_hints, 0, const user_hint_data);
	if (hints)
	{
		for (int16 line_segment_index = 0; line_segment_index < (int16)hints->line_segment_geometry.count; line_segment_index++)
		{
			user_hint_render_line_segment(TAG_BLOCK_GET_ELEMENT(&hints->line_segment_geometry, line_segment_index, const user_hint_line_segment), global_real_argb_yellow);
		}
		for (int16 parallelogram_index = 0; parallelogram_index < (int16)hints->parallelogram_geometry.count; parallelogram_index++)
		{
			user_hint_render_parallelogram(TAG_BLOCK_GET_ELEMENT(&hints->parallelogram_geometry, parallelogram_index, const user_hint_parallelogram), false);
		}
		render_flight_hints();
	}
	render_well_hints(NULL, true);
	render_sector_hints(NULL);
}

void render_arrow(const real_point3d* point, const real_vector3d* vector, real32 length, const real_argb_color* color, bool bidirectional)
{
	real_point3d center{};
	for (int16 c = 1; c < 3; c++)
	{
		point_from_line3d(point, vector, length - (((real32)c * length) / 20.0f), &center);
		render_debug_sphere(true, &center, ((real32)c * length) / 40.0f, color);
	}
	if (bidirectional)
	{
		for (int16 c = 1; c < 3; c++)
		{
			point_from_line3d(point, vector, (((real32)c * length) / 20.0f), &center);
			render_debug_sphere(true, &center, ((real32)c * length) / 40.0f, color);
		}
	}
	render_debug_vector(true, point, vector, length, color);
}

void render_arrow(real_point3d* point0, const real_point3d* point1, const real_argb_color* color, bool bidirectional)
{
	real_vector3d vector{};
	vector_from_points3d(point0, point1, &vector);
	real32 length = normalize3d(&vector);
	render_arrow(point0, &vector, length, color, bidirectional);
}

void render_flight_hint(const user_flight_hint* flight_hint)
{
	if (flight_hint->points.count > 0)
	{
		for (int16 point_index = 0; point_index < (int16)flight_hint->points.count; point_index++)
		{
			user_hint_flight_point* flight_point0 = TAG_BLOCK_GET_ELEMENT(&flight_hint->points, point_index, user_hint_flight_point);
			if (point_index < (int16)flight_hint->points.count - 1)
			{
				user_hint_flight_point* flight_point1 = TAG_BLOCK_GET_ELEMENT(&flight_hint->points, point_index + 1, user_hint_flight_point);
				render_debug_line(true, &flight_point0->point, &flight_point1->point, global_real_argb_yellow);
			}
			render_debug_point(true, &flight_point0->point, 0.1f, global_real_argb_yellow);
		}
	}
}

void render_flight_hints()
{
	const user_hint_data* user = TAG_BLOCK_GET_ELEMENT_SAFE(&global_scenario_get()->ai_user_hints, 0, const user_hint_data);
	if (user)
	{
		for (int16 flight_hint_index = 0; flight_hint_index < (int16)user->flight_hints.count; flight_hint_index++)
		{
			user_flight_hint* flight_hint = TAG_BLOCK_GET_ELEMENT(&user->flight_hints, flight_hint_index, user_flight_hint);
			render_flight_hint(flight_hint);
		}
	}
}

void render_giant_rail_hints(const real_argb_color* color)
{
	const user_hint_data* user = TAG_BLOCK_GET_ELEMENT_SAFE(&global_scenario_get()->ai_user_hints, 0, const user_hint_data);
	if (user && user->giant_hints.count > 0)
	{
		const s_user_hint_giant* giant_data = TAG_BLOCK_GET_ELEMENT(&user->giant_hints, 0, const s_user_hint_giant);
		for (int16 rail_index = 0; rail_index < (int16)giant_data->giant_rail_hints.count; rail_index++)
		{
			s_user_hint_giant_rail* rail = TAG_BLOCK_GET_ELEMENT(&giant_data->giant_rail_hints, rail_index, s_user_hint_giant_rail);
			user_hint_line_segment* line_segment = TAG_BLOCK_GET_ELEMENT_SAFE(&user->line_segment_geometry, rail->geometry_index, user_hint_line_segment);
			if (user_hint_line_segment_on_active_structure(line_segment))
			{
				user_hint_render_line_segment(line_segment, color);
			}
		}
	}
}

void render_giant_sector_hints(const real_argb_color* color)
{
	const user_hint_data* user = TAG_BLOCK_GET_ELEMENT_SAFE(&global_scenario_get()->ai_user_hints, 0, const user_hint_data);
	if (user && user->giant_hints.count > 0)
	{
		const s_user_hint_giant* giant_data = TAG_BLOCK_GET_ELEMENT(&user->giant_hints, 0, const s_user_hint_giant);
		for (int16 sector_hint_index = 0; sector_hint_index < (int16)giant_data->giant_sector_hints.count; sector_hint_index++)
		{
			s_user_hint_sector* sector_hint = TAG_BLOCK_GET_ELEMENT(&giant_data->giant_sector_hints, sector_hint_index, s_user_hint_sector);
			if (user_hint_sector_on_active_structure(sector_hint))
			{
				render_sector_hint(sector_hint, color);
			}
		}
	}
}

void render_hoist_hint(const pathfinding_data* pf_data, int32 hint_index)
{
	pathfinding_hint_data* hint = TAG_BLOCK_GET_ELEMENT(&pf_data->hints, hint_index, pathfinding_hint_data);
	const hint_hoist_data* hoist = &hint->hoist;
	if (hoist->point_index0 != NONE && hoist->point_index1 != NONE)
	{
		real_point3d* point0 = &TAG_BLOCK_GET_ELEMENT(&pf_data->sector_vertices, hoist->point_index0, sector_vertex)->point;
		real_point3d* point1 = &TAG_BLOCK_GET_ELEMENT(&pf_data->sector_vertices, hoist->point_index1, sector_vertex)->point;
		real_point3d midpoint{};
		add_vectors3d((real_vector3d*)point0, (real_vector3d*)point1, (real_vector3d*)&midpoint);
		scale_vector3d((real_vector3d*)&midpoint, 0.5f, (real_vector3d*)&midpoint);
		render_arrow(point0, point1, global_real_argb_blue, false);
		ai_debug_drawstack_setup(&midpoint);
		for (int16 height_index = 0; height_index < 3; height_index++)
		{
			if (TEST_BIT(hoist->flags, height_index))
			{
				render_debug_string_at_point(ai_debug_drawstack(), g_hoist_height_names[height_index], global_real_argb_blue);
			}
		}
		render_debug_string_at_point(ai_debug_drawstack(), "hoist", global_real_argb_blue);
	}
}

void render_jump_hint(int16 structure_index, int32 hint_index)
{
	const pathfinding_data* pf_data = pathfinding_data_get(structure_index);
	ASSERT(pf_data);

	pathfinding_hint_data* hint = TAG_BLOCK_GET_ELEMENT(&pf_data->hints, hint_index, pathfinding_hint_data);

	real_point3d destination_point0{};
	real_point3d destination_point1{};
	int16 destination_reference_frame;
	if (hint->jump.point_index0 != NONE
		&& ai_hint_get_jump_destination_rail(structure_index,
			&hint->jump,
			NULL,
			&destination_point0,
			&destination_point1,
			NULL,
			&destination_reference_frame))
	{
		c_ai_point3d ai_point0{};
		c_ai_point3d ai_point1{};
		c_ai_point3d ai_point2{};
		c_ai_point3d ai_point3{};

		ai_point0.set(&TAG_BLOCK_GET_ELEMENT(&pf_data->sector_vertices, hint->jump.point_index0, sector_vertex)->point, hint->jump.reference_frame0);

		if (TEST_BIT(hint->jump.control_flags, _jump_height_step_bit) || hint->jump.point_index1x != NONE)
		{
			ai_point1.set(ai_point0.point(), hint->jump.reference_frame1);
		}
		else
		{
			ai_point1.set(&TAG_BLOCK_GET_ELEMENT(&pf_data->sector_vertices, hint->jump.point_index1x, sector_vertex)->point, hint->jump.reference_frame1);
		}
		ai_point2.set(&destination_point0, destination_reference_frame);
		ai_point3.set(&destination_point1, destination_reference_frame);

		real_point3d center{};
		{
			real_point3d point0{};
			real_point3d point1{};
			real_point3d point2{};
			real_point3d point3{};
			ai_point_get_position(&ai_point0, &point0);
			ai_point_get_position(&ai_point1, &point1);
			ai_point_get_position(&ai_point2, &point2);
			ai_point_get_position(&ai_point3, &point3);

			render_debug_sphere(true, &point0, 0.06f, global_real_argb_blue);
			render_debug_sphere(true, &point1, 0.06f, global_real_argb_blue);
			render_debug_sphere(true, &point2, 0.06f, global_real_argb_blue);
			render_debug_sphere(true, &point3, 0.06f, global_real_argb_blue);

			render_debug_line(true, &point0, &point1, global_real_argb_blue);
			render_debug_line(true, &point2, &point3, global_real_argb_blue);
			render_arrow(&point0, &point2, global_real_argb_yellow, false);
			render_arrow(&point1, &point3, global_real_argb_yellow, false);

			add_vectors3d((real_vector3d*)&point0, (real_vector3d*)&point1, (real_vector3d*)&center);
			add_vectors3d((real_vector3d*)&center, (real_vector3d*)&point2, (real_vector3d*)&center);
			add_vectors3d((real_vector3d*)&center, (real_vector3d*)&point3, (real_vector3d*)&center);
			scale_vector3d((real_vector3d*)&center, 0.25f, (real_vector3d*)&center);
		}

		if (g_ai_render_hints_detailed)
		{
			for (int16 height_index = 0; height_index < NUMBEROF(g_jump_height_names); height_index++)
			{
				if (TEST_BIT(hint->jump.control_flags, height_index))
				{
					render_debug_string_at_point(&center, g_jump_height_names[height_index], global_real_argb_blue);
					break;
				}
			}
		}
	}
}

void render_sector_hint(const s_user_hint_sector* sector_hint, const real_argb_color* color)
{
	for (int16 point_index = 1; point_index < (int16)sector_hint->points.count; point_index++)
	{
		s_user_hint_sector_point* sector_point0 = TAG_BLOCK_GET_ELEMENT(&sector_hint->points, point_index, s_user_hint_sector_point);
		s_user_hint_sector_point* sector_point1 = TAG_BLOCK_GET_ELEMENT(&sector_hint->points, point_index - 1, s_user_hint_sector_point);

		real_point3d point0{};
		real_point3d point1{};
		ai_point_get_position(&sector_point0->position, &point0);
		ai_point_get_position(&sector_point1->position, &point1);

		real_vector3d normal0{};
		real_vector3d normal1{};
		vector3d_from_euler_angles2d(&normal0, &sector_point0->normal);
		vector3d_from_euler_angles2d(&normal1, &sector_point1->normal);

		point_from_line3d(&point0, &normal0, 0.02f, &point0);
		point_from_line3d(&point1, &normal1, 0.02f, &point1);

		if (color)
		{
			render_debug_line(true, &point0, &point1, color);
		}
		else
		{
			render_debug_line(true, &point0, &point1, global_real_argb_blue);
		}
	}

	if (sector_hint->points.count > 2)
	{
		s_user_hint_sector_point* sector_point0 = TAG_BLOCK_GET_ELEMENT(&sector_hint->points, sector_hint->points.count - 1, s_user_hint_sector_point);
		s_user_hint_sector_point* sector_point1 = TAG_BLOCK_GET_ELEMENT(&sector_hint->points, 0, s_user_hint_sector_point);

		real_point3d point0{};
		real_point3d point1{};
		ai_point_get_position(&sector_point0->position, &point0);
		ai_point_get_position(&sector_point1->position, &point1);

		real_vector3d normal0{};
		real_vector3d normal1{};
		vector3d_from_euler_angles2d(&normal0, &sector_point0->normal);
		vector3d_from_euler_angles2d(&normal1, &sector_point1->normal);

		point_from_line3d(&point0, &normal0, 0.02f, &point0);
		point_from_line3d(&point1, &normal1, 0.02f, &point1);

		if (color)
		{
			render_debug_line(true, &point0, &point1, color);
		}
		else
		{
			render_debug_line(true, &point0, &point1, global_real_argb_blue);
		}
	}
}

void render_sector_hints(const real_argb_color* color)
{
	const user_hint_data* user = TAG_BLOCK_GET_ELEMENT_SAFE(&global_scenario_get()->ai_user_hints, 0, const user_hint_data);
	if (user && user->flood_hints.count > 0)
	{
		const s_user_hint_flood* flood_hints = TAG_BLOCK_GET_ELEMENT(&user->flood_hints, 0, const s_user_hint_flood);
		for (int16 flood_sector_index = 0; flood_sector_index < (int16)flood_hints->flood_sector_hints.count; flood_sector_index++)
		{
			s_user_hint_sector* hint = TAG_BLOCK_GET_ELEMENT(&flood_hints->flood_sector_hints, flood_sector_index, s_user_hint_sector);
			if (user_hint_sector_on_active_structure(hint))
			{
				render_sector_hint(hint, color);
			}
		}
	}
}

void render_well_hint(const user_hint_well* well, const real_argb_color* color, bool full)
{
	for (int16 point_index = 1; point_index < (int16)well->points.count; point_index++)
	{
		user_hint_well_point* point_data0 = TAG_BLOCK_GET_ELEMENT(&well->points, point_index - 1, user_hint_well_point);
		user_hint_well_point* point_data1 = TAG_BLOCK_GET_ELEMENT(&well->points, point_index, user_hint_well_point);

		real_point3d point0{};
		real_point3d point1{};
		ai_point_get_position(&point_data0->position, &point0);
		ai_point_get_position(&point_data1->position, &point1);

		if (!color)
		{
			color = global_real_argb_blue;
		}

		if (point_data1->type == 1)
		{
			color = global_real_argb_red;
		}

		if (full)
		{
			render_arrow(&point0, &point1, color, TEST_BIT(well->flags, _user_hint_well_bidirectional_bit));
		}
		else
		{
			render_debug_line(true, &point0, &point1, color);
		}
	}
}

void render_well_hints(const real_argb_color* color, bool full)
{
	const user_hint_data* user = TAG_BLOCK_GET_ELEMENT_SAFE(&global_scenario_get()->ai_user_hints, 0, const user_hint_data);
	if (user)
	{
		for (int16 well_index = 0; well_index < (int16)user->well_hints.count; well_index++)
		{
			user_hint_well* well = TAG_BLOCK_GET_ELEMENT(&user->well_hints, well_index, user_hint_well);
			if (user_hint_well_on_active_structure(well))
			{
				render_well_hint(well, color, full);
			}
		}
	}
}

void user_hint_handle_parallelogram_point_move(user_hint_parallelogram* parallelogram, int16 point_index, const real_point3d* point)
{
#if 0
	ASSERT(VALID_INDEX(point_index, 4));

	bool valid = true;

	const user_hint_data* user_data = TAG_BLOCK_GET_ELEMENT_SAFE(&global_scenario_get()->ai_user_hints, 0, const user_hint_data);
	if (user_data)
	{
		for (int16 jump_index = 0; jump_index < user_data->jump_hints.count; jump_index++)
		{
			user_hint_jump* jump_hint = TAG_BLOCK_GET_ELEMENT(&user_data->jump_hints, jump_index, user_hint_jump);
			if (VALID_INDEX(jump_hint->geometry_index, user_data->parallelogram_geometry.count))
			{
				user_hint_parallelogram* other = TAG_BLOCK_GET_ELEMENT(&user_data->parallelogram_geometry, jump_hint->geometry_index, user_hint_parallelogram);
				if (other == parallelogram)
				{
					c_sector_ref sector_ref = sector_from_point(point, NULL);
					if (sector_ref.valid())
					{
						const sector* sector = pathfinding_get_sector(sector_ref);
						valid = TEST_BIT(sector->flags, _sector_floor_bit);
					}
				}
			}
		}
	}

	SET_BIT(parallelogram->points_invalid_flags, point_index, !valid);
#endif
}

bool user_hint_line_segment_on_active_structure(user_hint_line_segment* line)
{
	bool result = false;
	const scenario* scenario = global_scenario_get();
	int16 active_bsp_mask = (int16)global_structure_bsp_active_mask_get();
	bool ambiguous = false;
	for (int16 structure_index = 0; structure_index < (int16)scenario->structure_bsp_references.count; structure_index++)
	{
		if (TEST_BIT(active_bsp_mask, structure_index))
		{
			result = user_hint_line_segment_on_structure(line, structure_index, false, &ambiguous);
			if (result)
			{
				break;
			}
		}
	}
	return result;
}

bool user_hint_line_segment_on_structure(user_hint_line_segment* line, int16 structure_index, bool strict, bool* ambiguous)
{
	bool result = false;
	if (strict)
	{
		result = ai_point_on_structure(&line->point0, structure_index, ambiguous)
			&& ai_point_on_structure(&line->point1, structure_index, ambiguous);
	}
	else
	{
		result = ai_point_on_structure(&line->point0, structure_index, ambiguous)
			|| ai_point_on_structure(&line->point1, structure_index, ambiguous);
	}
	return result;
}

// Seems like this was somewhat replaced with `render_jump_hint`?
bool user_hint_render_jump(int16 jump_hint_index)
{
	bool result = false;
	const user_hint_data* user_data = TAG_BLOCK_GET_ELEMENT_SAFE(&global_scenario_get()->ai_user_hints, 0, const user_hint_data);
	if (user_data && VALID_INDEX(jump_hint_index, user_data->jump_hints.count))
	{
		user_hint_jump* jump_hint = TAG_BLOCK_GET_ELEMENT(&user_data->jump_hints, jump_hint_index, user_hint_jump);
		if (VALID_INDEX(jump_hint->geometry_index, user_data->parallelogram_geometry.count))
		{
			user_hint_parallelogram* parallelogram = TAG_BLOCK_GET_ELEMENT_SAFE(&user_data->parallelogram_geometry, jump_hint->geometry_index, user_hint_parallelogram);

			bool bi = TEST_BIT(parallelogram->flags, _user_hint_geometry_bidirectional);
			user_hint_render_parallelogram(parallelogram, bi);

			real_point3d point0{};
			real_point3d point1{};
			real_point3d point2{};
			real_point3d point3{};
			ai_point_get_position(&parallelogram->point0, &point0);
			ai_point_get_position(&parallelogram->point1, &point1);
			ai_point_get_position(&parallelogram->point2, &point2);
			ai_point_get_position(&parallelogram->point3, &point3);

			if (TEST_BIT(jump_hint->user_flags, _user_jump_hint_vault_bit))
			{
				user_hint_render_jump_vault_internal(&point0, &point1, &point2, &point3);
				if (bi)
				{
					user_hint_render_jump_vault_internal(&point2, &point3, &point0, &point1);
				}
			}

			if (parallelogram->point0.reference_frame() == parallelogram->point1.reference_frame() &&
				parallelogram->point2.reference_frame() == parallelogram->point3.reference_frame() &&
				parallelogram->point0.reference_frame() == parallelogram->point2.reference_frame())
			{
				int16 height_index = 0;
				for (; height_index < NUMBEROF(g_jump_height_names); height_index++)
				{
					if (ai_hint_test_jump(&point0, &point1, &point2, &point3, height_index))
					{
						break;
					}
				}

				{
					real_point3d mean_point;
					add_vectors3d((real_vector3d*)&point0, (real_vector3d*)&point1, (real_vector3d*)&mean_point);
					add_vectors3d((real_vector3d*)&mean_point, (real_vector3d*)&point2, (real_vector3d*)&mean_point);
					add_vectors3d((real_vector3d*)&mean_point, (real_vector3d*)&point3, (real_vector3d*)&mean_point);
					scale_vector3d((real_vector3d*)&mean_point, 0.25f, (real_vector3d*)&mean_point);

					if (height_index < NUMBEROF(g_jump_height_names))
					{
						render_debug_string_at_point(&mean_point,
							c_string_builder("[%s]", g_jump_height_names[height_index]).get_string(),
							global_real_argb_blue);
					}
					else
					{
						render_debug_string_at_point(&mean_point,
							c_string_builder("[infinite]", g_jump_height_names[height_index]).get_string(),
							global_real_argb_blue);
					}
				}
			}
		}
	}
	return result;
}

void user_hint_render_jump_vault_internal(real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_point3d const* point3)
{
	real_vector3d offset0{};
	real_vector3d offset1{};
	vector_from_points3d(point0, point2, &offset0);
	vector_from_points3d(point1, point3, &offset1);

	real_point3d vault_point0;
	real_point3d vault_point1;
	if (normalize3d(&offset0) > 0.0f && normalize3d(&offset1) > 0.0f)
	{
		int16 const k_num_ticks = 4;

		point_from_line3d(point0, &offset0, 0.15f, &vault_point0);
		point_from_line3d(&vault_point0, global_up3d, 0.4f, &vault_point0);
		point_from_line3d(point1, &offset1, 0.15f, &vault_point1);
		point_from_line3d(&vault_point1, global_up3d, 0.4f, &vault_point1);
		render_debug_line(true, &vault_point0, &vault_point1, global_real_argb_red);

		real_vector3d side{};
		vector_from_points3d(&vault_point0, &vault_point1, &side);
		point_from_line3d(&vault_point0, global_up3d, 0.2f, &vault_point0);
		point_from_line3d(&vault_point0, global_down3d, 2.0f * 0.2f, &vault_point1);

		real_point3d anchor_point = vault_point0;
		for (int16 index = 0; k_num_ticks; index++)
		{
			point_from_line3d(&anchor_point, &side, ((real32)index * 1.0f) / 3.0f, &vault_point0);
			point_from_line3d(&vault_point0, global_down3d, 2.0f * 0.2f, &vault_point1);
			render_debug_line(true, &vault_point0, &vault_point1, global_real_argb_red);
		}
	}
}

void user_hint_render_line_segment(const user_hint_line_segment* line_segment, const real_argb_color* color)
{
	real_point3d world_point0;
	real_point3d world_point1;
	ai_point_get_position(&line_segment->point0, &world_point0);
	ai_point_get_position(&line_segment->point1, &world_point1);

	if (!color)
	{
		color = global_real_argb_yellow;
	}

	render_debug_sphere(true, &world_point0, 0.06f, global_real_argb_blue);
	render_debug_sphere(true, &world_point1, 0.06f, global_real_argb_blue);

	real_point3d guide_point;
	if (world_point0.z < world_point1.z)
	{
		set_real_point3d(&guide_point, world_point0.x, world_point0.y, world_point1.z);
		render_debug_line(true, &world_point0, &guide_point, global_real_argb_white);
	}
	else
	{
		set_real_point3d(&guide_point, world_point1.x, world_point1.y, world_point0.z);
		render_debug_line(true, &world_point1, &guide_point, global_real_argb_white);
	}
}

void user_hint_render_parallelogram(const user_hint_parallelogram* parallelogram, bool bidirectional)
{
	real_point3d wp0{};
	real_point3d wp1{};
	real_point3d wp2{};
	real_point3d wp3{};
	ai_point_get_position(&parallelogram->point0, &wp0);
	ai_point_get_position(&parallelogram->point1, &wp1);
	ai_point_get_position(&parallelogram->point2, &wp2);
	ai_point_get_position(&parallelogram->point3, &wp3);

	render_debug_sphere(true, &wp0, 0.06f, TEST_BIT(parallelogram->points_invalid_flags, 0) ? global_real_argb_red : global_real_argb_blue);
	render_debug_sphere(true, &wp1, 0.06f, TEST_BIT(parallelogram->points_invalid_flags, 1) ? global_real_argb_red : global_real_argb_blue);
	render_debug_sphere(true, &wp2, 0.06f, TEST_BIT(parallelogram->points_invalid_flags, 2) ? global_real_argb_red : global_real_argb_blue);
	render_debug_sphere(true, &wp3, 0.06f, TEST_BIT(parallelogram->points_invalid_flags, 3) ? global_real_argb_red : global_real_argb_blue);

	render_debug_line(true, &wp0, &wp1, global_real_argb_blue);
	render_debug_line(true, &wp2, &wp3, global_real_argb_blue);

	render_arrow(&wp0, &wp2, global_real_argb_yellow, bidirectional || TEST_BIT(parallelogram->flags, 0));
	render_arrow(&wp1, &wp3, global_real_argb_yellow, bidirectional || TEST_BIT(parallelogram->flags, 0));
}

bool user_hint_sector_on_active_structure(s_user_hint_sector* sector_hint)
{
	bool result = false;
	const scenario* scenario = global_scenario_get();
	int16 active_bsp_mask = (int16)global_structure_bsp_active_mask_get();
	bool ambiguous = false;
	for (int16 structure_index = 0; structure_index < (int16)scenario->structure_bsp_references.count; structure_index++)
	{
		if (TEST_BIT(active_bsp_mask, structure_index))
		{
			result = user_hint_sector_on_structure(sector_hint, structure_index, false, &ambiguous);
			if (result)
			{
				break;
			}
		}
	}
	return result;
}

bool user_hint_sector_on_structure(const s_user_hint_sector* sector_hint, int16 structure_index, bool strict, bool* ambiguous)
{
	bool result = false;
	*ambiguous = false;
	if (sector_hint->points.count > 0)
	{
		result = true;
		for (int32 point_index = 0; point_index < sector_hint->points.count; point_index++)
		{
			s_user_hint_sector_point* sector_point = TAG_BLOCK_GET_ELEMENT(&sector_hint->points, point_index, s_user_hint_sector_point);
			if (ai_point_on_structure(&sector_point->position, structure_index, ambiguous))
			{
				if (!strict)
				{
					result = true;
					break;
				}
			}
			else if (strict)
			{
				result = false;
				break;
			}
		}
	}
	return result;
}

bool user_hint_well_on_active_structure(user_hint_well* well)
{
	bool result = false;
	const scenario* scenario = global_scenario_get();
	int16 active_bsp_mask = (int16)global_structure_bsp_active_mask_get();
	bool ambiguous = false;
	for (int16 structure_index = 0; structure_index < (int16)scenario->structure_bsp_references.count; structure_index++)
	{
		if (TEST_BIT(active_bsp_mask, structure_index))
		{
			result = user_hint_well_on_structure(well, structure_index, false, &ambiguous);
			if (result)
			{
				break;
			}
		}
	}
	return result;
}

bool user_hint_well_on_structure(const user_hint_well* well, int16 structure_index, bool strict, bool* ambiguous)
{
	bool result = true;
	for (int32 point_index = 0; point_index < well->points.count; point_index++)
	{
		user_hint_well_point* point = TAG_BLOCK_GET_ELEMENT(&well->points, point_index, user_hint_well_point);
		if (ai_point_on_structure(&point->position, structure_index, ambiguous))
		{
			if (!strict)
			{
				result = true;
				break;
			}
		}
		else if (strict)
		{
			result = false;
			break;
		}
	}
	return result;
}

