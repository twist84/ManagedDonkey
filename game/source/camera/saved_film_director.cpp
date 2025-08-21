#include "camera/saved_film_director.hpp"

#include "game/game.hpp"
#include "input/input_abstraction.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_utilities.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "memory/module.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "saved_games/saved_film_snippet.hpp"

#include <math.h>

HOOK_DECLARE_CLASS_MEMBER(0x00727B70, c_saved_film_director, update_);

bool __cdecl player_is_film_recorder_for_unit_is_film_recorder(int32 player_index)
{
	static bool x_first_person_film_playback_enabled = true;
	if (x_first_person_film_playback_enabled)
	{
		return x_first_person_film_playback_enabled;
	}

	return c_saved_film_director::player_is_film_recorder(player_index);
}
HOOK_DECLARE_CALL(0x00727B51, player_is_film_recorder_for_unit_is_film_recorder);

void __thiscall c_saved_film_director::update_(real32 dt)
{
	e_controller_index controller_index = controller_index_from_user_index(m_user_index);
	if (controller_index != k_no_controller && !camera_input_inhibited(controller_index))
	{
		s_game_input_state* input_state = NULL;
		input_abstraction_get_input_state(controller_index, &input_state);

		bool screen_visible = true;
		if (!window_manager_get()->get_screen_by_name(k_number_of_player_windows, STRING_ID(gui, saved_film_control_pad)))
		{
			if (!window_manager_get()->get_screen_by_name(k_number_of_player_windows, STRING_ID(gui, saved_film_snippet)))
			{
				screen_visible = false;
			}
		}

		if (game_is_authoritative_playback())
		{
			real32 down_amount = input_state->get_button(_button_action_right_trigger).down_amount();
			if (!player_control_get_machinima_camera_use_old_controls() && down_amount > 0.0f)
			{
				m_speed_controlled_by_trigger = true;
				real32 max_speed = g_camera_globals.saved_film_director_max_playback_speed;
				real32 speed = powf(max_speed + 1.0f, down_amount) - 1.0f;
				saved_film_manager_set_playback_game_speed(speed);
			}
			else
			{
				if (m_speed_controlled_by_trigger)
				{
					m_speed_controlled_by_trigger = false;
					saved_film_manager_set_playback_game_speed(0.0f);
				}
			}
		}

		if (!screen_visible)
		{
			bool play_or_eject = false;

			bool x_is_down = input_state->get_button(_button_action_x).is_down();
			x_is_down |= input_key_frames_down(_key_x, _input_type_game) != 0;
			if (!x_is_down && m_show_control_pad_button_pressed)
			{
				if (game_is_authoritative_playback())
				{
					screen_visible = true;
				}
				else
				{
					user_interface_play_sound(_ui_global_sound_effect_failure, NONE);
				}
			}

			m_show_control_pad_button_pressed = x_is_down;

			if (game_is_authoritative_playback()
				&& !saved_film_manager_get_ticks_remaining()
				&& saved_film_manager_get_snippet_state() == _saved_film_snippet_state_none)
			{
				play_or_eject = true;
			}

			if (!saved_film_manager_film_is_ended(NULL) && screen_visible)
			{
				c_load_screen_message* screen_message = UI_MALLOC(c_load_screen_message,
					STRING_ID(gui, saved_film_control_pad),
					k_any_controller,
					k_number_of_player_windows,
					STRING_ID(gui, top_most));
				if (screen_message)
				{
					int32 value = STRING_ID(gui, play_film);
					if (play_or_eject)
					{
						value = STRING_ID(gui, eject);
					}
					else if (saved_film_manager_get_playback_game_speed() > 0.0f)
					{
						value = STRING_ID(gui, play_film) + 1;
					}
					else
					{
						value = STRING_ID(gui, play_film);
					}

					switch (saved_film_manager_get_snippet_state())
					{
					case _saved_film_snippet_state_previewing_waiting_for_seek:
					case _saved_film_snippet_state_previewing:
					{
						value = STRING_ID(gui, end_preview);
					}
					break;
					case _saved_film_snippet_state_recording_waiting_for_seek:
					case _saved_film_snippet_state_recording_waiting_for_start:
					case _saved_film_snippet_state_recording:
					case _saved_film_snippet_state_recorded_and_ready:
					{
						value = STRING_ID(gui, stop_recording);
					}
					break;
					}

					user_interface_play_sound(_ui_global_sound_effect_button_x, NONE);
					screen_message->set_focus_on_load_by_name(STRING_ID(gui, button_list), STRING_ID(gui, gui_item), value);
					user_interface_messaging_post(screen_message);
					screen_visible = true;
				}
			}
		}

		real32 pending_speed = saved_film_manager_get_pending_playback_game_speed();
		if (pending_speed > 0.0f)
		{
			if (saved_film_manager_set_playback_game_speed(pending_speed))
			{
				saved_film_manager_set_pending_playback_game_speed(-1.0f);
			}
		}

		if (!screen_visible)
		{
			if (input_state->get_button(_button_action_a).down_frames() == 1)
			{
				if (game_is_authoritative_playback())
				{
					if (saved_film_manager_get_playback_game_speed() == 0.0f)
					{
						saved_film_manager_set_playback_game_speed(1.0f);
						m_speed_controlled_by_trigger = false;
					}
					else
					{
						saved_film_manager_set_playback_game_speed(0.0f);
					}
				}
				else
				{
					user_interface_play_sound(_ui_global_sound_effect_failure, NONE);
				}
			}

			if (!player_control_get_machinima_camera_enabled())
			{
				if (input_state->get_button(_button_action_dpad_left).down_frames() == 1)
				{
					if (game_is_authoritative_playback() && saved_film_manager_can_revert(_saved_film_revert_backwards))
					{
						saved_film_manager_request_revert(_saved_film_revert_backwards);
					}
					else
					{
						user_interface_play_sound(_ui_global_sound_effect_failure, NONE);
					}
				}

				if (input_state->get_button(_button_action_dpad_right).down_frames() == 1)
				{
					if (game_is_authoritative_playback() && saved_film_manager_can_revert(_saved_film_revert_forwards))
					{
						saved_film_manager_request_revert(_saved_film_revert_forwards);
					}
					else
					{
						user_interface_play_sound(_ui_global_sound_effect_failure, NONE);
					}
				}
			}
		}
	}

	int32 snippet_tick = saved_film_manager_get_snippet_start_tick();
	if (snippet_tick != NONE && saved_film_manager_get_current_tick_estimate() < snippet_tick)
	{
		director_set_fade_timer(-1.0f);
	}

	//c_observer_director::update(dt);
	INVOKE_CLASS_MEMBER(0x00726C80, c_observer_director, update, dt);

	c_camera* camera = c_director::get_camera();
	if (camera->get_type() == _camera_mode_authored)
	{
		c_director::update_perspective();
	}

	if (director_in_scripted_camera())
	{
		c_director::set_camera_mode(_camera_mode_scripted, 0.0f);
	}
	else if (camera->get_type() == _camera_mode_scripted)
	{
		c_director::set_camera_mode(_camera_mode_first_person, 0.0f);
	}
}

//.text:007276C0 ; public: c_saved_film_director::c_saved_film_director(int32)
//.text:00727700 ; public: c_saved_film_director::c_saved_film_director()
//.text:00727720 ; public: static bool __cdecl c_saved_film_director::get_saved_film_recorder_player_index(int32, int32*)
//.text:00727940 ; public: virtual e_director_mode c_saved_film_director::get_type() const
//.text:00727950 ; 

void c_saved_film_director::notify_revert()
{
	INVOKE_CLASS_MEMBER(0x00727960, c_saved_film_director, notify_revert);
}

bool __cdecl c_saved_film_director::player_is_film_recorder(int32 player_index)
{
	return INVOKE(0x00727990, c_saved_film_director::player_is_film_recorder, player_index);
}

//.text:00727A20 ; 
//.text:00727A30 ; 
//.text:00727A40 ; 
//.text:00727A50 ; 
//.text:00727A60 ; 
//.text:00727A70 ; 
//.text:00727A80 ; public: virtual void c_saved_film_director::select_fallback_target()
//.text:00727B00 ; 
//.text:00727B20 ; public: static bool __cdecl c_saved_film_director::unit_is_film_recorder(int32)
//.text:00727B70 ; public: virtual void c_saved_film_director::update(real32)

