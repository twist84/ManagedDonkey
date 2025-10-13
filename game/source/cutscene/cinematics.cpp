#include "cutscene/cinematics.hpp"

#include "game/game.hpp"
#include "input/input_windows.hpp"

REFERENCE_DECLARE(0x024B0A3E, bool, g_cinematic_debug_mode) = true;

//bool g_debug_cinematic_controls_enable = false;
//bool g_cinematic_debugging_enable = false;
//bool g_cinematic_render_enable = false;

void __cdecl cinematic_dispose()
{
	INVOKE(0x0067CE30, cinematic_dispose);
}

void __cdecl cinematic_dispose_from_old_map()
{
	INVOKE(0x0067CE40, cinematic_dispose_from_old_map);
}

void __cdecl cinematic_dispose_from_old_non_bsp_zone_set(const s_game_non_bsp_zone_set* old_non_bsp_zone_set)
{
	INVOKE(0x0067CE70, cinematic_dispose_from_old_non_bsp_zone_set, old_non_bsp_zone_set);
}

void __cdecl cinematic_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	INVOKE(0x0067CE80, cinematic_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

//.text:0067CE90 ; void __cdecl cinematic_force_title(int16)

void __cdecl cinematic_game_pause_handler(bool paused)
{
	INVOKE(0x0067CEC0, cinematic_game_pause_handler, paused);
}

//.text:0067CF10 ; e_cinematic_channel_type __cdecl cinematic_get_channel_type()
//.text:0067CF60 ; const s_cinematic_clip_global* __cdecl cinematic_get_clip_planes(int32*)

bool __cdecl cinematic_get_debug_mode()
{
	//return INVOKE(0x0067CF90, cinematic_get_debug_mode);

	return g_cinematic_debug_mode;
}

//.text:0067CFA0 ; void __cdecl cinematic_get_default_chapter_title_bounds(rectangle2d*)
//.text:0067CFE0 ; int16 __cdecl cinematic_get_descriptor()
//.text:0067D0A0 ; int32 __cdecl cinematic_get_early_exit()
//.text:0067D0C0 ; real64 __cdecl cinematic_get_fade_amount()
//.text:0067D130 ; real64 __cdecl cinematic_get_letterbox_coverage()
//.text:0067D1C0 ; int32 __cdecl cinematic_get_scene_index(int32, int32)
//.text:0067D220 ; void __cdecl cinematic_get_shot()
//.text:0067D230 ; int32 __cdecl cinematic_get_shot_index()
//.text:0067D250 ; int32 __cdecl cinematic_get_tag_index()

bool __cdecl cinematic_in_progress()
{
	return INVOKE(0x0067D270, cinematic_in_progress);
}

void __cdecl cinematic_initialize()
{
	INVOKE(0x0067D2A0, cinematic_initialize);
}

void __cdecl cinematic_initialize_for_new_map()
{
	INVOKE(0x0067D300, cinematic_initialize_for_new_map);
}

void __cdecl cinematic_initialize_for_new_non_bsp_zone_set(const s_game_non_bsp_zone_set* new_non_bsp_zone_set)
{
	INVOKE(0x0067D400, cinematic_initialize_for_new_non_bsp_zone_set, new_non_bsp_zone_set);
}

void __cdecl cinematic_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	INVOKE(0x0067D410, cinematic_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

bool __cdecl cinematic_is_letterbox()
{
	return INVOKE(0x0067D420, cinematic_is_letterbox);
}

//.text:0067D480 ; void __cdecl cinematic_move_attached_objects(int32, int32, int32, int32)
//.text:0067D550 ; void __cdecl cinematic_new_globals_initialize()
//.text:0067D620 ; int32 __cdecl cinematic_object_create(const char*, int32, int32, uns32)
//.text:0067D750 ; void __cdecl cinematic_object_create_cinematic_anchor(const char*, int32)
//.text:0067D820 ; void __cdecl cinematic_object_create_from_cinematic_scene(const char*)
//.text:0067D950 ; void __cdecl cinematic_object_destroy(const char*)
//.text:0067DA30 ; void __cdecl cinematic_object_destroy_internal(int32)

int32 __cdecl cinematic_object_get(const char* object_name)
{
	return INVOKE(0x0067DAB0, cinematic_object_get, object_name);
}

//.text:0067DB30 ; bool __cdecl cinematic_object_get_name_from_object_index(int32, char*, int32)
//.text:0067DBC0 ; int32 __cdecl cinematic_object_try_and_get_by_index(int32)
//.text:0067DC40 ; void __cdecl cinematic_outro_start()
//.text:0067DC60 ; void __cdecl cinematic_predict_shot(s_cinematic_scene*, int32, int32, c_tag_resource_collector*)

void __cdecl cinematic_prepare_for_new_zone_set(uns32 old_active_structure_bsp_mask, uns32 new_active_structure_bsp_mask)
{
	INVOKE(0x0067DCE0, cinematic_prepare_for_new_zone_set, old_active_structure_bsp_mask, new_active_structure_bsp_mask);
}

void __cdecl cinematic_prepare_for_non_bsp_zone_set_switch(const s_game_non_bsp_zone_set* old_non_bsp_zone_set, const s_game_non_bsp_zone_set* new_non_bsp_zone_set, c_scenario_resource_registry* active_tags_registry)
{
	INVOKE(0x0067DCF0, cinematic_prepare_for_non_bsp_zone_set_switch, old_non_bsp_zone_set, new_non_bsp_zone_set, active_tags_registry);
}

//.text:0067DDF0 ; void __cdecl cinematic_print(const char*)

void __cdecl cinematic_render(bool a1, bool a2)
{
	INVOKE(0x0067DE00, cinematic_render, a1, a2);
}

//.text:0067E4C0 ; bool __cdecl cinematic_reverts_when_skipped()
//.text:0067E4F0 ; 
//.text:0067E590 ; 
//.text:0067E5C0 ; 
//.text:0067E610 ; 
//.text:0067E640 ; void __cdecl cinematic_scripting_suppress_bsp_object_creation(bool)
//.text:0067E660 ; void __cdecl cinematic_set(int32)
//.text:0067E680 ; void __cdecl cinematic_set_chud_objective(int16)
//.text:0067E700 ; void __cdecl cinematic_set_chud_objective(int32, int16)
//.text:0067E7B0 ; void __cdecl cinematic_set_debug_mode(bool)
//.text:0067E7C0 ; void __cdecl cinematic_set_early_exit(int32)
//.text:0067E7E0 ; void __cdecl cinematic_set_shot(int32, int32)
//.text:0067E810 ; void __cdecl cinematic_set_title(int16)
//.text:0067E830 ; void __cdecl cinematic_set_title_delayed(int16, real32)
//.text:0067E8B0 ; void __cdecl cinematic_show_letterbox(bool)
//.text:0067E8D0 ; void __cdecl cinematic_show_letterbox_immediate(bool)
//.text:0067E920 ; bool __cdecl cinematic_skip_in_progress()
//.text:0067E950 ; void __cdecl cinematic_skip_start()
//.text:0067E970 ; void __cdecl cinematic_skip_stop()
//.text:0067E990 ; void __cdecl cinematic_start()

void __cdecl cinematic_start_user_skip_fade_out()
{
	INVOKE(0x0067EA10, cinematic_start_user_skip_fade_out);
}

//.text:0067EA60 ; void __cdecl cinematic_stop()
//.text:0067EAD0 ; bool __cdecl cinematic_suppressing_bsp_object_creation()
//.text:0067EAF0 ; int32 __cdecl cinematic_tag_reference_get_animation(int32, int32, int32)
//.text:0067EB70 ; int32 __cdecl cinematic_tag_reference_get_bink(int32)
//.text:0067EBB0 ; int32 __cdecl cinematic_tag_reference_get_cinematic(int32)
//.text:0067EBD0 ; int32 __cdecl cinematic_tag_reference_get_dialogue(int32, int32, int32, int32)
//.text:0067EC60 ; int32 __cdecl cinematic_tag_reference_get_effect(int32, int32, int32, int32)
//.text:0067ED00 ; int32 __cdecl cinematic_tag_reference_get_music(int32, int32, int32, int32)
//.text:0067ED90 ; int32 __cdecl cinematic_tag_reference_get_music_looping(int32, int32, int32, int32)
//.text:0067EDA0 ; int32 __cdecl cinematic_tag_reference_get_scene(int32)

void __cdecl cinematic_update()
{
	return INVOKE(0x0067EDF0, cinematic_update);
}

void __cdecl cinematics_game_tick()
{
	// CINEMATIC SKIP HACK

	if (game_is_campaign() && cinematic_in_progress())
	{
		e_input_key_code keys[]
		{
			_key_tab,
			_key_space
		};

		for (int32 key_index = 0; key_index < NUMBEROF(keys); key_index++)
		{
			if (input_key_frames_down(keys[key_index], _input_type_ui))
			{
				cinematic_start_user_skip_fade_out();
				break;
			}
		}
	}

	return INVOKE(0x0067F080, cinematics_game_tick);
}

//.text:0067F140 ; int32 __cdecl cortana_tag_reference_get_scene(int32)
//.text:0067F160 ; 
//.text:0067F170 ; 
//.text:0067F1A0 ; 
//.text:0067F1D0 ; 

void __cdecl draw_quad(rectangle2d* rect, uns32 color)
{
	return INVOKE(0x0067F200, draw_quad, rect, color);
}

//.text:0067F260 ; 
//.text:0067F270 ; 
//.text:0067F2A0 ; 
//.text:0067F2D0 ; 
//.text:0067F300 ; 
//.text:0067F310 ; void __cdecl get_cinematic_bounds(rectangle2d*, rectangle2d*, rectangle2d*)
//.text:0067F520 ; void __cdecl get_cinematic_bounds_screen_space(rectangle2d*, rectangle2d*, rectangle2d *)
//.text:0067F570 ; s_cinematic_light_globals* __cdecl get_cinematic_light_globals()
//.text:0067F590 ; 
//.text:0067F5A0 ; s_cinematic_scene* __cdecl global_cinematic_scene_try_and_get()
//.text:0067F5D0 ; void __cdecl hs_subtitle(int32, real32)
//.text:0067F5F0 ; void __cdecl iterate_future_shots(int32, void (__cdecl*)(s_cinematic_scene*, int32, int32, c_tag_resource_collector*), class c_tag_resource_collector*)
//.text:0067F720 ; void __cdecl predict_shot_dialogue_by_frame(s_cinematic_scene*, int32, int32, int32)
//.text:0067F7E0 ; void __cdecl predict_shot_dialogue(s_cinematic_scene*, int32, int32, c_tag_resource_collector*)
//.text:0067F800 ; 
//.text:0067F810 ; 
//.text:0067F830 ; 
//.text:0067F870 ; 
//.text:0067F8B0 ; 
//.text:0067F8F0 ; 
//.text:0067F950 ; 
//.text:0067F960 ; void __cdecl subtitle_add(int32, real32)
//.text:0067F9E0 ; real32 __cdecl subtitle_compute_alpha(const s_subtitle*)
//.text:0067FA30 ; void __cdecl subtitle_delete()
//.text:0067FA60 ; void __cdecl subtitle_render()
//.text:0067FF40 ; bool __cdecl subtitle_should_render()
//.text:0067FFC0 ; 

void cinematic_debug_camera_control_update()
{
	//int32 input_user_index = player_mapping_first_active_input_user();
	//int32 player_index = NONE;
	//
	//if (input_user_index != NONE)
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
	//if (input_state->get_button(_button_left_trigger).down_amount())
	//	debug_game_speed = 1.0f + (-1.0f * input_state->get_button(_button_left_trigger).down_amount());
	//
	//if (input_state->get_button(_button_right_trigger).down_amount())
	//	debug_game_speed = 4.0f * input_state->get_button(_button_right_trigger).down_amount();
	//
	//if (input_state->get_button(_button_y).down_amount())
	//{
	//	input_state->get_button(_button_y).latch();
	//
	//	bool cinematic_render_enable = !g_cinematic_render_enable;
	//	*console_status_render = cinematic_render_enable;
	//	//events_debug_spam_render = cinematic_render_enable;
	//	g_cinematic_render_enable = cinematic_render_enable;
	//}
	//
	//if (input_state->get_button(_button_b).down_frames())
	//{
	//	input_state->get_button(_button_b).latch();
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
	//if (input_state->get_button(_button_dpad_down).down_amount())
	//{
	//	input_state->get_button(_button_dpad_down).latch();
	//	debug_game_speed = 1.0f;
	//	g_cinematic_debugging_enable = true;
	//}
	//
	//if (debug_director_should_camera_mode_change(input_state))
	//{
	//	input_state->get_button(_button_x).latch();
	//	if (director_in_scripted_camera())
	//	{
	//		int32 user_index = player_mapping_first_active_output_user();
	//		director_script_camera(false);
	//		if (output_user_index != NONE)
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

