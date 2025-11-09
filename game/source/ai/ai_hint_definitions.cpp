#include "ai/ai_hint_definitions.hpp"

#include "ai/ai_reference_frame.hpp"
#include "ai/sector.hpp"
#include "ai/sector_definitions.hpp"
#include "cache/cache_files.hpp"
#include "objects/objects.hpp"
#include "render/render_debug.hpp"
#include "scenario/scenario.hpp"

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
//.text:0148B1C0 ; bool __cdecl ai_hint_get_jump_destination_rail(int16, hint_jump_data*, const real_point3d*, real_point3d*, real_point3d*, c_sector_ref*, int16*)
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
//.text:0148BD20 ; bool __cdecl ai_hint_test_jump(const real_point3d*, const real_point3d*, const real_point3d*, const real_point3d*, int16)
//.text:0148BE80 ; bool __cdecl ai_hint_vault_range_valid(const real_point3d*, const real_point3d*, const special_movement*)
//.text:0148BF30 ; bool __cdecl ai_link_hint_traversable(const struct pathfinding_data*, int32)

bool user_hint_well_on_active_structure(user_hint_well* well)
{
	return false;

	//bool result = false;
	//
	//scenario const* scenario = global_scenario_get();
	//int16 active_bsp_mask = global_designer_zone_active_mask_get();
	//
	//bool ambiguous;
	//{
	//	int16 structure_index;
	//}
}

void ai_render_hints()
{
	//{
	//	const struct scenario* scenario;
	//	int16 bsp_mask;
	//	{
	//		int16 structure_index;
	//		{
	//			pathfinding_data* pf_data;
	//			{
	//				int32 hint_index;
	//				{
	//					pathfinding_hint_data* hint;
	//				}
	//				{
	//					int32 link_index;
	//					{
	//						sector_link* link;
	//						{
	//							real_point3d* p0;
	//							real_point3d* p1;
	//							real32 length;
	//							real_vector3d link_vector;
	//							{
	//								real_point3d center;
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
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
	if (global_scenario_get()->ai_user_hints.count > 0)
	{
		const user_hint_data* hints = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->ai_user_hints, 0, const user_hint_data);
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
	if (global_scenario_get()->ai_user_hints.count > 0)
	{
		const user_hint_data* user = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->ai_user_hints, 0, const user_hint_data);
		if (user)
		{
			for (int16 flight_hint_index = 0; flight_hint_index < (int16)user->flight_hints.count; flight_hint_index++)
			{
				user_flight_hint* flight_hint = TAG_BLOCK_GET_ELEMENT(&user->flight_hints, flight_hint_index, user_flight_hint);
				render_flight_hint(flight_hint);
			}
		}
	}
}

void render_giant_sector_hints(const real_argb_color* color)
{
	//{
	//	const user_hint_data* user;
	//	{
	//		const s_user_hint_giant* giant_data;
	//		{
	//			int16 sector_hint_index;
	//			{
	//				s_user_hint_sector* sector_hint;
	//			}
	//		}
	//	}
	//}
}

void render_giant_rail_hints(const real_argb_color* color)
{
	//{
	//	const user_hint_data* user;
	//	{
	//		const s_user_hint_giant* giant_data;
	//		{
	//			int16 rail_index;
	//			{
	//				s_user_hint_giant_rail* rail;
	//				user_hint_line_segment* line_segment;
	//			}
	//		}
	//	}
	//}
}

void render_hoist_hint(const pathfinding_data* pf_data, int32 hint_index)
{
	//{
	//	pathfinding_hint_data* hint;
	//	const hint_hoist_data* hoist;
	//	{
	//		real_point3d* point0;
	//		real_point3d* point1;
	//		real_point3d midpoint;
	//		{
	//			int16 height_index;
	//		}
	//	}
	//}
}

void render_sector_hint(const s_user_hint_sector* sector_hint, const real_argb_color* color)
{
	//{
	//	{
	//		int16 point_index;
	//		{
	//			real_point3d point0;
	//			real_point3d point1;
	//			s_user_hint_sector_point* sector_point0;
	//			s_user_hint_sector_point* sector_point1;
	//			real_vector3d normal0;
	//			real_vector3d normal1;
	//		}
	//		{
	//			real_point3d point0;
	//			real_point3d point1;
	//			s_user_hint_sector_point* sector_point0;
	//			s_user_hint_sector_point* sector_point1;
	//			real_vector3d normal0;
	//			real_vector3d normal1;
	//		}
	//	}
	//}
}

void render_sector_hints(const real_argb_color* color)
{
	//{
	//	const user_hint_data* user;
	//	{
	//		const s_user_hint_flood* flood_hints;
	//		{
	//			int16 flood_sector_index;
	//			{
	//				s_user_hint_sector* hint;
	//			}
	//		}
	//	}
	//}
}

void render_well_hint(const user_hint_well* well, const real_argb_color* color, bool full)
{
	if (well->points.count > 1)
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
}

void render_well_hints(const real_argb_color* color, bool full)
{
	if (global_scenario_get()->ai_user_hints.count > 0)
	{
		const user_hint_data* user = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->ai_user_hints, 0, const user_hint_data);
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
}

void user_hint_handle_parallelogram_point_move(user_hint_parallelogram* parallelogram, int16 point_index, const real_point3d* point)
{
	//{
	//	const user_hint_data* user_data;
	//	bool valid;
	//	{
	//		int16 jump_index;
	//		{
	//			user_hint_jump* jump_hint;
	//			{
	//				user_hint_parallelogram* other;
	//				{
	//					c_sector_ref sector_ref;
	//					{
	//						const sector* sector;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}

bool user_hint_render_jump(int16 jump_hint_index)
{
	return false;

	//{
	//	real_vector3d offset0;
	//	real_vector3d offset1;
	//	real_point3d vault_point0;
	//	real_point3d vault_point1;
	//	{
	//		real_vector3d side;
	//		const int16 k_num_ticks;
	//		{
	//			real_point3d anchor_point;
	//			{
	//				int16 index;
	//			}
	//		}
	//	}
	//}
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

