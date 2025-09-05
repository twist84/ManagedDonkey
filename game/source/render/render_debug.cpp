#include "render/render_debug.hpp"

#include "ai/ai_debug.hpp"
#include "ai/ai_profile.hpp"
#include "ai/path_obstacle_avoidance.hpp"
#include "cache/fmod_sound_cache.hpp"
#include "cache/pc_geometry_cache.hpp"
#include "cache/pc_texture_cache.hpp"
#include "cache/restricted_memory_regions.hpp"
#include "camera/observer.hpp"
#include "cseries/cseries_events.hpp"
#include "cubemaps/cubemap_debug.hpp"
#include "cutscene/recorded_animations.hpp"
#include "data_mining/data_mine_usability.hpp"
#include "editor/editor_flags.hpp"
#include "editor/editor_render_stubs.hpp"
#include "effects/effects.hpp"
#include "game/game.hpp"
#include "hs/hs_runtime.hpp"
#include "interface/chud/chud_draw.hpp"
#include "interface/interface.hpp"
#include "interface/interface_constants.hpp"
#include "items/projectiles.hpp"
#include "math/color_math.hpp"
#include "math/random_math.hpp"
#include "networking/network_utilities.hpp"
#include "objects/damage.hpp"
#include "objects/object_early_movers.hpp"
#include "objects/object_types.hpp"
#include "physics/collision_debug.hpp"
#include "physics/collision_usage.hpp"
#include "physics/havok.hpp"
#include "rasterizer/rasterizer.hpp"
#include "render/old_render_debug.hpp"
#include "render/render_cameras.hpp"
#include "render/render_debug_structure.hpp"
#include "render/render_error_report.hpp"
#include "render/render_visibility.hpp"
#include "render/simple_font.hpp"
#include "render/views/render_view.hpp"
#include "saved_games/saved_film.hpp"
#include "saved_games/saved_film_history.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "simulation/simulation.hpp"
#include "sound/sound_manager.hpp"
#include "structures/structure_detail_objects.hpp"
#include "structures/structures.hpp"
#include "text/draw_string.hpp"
#include "visibility/visibility_collection.hpp"

#include <math.h>
#include <windows.h>

#define MAXIMUM_CACHE_STRING_LENGTH 4096

enum e_render_debug_type
{
	_render_debug_type_circle = 0,
	_render_debug_type_point,
	_render_debug_type_line,
	_render_debug_type_line2d,
	_render_debug_type_sphere,
	_render_debug_type_cylinder,
	_render_debug_type_pill,
	_render_debug_type_box,
	_render_debug_type_box_outline,
	_render_debug_type_triangle,
	_render_debug_type_string,
	_render_debug_type_string_at_point,
	_render_debug_type_box2d_outline,

	k_render_debug_type_count
};

struct cache_entry
{
	int16 type;
	int16 layer;
	real32 sort_key;

	union
	{
		struct // _render_debug_type_circle
		{
			real_plane3d plane;
			int16 projection_axis;
			bool projection_sign;
			real_point2d center;
			real32 radius;
			real_argb_color color;
			real32 offset;
		} circle;

		struct // _render_debug_type_point
		{
			real_point3d point;
			real32 scale;
			real_argb_color color;
		} point;

		struct // _render_debug_type_line
		{
			real_point3d point0;
			real_point3d point1;
			real_argb_color color0;
			real_argb_color color1;
		} line;

		struct // _render_debug_type_line2d
		{
			real_point2d point0;
			real_point2d point1;
			real_argb_color color0;
			real_argb_color color1;
		} line2d;

		struct // _render_debug_type_sphere
		{
			real_point3d center;
			real32 radius;
			real_argb_color color;
		} sphere;

		struct // _render_debug_type_cylinder
		{
			real_point3d base;
			real_vector3d height;
			real32 width;
			real_argb_color color;
		} cylinder;

		struct // _render_debug_type_pill
		{
			real_point3d base;
			real_vector3d height;
			real32 width;
			real_argb_color color;
		} pill;

		struct // _render_debug_type_box, _render_debug_type_box_outline
		{
			real_rectangle3d bounds;
			real_argb_color color;
		} box;

		struct // _render_debug_type_triangle
		{
			real_point3d point0;
			real_point3d point1;
			real_point3d point2;
			real_argb_color color;
		} triangle;

		struct // _render_debug_type_string
		{
			int32 string_index;
			int16 tab_stops[16];
			int16 tab_stop_count;
		} string;
		
		struct // _render_debug_type_string_at_point
		{
			int32 string_index;
			real_point3d point;
			real_argb_color color;
		} string_at_point;

		struct // _render_debug_type_box2d_outline
		{
			real_rectangle2d bounds;
			real_argb_color color;
		} box2d_outline;

		byte type_storage[0x38];
	};
};

struct s_render_debug_globals
{
	bool active;
	bool drawing_cached_geometry;
	bool use_simple_font_text_rendering;

	real32 group_key;

	int32 group_ids[2];
	int32 group_level;

	int16 cache_count;
	int16 game_tick_cache_count;

	bool inside_game_tick;

	cache_entry cache[8192];

	int16 cache_string_length;
	int16 game_tick_cache_string_length;

	char cache_string[MAXIMUM_CACHE_STRING_LENGTH];
};
static_assert(sizeof(s_render_debug_globals) == 0x81020);

static s_render_debug_globals _render_debug_globals{};

thread_local s_render_debug_globals* g_render_debug_globals = NULL;

int32 type_list[] = { 0, 0, 0, 3, 0, 0, 0, 1, 0, 1, 2, 2, 0 };

void __cdecl render_debug_initialize()
{
	g_render_debug_globals = &_render_debug_globals;
	csmemset(g_render_debug_globals, 0, sizeof(s_render_debug_globals));

	g_render_debug_globals->use_simple_font_text_rendering = true;
}

s_render_debug_globals* __cdecl get_render_debug_globals()
{
	ASSERT(g_render_debug_globals);

	return g_render_debug_globals;
}

bool __cdecl render_debug_allowed_in_current_thread()
{
	return restricted_region_locked_for_current_thread(2 /* render thread */) && c_rasterizer::rasterizer_thread_owns_device();
}

bool __cdecl render_debug_active()
{
	return get_render_debug_globals()->active;
}

bool __cdecl render_debug_cache_currently_drawing()
{
	return get_render_debug_globals()->drawing_cached_geometry;
}

void __cdecl render_debug_text_using_simple_font(bool use_simple_font)
{
	get_render_debug_globals()->use_simple_font_text_rendering = use_simple_font;
}

void __cdecl render_debug_reset_cache_to_game_tick_entires()
{
	s_render_debug_globals* render_debug_globals = get_render_debug_globals();

	render_debug_globals->cache_count = render_debug_globals->game_tick_cache_count;
	render_debug_globals->cache_string_length = render_debug_globals->game_tick_cache_string_length;
}

void __cdecl render_debug_notify_game_tick_begin()
{
	s_render_debug_globals* render_debug_globals = get_render_debug_globals();

	ASSERT(!render_debug_globals->inside_game_tick);
	render_debug_globals->inside_game_tick = true;

	render_debug_globals->cache_count = 0;
	render_debug_globals->game_tick_cache_count = 0;

	render_debug_globals->cache_string_length = 0;
	render_debug_globals->game_tick_cache_string_length = 0;
	render_debug_globals->cache_string[0] = 0;
}

void __cdecl render_debug_notify_game_tick_end()
{
	s_render_debug_globals* render_debug_globals = get_render_debug_globals();

	ASSERT(render_debug_globals->inside_game_tick);
	render_debug_globals->inside_game_tick = false;

	render_debug_globals->game_tick_cache_count = render_debug_globals->cache_count;
	render_debug_globals->game_tick_cache_string_length = render_debug_globals->cache_string_length;
}

void __cdecl rasterizer_debug_line(const real_point3d* p0, const real_point3d* p1, const real_argb_color* color0, const real_argb_color* color1)
{
	ASSERT(p0 && p1 && color0);

	real_argb_color color0_ = *color0;
	real_argb_color color1_ = *color1;
	color0_.alpha = 1.0f;
	color1_.alpha = 1.0f;

	for (int32 i = 0; i < NUMBEROF(color0_.rgb.n); i++)
	{
		color0_.rgb.n[i] = color0_.rgb.n[i] * color0_.rgb.n[i];
		color1_.rgb.n[i] = color1_.rgb.n[i] * color1_.rgb.n[i];
	}

	c_rasterizer::draw_debug_line(*p0, *p1, real_argb_color_to_pixel32(&color0_), real_argb_color_to_pixel32(&color1_));
}

void __cdecl rasterizer_debug_line2d(const real_point2d* p0, const real_point2d* p1, const real_argb_color* color0, const real_argb_color* color1)
{
	ASSERT(p0 && p1 && color0);

	real_point3d p0_{};
	real_point3d p1_{};
	set_real_point3d(&p0_, p0->x, p0->y, 0.0f);
	set_real_point3d(&p1_, p1->x, p1->y, 0.0f);

	real_argb_color color0_ = *color0;
	real_argb_color color1_ = *color1;
	color0_.alpha = 1.0f;
	color1_.alpha = 1.0f;

	c_rasterizer::draw_debug_line2d(p0_, p1_, real_argb_color_to_pixel32(&color0_), real_argb_color_to_pixel32(&color1_));
}

void __cdecl rasterizer_debug_triangle(const real_point3d* point0, const real_point3d* point1, const real_point3d* point2, const real_argb_color* color)
{
	rasterizer_vertex_debug vertex_debug[3]{};
	vertex_debug[0].position = *point0;
	vertex_debug[1].position = *point1;
	vertex_debug[2].position = *point2;

	uns32 _color = real_argb_color_to_pixel32(color);
	vertex_debug[0].color = _color;
	vertex_debug[1].color = _color;
	vertex_debug[2].color = _color;

	c_rasterizer::draw_debug_polygon(vertex_debug, NUMBEROF(vertex_debug) / 3, c_rasterizer_index_buffer::_primitive_type_triangle_strip); // D3DPT_TRIANGLESTRIP
}

int __cdecl render_debug_cache_sort_function(const void* a, const void* b)
{
	const cache_entry* entry0 = (const cache_entry*)a;
	const cache_entry* entry1 = (const cache_entry*)b;

	if (entry0->layer < entry1->layer)
		return false;

	if (entry0->layer > entry1->layer)
		return true;

	if (type_list[entry0->type] < type_list[entry1->type])
		return false;

	if (type_list[entry0->type] > type_list[entry1->type])
		return true;

	if (entry0->sort_key < entry1->sort_key)
		return false;

	return entry0->sort_key > entry1->sort_key;
}

void __cdecl render_debug_cache_sort()
{
	s_render_debug_globals* g_render_debug_globals = get_render_debug_globals();
	if (g_render_debug_globals->cache_count > 0)
	{
		qsort(g_render_debug_globals->cache,
			g_render_debug_globals->game_tick_cache_count,
			sizeof(cache_entry),
			render_debug_cache_sort_function);

		qsort(g_render_debug_globals->cache + g_render_debug_globals->game_tick_cache_count,
			g_render_debug_globals->cache_count - g_render_debug_globals->game_tick_cache_count,
			sizeof(cache_entry),
			render_debug_cache_sort_function);
	}
}

void __cdecl render_debug_begin(bool render_game_tick_cache, bool only_render_strings, bool clear_cache)
{
	s_render_debug_globals* g_render_debug_globals = get_render_debug_globals();
	ASSERT(!g_render_debug_globals->active);
	
	g_render_debug_globals->active = true;

	c_rasterizer::set_z_buffer_mode(c_rasterizer::_z_buffer_mode_debug_geometry);
	c_rasterizer::set_sampler_filter_mode(0, (c_rasterizer::e_sampler_filter_mode)0);
	c_rasterizer::set_cull_mode(c_rasterizer::_cull_mode_cw);
	c_rasterizer::set_color_write_enable(0, 7);
	c_rasterizer::set_alpha_blend_mode(c_rasterizer::_alpha_blend_alpha_blend);

	render_debug_cache_draw(render_game_tick_cache, only_render_strings, clear_cache);
}

void __cdecl render_debug_end(bool render_game_tick_cache, bool only_render_strings, bool clear_cache)
{
	s_render_debug_globals* g_render_debug_globals = get_render_debug_globals();
	ASSERT(g_render_debug_globals->active);
	
	render_debug_cache_sort();
	render_debug_cache_draw(render_game_tick_cache, only_render_strings, clear_cache);
	g_render_debug_globals->active = false;
}

void __cdecl render_debug_visibility_render()
{
	if (visibility_debug_visible_clusters)
	{
		const s_visibility_region* region = get_global_camera_collection()->get_region();
		if (simple_font::begin_rendering(1.0f, true))
		{
			char output_text[256]{};
			csnzprintf(output_text, sizeof(output_text), "vis clusters: (bsp:cluster)");
			for (int32 cluster_index = 0; cluster_index < region->cluster_count; cluster_index++)
			{
				const visibility_cluster& cluster = region->clusters[cluster_index];
				csnzappendf(output_text, sizeof(output_text), "%d:%d, ",
					cluster.cluster_reference.bsp_index,
					cluster.cluster_reference.cluster_index);
			}
			simple_font::print(100, 500, 0xFFFFFFFF, output_text, csstrnlen(output_text, sizeof(output_text)), false);
		}
		simple_font::end_rendering();
	}
}

void __cdecl render_debug_clients(int32 user_index)
{
	geometry_cache_debug_render();
	texture_cache_debug_render();
	sound_cache_debug_render();
	file_activity_debug_render();

	if (game_in_progress())
	{
		// this does not belong here
		sound_debug_render();

		c_cubemap_debug::render();
		render_debug_camera_projection();
		render_debug_objects();
		render_debug_object_damage();
		render_debug_projectiles();
		render_debug_damage_effects();
		render_debug_trigger_volumes();
		render_debug_recording();
		render_debug_detail_objects();
		render_debug_obstacle_path();
		render_debug_fog_planes();
		render_debug_player();
		render_debug_camera();
		render_debug_structure();
		render_debug_bsp();
		render_debug_input();
		render_debug_structure_decals();
		ai_debug_render();
		ai_profile_render();
		aim_assist_debug_render();
		collision_debug_render();
		chud_debug_render();
		havok_debug_render();
		object_early_mover_render_debug();
		collision_log_render();
		game_pvs_debug_render();
		players_debug_render();
		simulation_debug_render();
		//voice_render();
		debug_render_observer();
		render_debug_scenario_comments();
		render_report_render_debug(user_index, true);
		saved_film_manager_render_debug();
		saved_film_history_render_debug();
		render_debug_visibility_render();
		//debug_aim_assist_targets();
		events_debug_render();
		data_mine_render_mission_segment();
		bandwidth_profiler_render();
		//scenario_zones_render_debug();

		// this does not belong here
		// location_messages
		//real_point3d point = { 81.6f, -72.4f, 7.2f };
		//render_debug_point(true, &point, 2.0f, global_real_argb_magenta);
		//render_debug_string_at_point(&point, "test location", global_real_argb_magenta);
	}

	editor_render_debug();
}

int32 __cdecl render_debug_add_cache_string(const char* string)
{
	s_render_debug_globals* render_debug_globals = get_render_debug_globals();

	for (int32 i = 0; i < render_debug_globals->cache_string_length; i++)
	{
		if (csstrcmp(string, &render_debug_globals->cache_string[i]) == 0)
		{
			if (i != NONE)
				return i;
		}
	}

	int16 cache_string_length = render_debug_globals->cache_string_length;
	if (cache_string_length < MAXIMUM_CACHE_STRING_LENGTH)
	{
		int32 string_offset = cache_string_length;
		csstrnzcpy(&g_render_debug_globals->cache_string[cache_string_length], string, MAXIMUM_CACHE_STRING_LENGTH - cache_string_length);
		int16 v8 = (int16)strlen(string);
		// length assert

		g_render_debug_globals->cache_string_length += v8 + 1;
		int16 v9 = g_render_debug_globals->cache_string_length;
		if (v9 > 4095)
			v9 = 4095;
		g_render_debug_globals->cache_string_length = v9;

		return string_offset;
	}

	static bool render_debug_cache_string_overflow = false;
	if (!render_debug_cache_string_overflow)
	{
		event(_event_warning, "render debug cache string overflow");
		render_debug_cache_string_overflow = true;
	}

	return NONE;
}

const real_argb_color* __cdecl render_debug_random_color(real_argb_color* color)
{
	return render_debug_unique_color(_random(get_local_random_seed_address(), 0, __FILE__, __LINE__), color);
}

real_argb_color const global_render_debug_colors[17]
{
	{ 1.0f, 1.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 0.5f, 0.0f },
	{ 1.0f, 0.0f, 1.0f, 0.5f },
	{ 1.0f, 0.5f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 0.0f, 0.5f },
	{ 1.0f, 0.5f, 1.0f, 0.0f },
	{ 1.0f, 0.0f, 0.5f, 1.0f },
	{ 1.0f, 0.5f, 0.0f, 0.0f },
	{ 1.0f, 0.0f, 0.5f, 0.0f },
	{ 1.0f, 0.0f, 0.0f, 0.5f },
	{ 1.0f, 1.0f, 1.0f, 0.5f },
	{ 1.0f, 1.0f, 0.5f, 1.0f },
	{ 1.0f, 0.5f, 1.0f, 1.0f },
	{ 1.0f, 0.5f, 0.5f, 0.0f },
	{ 1.0f, 0.0f, 0.5f, 0.5f },
	{ 1.0f, 0.5f, 0.0f, 0.5f },
};

const real_argb_color* __cdecl render_debug_unique_color(int32 index, real_argb_color* color)
{
	ASSERT(color);

	*color = global_render_debug_colors[abs(index) % NUMBEROF(global_render_debug_colors)];

	return color;
}

//render_debug_spray

void __cdecl render_debug_point2d(bool draw_immediately, const real_plane3d* plane, int16 projection_axis, bool projection_sign, const real_point2d* point, real32 scale, const real_argb_color* color, real32 a8)
{
	ASSERT(plane);
	ASSERT(point);
	ASSERT(color);

	real_point3d q{};
	project_point2d(point, plane, projection_axis, projection_sign, &q);
	ASSERT(VALID_INDEX(projection_axis, NUMBEROF(q.n)));

	q.n[projection_axis] += projection_sign ? a8 : -a8;

	render_debug_point(draw_immediately, &q, scale, color);
}

void __cdecl render_debug_line2d(const real_point2d* point0, const real_point2d* point1, const real_argb_color* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(color);

	if (render_debug_draw_immediately(color))
	{
		rasterizer_debug_line2d(point0, point1, color, color);
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_line2d, point0, point1, color, color);
	}
}

void __cdecl render_debug_line2d(bool draw_immediately, const real_plane3d* plane, int16 projection_axis, bool projection_sign, const real_point2d* p0, const real_point2d* p1, const real_argb_color* color, real32 a8)
{
	ASSERT(plane);
	ASSERT(p0);
	ASSERT(p1);
	ASSERT(color);

	real_point3d point0{};
	real_point3d point1{};

	project_point2d(p0, plane, projection_axis, projection_sign, &point0);
	project_point2d(p1, plane, projection_axis, projection_sign, &point1);

	point0.n[projection_axis] += projection_sign ? a8 : -a8;
	point1.n[projection_axis] += projection_sign ? a8 : -a8;

	render_debug_line(draw_immediately, &point0, &point1, color);
}

void __cdecl render_debug_vector2d(bool draw_immediately, const real_plane3d* plane, int16 projection_axis, bool projection_sign, const real_point2d* point, const real_vector2d* vector, real32 scale, const real_argb_color* color, real32 a9)
{
	ASSERT(plane);
	ASSERT(point);
	ASSERT(vector);
	ASSERT(color);

	real_point2d point1{};
	point_from_line2d(point, vector, scale, &point1);
	render_debug_line2d(draw_immediately, plane, projection_axis, projection_sign, point, &point1, color, a9);
}

void __cdecl render_debug_circle(bool draw_immediately, const real_plane3d* plane, int16 projection_axis, bool projection_sign, const real_point2d* center, real32 radius, const real_argb_color* color, real32 a8)
{
	ASSERT(plane);
	ASSERT(center);
	ASSERT(color);

	if (/*draw_immediately || */render_debug_draw_immediately(color))
	{
		real_point2d circle_points[CIRCLE_DIVISIONS + 1]{};
		render_debug_build_circle_points(radius, circle_points, CIRCLE_DIVISIONS + 1);
		for (int32 i = 0; i < CIRCLE_DIVISIONS; i++)
		{
			real_point2d* circle_point0 = &circle_points[i];
			real_point2d* circle_point1 = &circle_points[i + 1];

			real_point2d point0{};
			real_point2d point1{};

			set_real_point2d(&point0, (center->x + circle_point0->x), (center->y + circle_point0->y));
			set_real_point2d(&point1, (center->x + circle_point1->x), (center->y + circle_point1->y));
			render_debug_line2d(draw_immediately, plane, projection_axis, projection_sign, &point0, &point1, color, a8);
		}
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_circle, plane, projection_axis, projection_sign, center, radius, color);
	}
}

void __cdecl render_debug_circle(bool draw_immediately, const real_point3d* center, const real_vector3d* normal, real32 radius, const real_argb_color* color)
{
	render_debug_polygon_regular(draw_immediately, center, CIRCLE_DIVISIONS, normal, radius, color);
}

void __cdecl render_debug_polygon_regular(bool draw_immediately, const real_point3d* center, int32 point_count, const real_vector3d* normal, real32 radius, const real_argb_color* color)
{
	ASSERT(center != NULL);
	ASSERT(normal != NULL);
	ASSERT(point_count <= CIRCLE_DIVISIONS);

	real_matrix4x3 matrix{};
	matrix.forward = *normal;
	generate_up_vector3d(&matrix.forward, &matrix.up);
	cross_product3d(&matrix.up, &matrix.forward, &matrix.left);
	matrix.position = *global_origin3d;
	matrix.scale = 1.0f;

	// asserts

	real_point2d circle_points[CIRCLE_DIVISIONS + 1]{};
	render_debug_build_circle_points(radius, circle_points, point_count + 1);

	for (int32 i = 0; i < point_count; i++)
	{
		real_point3d point0{};
		real_point3d point1{};

		set_real_point3d(&point0, 0.0f, circle_points[i].x, circle_points[i].y);
		set_real_point3d(&point1, 0.0f, circle_points[i + 1].x, circle_points[i + 1].y);

		matrix4x3_transform_point(&matrix, &point0, &point0);
		matrix4x3_transform_point(&matrix, &point1, &point1);

		point0.x += center->x;
		point0.y += center->y;
		point0.z += center->z;

		point1.x += center->x;
		point1.y += center->y;
		point1.z += center->z;

		render_debug_line(draw_immediately, &point0, &point1, color);
	}

}

void __cdecl render_debug_point(bool draw_immediately, const real_point3d* point, real32 scale, const real_argb_color* color)
{
	ASSERT(point);
	ASSERT(color);

	if (/*draw_immediately || */render_debug_draw_immediately(color))
	{
		scale *= 0.5f;
		real_point3d points[6]{};
		set_real_point3d(points, point->x - scale, point->y, point->z);
		set_real_point3d(&points[1], point->x + scale, point->y, point->z);
		set_real_point3d(&points[2], point->x, point->y - scale, point->z);
		set_real_point3d(&points[3], point->x, point->y + scale, point->z);
		set_real_point3d(&points[4], point->x, point->y, point->z - scale);
		set_real_point3d(&points[5], point->x, point->y, point->z + scale);

		for (int32 i = 0; i < 6; i += 2)
			render_debug_line(true, &points[i], &points[i + 1], color);
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_point, point, scale, color);
	}
}

void __cdecl render_debug_line(bool draw_immediately, const real_point3d* point0, const real_point3d* point1, const real_argb_color* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(color);

	if (/*draw_immediately || */render_debug_draw_immediately(color))
	{
		rasterizer_debug_line(point0, point1, color, color);
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_line, point0, point1, color, color);
	}
}

void __cdecl render_debug_line_shaded(bool draw_immediately, const real_point3d* point0, const real_point3d* point1, const real_argb_color* color0, const real_argb_color* color1)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(color0);
	ASSERT(color1);

	if (/*draw_immediately || */(render_debug_draw_immediately(color0) && render_debug_draw_immediately(color1)))
	{
		rasterizer_debug_line(point0, point1, color0, color1);
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_line, point0, point1, color0, color1);
	}
}

void __cdecl render_debug_line2d_shaded(const real_point2d* point0, const real_point2d* point1, const real_argb_color* color0, const real_argb_color* color1)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(color0);
	ASSERT(color1);

	if (render_debug_draw_immediately(color0) && render_debug_draw_immediately(color1))
	{
		rasterizer_debug_line2d(point0, point1, color0, color1);
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_line2d, point0, point1, color0, color1);
	}
}

void __cdecl render_debug_line_unclipped(bool draw_immediately, const real_point3d* point0, const real_point3d* point1, const real_argb_color* color)
{
	ASSERT(point0);
	ASSERT(point1);

	real_matrix4x3 camera{};
	c_player_view::get_player_render_camera_orientation(&camera);

	real_point3d p0 = *point0;
	real_point3d p1 = *point1;

	real_vector3d vector0{};
	real_vector3d vector1{};
	vector_from_points3d(&camera.position, &p0, &vector0);
	vector_from_points3d(&camera.position, &p1, &vector1);

	real32 clip_distance = magnitude3d(&vector0);
	if (magnitude3d(&vector0) <= magnitude3d(&vector1))
		clip_distance = magnitude3d(&vector1);

	real32 z_far = c_player_view::get_current()->get_rasterizer_camera()->z_far;
	if (clip_distance > (0.5f * z_far))
	{
		real32 distance = (0.5f * z_far) / clip_distance;
		point_from_line3d(&camera.position, &vector0, distance, &p0);
		point_from_line3d(&camera.position, &vector1, distance, &p1);
	}
	render_debug_line(draw_immediately, &p0, &p1, color);
}

void __cdecl render_debug_line_non_occluded(bool draw_immediately, const real_point3d* point0, const real_point3d* point1, const real_argb_color* color)
{
	ASSERT(point0);
	ASSERT(point1);

	real_matrix4x3 camera{};
	c_player_view::get_player_render_camera_orientation(&camera);

	real_point3d p0 = *point0;
	real_point3d p1 = *point1;

	real_vector3d vector0{};
	real_vector3d vector1{};
	vector_from_points3d(&camera.position, &p0, &vector0);
	vector_from_points3d(&camera.position, &p1, &vector1);

	real32 clip_distance = magnitude3d(&vector0);
	if (magnitude3d(&vector0) <= magnitude3d(&vector1))
		clip_distance = magnitude3d(&vector1);

	real32 z_near = c_player_view::get_current()->get_rasterizer_camera()->z_near;
	if (clip_distance > (0.5f * z_near))
	{
		real32 distance = (0.5f * z_near) / clip_distance;
		point_from_line3d(&camera.position, &vector0, distance, &p0);
		point_from_line3d(&camera.position, &vector1, distance, &p1);
	}
	render_debug_line(draw_immediately, &p0, &p1, color);
}

void __cdecl render_debug_vector(bool draw_immediately, const real_point3d* point, const real_vector3d* vector, real32 scale, const real_argb_color* color)
{
	ASSERT(point);
	ASSERT(vector);
	ASSERT(color);

	real_point3d point1{};
	point_from_line3d(point, vector, scale, &point1);
	render_debug_line(draw_immediately, point, &point1, color);
}

void __cdecl render_debug_tick(bool draw_immediately, const real_point3d* point, const real_vector3d* vector, real32 scale, const real_argb_color* color)
{
	real_point3d point0{};
	real_point3d point1{};
	point_from_line3d(point, vector, scale, &point0);
	point_from_line3d(point, vector, scale, &point1);
	render_debug_line(draw_immediately, &point0, &point1, color);
}

void __cdecl render_debug_line_offset(bool draw_immediately, const real_point3d* point0, const real_point3d* point1, const real_argb_color* color, real32 scale)
{
	real_point3d p0{};
	real_point3d p1{};
	point_from_line3d(point0, global_up3d, scale, &p0);
	point_from_line3d(point1, global_up3d, scale, &p1);
	render_debug_line(draw_immediately, &p0, &p1, color);
}

void __cdecl render_debug_vectors(bool draw_immediately, const real_point3d* point, const real_vector3d* forward, const real_vector3d* up, real32 radius)
{
	real_matrix4x3 matrix{};
	matrix4x3_from_point_and_vectors(&matrix, point, forward, up);
	render_debug_matrix(draw_immediately, &matrix, radius);
}

void __cdecl render_debug_quaternion(bool draw_immediately, const real_point3d* point, const real_quaternion* quaternion, real32 radius)
{
	real_matrix4x3 matrix{};
	matrix4x3_from_point_and_quaternion(&matrix, point, quaternion);
	render_debug_matrix(draw_immediately, &matrix, radius);
}

void __cdecl render_debug_matrix(bool draw_immediately, const real_matrix4x3* matrix, real32 radius)
{
	render_debug_vector(draw_immediately, &matrix->position, &matrix->forward, radius * matrix->scale, global_real_argb_red);
	render_debug_vector(draw_immediately, &matrix->position, &matrix->left, radius * matrix->scale, global_real_argb_green);
	render_debug_vector(draw_immediately, &matrix->position, &matrix->up, radius * matrix->scale, global_real_argb_blue);
}

void __cdecl render_debug_matrix3x3(bool draw_immediately, const real_matrix3x3* matrix, const real_point3d* point, real32 radius)
{
	render_debug_vector(draw_immediately, point, &matrix->forward, radius, global_real_argb_red);
	render_debug_vector(draw_immediately, point, &matrix->left, radius, global_real_argb_green);
	render_debug_vector(draw_immediately, point, &matrix->up, radius, global_real_argb_blue);
}

void __cdecl render_debug_triangle(bool draw_immediately, const real_point3d* point0, const real_point3d* point1, const real_point3d* point2, const real_argb_color* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(point2);
	ASSERT(color);

	if (/*draw_immediately || */render_debug_draw_immediately(color))
	{
		rasterizer_debug_triangle(point0, point1, point2, color);
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_triangle, point0, point1, point2, color);
	}
}

void __cdecl render_debug_triangle_outline(bool draw_immediately, const real_point3d* point0, const real_point3d* point1, const real_point3d* point2, const real_argb_color* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(point2);
	ASSERT(color);

	render_debug_line(draw_immediately, point0, point1, color);
	render_debug_line(draw_immediately, point1, point2, color);
	render_debug_line(draw_immediately, point2, point0, color);
}

void __cdecl render_debug_quadrilateral(bool draw_immediately, const real_point3d* point0, const real_point3d* point1, const real_point3d* point2, const real_point3d* point3, const real_argb_color* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(point2);
	ASSERT(point3);
	ASSERT(color);

	render_debug_triangle(draw_immediately, point0, point1, point2, color);
	render_debug_triangle(draw_immediately, point0, point2, point3, color);
}

void __cdecl render_debug_sphere(bool draw_immediately, const real_point3d* center, real32 radius, const real_argb_color* color)
{
	ASSERT(center);
	ASSERT(color);

	if (/*draw_immediately || */render_debug_draw_immediately(color))
	{
		if (render_sphere_visible(center, radius))
		{
			real_point2d circle_points[CIRCLE_DIVISIONS + 1]{};
			render_debug_build_circle_points(radius, circle_points, CIRCLE_DIVISIONS + 1);
			for (int32 i = 0; i < CIRCLE_DIVISIONS; i++)
			{
				real_point2d* circle_point0 = &circle_points[i];
				real_point2d* circle_point1 = &circle_points[i + 1];

				real_point3d point0{};
				real_point3d point1{};

				set_real_point3d(&point0, (center->x + circle_point0->x), (center->y + circle_point0->y), center->z);
				set_real_point3d(&point1, (center->x + circle_point1->x), (center->y + circle_point1->y), center->z);
				render_debug_line(draw_immediately, &point0, &point1, color);

				set_real_point3d(&point0, (center->x + circle_point0->y), center->y, (center->z + circle_point0->x));
				set_real_point3d(&point1, (center->x + circle_point1->y), center->y, (center->z + circle_point1->x));
				render_debug_line(draw_immediately, &point0, &point1, color);

				set_real_point3d(&point0, center->x, (center->y + circle_point0->x), (center->z + circle_point0->y));
				set_real_point3d(&point1, center->x, (center->y + circle_point1->x), (center->z + circle_point1->y));
				render_debug_line(draw_immediately, &point0, &point1, color);
			}
		}
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_sphere, center, radius, color);
	}
}

void __cdecl render_debug_cylinder(bool draw_immediately, const real_point3d* base, const real_vector3d* height, real32 radius, const real_argb_color* color)
{
	ASSERT(base);
	ASSERT(height);
	ASSERT(color);

	if (/*draw_immediately || */render_debug_draw_immediately(color))
	{
		real_point3d points0[CIRCLE_DIVISIONS + 1]{};
		real_point3d points1[CIRCLE_DIVISIONS + 1]{};

		render_debug_build_pill_points(base, height, radius, points0, points1, NULL, NULL, NULL, NULL);

		for (int32 i = 0; i < CIRCLE_DIVISIONS; i++)
		{
			render_debug_line(draw_immediately, &points0[i], &points0[i + 1], color);
			render_debug_line(draw_immediately, &points1[i], &points1[i + 1], color);
		}

		for (int32 i = 0; i < CIRCLE_DIVISIONS; i += 4)
		{
			render_debug_line(draw_immediately, &points0[i], &points1[i], color);
		}

		for (int32 i = 0; i < 8; i += 4)
		{
			render_debug_line(draw_immediately, &points0[i], &points0[i + 8], color);
			render_debug_line(draw_immediately, &points1[i], &points1[i + 8], color);
		}
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_cylinder, base, height, radius, color);
	}
}

void __cdecl render_debug_pill(bool draw_immediately, const real_point3d* base, const real_vector3d* height, real32 radius, const real_argb_color* color)
{
	ASSERT(base);
	ASSERT(height);
	ASSERT(color);

	if (/*draw_immediately || */render_debug_draw_immediately(color))
	{
		real_point3d points0[CIRCLE_DIVISIONS + 1]{};
		real_point3d points1[CIRCLE_DIVISIONS + 1]{};
		real_point3d points2[(CIRCLE_DIVISIONS / 2) + 1]{};
		real_point3d points3[(CIRCLE_DIVISIONS / 2) + 1]{};
		real_point3d points4[(CIRCLE_DIVISIONS / 2) + 1]{};
		real_point3d points5[(CIRCLE_DIVISIONS / 2) + 1]{};

		render_debug_build_pill_points(base, height, radius, points0, points1, points2, points3, points4, points5);

		for (int32 i = 0; i < CIRCLE_DIVISIONS; ++i)
		{
			render_debug_line(draw_immediately, &points0[i], &points0[i + 1], color);
			render_debug_line(draw_immediately, &points1[i], &points1[i + 1], color);
		}

		for (int32 i = 0; i < CIRCLE_DIVISIONS; i += 4)
			render_debug_line(draw_immediately, &points0[i], &points1[i], color);

		for (int32 i = 0; i < CIRCLE_DIVISIONS / 2; i++)
		{
			render_debug_line(draw_immediately, &points2[i], &points2[i + 1], color);
			render_debug_line(draw_immediately, &points3[i], &points3[i + 1], color);
			render_debug_line(draw_immediately, &points4[i], &points4[i + 1], color);
			render_debug_line(draw_immediately, &points5[i], &points5[i + 1], color);
		}
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_pill, base, height, radius, color);
	}
}

void __cdecl render_debug_box2d_outline(bool draw_immediately, const real_rectangle2d* bounds, const real_argb_color* color)
{
	ASSERT(bounds);
	ASSERT(color);

	if (/*draw_immediately || */render_debug_draw_immediately(color))
	{
		real_point3d points[4]{};

		set_real_point3d(points, bounds->x0, bounds->y0, -1.0);
		set_real_point3d(&points[1], bounds->x1, bounds->y0, -1.0);
		set_real_point3d(&points[2], bounds->x1, bounds->y1, -1.0);
		set_real_point3d(&points[3], bounds->x0, bounds->y1, -1.0);

		const render_projection* projection = c_player_view::get_current()->get_rasterizer_projection();
		matrix4x3_transform_point(&projection->view_to_world, points, points);
		matrix4x3_transform_point(&projection->view_to_world, &points[1], &points[1]);
		matrix4x3_transform_point(&projection->view_to_world, &points[2], &points[2]);
		matrix4x3_transform_point(&projection->view_to_world, &points[3], &points[3]);
		render_debug_polygon_edges(points, NUMBEROF(points), color);
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_box2d_outline, bounds, color);
	}
}

void __cdecl render_debug_box(bool draw_immediately, const real_rectangle3d* bounds, const real_argb_color* color)
{
	ASSERT(bounds);
	ASSERT(color);

	if (/*draw_immediately || */render_debug_draw_immediately(color))
	{
		real_point3d faces[k_faces_per_cube_count][4]{};
		rectangle3d_build_faces(bounds, k_faces_per_cube_count, faces);

		for (int32 i = 0; i < k_faces_per_cube_count; i++)
		{
			render_debug_quadrilateral(1, &faces[i][0], &faces[i][1], &faces[i][2], &faces[i][3], color);
		}
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_box, bounds, color);
	}
}

void __cdecl render_debug_box_oriented(bool draw_immediately, const real_rectangle3d* bounds, const real_matrix4x3* matrix, const real_argb_color* color)
{
	real_point3d faces[k_faces_per_cube_count][4]{};
	matrix4x3_transform_points(matrix, k_faces_per_cube_count * 4, (real_point3d*)faces, (real_point3d*)faces);
	rectangle3d_build_faces(bounds, k_faces_per_cube_count, faces);

	for (int32 i = 0; i < k_faces_per_cube_count; i++)
	{
		render_debug_quadrilateral(draw_immediately, &faces[i][0], &faces[i][1], &faces[i][2], &faces[i][3], color);
	}
}

void __cdecl render_debug_box_outline(bool draw_immediately, const real_rectangle3d* bounds, const real_argb_color* color)
{
	ASSERT(bounds);
	ASSERT(color);

	if (/*draw_immediately || */render_debug_draw_immediately(color))
	{
		real_point3d edges[k_edges_per_cube_count][2]{};
		rectangle3d_build_edges(bounds, k_edges_per_cube_count, edges);
		for (int32 i = 0; i < k_edges_per_cube_count; i++)
		{
			render_debug_line(true, &edges[i][0], &edges[i][1], color);
		}
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_box_outline, bounds, color);
	}
}

void __cdecl render_debug_box_outline_oriented(bool draw_immediately, const real_rectangle3d* bounds, const real_matrix4x3* matrix, const real_argb_color* color)
{
	ASSERT(bounds);
	ASSERT(matrix);
	ASSERT(color);

	real_point3d edges[k_edges_per_cube_count][2]{};
	rectangle3d_build_edges(bounds, k_edges_per_cube_count, edges);
	for (int32 i = 0; i < k_edges_per_cube_count; i++)
	{
		matrix4x3_transform_point(matrix, &edges[i][0], &edges[i][0]);
		matrix4x3_transform_point(matrix, &edges[i][1], &edges[i][1]);
		render_debug_line(draw_immediately, &edges[i][0], &edges[i][1], color);
	}
}

void __cdecl render_debug_polygon(const real_point3d* points, int16 point_count, const real_argb_color* color)
{
	if (point_count > 1)
	{
		ASSERT(points);
		ASSERT(color);

		for (int16 i = 1; i < point_count - 1; i++)
		{
			render_debug_triangle(true, points, &points[i], &points[i + 1], color);
		}
	}
}

void __cdecl render_debug_polygon_edges(const real_point3d* points, int16 point_count, const real_argb_color* color)
{
	if (point_count > 1)
	{
		ASSERT(points);
		ASSERT(color);

		if (point_count > 2)
		{
			render_debug_line(true, &points[point_count - 1], points, color);
			for (int16 i = 1; i < point_count; i++)
			{
				render_debug_line(true, &points[i - 1], &points[i], color);
			}
		}
	}
}

void __cdecl render_debug_k_graph(const real_point3d* points, int16 point_count, const real_argb_color* color)
{
	ASSERT(points);
	ASSERT(color);

	if (point_count > 2)
	{
		for (int16 i = 0; i < point_count; i++)
		{
			for (int16 j = i + 1; j < point_count; j++)
			{
				render_debug_line(1, &points[i], &points[j], color);
			}
		}
	}
}

void __cdecl render_debug_cone_outline(bool draw_immediately, const real_point3d* point, const real_vector3d* direction, real32 radius, real32 cone_angle, const real_argb_color* color)
{
	real_vector3d normalized_cone_direction = *direction;
	normalize3d(&normalized_cone_direction);

	real32 cone_vertex_offset = sinf(cone_angle) * radius;
	real32 cone_base_offset = cosf(cone_angle) * radius;

	real_point3d vertex_on_cone{};
	point_from_line3d(point, &normalized_cone_direction, cone_base_offset, &vertex_on_cone);

	real_vector3d perpendicular_to_cone_direction{};
	perpendicular3d(&normalized_cone_direction, &perpendicular_to_cone_direction);
	normalize3d(&perpendicular_to_cone_direction);

	real_vector3d perpendicular_cross_product{};
	cross_product3d(&perpendicular_to_cone_direction, &normalized_cone_direction, &perpendicular_cross_product);
	normalize3d(&perpendicular_cross_product);

	real_point3d cone_outline_points[10]{};
	for (int32 i = 0; i < NUMBEROF(cone_outline_points); i++)
	{
		real32 angle = (TWO_PI * i) / NUMBEROF(cone_outline_points);

		real_point3d* outline_point = &cone_outline_points[i];
		point_from_line3d(&vertex_on_cone, &perpendicular_to_cone_direction, (cone_vertex_offset * sinf(angle)), outline_point);

		real_point3d* cross_product_point = &cone_outline_points[i];
		point_from_line3d(&cone_outline_points[i], &perpendicular_cross_product, (cone_vertex_offset * cosf(angle)), cross_product_point);
	}

	for (int32 i = 0; i < NUMBEROF(cone_outline_points); i++)
	{
		int32 next_point_index = (i + 1) % NUMBEROF(cone_outline_points);
		render_debug_line(draw_immediately, &cone_outline_points[i], &cone_outline_points[next_point_index], color);
		render_debug_line(draw_immediately, &cone_outline_points[i], point, color);
	}
}

void __cdecl render_debug_string(const char* string)
{
	ASSERT(string);

	//render_debug_string_immediate(false, NULL, 0, string);
	render_debug_add_cache_entry(_render_debug_type_string, string);
}

void __cdecl render_debug_string_at_point(const real_point3d* point, const char* string, const real_argb_color* color)
{
	ASSERT(point);
	ASSERT(string);
	ASSERT(color);

	render_debug_add_cache_entry(_render_debug_type_string_at_point, string, point, color);
}

void __cdecl render_debug_string_immediate(bool draw_immediately, const int16* tab_stops, int16 tab_stop_count, const char* string)
{
	if (string && *string)
	{
		if (/*draw_immediately || */get_render_debug_globals()->use_simple_font_text_rendering)
		{
			c_simple_font_draw_string draw_string;
			draw_string.set_tab_stops(tab_stops, tab_stop_count);
			interface_set_bitmap_text_draw_mode(&draw_string, _terminal_font, _text_style_plain, _text_justification_left, 0, 5, 0);
			draw_string.draw(NULL, string);
		}
		else
		{
			c_rasterizer_draw_string draw_string;
			c_font_cache_mt_safe font_cache;

			draw_string.set_tab_stops(tab_stops, tab_stop_count);
			interface_set_bitmap_text_draw_mode(&draw_string, _terminal_font, _text_style_plain, _text_justification_left, 0, 5, 0);
			draw_string.draw(&font_cache, string);
		}
	}
}

void __cdecl render_debug_string_at_point_immediate(const real_point3d* point, const char* string, const real_argb_color* color)
{
	ASSERT(point);
	ASSERT(string);
	ASSERT(color);

	const render_camera* camera = c_player_view::get_current()->get_rasterizer_camera();
	const render_projection* projection = c_player_view::get_current()->get_rasterizer_projection();

	real_vector2d inverse_aspect_ratio_scale{};
	real_vector2d aspect_ratio_scaling = interface_get_aspect_ratio_scaling();
	inverse_aspect_ratio_scale.i = 1.0f / aspect_ratio_scaling.i;
	inverse_aspect_ratio_scale.j = 1.0f / aspect_ratio_scaling.j;

	rectangle2d window_display_bounds = camera->window_pixel_bounds;
	interface_scale_rectangle2d_for_xenon_scaler(&window_display_bounds, &inverse_aspect_ratio_scale);

	real_point2d screen_point{};
	if (render_camera_world_to_window(camera, projection, &window_display_bounds, point, &screen_point))
	{
		rectangle2d bounds{};
		bounds.x0 = (int16)real32(screen_point.x - (real32)window_display_bounds.x0);
		bounds.y0 = (int16)real32(screen_point.y - (real32)window_display_bounds.y0);
		bounds.x1 = SHRT_MAX;
		bounds.y1 = SHRT_MAX;

		if (g_render_debug_globals->use_simple_font_text_rendering)
		{
			c_simple_font_draw_string draw_string;
			interface_set_bitmap_text_draw_mode(&draw_string, _terminal_font, _text_style_plain, _text_justification_left, 0, 5, 0);
			draw_string.set_shadow_color(global_real_argb_black);
			draw_string.set_color(color);
			draw_string.set_bounds(&bounds);
			draw_string.draw(NULL, string);
		}
		else
		{
			c_rasterizer_draw_string draw_string;
			c_font_cache_mt_safe font_cache;

			interface_set_bitmap_text_draw_mode(&draw_string, _terminal_font, _text_style_plain, _text_justification_left, 0, 5, 0);
			draw_string.set_shadow_color(global_real_argb_black);
			draw_string.set_color(color);
			draw_string.set_bounds(&bounds);
			draw_string.draw(&font_cache, string);
		}
	}
}

bool __cdecl render_debug_draw_immediately(const real_argb_color* color)
{
	ASSERT(g_render_debug_globals);

	if (!render_debug_allowed_in_current_thread())
	{
		return false;
	}

	if (!render_debug_active())
	{
		return false;
	}

	if (color)
	{
		if (!render_debug_cache_currently_drawing())
		{
			return color->alpha >= 1.0f;
		}
	}

	return true;
}

void __cdecl render_debug_add_cache_entry(int16 type, ...)
{
	ASSERT(g_render_debug_globals);

	if (g_render_debug_globals->cache_count < NUMBEROF(g_render_debug_globals->cache))
	{
		cache_entry* entry = &g_render_debug_globals->cache[g_render_debug_globals->cache_count++];

		real32 alpha = 1.0f;

		va_list list;
		va_start(list, type);

		switch (type)
		{
		case _render_debug_type_circle:
		{
			entry->circle.plane = *va_arg(list, real_plane3d*);
			entry->circle.projection_axis = (int16)va_arg(list, int);
			entry->circle.projection_sign = (bool)va_arg(list, int);
			entry->circle.center = *va_arg(list, real_point2d*);
			entry->circle.radius = (real32)va_arg(list, real64);
			entry->circle.color = *va_arg(list, real_argb_color*);
			entry->circle.offset = (real32)va_arg(list, real64);
			alpha = entry->circle.color.alpha;

			real_point3d centroid{};
			project_point2d(&entry->circle.center, &entry->circle.plane, entry->circle.projection_axis, entry->circle.projection_sign, &centroid);
			ASSERT(VALID_INDEX(entry->circle.projection_axis, NUMBEROF(centroid.n)));
		}
		break;
		case _render_debug_type_point:
		{
			entry->point.point = *va_arg(list, real_point3d*);
			entry->point.scale = (real32)va_arg(list, real64);
			entry->point.color = *va_arg(list, real_argb_color*);
			alpha = entry->point.color.alpha;
		}
		break;
		case _render_debug_type_line:
		{
			
			entry->line.point0 = *va_arg(list, real_point3d*);
			entry->line.point1 = *va_arg(list, real_point3d*);
			entry->line.color0 = *va_arg(list, real_argb_color*);
			entry->line.color1 = *va_arg(list, real_argb_color*);
			alpha = fminf(entry->line.color1.alpha, entry->line.color0.alpha);
		}
		break;
		case _render_debug_type_line2d:
		{
			entry->line2d.point0 = *va_arg(list, real_point2d*);
			entry->line2d.point1 = *va_arg(list, real_point2d*);
			entry->line2d.color0 = *va_arg(list, real_argb_color*);
			entry->line2d.color1 = *va_arg(list, real_argb_color*);
			alpha = fminf(entry->line.color1.alpha, entry->line.color0.alpha);
		}
		break;
		case _render_debug_type_sphere:
		{
			entry->sphere.center = *va_arg(list, real_point3d*);
			entry->sphere.radius = (real32)va_arg(list, real64);
			entry->sphere.color = *va_arg(list, real_argb_color*);
			alpha = entry->sphere.color.alpha;
		}
		break;
		case _render_debug_type_cylinder:
		{
			entry->cylinder.base = *va_arg(list, real_point3d*);
			entry->cylinder.height = *va_arg(list, real_vector3d*);
			entry->cylinder.width = (real32)va_arg(list, real64);
			entry->cylinder.color = *va_arg(list, real_argb_color*);
			alpha = entry->sphere.color.alpha;
		}
		break;
		case _render_debug_type_pill:
		{
			entry->pill.base = *va_arg(list, real_point3d*);
			entry->pill.height = *va_arg(list, real_vector3d*);
			entry->pill.width = (real32)va_arg(list, real64);
			entry->pill.color = *va_arg(list, real_argb_color*);
			alpha = entry->pill.color.alpha;
		}
		break;
		case _render_debug_type_box:
		case _render_debug_type_box_outline:
		{
			entry->box.bounds = *va_arg(list, real_rectangle3d*);
			entry->box.color = *va_arg(list, real_argb_color*);
			alpha = entry->box.color.alpha;
		}
		break;
		case _render_debug_type_triangle:
		{
			entry->triangle.point0 = *va_arg(list, real_point3d*);
			entry->triangle.point1 = *va_arg(list, real_point3d*);
			entry->triangle.point2 = *va_arg(list, real_point3d*);
			entry->triangle.color = *va_arg(list, real_argb_color*);
			alpha = entry->triangle.color.alpha;
		}
		break;
		case _render_debug_type_string:
		{
			const char* string = va_arg(list, const char*);
			int32 string_index = render_debug_add_cache_string(string);
			if (string_index != NONE)
			{
				entry->string.string_index = string_index;
				entry->string.tab_stop_count = 0;

				//LABEL_46
				entry->type = type;
				entry->layer = 0;
				entry->sort_key = 0.0f;
				if (g_render_debug_globals->group_level > 0)
					entry->sort_key = g_render_debug_globals->group_key;
			}
		}
		break;
		case _render_debug_type_string_at_point:
		{
			const char* string = va_arg(list, const char*);
			int32 string_index = render_debug_add_cache_string(string);
			if (string_index != NONE)
			{
				entry->string_at_point.string_index = string_index;
				entry->string_at_point.point = *va_arg(list, real_point3d*);
				entry->string_at_point.color = *va_arg(list, real_argb_color*);
				alpha = entry->string_at_point.color.alpha;
			}
		}
		break;
		case _render_debug_type_box2d_outline:
		{
			entry->box2d_outline.bounds = *va_arg(list, real_rectangle2d*);
			entry->box2d_outline.color = *va_arg(list, real_argb_color*);
			alpha = entry->box2d_outline.color.alpha;

			//LABEL_46
			entry->type = type;
			entry->layer = 0;
			entry->sort_key = 0.0f;
			if (g_render_debug_globals->group_level > 0)
				entry->sort_key = g_render_debug_globals->group_key;
			return;
		}
		break;
		default:
		{
			entry->type = type;
			entry->layer = 0;
			entry->sort_key = 0.0f;
			if (g_render_debug_globals->group_level > 0)
				entry->sort_key = g_render_debug_globals->group_key;
			return;
		}
		break;
		}

		entry->type = type;

		if (alpha > 0.0f)
		{
			entry->layer = alpha < 1.0f ? 1 : 0;
		}
		else
		{
			g_render_debug_globals->cache_count--;
		}

		if (g_render_debug_globals->group_level <= 0)
		{
			entry->sort_key = 0.0f;
		}
		else
		{
			entry->sort_key = g_render_debug_globals->group_key;
		}
	}

	static bool render_debug_cache_overflow = false;
	if (!render_debug_cache_overflow)
	{
		event(_event_warning, "render debug cache overflow.");
		render_debug_cache_overflow = true;
	}
}

void __cdecl render_debug_cache_draw(bool render_game_tick_cache, bool only_render_strings, bool clear_cache)
{
	s_render_debug_globals* g_render_debug_globals = get_render_debug_globals();

	int16 cache_start_index = 0;
	if (!render_game_tick_cache)
	{
		cache_start_index = g_render_debug_globals->game_tick_cache_count;
	}

	if (g_render_debug_globals->cache_count - cache_start_index > 0)
	{
		int16 cache_layer = 0;
		int32 type = 0;

		ASSERT(!g_render_debug_globals->drawing_cached_geometry);

		g_render_debug_globals->drawing_cached_geometry = true;

		c_rasterizer::e_alpha_blend_mode alpha_blend_modes[2] = { c_rasterizer::_alpha_blend_opaque, c_rasterizer::_alpha_blend_alpha_blend };
		for (int16 cache_index = cache_start_index; cache_index < g_render_debug_globals->cache_count; cache_index++)
		{
			cache_entry* entry = &g_render_debug_globals->cache[cache_index];
			if (VALID_INDEX(entry->layer, NUMBEROF(alpha_blend_modes))) // `NUMBEROF(g_render_debug_globals->group_ids)` ?
			{
				c_rasterizer::set_alpha_blend_mode(alpha_blend_modes[entry->layer]);
			}
			else
			{
				event(_event_warning, "render:debug: unknown debug render cache layer %d!!!", cache_layer);
			}

			if (type_list[entry->type] != type)
			{
				ASSERT(VALID_INDEX(entry->type, NUMBEROF(type_list)));
				type = type_list[entry->type];
			}

			if (!only_render_strings || entry->type == _render_debug_type_string)
			{
				switch (entry->type)
				{
				case _render_debug_type_circle:
				{
					render_debug_circle(true,
						&entry->circle.plane,
						entry->circle.projection_axis,
						entry->circle.projection_sign,
						&entry->circle.center,
						entry->circle.radius,
						&entry->circle.color,
						entry->circle.offset);
				}
				break;
				case _render_debug_type_point:
				{
					render_debug_point(true,
						&entry->point.point,
						entry->point.scale,
						&entry->point.color);
				}
				break;
				case _render_debug_type_line:
				{
					render_debug_line_shaded(true,
						&entry->line.point0,
						&entry->line.point1,
						&entry->line.color0,
						&entry->line.color1);
				}
				break;
				case _render_debug_type_line2d:
				{
					render_debug_line2d_shaded(
						&entry->line2d.point0,
						&entry->line2d.point1,
						&entry->line2d.color0,
						&entry->line2d.color1);
				}
				break;
				case _render_debug_type_sphere:
				{
					render_debug_sphere(true,
						&entry->sphere.center,
						entry->sphere.radius,
						&entry->sphere.color);
				}
				break;
				case _render_debug_type_cylinder:
				{
					render_debug_cylinder(true,
						&entry->cylinder.base,
						&entry->cylinder.height,
						entry->cylinder.width,
						&entry->cylinder.color);
				}
				break;
				case _render_debug_type_pill:
				{
					render_debug_pill(true,
						&entry->pill.base,
						&entry->pill.height,
						entry->pill.width,
						&entry->pill.color);
				}
				break;
				case _render_debug_type_box:
				{
					render_debug_box(true,
						&entry->box.bounds,
						&entry->box.color);
				}
				break;
				case _render_debug_type_box_outline:
				{
					render_debug_box_outline(true,
						&entry->box.bounds,
						&entry->box.color);
				}
				break;
				case _render_debug_type_triangle:
				{
					render_debug_triangle(true,
						&entry->triangle.point0,
						&entry->triangle.point1,
						&entry->triangle.point2,
						&entry->triangle.color);
				}
				break;
				case _render_debug_type_string:
				{
					const char* string = g_render_debug_globals->cache_string + entry->string.string_index;
					ASSERT(string != NULL);

					render_debug_string_immediate(true,
						entry->string.tab_stops,
						entry->string.tab_stop_count,
						string);
				}
				break;
				case _render_debug_type_string_at_point:
				{
					const char* string = g_render_debug_globals->cache_string + entry->string_at_point.string_index;
					ASSERT(string != NULL);

					render_debug_string_at_point_immediate(
						&entry->string_at_point.point,
						string,
						&entry->string_at_point.color);
				}
				break;
				case _render_debug_type_box2d_outline:
				{
					render_debug_box2d_outline(true,
						&entry->box2d_outline.bounds,
						&entry->box2d_outline.color);
				}
				break;
				default:
				{
					// ASSERT(halt());
				}
				break;
				}
			}
		}

		g_render_debug_globals->drawing_cached_geometry = false;
	}

	if (clear_cache)
	{
		g_render_debug_globals->cache_count = g_render_debug_globals->game_tick_cache_count;
		g_render_debug_globals->cache_string_length = g_render_debug_globals->game_tick_cache_string_length;
	}
}

// `sound/game_sound_spatialization.cpp`
void __cdecl render_debug_polygon_fan(const real_point3d* points, int16 point_count, const real_argb_color* color)
{
	if (point_count > 1)
	{
		ASSERT(points);
		ASSERT(color);

		real_argb_color edge_color = *color;
		edge_color.alpha = 1.0f;

		for (int16 i = 1; i < point_count - 1; i++)
		{
			render_debug_triangle(true, &points[0], &points[i], &points[i + 1], color);
			render_debug_line(true, &points[0], &points[i], &edge_color);
			render_debug_line(true, &points[i], &points[i + 1], &edge_color);
			render_debug_line(true, &points[0], &points[i + 1], &edge_color);
		}
	}
}

real32 __cdecl build_height_matrix(const real_point3d* base, const real_vector3d* height, real_matrix4x3* out_matrix)
{
	out_matrix->scale = 1.0f;
	out_matrix->up = *height;
	perpendicular3d(&out_matrix->up, &out_matrix->left);
	real32 result = normalize3d(&out_matrix->up);
	normalize3d(&out_matrix->left);
	cross_product3d(&out_matrix->left, &out_matrix->up, &out_matrix->forward);
	out_matrix->position = *base;

	return result;
}

void __cdecl render_debug_build_circle_points(real32 radius, real_point2d* points, int32 total_point_count)
{
	ASSERT(total_point_count > 0);

	real32 angle = (TWO_PI / (total_point_count - 1));
	real32 sin_angle = sinf(angle);
	real32 cos_angle = cosf(angle);
	set_real_point2d(points, radius, 0.0f);
	for (int32 i = 0; i + 1 < total_point_count; i++)
	{
		rotate_vector2d((real_vector2d*)&points[i], sin_angle, cos_angle, (real_vector2d*)&points[i + 1]);
	}

	points[total_point_count - 1] = *points;
}

void __cdecl render_debug_build_pill_points(const real_point3d* base, const real_vector3d* height, real32 radius, real_point3d* points0, real_point3d* points1, real_point3d* points2, real_point3d* points3, real_point3d* points4, real_point3d* points5)
{
	real_matrix4x3 height_matrix{};
	real32 normalized_up = build_height_matrix(base, height, &height_matrix);

	real_point2d circle_points[CIRCLE_DIVISIONS + 1]{};
	render_debug_build_circle_points(radius, circle_points, CIRCLE_DIVISIONS + 1);

	if (points0 && points1)
	{
		for (int32 i = 0; i < CIRCLE_DIVISIONS + 1; i++)
		{
			real_point2d* circle_point = &circle_points[i];

			set_real_point3d(&points0[i], circle_point->x, circle_point->y, normalized_up);
			set_real_point3d(&points1[i], circle_point->x, circle_point->y, 0.0f);

			matrix4x3_transform_point(&height_matrix, &points0[i], &points0[i]);
			matrix4x3_transform_point(&height_matrix, &points1[i], &points1[i]);
		}
	}

	if (points2 && points3 && points4 && points5)
	{
		for (int32 i = 0; i < (CIRCLE_DIVISIONS / 2) + 1; i++)
		{
			real_point2d* circle_point1 = &circle_points[i];
			real_point2d* circle_point0 = &circle_points[i + 8];

			set_real_point3d(&points2[i], 0.0, circle_point1->x, circle_point1->y + normalized_up);
			set_real_point3d(&points3[i], 0.0, circle_point0->x, circle_point0->y);
			set_real_point3d(&points4[i], circle_point1->x, 0.0f, circle_point1->y + normalized_up);
			set_real_point3d(&points5[i], circle_point0->x, 0.0f, circle_point0->y);

			matrix4x3_transform_point(&height_matrix, &points2[i], &points2[i]);
			matrix4x3_transform_point(&height_matrix, &points3[i], &points3[i]);
			matrix4x3_transform_point(&height_matrix, &points4[i], &points4[i]);
			matrix4x3_transform_point(&height_matrix, &points5[i], &points5[i]);
		}
	}
}

uns16 __cdecl _random(uns32* seed, const char* string, const char* file, uns32 line)
{
	//random_seed_debug(seed, "random", string, file, line);
	*seed = 0x19660D * *seed + 0x3C6EF35F;

	return HIWORD(*seed);
}

real_point3d* __cdecl rectangle3d_center(const real_rectangle3d* rect, real_point3d* center)
{
	center->x = 0.5f * (rect->x0 + rect->x1);
	center->y = 0.5f * (rect->y0 + rect->y1);
	center->z = 0.5f * (rect->z0 + rect->z1);

	return center;
}

c_render_debug_line_drawer::c_render_debug_line_drawer()
{
	rectangle2d screen_bounds{};
	c_rasterizer::get_fullscreen_render_pixel_bounds(&screen_bounds);

	m_oo_screen_width = 1.0f / (screen_bounds.x1 - screen_bounds.x0);
	m_oo_screen_height = 1.0f / (screen_bounds.y1 - screen_bounds.y0);
	m_internal_queue_vertex_count = 0;
	m_line_type = _line_type_none;
	c_render_debug_line_drawer::set_color(global_real_argb_white);
}

c_render_debug_line_drawer::~c_render_debug_line_drawer()
{
	c_render_debug_line_drawer::flush();
}

void c_render_debug_line_drawer::flush()
{
	if (render_debug_draw_immediately(&m_color_argb))
	{
		if (m_internal_queue_vertex_count > 0)
		{
			switch (m_line_type)
			{
			case _line_type_2d:
			{
				c_rasterizer::draw_debug_line_list2d_explicit((rasterizer_vertex_debug*)m_internal_queue, m_internal_queue_vertex_count / 2);
			}
			break;
			case _line_type_3d:
			{
				c_rasterizer::draw_debug_line_list_explicit((rasterizer_vertex_debug*)m_internal_queue, m_internal_queue_vertex_count / 2);
			}
			break;
			}
		}
	}
	else
	{
		event(_event_error, "can't use debug_line_drawer w/o immediate debug drawing");
	}

	m_internal_queue_vertex_count = 0;
}

void c_render_debug_line_drawer::set_color(const real_argb_color* color_)
{
	color_degamma(&color_->rgb, reinterpret_cast<real_linear_rgb_color*>(&m_color_argb.rgb));
	m_color_argb.alpha = color_->alpha;
	m_color = real_argb_color_to_pixel32(&m_color_argb);
}

void c_render_debug_line_drawer::add_line_2d(const real_point2d* p0, const real_point2d* p1)
{
	if (m_line_type != _line_type_2d || m_internal_queue_vertex_count + 2 >= 512)
	{
		c_render_debug_line_drawer::flush();
		m_line_type = _line_type_2d;
	}

	s_internal_vertex* vertex = m_internal_queue + m_internal_queue_vertex_count;
	m_internal_queue_vertex_count += 2;

	vertex[0].position.x = ((p0->x * m_oo_screen_width) * 2.0f) - 1.0f;
	vertex[0].position.y = -(((p0->y * m_oo_screen_height) * 2.0f) - 1.0f);
	vertex[0].position.z = 0.0f;
	vertex[0].color = m_color;

	vertex[1].position.x = ((p1->x * m_oo_screen_width) * 2.0f) - 1.0f;
	vertex[1].position.y = -(((p1->y * m_oo_screen_height) * 2.0f) - 1.0f);
	vertex[1].position.z = 0.0f;
	vertex[1].color = m_color;
}

void c_render_debug_line_drawer::add_line_3d(const real_point3d* p0, const real_point3d* p1)
{
	if (m_line_type != _line_type_3d || m_internal_queue_vertex_count + 2 >= 512)
	{
		c_render_debug_line_drawer::flush();
		m_line_type = _line_type_3d;
	}

	s_internal_vertex* vertex = m_internal_queue + m_internal_queue_vertex_count;
	m_internal_queue_vertex_count += 2;

	vertex[0].position = *p0;
	vertex[0].color = m_color;

	vertex[1].position = *p1;
	vertex[1].color = m_color;
}

void c_render_debug_line_drawer::add_line_3d_unclipped(const real_point3d* p0, const real_point3d* p1)
{
	ASSERT(p0);
	ASSERT(p1);

	real_matrix4x3 camera{};
	c_player_view::get_player_render_camera_orientation(&camera);

	real_point3d point0 = *p0;
	real_point3d point1 = *p1;

	real_vector3d vector0{};
	real_vector3d vector1{};
	vector_from_points3d(&camera.position, &point0, &vector0);
	vector_from_points3d(&camera.position, &point1, &vector1);

	real32 clip_distance = magnitude3d(&vector0);
	if (magnitude3d(&vector0) <= magnitude3d(&vector1))
	{
		clip_distance = magnitude3d(&vector1);
	}

	real32 z_far = c_player_view::get_current()->get_rasterizer_camera()->z_far;
	if (clip_distance > (0.5f * z_far))
	{
		real32 distance = (0.5f * z_far) / clip_distance;
		point_from_line3d(&camera.position, &vector0, distance, &point0);
		point_from_line3d(&camera.position, &vector1, distance, &point1);
	}

	c_render_debug_line_drawer::add_line_3d(&point0, &point1);
}

