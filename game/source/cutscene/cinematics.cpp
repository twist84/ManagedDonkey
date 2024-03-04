#include "cutscene/cinematics.hpp"

//bool g_debug_cinematic_controls_enable = false;
//bool g_cinematic_debugging_enable = false;
//bool g_cinematic_render_enable = false;
//bool* console_status_render = &console_globals.status_render;

void __cdecl cinematic_game_pause_handler(bool paused)
{
	INVOKE(0x0067CEC0, cinematic_game_pause_handler, paused);
}

bool __cdecl cinematic_in_progress()
{
	return INVOKE(0x0067D270, cinematic_in_progress);
}

void __cdecl cinematic_update()
{
	return INVOKE(0x0067EDF0, cinematic_update);
}

void __cdecl cinematics_game_tick()
{
	return INVOKE(0x0067F080, cinematics_game_tick);
}

void __cdecl cinematic_debug_camera_control_update()
{
	//TLS_DATA_GET_VALUE_REFERENCE(cinematic_new_globals);
	//
	//e_input_user_index input_user_index = player_mapping_first_active_input_user();
	//long player_index = NONE;
	//
	//if (input_user_index != k_input_user_none)
	//	player_index = player_mapping_get_player_by_input_user(input_user_index);
	//
	//if (player_index == NONE)
	//	return;
	//
	//if (cinematic_new_globals->cinematic_tag_index == NONE)
	//	return;
	//
	//if (!g_debug_cinematic_controls_enable)
	//	return;
	//
	//e_controller_index controller_index = player_mapping_get_input_controller(player_index);
	//if (controller_index == k_no_controller || camera_input_inhibited(controller_index))
	//	return;
	//
	//s_game_input_state* input_state = NULL;
	//input_abstraction_get_input_state(controller_index, &input_state);
	//
	//if (!g_cinematic_debugging_enable)
	//	debug_game_speed = 1.0f;
	//
	//if (input_state->get_button(_button_action_left_trigger).down_amount())
	//	debug_game_speed = 1.0f + (-1.0f * input_state->get_button(_button_action_left_trigger).down_amount());
	//
	//if (input_state->get_button(_button_action_right_trigger).down_amount())
	//	debug_game_speed = 4.0f  * input_state->get_button(_button_action_right_trigger).down_amount();
	//
	//if (input_state->get_button(_button_action_y).down_amount())
	//{
	//	input_state->get_button(_button_action_y).latch();
	//
	//	bool cinematic_render_enable = !g_cinematic_render_enable;
	//	*console_status_render = cinematic_render_enable;
	//	//events_debug_spam_render = cinematic_render_enable;
	//	g_cinematic_render_enable = cinematic_render_enable;
	//}
	//
	//if (input_state->get_button(_button_action_b).down_frames())
	//{
	//	input_state->get_button(_button_action_b).latch();
	//
	//	if (fabsf((debug_game_speed - 0.0f)) >= _real_epsilon)
	//	{
	//		debug_game_speed = 0.0f;
	//		g_cinematic_debugging_enable = true;
	//	}
	//	else
	//	{
	//		debug_game_speed = 1.0f;
	//		g_cinematic_debugging_enable = true;
	//	}
	//}
	//
	//if (input_state->get_button(_button_action_dpad_down).down_amount())
	//{
	//	input_state->get_button(_button_action_dpad_down).latch();
	//	debug_game_speed = 1.0f;
	//	g_cinematic_debugging_enable = true;
	//}
	//
	//if (debug_director_should_camera_mode_change(input_state))
	//{
	//	input_state->get_button(_button_action_x).latch();
	//	if (director_in_scripted_camera())
	//	{
	//		e_output_user_index output_user_index = player_mapping_first_active_output_user();
	//		director_script_camera(false);
	//		if (output_user_index != k_output_user_none)
	//			director_set_mode(output_user_index, _director_mode_debug);
	//		set_debug_force_scripted_camera_disable(true);
	//	}
	//	else
	//	{
	//		set_debug_force_scripted_camera_disable(false);
	//		director_script_camera(true);
	//		scripted_camera_set_cinematic();
	//	}
	//}
}

void __cdecl draw_quad(short_rectangle2d* rect, dword color)
{
	return INVOKE(0x0067F200, draw_quad, rect, color);
}

