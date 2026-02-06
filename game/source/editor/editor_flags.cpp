#include "editor/editor_flags.hpp"

#include "camera/observer.hpp"
#include "game/player_mapping.hpp"
#include "toolbox/game_helpers.hpp"

bool enable_controller_flag_drop = false;
bool render_comment_flags = false;
bool render_comment_flags_look_at = false;

void editor_flag_new_at_camera()
{
	//if (enable_controller_flag_drop)
	//{
	//	int32 active_output_user = player_mapping_first_active_output_user();
	//	if (active_output_user != NONE)
	//	{
	//		const s_observer_result* camera = observer_get_camera(active_output_user);
	//		editor_flag_new_internal(g_default_flag_name.get_string(), g_default_flag_description.get_string(), &camera->position);
	//	}
	//}
}

void editor_flag_new_at_look_at_point()
{
	//if (enable_controller_flag_drop)
	//	editor_scripted_flag_new_at_look_at_point(g_default_flag_name.get_string(), g_default_flag_description.get_string());
}

void editor_flag_render_look_at_point()
{
	//int32 active_output_user = player_mapping_first_active_output_user();
	//real_point3d look_at_point{};
	//if (user_get_look_at_point(active_output_user, &look_at_point))
	//	editor_render_flag_internal(g_default_flag_name.get_string(), g_default_flag_description.get_string(), &look_at_point, global_real_argb_blue, global_real_argb_cyan);
}

void render_debug_scenario_comments()
{
	//if (render_comment_flags)
	//{
	//	int32 flag_count = 0;
	//	editor_comment_definition* flags = editor_flags_get(&flag_count);
	//	for (int32 flag_index = 0; flag_index < flag_count; flag_index++)
	//	{
	//		editor_comment_definition* flag = &flags[flag_index];
	//		editor_flag_render(flag->name,
	//			render_comment_flags_text ? flag->description : nullptr,
	//			&flag->position,
	//			global_real_argb_red,
	//			global_real_argb_salmon);
	//	}
	//}
	//
	//if (enable_controller_flag_drop || render_comment_flags_look_at)
	//	editor_flag_render_look_at_point();
}

