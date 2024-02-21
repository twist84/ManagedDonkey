#include "editor/editor_flags.hpp"

#include "camera/observer.hpp"
#include "game/player_mapping.hpp"
#include "toolbox/game_helpers.hpp"

bool render_comment_flags = false;
bool enable_controller_flag_drop = false;
bool render_comment_flags_look_at = false;

void render_debug_scenario_comments()
{
	if (render_comment_flags)
	{
		// #TODO: implement
	}

	if (enable_controller_flag_drop || render_comment_flags_look_at)
		editor_flag_render_look_at_point();
}

void editor_flag_new_at_camera()
{
	if (enable_controller_flag_drop)
	{
		long active_user = player_mapping_first_active_input_user();
		if (active_user != NONE)
		{
			s_observer_result const* result = observer_get_camera(active_user);
			//editor_flag_new_internal(controller_flag_drop_name, controller_flag_drop_comment, &result->focus_point);
		}
	}
}

void editor_flag_render_look_at_point()
{
	// #TODO: implement
}

void editor_flag_new_at_look_at_point()
{
	if (enable_controller_flag_drop)
	{
		long active_user = player_mapping_first_active_input_user();
		real_point3d result_point = {};
		if (user_get_look_at_point(active_user, &result_point))
		{
			s_observer_result const* result = observer_get_camera(active_user);
			//editor_flag_new_internal(controller_flag_drop_name, controller_flag_drop_comment, &result_point);
		}
	}
}

