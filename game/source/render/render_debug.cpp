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
#include "render/views/render_view.hpp"
#include "saved_games/saved_film.hpp"
#include "saved_games/saved_film_history.hpp"
#include "simulation/simulation.hpp"
#include "sound/sound_manager.hpp"
#include "structures/structure_detail_objects.hpp"
#include "structures/structures.hpp"
#include "text/draw_string.hpp"

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

struct s_cache_entry
{
	short type;
	short layer;
	real __unknown4;

	union
	{
		struct // _render_debug_type_circle
		{
			plane3d plane;
			short projection_axis;
			bool projection_sign;
			real_point2d center;
			real radius;
			real_argb_color color;
			real a8;
		} circle;

		struct // _render_debug_type_point
		{
			real_point3d point;
			real scale;
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
			real radius;
			real_argb_color color;
		} sphere;

		struct // _render_debug_type_cylinder
		{
			real_point3d base;
			vector3d height;
			real radius;
			real_argb_color color;
		} cylinder;

		struct // _render_debug_type_pill
		{
			real_point3d base;
			vector3d height;
			real radius;
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
			long string_offset;
			short tab_stops[16];
			short tab_stop_count;
		} string;
		
		struct // _render_debug_type_string_at_point
		{
			long string_offset;
			real_point3d point;
			real_argb_color color;
			real scale;
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
	bool use_simple_font;

	real __unknown4;

	long group_ids[2];
	long group_level;

	short cache_count;
	short cache_start_index;

	bool inside_game_tick;
	char __data19[3];

	s_cache_entry cache_entries[8192];

	short cache_string_length;
	short __unknown8001E;

	char cache_string[MAXIMUM_CACHE_STRING_LENGTH];
};
static_assert(sizeof(s_render_debug_globals) == 0x81020);

static s_render_debug_globals _render_debug_globals
{
	.use_simple_font = false
};

thread_local s_render_debug_globals* g_render_debug_globals = &_render_debug_globals;

long type_list[] = { 0, 0, 0, 3, 0, 0, 0, 1, 0, 1, 2, 2, 0 };

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
	get_render_debug_globals()->use_simple_font = use_simple_font;
}

void __cdecl render_debug_notify_game_tick_end()
{
	ASSERT(g_render_debug_globals->inside_game_tick);

	//render_debug_process_deffered_events();
	g_render_debug_globals->inside_game_tick = false;
}

void __cdecl rasterizer_debug_line(real_point3d const* p0, real_point3d const* p1, real_argb_color const* color0, real_argb_color const* color1)
{
	ASSERT(p0 && p1 && color0);

	real_argb_color color0_ = *color0;
	real_argb_color color1_ = *color1;
	color0_.alpha = 1.0f;
	color1_.alpha = 1.0f;

	for (long i = 0; i < NUMBEROF(color0_.color.n); i++)
	{
		color0_.color.n[i] = color0_.color.n[i] * color0_.color.n[i];
		color1_.color.n[i] = color1_.color.n[i] * color1_.color.n[i];
	}

	c_rasterizer::draw_debug_line(*p0, *p1, real_argb_color_to_pixel32(&color0_), real_argb_color_to_pixel32(&color1_));
}

void __cdecl rasterizer_debug_line2d(real_point2d const* p0, real_point2d const* p1, real_argb_color const* color0, real_argb_color const* color1)
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

void __cdecl rasterizer_debug_triangle(real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_argb_color const* color)
{
	rasterizer_vertex_debug vertex_debug[3]{};
	vertex_debug[0].point = *point0;
	vertex_debug[1].point = *point1;
	vertex_debug[2].point = *point2;

	dword _color = real_argb_color_to_pixel32(color);
	vertex_debug[0].color.value = _color;
	vertex_debug[1].color.value = _color;
	vertex_debug[2].color.value = _color;

	c_rasterizer::draw_debug_polygon(vertex_debug, NUMBEROF(vertex_debug) / 3, c_rasterizer_index_buffer::_primitive_type_triangle_strip); // D3DPT_TRIANGLESTRIP
}

int __cdecl render_debug_cache_entry_sort_proc(void const* a, void const* b)
{
	s_cache_entry const* entry0 = static_cast<s_cache_entry const*>(a);
	s_cache_entry const* entry1 = static_cast<s_cache_entry const*>(b);

	if (entry0->layer < entry1->layer)
		return false;

	if (entry0->layer > entry1->layer)
		return true;

	if (type_list[entry0->type] < type_list[entry1->type])
		return false;

	if (type_list[entry0->type] > type_list[entry1->type])
		return true;

	if (entry0->__unknown4 < entry1->__unknown4)
		return false;

	return entry0->__unknown4 > entry1->__unknown4;
}

void __cdecl render_debug_sort_cache_entries()
{
	s_render_debug_globals* g_render_debug_globals = get_render_debug_globals();
	if (g_render_debug_globals->cache_count > 0)
	{
		qsort(g_render_debug_globals->cache_entries,
			g_render_debug_globals->cache_start_index,
			sizeof(s_cache_entry),
			render_debug_cache_entry_sort_proc);

		qsort(g_render_debug_globals->cache_entries + g_render_debug_globals->cache_start_index,
			g_render_debug_globals->cache_count - g_render_debug_globals->cache_start_index,
			sizeof(s_cache_entry),
			render_debug_cache_entry_sort_proc);
	}
}

void __cdecl render_debug_begin(bool a1, bool a2, bool a3)
{
	s_render_debug_globals* g_render_debug_globals = get_render_debug_globals();
	ASSERT(!g_render_debug_globals->active);
	
	g_render_debug_globals->active = true;

	c_rasterizer::set_z_buffer_mode(c_rasterizer::_z_buffer_mode_unknown7);
	c_rasterizer::set_sampler_filter_mode(0, (c_rasterizer::e_sampler_filter_mode)0);
	c_rasterizer::set_cull_mode(c_rasterizer::_cull_mode_clockwise);
	c_rasterizer::set_color_write_enable(0, 7);
	c_rasterizer::set_alpha_blend_mode(c_rasterizer::_alpha_blend_mode_unknown3);

	render_debug_cache_draw(a1, a2, a3);
}

void __cdecl render_debug_end(bool a1, bool a2, bool a3)
{
	s_render_debug_globals* g_render_debug_globals = get_render_debug_globals();
	ASSERT(g_render_debug_globals->active);
	
	render_debug_sort_cache_entries();
	render_debug_cache_draw(a1, a2, a3);
	g_render_debug_globals->active = false;
}

void __cdecl render_debug_clients(long user_index)
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
		//simulation_debug_render();
		//voice_render();
		debug_render_observer();
		render_debug_scenario_comments();
		render_report_render_debug(user_index, true);
		saved_film_render_debug();
		saved_film_history_render_debug();
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

long __cdecl render_debug_add_cache_string(char const* string)
{
	s_render_debug_globals* render_debug_globals = get_render_debug_globals();

	for (long i = 0; i < render_debug_globals->cache_string_length; i++)
	{
		if (csstrcmp(string, &render_debug_globals->cache_string[i]) == 0)
		{
			if (i != NONE)
				return i;
		}
	}

	short cache_string_length = render_debug_globals->cache_string_length;
	if (cache_string_length < MAXIMUM_CACHE_STRING_LENGTH)
	{
		long string_offset = cache_string_length;
		csstrnzcpy(&g_render_debug_globals->cache_string[cache_string_length], string, MAXIMUM_CACHE_STRING_LENGTH - cache_string_length);
		short v8 = (short)strlen(string);
		// length assert

		g_render_debug_globals->cache_string_length += v8 + 1;
		short v9 = g_render_debug_globals->cache_string_length;
		if (v9 > 4095)
			v9 = 4095;
		g_render_debug_globals->cache_string_length = v9;

		return string_offset;
	}

	static bool render_debug_cache_string_overflow = false;
	if (!render_debug_cache_string_overflow)
	{
		generate_event(_event_level_warning, "render debug cache string overflow");
		render_debug_cache_string_overflow = true;
	}

	return NONE;
}

real_argb_color const* __cdecl render_debug_random_color(real_argb_color* color)
{
	return render_debug_unique_color(_random(get_local_random_seed_address(), 0, __FILE__, __LINE__), color);
}

real_argb_color const* __cdecl render_debug_unique_color(long index, real_argb_color* color)
{
	ASSERT(color);

	static real_argb_color unique_colors[]
	{
		{ 1.0, { 1.0, 0.0, 1.0 } },
		{ 1.0, { 0.0, 1.0, 1.0 } },
		{ 1.0, { 1.0, 0.5, 0.0 } },
		{ 1.0, { 0.0, 1.0, 0.5 } },
		{ 1.0, { 0.5, 0.0, 1.0 } },
		{ 1.0, { 1.0, 0.0, 0.5 } },
		{ 1.0, { 0.5, 1.0, 0.0 } },
		{ 1.0, { 0.0, 0.5, 1.0 } },
		{ 1.0, { 0.5, 0.0, 0.0 } },
		{ 1.0, { 0.0, 0.5, 0.0 } },
		{ 1.0, { 0.0, 0.0, 0.5 } },
		{ 1.0, { 1.0, 1.0, 0.5 } },
		{ 1.0, { 1.0, 0.5, 1.0 } },
		{ 1.0, { 0.5, 1.0, 1.0 } },
		{ 1.0, { 0.5, 0.5, 0.0 } },
		{ 1.0, { 0.0, 0.5, 0.5 } },
		{ 1.0, { 0.5, 0.0, 0.5 } },
	};

	*color = unique_colors[abs(index) % NUMBEROF(unique_colors)];

	return color;
}

//render_debug_spray

void __cdecl render_debug_point2d(bool draw_immediately, plane3d const* plane, short projection_axis, bool projection_sign, real_point2d const* point, real scale, real_argb_color const* color, real a8)
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

void __cdecl render_debug_line2d(real_point2d const* point0, real_point2d const* point1, real_argb_color const* color)
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

void __cdecl render_debug_line2d(bool draw_immediately, plane3d const* plane, short projection_axis, bool projection_sign, real_point2d const* p0, real_point2d const* p1, real_argb_color const* color, real a8)
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

void __cdecl render_debug_vector2d(bool draw_immediately, plane3d const* plane, short projection_axis, bool projection_sign, real_point2d const* point, vector2d const* vector, real scale, real_argb_color const* color, real a9)
{
	ASSERT(plane);
	ASSERT(point);
	ASSERT(vector);
	ASSERT(color);

	real_point2d point1{};
	point_from_line2d(point, vector, scale, &point1);
	render_debug_line2d(draw_immediately, plane, projection_axis, projection_sign, point, &point1, color, a9);
}

void __cdecl render_debug_circle(bool draw_immediately, plane3d const* plane, short projection_axis, bool projection_sign, real_point2d const* center, real radius, real_argb_color const* color, real a8)
{
	ASSERT(plane);
	ASSERT(center);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		real_point2d circle_points[CIRCLE_DIVISIONS + 1]{};
		render_debug_build_circle_points(radius, circle_points, CIRCLE_DIVISIONS + 1);
		for (long i = 0; i < CIRCLE_DIVISIONS; i++)
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

void __cdecl render_debug_circle(bool draw_immediately, real_point3d const* center, vector3d const* normal, real radius, real_argb_color const* color)
{
	render_debug_polygon_regular(draw_immediately, center, CIRCLE_DIVISIONS, normal, radius, color);
}

void __cdecl render_debug_polygon_regular(bool draw_immediately, real_point3d const* center, long point_count, vector3d const* normal, real radius, real_argb_color const* color)
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

	for (long i = 0; i < point_count; i++)
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

void __cdecl render_debug_point(bool draw_immediately, real_point3d const* point, real scale, real_argb_color const* color)
{
	ASSERT(point);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		scale *= 0.5f;
		real_point3d points[6]{};
		set_real_point3d(points, point->x - scale, point->y, point->z);
		set_real_point3d(&points[1], point->x + scale, point->y, point->z);
		set_real_point3d(&points[2], point->x, point->y - scale, point->z);
		set_real_point3d(&points[3], point->x, point->y + scale, point->z);
		set_real_point3d(&points[4], point->x, point->y, point->z - scale);
		set_real_point3d(&points[5], point->x, point->y, point->z + scale);

		for (long i = 0; i < 6; i += 2)
			render_debug_line(true, &points[i], &points[i + 1], color);
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_point, point, scale, color);
	}
}

void __cdecl render_debug_line(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		rasterizer_debug_line(point0, point1, color, color);
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_line, point0, point1, color, color);
	}
}

void __cdecl render_debug_line_shaded(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color0, real_argb_color const* color1)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(color0);
	ASSERT(color1);

	if (draw_immediately || (render_debug_draw_immediately(color0) && render_debug_draw_immediately(color1)))
	{
		rasterizer_debug_line(point0, point1, color0, color1);
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_line, point0, point1, color0, color1);
	}
}

void __cdecl render_debug_line2d_shaded(real_point2d const* point0, real_point2d const* point1, real_argb_color const* color0, real_argb_color const* color1)
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

void __cdecl render_debug_line_unclipped(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);

	real_matrix4x3 camera{};
	c_player_view::get_player_render_camera_orientation(&camera);

	real_point3d p0 = *point0;
	real_point3d p1 = *point1;

	vector3d vector0{};
	vector3d vector1{};
	vector_from_points3d(&camera.position, &p0, &vector0);
	vector_from_points3d(&camera.position, &p1, &vector1);

	real clip_distance = magnitude3d(&vector0);
	if (magnitude3d(&vector0) <= magnitude3d(&vector1))
		clip_distance = magnitude3d(&vector1);

	real z_far = c_player_view::get_current()->get_rasterizer_camera()->z_far;
	if (clip_distance > (0.5f * z_far))
	{
		real distance = (0.5f * z_far) / clip_distance;
		point_from_line3d(&camera.position, &vector0, distance, &p0);
		point_from_line3d(&camera.position, &vector1, distance, &p1);
	}
	render_debug_line(draw_immediately, &p0, &p1, color);
}

void __cdecl render_debug_line_non_occluded(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);

	real_matrix4x3 camera{};
	c_player_view::get_player_render_camera_orientation(&camera);

	real_point3d p0 = *point0;
	real_point3d p1 = *point1;

	vector3d vector0{};
	vector3d vector1{};
	vector_from_points3d(&camera.position, &p0, &vector0);
	vector_from_points3d(&camera.position, &p1, &vector1);

	real clip_distance = magnitude3d(&vector0);
	if (magnitude3d(&vector0) <= magnitude3d(&vector1))
		clip_distance = magnitude3d(&vector1);

	real z_near = c_player_view::get_current()->get_rasterizer_camera()->z_near;
	if (clip_distance > (0.5f * z_near))
	{
		real distance = (0.5f * z_near) / clip_distance;
		point_from_line3d(&camera.position, &vector0, distance, &p0);
		point_from_line3d(&camera.position, &vector1, distance, &p1);
	}
	render_debug_line(draw_immediately, &p0, &p1, color);
}

void __cdecl render_debug_vector(bool draw_immediately, real_point3d const* point, vector3d const* vector, real scale, real_argb_color const* color)
{
	ASSERT(point);
	ASSERT(vector);
	ASSERT(color);

	real_point3d point1{};
	point_from_line3d(point, vector, scale, &point1);
	render_debug_line(draw_immediately, point, &point1, color);
}

void __cdecl render_debug_tick(bool draw_immediately, real_point3d const* point, vector3d const* vector, real scale, real_argb_color const* color)
{
	real_point3d point0{};
	real_point3d point1{};
	point_from_line3d(point, vector, scale, &point0);
	point_from_line3d(point, vector, scale, &point1);
	render_debug_line(draw_immediately, &point0, &point1, color);
}

void __cdecl render_debug_line_offset(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_argb_color const* color, real scale)
{
	real_point3d p0{};
	real_point3d p1{};
	point_from_line3d(point0, global_up3d, scale, &p0);
	point_from_line3d(point1, global_up3d, scale, &p1);
	render_debug_line(draw_immediately, &p0, &p1, color);
}

void __cdecl render_debug_vectors(bool draw_immediately, real_point3d const* point, vector3d const* forward, vector3d const* up, real radius)
{
	real_matrix4x3 matrix{};
	matrix4x3_from_point_and_vectors(&matrix, point, forward, up);
	render_debug_matrix(draw_immediately, &matrix, radius);
}

void __cdecl render_debug_quaternion(bool draw_immediately, real_point3d const* point, real_quaternion const* quaternion, real radius)
{
	real_matrix4x3 matrix{};
	matrix4x3_from_point_and_quaternion(&matrix, point, quaternion);
	render_debug_matrix(draw_immediately, &matrix, radius);
}

void __cdecl render_debug_matrix(bool draw_immediately, real_matrix4x3 const* matrix, real radius)
{
	render_debug_vector(draw_immediately, &matrix->position, &matrix->forward, radius * matrix->scale, global_real_argb_red);
	render_debug_vector(draw_immediately, &matrix->position, &matrix->left, radius * matrix->scale, global_real_argb_green);
	render_debug_vector(draw_immediately, &matrix->position, &matrix->up, radius * matrix->scale, global_real_argb_blue);
}

void __cdecl render_debug_matrix3x3(bool draw_immediately, matrix3x3 const* matrix, real_point3d const* point, real radius)
{
	render_debug_vector(draw_immediately, point, &matrix->forward, radius, global_real_argb_red);
	render_debug_vector(draw_immediately, point, &matrix->left, radius, global_real_argb_green);
	render_debug_vector(draw_immediately, point, &matrix->up, radius, global_real_argb_blue);
}

void __cdecl render_debug_triangle(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(point2);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		rasterizer_debug_triangle(point0, point1, point2, color);
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_triangle, point0, point1, point2, color);
	}
}

void __cdecl render_debug_triangle_outline(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(point2);
	ASSERT(color);

	render_debug_line(draw_immediately, point0, point1, color);
	render_debug_line(draw_immediately, point1, point2, color);
	render_debug_line(draw_immediately, point2, point0, color);
}

void __cdecl render_debug_quadrilateral(bool draw_immediately, real_point3d const* point0, real_point3d const* point1, real_point3d const* point2, real_point3d const* point3, real_argb_color const* color)
{
	ASSERT(point0);
	ASSERT(point1);
	ASSERT(point2);
	ASSERT(point3);
	ASSERT(color);

	render_debug_triangle(draw_immediately, point0, point1, point2, color);
	render_debug_triangle(draw_immediately, point0, point2, point3, color);
}

void __cdecl render_debug_sphere(bool draw_immediately, real_point3d const* center, real radius, real_argb_color const* color)
{
	ASSERT(center);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		if (render_sphere_visible(center, radius))
		{
			real_point2d circle_points[CIRCLE_DIVISIONS + 1]{};
			render_debug_build_circle_points(radius, circle_points, CIRCLE_DIVISIONS + 1);
			for (long i = 0; i < CIRCLE_DIVISIONS; i++)
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

void __cdecl render_debug_cylinder(bool draw_immediately, real_point3d const* base, vector3d const* height, real radius, real_argb_color const* color)
{
	ASSERT(base);
	ASSERT(height);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		real_point3d points0[CIRCLE_DIVISIONS + 1]{};
		real_point3d points1[CIRCLE_DIVISIONS + 1]{};

		render_debug_build_pill_points(base, height, radius, points0, points1, NULL, NULL, NULL, NULL);

		for (long i = 0; i < CIRCLE_DIVISIONS; i++)
		{
			render_debug_line(draw_immediately, &points0[i], &points0[i + 1], color);
			render_debug_line(draw_immediately, &points1[i], &points1[i + 1], color);
		}

		for (long i = 0; i < CIRCLE_DIVISIONS; i += 4)
			render_debug_line(draw_immediately, &points0[i], &points1[i], color);

		for (long i = 0; i < 8; i += 4)
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

void __cdecl render_debug_pill(bool draw_immediately, real_point3d const* base, vector3d const* height, real radius, real_argb_color const* color)
{
	ASSERT(base);
	ASSERT(height);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		real_point3d points0[CIRCLE_DIVISIONS + 1]{};
		real_point3d points1[CIRCLE_DIVISIONS + 1]{};
		real_point3d points2[(CIRCLE_DIVISIONS / 2) + 1]{};
		real_point3d points3[(CIRCLE_DIVISIONS / 2) + 1]{};
		real_point3d points4[(CIRCLE_DIVISIONS / 2) + 1]{};
		real_point3d points5[(CIRCLE_DIVISIONS / 2) + 1]{};

		render_debug_build_pill_points(base, height, radius, points0, points1, points2, points3, points4, points5);

		for (long i = 0; i < CIRCLE_DIVISIONS; ++i)
		{
			render_debug_line(draw_immediately, &points0[i], &points0[i + 1], color);
			render_debug_line(draw_immediately, &points1[i], &points1[i + 1], color);
		}

		for (long i = 0; i < CIRCLE_DIVISIONS; i += 4)
			render_debug_line(draw_immediately, &points0[i], &points1[i], color);

		for (long i = 0; i < CIRCLE_DIVISIONS / 2; i++)
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

void __cdecl render_debug_box2d_outline(bool draw_immediately, real_rectangle2d const* bounds, real_argb_color const* color)
{
	ASSERT(bounds);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		real_point3d points[4]{};

		set_real_point3d(points, bounds->x.lower, bounds->y.lower, -1.0);
		set_real_point3d(&points[1], bounds->x.upper, bounds->y.lower, -1.0);
		set_real_point3d(&points[2], bounds->x.upper, bounds->y.upper, -1.0);
		set_real_point3d(&points[3], bounds->x.lower, bounds->y.upper, -1.0);

		render_projection const* projection = c_player_view::get_current()->get_rasterizer_projection();
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

void __cdecl render_debug_box(bool draw_immediately, real_rectangle3d const* bounds, real_argb_color const* color)
{
	ASSERT(bounds);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		real_point3d faces[k_faces_per_cube_count][4]{};
		rectangle3d_build_faces(bounds, k_faces_per_cube_count, faces);

		for (long i = 0; i < k_faces_per_cube_count; i++)
			render_debug_quadrilateral(1, faces[i], &faces[i][1], &faces[i][2], &faces[i][3], color);
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_box, bounds, color);
	}
}

void __cdecl render_debug_box_oriented(bool draw_immediately, real_rectangle3d const* bounds, real_matrix4x3 const* matrix, real_argb_color const* color)
{
	real_point3d faces[k_faces_per_cube_count][4]{};
	matrix4x3_transform_points(matrix, k_faces_per_cube_count * 4, (real_point3d*)faces, (real_point3d*)faces);
	rectangle3d_build_faces(bounds, k_faces_per_cube_count, faces);

	for (long i = 0; i < k_faces_per_cube_count; i++)
		render_debug_quadrilateral(draw_immediately, faces[i], &faces[i][1], &faces[i][2], &faces[i][3], color);
}

void __cdecl render_debug_box_outline(bool draw_immediately, real_rectangle3d const* bounds, real_argb_color const* color)
{
	ASSERT(bounds);
	ASSERT(color);

	if (draw_immediately || render_debug_draw_immediately(color))
	{
		real_point3d edges[k_edges_per_cube_count][2]{};
		rectangle3d_build_edges(bounds, k_edges_per_cube_count, edges);
		for (long i = 0; i < k_edges_per_cube_count; i++)
			render_debug_line(true, edges[i], &edges[i][1], color);
	}
	else
	{
		render_debug_add_cache_entry(_render_debug_type_box_outline, bounds, color);
	}
}

void __cdecl render_debug_box_outline_oriented(bool draw_immediately, real_rectangle3d const* bounds, real_matrix4x3 const* matrix, real_argb_color const* color)
{
	ASSERT(bounds);
	ASSERT(matrix);
	ASSERT(color);

	real_point3d edges[k_edges_per_cube_count][2]{};
	rectangle3d_build_edges(bounds, k_edges_per_cube_count, edges);
	for (long i = 0; i < k_edges_per_cube_count; i++)
	{
		matrix4x3_transform_point(matrix, edges[i], edges[i]);
		matrix4x3_transform_point(matrix, &edges[i][1], &edges[i][1]);
		render_debug_line(draw_immediately, edges[i], &edges[i][1], color);
	}
}

void __cdecl render_debug_polygon(real_point3d const* points, short total_point_count, real_argb_color const* color)
{
	if (total_point_count > 1)
	{
		ASSERT(points);
		ASSERT(color);

		for (short i = 1; i < total_point_count - 1; i++)
			render_debug_triangle(true, points, &points[i], &points[i + 1], color);
	}
}

void __cdecl render_debug_polygon_edges(real_point3d const* points, short total_point_count, real_argb_color const* color)
{
	if (total_point_count > 1)
	{
		ASSERT(points);
		ASSERT(color);

		if (total_point_count > 2)
		{
			render_debug_line(true, &points[total_point_count - 1], points, color);
			for (short i = 1; i < total_point_count; i++)
				render_debug_line(true, &points[i - 1], &points[i], color);
		}
	}
}

void __cdecl render_debug_k_graph(real_point3d const* points, short total_point_count, real_argb_color const* color)
{
	ASSERT(points);
	ASSERT(color);

	if (total_point_count > 2)
	{
		for (short i = 0; i < total_point_count; i++)
		{
			for (short j = i + 1; j < total_point_count; j++)
				render_debug_line(1, &points[i], &points[j], color);
		}
	}
}

void __cdecl render_debug_cone_outline(bool draw_immediately, real_point3d const* point, vector3d const* direction, real radius, real cone_angle, real_argb_color const* color)
{
	vector3d normalized_cone_direction = *direction;
	normalize3d(&normalized_cone_direction);

	real cone_vertex_offset = sinf(cone_angle) * radius;
	real cone_base_offset = cosf(cone_angle) * radius;

	real_point3d vertex_on_cone{};
	point_from_line3d(point, &normalized_cone_direction, cone_base_offset, &vertex_on_cone);

	vector3d perpendicular_to_cone_direction{};
	perpendicular3d(&normalized_cone_direction, &perpendicular_to_cone_direction);
	normalize3d(&perpendicular_to_cone_direction);

	vector3d perpendicular_cross_product{};
	cross_product3d(&perpendicular_to_cone_direction, &normalized_cone_direction, &perpendicular_cross_product);
	normalize3d(&perpendicular_cross_product);

	real_point3d cone_outline_points[10]{};
	for (long i = 0; i < NUMBEROF(cone_outline_points); i++)
	{
		real angle = (TWO_PI * i) / NUMBEROF(cone_outline_points);

		real_point3d* outline_point = &cone_outline_points[i];
		point_from_line3d(&vertex_on_cone, &perpendicular_to_cone_direction, (cone_vertex_offset * sinf(angle)), outline_point);

		real_point3d* cross_product_point = &cone_outline_points[i];
		point_from_line3d(&cone_outline_points[i], &perpendicular_cross_product, (cone_vertex_offset * cosf(angle)), cross_product_point);
	}

	for (long i = 0; i < NUMBEROF(cone_outline_points); i++)
	{
		long next_point_index = (i + 1) % NUMBEROF(cone_outline_points);
		render_debug_line(draw_immediately, &cone_outline_points[i], &cone_outline_points[next_point_index], color);
		render_debug_line(draw_immediately, &cone_outline_points[i], point, color);
	}
}

void __cdecl render_debug_string(char const* string)
{
	ASSERT(string);

	//render_debug_string_immediate(false, NULL, 0, string);
	render_debug_add_cache_entry(_render_debug_type_string, string);
}

void __cdecl render_debug_string_at_point(real_point3d const* point, char const* string, union real_argb_color const* color)
{
	ASSERT(point);
	ASSERT(string);
	ASSERT(color);

	//render_debug_string_at_point_immediate(point, string, color, 0.6f);
	render_debug_add_cache_entry(_render_debug_type_string_at_point, string, point, color, 0.6f);
}

void __cdecl render_debug_string_immediate(bool draw_immediately, short const* tab_stops, short tab_stop_count, char const* string)
{
	if (string && *string)
	{
		if (draw_immediately || get_render_debug_globals()->use_simple_font)
		{
			c_simple_font_draw_string draw_string;
			draw_string.set_tab_stops(tab_stops, tab_stop_count);
			interface_set_bitmap_text_draw_mode(&draw_string, 0, -1, 0, 0, 5, 0);
			draw_string.draw(NULL, string);
		}
		else
		{
			c_rasterizer_draw_string draw_string;
			c_font_cache_mt_safe font_cache;

			draw_string.set_tab_stops(tab_stops, tab_stop_count);
			interface_set_bitmap_text_draw_mode(&draw_string, 0, -1, 0, 0, 5, 0);
			draw_string.draw(&font_cache, string);
		}
	}
}

void __cdecl render_debug_string_at_point_immediate(real_point3d const* point, char const* string, real_argb_color const* color, real scale)
{
	ASSERT(point);
	ASSERT(string);
	ASSERT(color);

	render_camera const* camera = c_player_view::get_current()->get_rasterizer_camera();
	render_projection const* projection = c_player_view::get_current()->get_rasterizer_projection();

	vector2d aspect_ratio_scale{};
	vector2d aspect_ratio_scaling = interface_get_aspect_ratio_scaling();
	aspect_ratio_scale.i = 1.0f / aspect_ratio_scaling.i;
	aspect_ratio_scale.j = 1.0f / aspect_ratio_scaling.j;

	short_rectangle2d window_display_bounds = camera->window_pixel_bounds;
	interface_scale_rectangle2d_for_xenon_scaler(&window_display_bounds, &aspect_ratio_scale);

	real_point2d screen_point{};
	if (render_camera_world_to_window(camera, projection, &window_display_bounds, point, &screen_point))
	{
		real text_scale = aspect_ratio_scale.i;
		if (scale > 0.01f)
		{
			real v9 = point->y - camera->position.y;
			real v10 = point->z - camera->position.z;
			text_scale = scale / (square_root(((v9 * v9) + ((point->x - camera->position.x) * (point->x - camera->position.x))) + (v10 * v10)) + scale);
		}

		short_rectangle2d bounds{};
		bounds.x0 = static_cast<short>(screen_point.x - window_display_bounds.x0);
		bounds.y0 = static_cast<short>(screen_point.y - window_display_bounds.y0);
		bounds.x1 = SHRT_MAX;
		bounds.y1 = SHRT_MAX;

		if (g_render_debug_globals->use_simple_font)
		{
			c_simple_font_draw_string draw_string;
			interface_set_bitmap_text_draw_mode(&draw_string, 0, -1, 0, 0, 5, 0);
			draw_string.set_shadow_color(global_real_argb_black);
			draw_string.set_color(color);
			draw_string.set_bounds(&bounds);
			if (scale > 0.01f)
				draw_string.set_scale(scale);
			draw_string.draw(NULL, string);
		}
		else
		{
			c_rasterizer_draw_string draw_string;
			c_font_cache_mt_safe font_cache;

			interface_set_bitmap_text_draw_mode(&draw_string, 0, -1, 0, 0, 5, 0);
			draw_string.set_shadow_color(global_real_argb_black);
			draw_string.set_color(color);
			draw_string.set_bounds(&bounds);
			if (scale > 0.01f)
				draw_string.set_scale(scale);
			draw_string.draw(&font_cache, string);
		}
	}
}

bool __cdecl render_debug_draw_immediately(real_argb_color const* color)
{
	ASSERT(g_render_debug_globals);

	if (!render_debug_allowed_in_current_thread() || !render_debug_active())
		return false;

	if (color)
	{
		if (!render_debug_cache_currently_drawing())
			return color->alpha >= 1.0f;
	}

	return true;
}

void __cdecl render_debug_add_cache_entry(short type, ...)
{
	ASSERT(g_render_debug_globals);

	if (g_render_debug_globals->cache_count < NUMBEROF(g_render_debug_globals->cache_entries))
	{
		s_cache_entry* entry = &g_render_debug_globals->cache_entries[g_render_debug_globals->cache_count++];

		real alpha = 1.0f;

		va_list list;
		va_start(list, type);

		switch (type)
		{
		case _render_debug_type_circle:
		{
			entry->circle.plane = *va_arg(list, plane3d*);
			entry->circle.projection_axis = (short)va_arg(list, int);
			entry->circle.projection_sign = (bool)va_arg(list, int);
			entry->circle.center = *va_arg(list, real_point2d*);
			entry->circle.radius = (real)va_arg(list, double);
			entry->circle.color = *va_arg(list, real_argb_color*);
			entry->circle.a8 = (real)va_arg(list, double);
			alpha = entry->circle.color.alpha;

			real_point3d centroid{};
			project_point2d(&entry->circle.center, &entry->circle.plane, entry->circle.projection_axis, entry->circle.projection_sign, &centroid);
			ASSERT(VALID_INDEX(entry->circle.projection_axis, NUMBEROF(centroid.n)));
		}
		break;
		case _render_debug_type_point:
		{
			entry->point.point = *va_arg(list, real_point3d*);
			entry->point.scale = (real)va_arg(list, double);
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
			entry->sphere.radius = (real)va_arg(list, double);
			entry->sphere.color = *va_arg(list, real_argb_color*);
			alpha = entry->sphere.color.alpha;
		}
		break;
		case _render_debug_type_cylinder:
		{
			entry->cylinder.base = *va_arg(list, real_point3d*);
			entry->cylinder.height = *va_arg(list, vector3d*);
			entry->cylinder.radius = (real)va_arg(list, double);
			entry->cylinder.color = *va_arg(list, real_argb_color*);
			alpha = entry->sphere.color.alpha;
		}
		break;
		case _render_debug_type_pill:
		{
			entry->pill.base = *va_arg(list, real_point3d*);
			entry->pill.height = *va_arg(list, vector3d*);
			entry->pill.radius = (real)va_arg(list, double);
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
			char const* string = va_arg(list, char const*);
			long string_offset = render_debug_add_cache_string(string);
			if (string_offset != NONE)
			{
				entry->string.string_offset = string_offset;
				entry->string.tab_stop_count = 0;

				//LABEL_46
				entry->type = type;
				entry->layer = 0;
				entry->__unknown4 = 0.0f;
				if (g_render_debug_globals->group_level > 0)
					entry->__unknown4 = g_render_debug_globals->__unknown4;
			}
		}
		break;
		case _render_debug_type_string_at_point:
		{
			char const* string = va_arg(list, char const*);
			long string_offset = render_debug_add_cache_string(string);
			if (string_offset != NONE)
			{
				entry->string_at_point.string_offset = string_offset;
				entry->string_at_point.point = *va_arg(list, real_point3d*);
				entry->string_at_point.color = *va_arg(list, real_argb_color*);
				entry->string_at_point.scale = (real)va_arg(list, double);
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
			entry->__unknown4 = 0.0f;
			if (g_render_debug_globals->group_level > 0)
				entry->__unknown4 = g_render_debug_globals->__unknown4;
			return;
		}
		break;
		default:
		{
			entry->type = type;
			entry->layer = 0;
			entry->__unknown4 = 0.0f;
			if (g_render_debug_globals->group_level > 0)
				entry->__unknown4 = g_render_debug_globals->__unknown4;
			return;
		}
		break;
		}

		entry->type = type;

		if (alpha > 0.0f)
			entry->layer = alpha < 1.0f ? 1 : 0;
		else
			g_render_debug_globals->cache_count--;

		if (g_render_debug_globals->group_level <= 0)
			entry->__unknown4 = 0.0f;
		else
			entry->__unknown4 = g_render_debug_globals->__unknown4;
	}

	static bool render_debug_cache_overflow = false;
	if (!render_debug_cache_overflow)
	{
		generate_event(_event_level_warning, "render debug cache overflow.");
		render_debug_cache_overflow = true;
	}
}

void __cdecl render_debug_cache_draw(bool a1, bool a2, bool a3)
{
	s_render_debug_globals* g_render_debug_globals = get_render_debug_globals();

	short cache_start_index = 0;
	if (!a1)
		cache_start_index = g_render_debug_globals->cache_start_index;

	if (g_render_debug_globals->cache_count - cache_start_index > 0)
	{
		short cache_layer = 0;
		long type = 0;

		ASSERT(!g_render_debug_globals->drawing_cached_geometry);

		g_render_debug_globals->drawing_cached_geometry = true;

		long alpha_blend_modes[2] = { 0, 3 };
		for (short cache_index = cache_start_index; cache_index < g_render_debug_globals->cache_count; cache_index++)
		{
			s_cache_entry* entry = &g_render_debug_globals->cache_entries[cache_index];
			if (VALID_INDEX(entry->layer, NUMBEROF(alpha_blend_modes))) // `NUMBEROF(g_render_debug_globals->group_ids)` ?
			{
				c_rasterizer::set_alpha_blend_mode(static_cast<c_rasterizer::e_alpha_blend_mode>(alpha_blend_modes[entry->layer]));
			}
			else
			{
				generate_event(_event_level_warning, "render:debug: unknown debug render cache layer %d!!!", cache_layer);
			}

			if (type_list[entry->type] != type)
			{
				ASSERT(VALID_INDEX(entry->type, NUMBEROF(type_list)));
				type = type_list[entry->type];
			}

			if (!a2 || entry->type == _render_debug_type_string)
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
						entry->circle.a8);
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
						entry->cylinder.radius,
						&entry->cylinder.color);
				}
				break;
				case _render_debug_type_pill:
				{
					render_debug_pill(true,
						&entry->pill.base,
						&entry->pill.height,
						entry->pill.radius,
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
					char const* string = g_render_debug_globals->cache_string + entry->string_at_point.string_offset;
					ASSERT(string != NULL);

					render_debug_string_immediate(true,
						entry->string.tab_stops,
						entry->string.tab_stop_count,
						string);
				}
				break;
				case _render_debug_type_string_at_point:
				{
					char const* string = g_render_debug_globals->cache_string + entry->string_at_point.string_offset;
					ASSERT(string != NULL);

					render_debug_string_at_point_immediate(
						&entry->string_at_point.point,
						string,
						&entry->string_at_point.color,
						entry->string_at_point.scale);
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
					// ASSERT(halt());
					break;
				}
			}
		}

		g_render_debug_globals->drawing_cached_geometry = false;
	}

	if (a3)
	{
		g_render_debug_globals->cache_count = g_render_debug_globals->cache_start_index;
		g_render_debug_globals->cache_string_length = g_render_debug_globals->__unknown8001E;
	}
}

// `sound/game_sound_spatialization.cpp`
void __cdecl render_debug_polygon_fan(real_point3d const* points, short total_point_count, real_argb_color const* color)
{
	if (total_point_count > 1)
	{
		ASSERT(points);
		ASSERT(color);

		real_argb_color color_ = *color;
		color_.alpha = 1.0f;

		for (short i = 1; i < total_point_count - 1; i++)
		{
			render_debug_triangle(true, points, &points[i], &points[i + 1], color);
			render_debug_line(true, points, &points[i], &color_);
			render_debug_line(true, &points[i], &points[i + 1], &color_);
			render_debug_line(true, points, &points[i + 1], &color_);
		}
	}
}

real __cdecl build_height_matrix(real_point3d const* base, vector3d const* height, real_matrix4x3* out_matrix)
{
	out_matrix->scale = 1.0f;
	out_matrix->up = *height;
	perpendicular3d(&out_matrix->up, &out_matrix->left);
	real result = normalize3d(&out_matrix->up);
	normalize3d(&out_matrix->left);
	cross_product3d(&out_matrix->left, &out_matrix->up, &out_matrix->forward);
	out_matrix->position = *base;

	return result;
}

void __cdecl render_debug_build_circle_points(real radius, real_point2d* points, long total_point_count)
{
	ASSERT(total_point_count > 0);

	real angle = (TWO_PI / (total_point_count - 1));
	real sin_angle = sinf(angle);
	real cos_angle = cosf(angle);
	set_real_point2d(points, radius, 0.0f);
	for (long i = 0; i + 1 < total_point_count; i++)
		rotate_vector2d((vector2d*)&points[i], sin_angle, cos_angle, (vector2d*)&points[i + 1]);

	points[total_point_count - 1] = *points;
}

void __cdecl render_debug_build_pill_points(real_point3d const* base, vector3d const* height, real radius, real_point3d* points0, real_point3d* points1, real_point3d* points2, real_point3d* points3, real_point3d* points4, real_point3d* points5)
{
	real_matrix4x3 height_matrix{};
	real normalized_up = build_height_matrix(base, height, &height_matrix);

	real_point2d circle_points[CIRCLE_DIVISIONS + 1]{};
	render_debug_build_circle_points(radius, circle_points, CIRCLE_DIVISIONS + 1);

	if (points0 && points1)
	{
		for (long i = 0; i < CIRCLE_DIVISIONS + 1; i++)
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
		for (long i = 0; i < (CIRCLE_DIVISIONS / 2) + 1; i++)
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

word __cdecl _random(dword* seed, char const* string, char const* file, dword line)
{
	//random_seed_debug(seed, "random", string, file, line);
	*seed = 0x19660D * *seed + 0x3C6EF35F;

	return HIWORD(*seed);
}

real_point3d* __cdecl rectangle3d_center(real_rectangle3d const* rect, real_point3d* center)
{
	center->x = 0.5f * (rect->x.lower + rect->x.upper);
	center->y = 0.5f * (rect->y.lower + rect->y.upper);
	center->z = 0.5f * (rect->z.lower + rect->z.upper);

	return center;
}

c_render_debug_line_drawer::c_render_debug_line_drawer()
{
	short_rectangle2d fullscreen_render_pixel_bounds{};
	c_rasterizer::get_fullscreen_render_pixel_bounds(&fullscreen_render_pixel_bounds);

	vertex_scale_x = 1.0f / (fullscreen_render_pixel_bounds.x1 - fullscreen_render_pixel_bounds.x0);
	vertex_scale_y = 1.0f / (fullscreen_render_pixel_bounds.y1 - fullscreen_render_pixel_bounds.y0);
	vertex_count = 0;
	type = 0;
	set_color(global_real_argb_white);
}

c_render_debug_line_drawer::~c_render_debug_line_drawer()
{
	flush();
}

void c_render_debug_line_drawer::flush()
{
	if (render_debug_draw_immediately(&real_color))
	{
		if (vertex_count > 0)
		{
			switch (type)
			{
			case 1:
				c_rasterizer::draw_debug_line_list2d_explicit(vertices, vertex_count / 2);
				break;
			case 2:
				c_rasterizer::draw_debug_line_list_explicit(vertices, vertex_count / 2);
				break;
			}
		}
	}
	else
	{
		generate_event(_event_level_error, "can't use debug_line_drawer w/o immediate debug drawing");
	}

	vertex_count = 0;
}

void c_render_debug_line_drawer::set_color(real_argb_color const* color_)
{
	color_degamma(&color_->color, reinterpret_cast<real_linear_rgb_color*>(&real_color.color));
	real_color.alpha = color_->alpha;
	color.value = real_argb_color_to_pixel32(&real_color);
}

void c_render_debug_line_drawer::add_line_2d(real_point2d const* p0, real_point2d const* p1)
{
	if (type != 1 || vertex_count + 2 >= 512)
	{
		flush();
		type = 1;
	}

	rasterizer_vertex_debug* vertex = vertices + vertex_count;
	vertex_count += 2;

	vertex[0].point.x = ((p0->x * vertex_scale_x) * 2.0f) - 1.0f;
	vertex[0].point.y = -(((p0->y * vertex_scale_y) * 2.0f) - 1.0f);
	vertex[0].point.z = 0.0f;
	vertex[0].color = color;

	vertex[1].point.x = ((p1->x * vertex_scale_x) * 2.0f) - 1.0f;
	vertex[1].point.y = -(((p1->y * vertex_scale_y) * 2.0f) - 1.0f);
	vertex[1].point.z = 0.0f;
	vertex[1].color = color;
}

void c_render_debug_line_drawer::add_line_3d(real_point3d const* p0, real_point3d const* p1)
{
	if (type != 2 || vertex_count + 2 >= 512)
	{
		flush();
		type = 2;
	}

	rasterizer_vertex_debug* vertex = vertices + vertex_count;
	vertex_count += 2;

	vertex[0].point = *p0;
	vertex[0].color = color;

	vertex[1].point = *p1;
	vertex[1].color = color;
}

void c_render_debug_line_drawer::add_line_3d_unclipped(real_point3d const* p0, real_point3d const* p1)
{
	ASSERT(p0);
	ASSERT(p1);

	real_matrix4x3 camera{};
	c_player_view::get_player_render_camera_orientation(&camera);

	real_point3d point0 = *p0;
	real_point3d point1 = *p1;

	vector3d vector0{};
	vector3d vector1{};
	vector_from_points3d(&camera.position, &point0, &vector0);
	vector_from_points3d(&camera.position, &point1, &vector1);

	real clip_distance = magnitude3d(&vector0);
	if (magnitude3d(&vector0) <= magnitude3d(&vector1))
		clip_distance = magnitude3d(&vector1);

	real z_far = c_player_view::get_current()->get_rasterizer_camera()->z_far;
	if (clip_distance > (0.5f * z_far))
	{
		real distance = (0.5f * z_far) / clip_distance;
		point_from_line3d(&camera.position, &vector0, distance, &point0);
		point_from_line3d(&camera.position, &vector1, distance, &point1);
	}

	add_line_3d(&point0, &point1);
}

