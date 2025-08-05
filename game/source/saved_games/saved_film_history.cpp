#include "saved_games/saved_film_history.hpp"

#include "game/game.hpp"
#include "rasterizer/rasterizer.hpp"
#include "saved_games/saved_film.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "text/draw_string.hpp"

void saved_film_history_render_debug()
{
	if (!game_is_playback() || !saved_film_manager_timestamp_enabled_internal())
	{
		return;
	}

	s_saved_film_hud_interface_state hud_state{};
	saved_film_manager_get_hud_interface_state(&hud_state);

	real_point3d box_points[4]
	{
		{ 100.0f, 100.0f, 0.0f },
		{ 300.0f, 100.0f, 0.0f },
		{ 300.0f, 125.0f, 0.0f },
		{ 100.0f, 125.0f, 0.0f }
	};

	c_rasterizer::draw_debug_line2d(box_points[0], box_points[1], 0xFFFFFFFF, 0xFFFFFFFF);
	c_rasterizer::draw_debug_line2d(box_points[1], box_points[2], 0xFFFFFFFF, 0xFFFFFFFF);
	c_rasterizer::draw_debug_line2d(box_points[2], box_points[3], 0xFFFFFFFF, 0xFFFFFFFF);
	c_rasterizer::draw_debug_line2d(box_points[3], box_points[0], 0xFFFFFFFF, 0xFFFFFFFF);

	box_points[1].y = 105.0f;
	box_points[0].y = 105.0f;
	box_points[2].y = 120.0f;
	box_points[3].y = 120.0f;

	box_points[1].x = 100.0f + (200.0f * hud_state.buffered_theta);
	box_points[2].x = 100.0f + (200.0f * hud_state.buffered_theta);

	if (hud_state.recording)
	{
		box_points[3].x = 100.0f + (200.0f * hud_state.recording_start_theta);
		box_points[0].x = box_points[3].x;
		box_points[2].x = 100.0f + (200.0f * hud_state.current_position_theta);
		box_points[1].x = box_points[2].x;

		c_rasterizer::draw_debug_line2d(box_points[0], box_points[1], 0xFFFF0000, 0xFFFF0000);
		c_rasterizer::draw_debug_line2d(box_points[1], box_points[2], 0xFFFF0000, 0xFFFF0000);
		c_rasterizer::draw_debug_line2d(box_points[2], box_points[3], 0xFFFF0000, 0xFFFF0000);
		c_rasterizer::draw_debug_line2d(box_points[3], box_points[0], 0xFFFF0000, 0xFFFF0000);
	}

	real_point3d triangle_points[3]
	{
		{  100.0f + (200.0f * hud_state.current_position_theta),         125.0f,        1.0f },
		{ (100.0f + (200.0f * hud_state.current_position_theta)) + 5.0f, 125.0f + 5.0f, 1.0f },
		{ (100.0f + (200.0f * hud_state.current_position_theta)) - 5.0f, 125.0f + 5.0f, 1.0f },
	};

	c_rasterizer::draw_debug_line2d(triangle_points[0], triangle_points[1], 0xFFFFFFFF, 0xFFFFFFFF);
	c_rasterizer::draw_debug_line2d(triangle_points[1], triangle_points[2], 0xFFFFFFFF, 0xFFFFFFFF);
	c_rasterizer::draw_debug_line2d(triangle_points[2], triangle_points[0], 0xFFFFFFFF, 0xFFFFFFFF);

	c_rasterizer_draw_string draw_string{};
	c_font_cache_mt_safe font_cache{};

	rectangle2d rectangle{};
	set_rectangle2d(&rectangle, (int16)triangle_points[0].x, (int16)(triangle_points[0].y + 25.0f), 1000, 1000);
	draw_string.set_bounds(&rectangle);

	char text[128]{};
	draw_string.draw(&font_cache, text);
}

