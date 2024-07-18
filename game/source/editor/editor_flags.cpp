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
		//long flag_count = 0;
		//editor_comment_definition* flags = editor_flags_get(&flag_count);
		//for (long flag_index = 0; flag_index < flag_count; flag_index++)
		//{
		//	editor_comment_definition* flag = &flags[flag_index];
		//	editor_flag_render(flag->name,
		//		render_comment_flags_text ? flag->description : NULL,
		//		&flag->position,
		//		global_real_argb_red,
		//		global_real_argb_salmon);
		//}
	}

	if (enable_controller_flag_drop || render_comment_flags_look_at)
		editor_flag_render_look_at_point();
}

void editor_flag_new_at_camera()
{
	if (enable_controller_flag_drop)
	{
		e_output_user_index user_index = player_mapping_first_active_output_user();
		if (user_index != NONE)
		{
			s_observer_result const* result = observer_get_camera(user_index);
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
		e_output_user_index user_index = player_mapping_first_active_output_user();
		real_point3d result_point = {};
		if (user_get_look_at_point(user_index, &result_point))
		{
			s_observer_result const* result = observer_get_camera(user_index);
			//editor_flag_new_internal(controller_flag_drop_name, controller_flag_drop_comment, &result_point);
		}
	}
}

