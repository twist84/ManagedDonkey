#include "camera/director.hpp"

#include "cache/cache_files.hpp"
#include "camera/camera_globals.hpp"
#include "camera/camera_scripting.hpp"
#include "camera/debug_director.hpp"
#include "camera/editor_director.hpp"
#include "camera/game_director.hpp"
#include "camera/observer_director.hpp"
#include "camera/saved_film_director.hpp"
#include "game/game.hpp"
#include "game/players.hpp"
#include "interface/c_controller.hpp"
#include "interface/first_person_weapons.hpp"
#include "interface/interface_constants.hpp"
#include "main/console.hpp"
#include "main/main_game.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "scenario/scenario.hpp"
#include "text/draw_string.hpp"

#include <math.h>

HOOK_DECLARE(0x00591F80, director_render);
HOOK_DECLARE(0x005926C0, director_update);

HOOK_DECLARE_CLASS_MEMBER(0x00592D80, c_director, player_switch_gui_create_or_update_);

bool g_director_use_dt = true;
bool survival_mode_allow_flying_camera = true;

const char* const k_camera_save_filename = "camera";
const char* k_director_mode_names[k_number_of_director_modes]
{
	"game",
	"saved_film",
	"observer",
	"debug",
	"unused",
	"editor"
};

void __thiscall c_director::player_switch_gui_create_or_update_()
{
	c_director::player_switch_gui_create_or_update();
}

s_director_globals* director_globals_get()
{
	return (director_globals && director_globals->directors[0][0]) ? director_globals : NULL;
}

//.text:005914B0 ; c_director::c_director(int32)
//.text:00591540 ; c_director::c_director()
//.text:00591550 ; c_null_camera::c_null_camera()
//.text:005915A0 ; byte(&__cdecl c_static_array<byte[0x160], 4>::operator[]<int32>(int32))[0x160]
//.text:005915C0 ; s_director_info& __cdecl c_static_array<s_director_info, 4>::operator[]<int32>(int32)

e_director_mode __cdecl choose_appropriate_director(int32 user_index)
{
	return INVOKE(0x005916B0, choose_appropriate_director, user_index);
}

int32 __cdecl dead_or_alive_unit_from_user(int32 user_index)
{
	return INVOKE(0x005916F0, dead_or_alive_unit_from_user, user_index);
}

void __cdecl director_dispose()
{
	INVOKE(0x005917B0, director_dispose);
}

void __cdecl director_dispose_from_old_map()
{
	INVOKE(0x005917C0, director_dispose_from_old_map);
}

void __cdecl director_fix_vtables_for_saved_game()
{
	INVOKE(0x005917D0, director_fix_vtables_for_saved_game);
}

//e_director_perspective __cdecl director_game_desired_perspective(int32 unit_index, e_seat_state* seat_state)
e_director_perspective __cdecl director_game_desired_perspective(int32 unit_index, int32* seat_state)
{
	return INVOKE(0x005917E0, director_game_desired_perspective, unit_index, seat_state);
}

void __cdecl director_game_tick()
{
	//INVOKE(0x00591980, director_game_tick);

	scripted_camera_game_tick();
}

c_director* __cdecl director_get(int32 user_index)
{
	//return INVOKE(0x00591990, director_get, user_index);

	if (!director_globals_get())
	{
		return NULL;
	}

	return (c_director*)&director_globals_get()->directors[user_index];
}

bool __cdecl director_get_camera_third_person(int32 user_index)
{
	//return INVOKE(0x005919C0, director_get_camera_third_person, user_index);

	return director_get(user_index)->get_camera()->get_type() == _camera_mode_orbiting;
}

s_cluster_reference __cdecl director_get_deterministic_scripted_camera_cluster_reference()
{
	return INVOKE(0x005919F0, director_get_deterministic_scripted_camera_cluster_reference);
}

real32 __cdecl director_get_fade_timer()
{
	//return INVOKE(0x00591A20, director_get_fade_timer);

	return director_globals->fade_timer;
}

e_director_perspective __cdecl director_get_perspective(int32 user_index)
{
	return INVOKE(0x00591A40, director_get_perspective, user_index);

	c_director* director = director_get(user_index);
	if (!director)
	{
		return _director_perspective_neutral;
	}

	return director->get_perspective();
}

void __cdecl director_get_position(int32 user_index, real_point3d* position)
{
	INVOKE(0x00591A70, director_get_position, user_index, position);

	//c_director* director = director_get(user_index);
	//const s_observer_command* observer_command = director->get_last_observer_command();
	//
	//real32 scale = -observer_command->focus_distance;
	//position->x = observer_command->focus_position.x + (observer_command->forward.i * scale);
	//position->y = observer_command->focus_position.y + (observer_command->forward.j * scale);
	//position->z = observer_command->focus_position.z + (observer_command->forward.k * scale);
}

void __cdecl director_handle_deleted_object(int32 object_index)
{
	INVOKE(0x00591AE0, director_handle_deleted_object, object_index);

	//for (int32 user_index = first_output_user(); user_index != NONE; user_index = next_output_user(user_index))
	//	director_get(user_index)->handle_deleted_object(object_index);
}

void __cdecl director_handle_deleted_player(int32 player_index)
{
	INVOKE(0x00591B30, director_handle_deleted_player, player_index);

	//for (int32 user_index = first_output_user(); user_index != NONE; user_index = next_output_user(user_index))
	//	director_get(user_index)->handle_deleted_player(player_index);
}

bool __cdecl director_in_scripted_camera()
{
	//return INVOKE(0x00591B80, director_in_scripted_camera);

	return director_camera_scripted ? *director_camera_scripted : false;
}

bool __cdecl director_in_unit_perspective(int32 user_index)
{
	return INVOKE(0x00591BA0, director_in_unit_perspective, user_index);
}

bool __cdecl director_inhibited_facing(int32 user_index)
{
	return INVOKE(0x00591BE0, director_inhibited_facing, user_index);

	//c_director* director = director_get(user_index);
	//return director->inhibits_facing();
}

bool __cdecl director_inhibited_input(int32 user_index)
{
	return INVOKE(0x00591C10, director_inhibited_input, user_index);

	//c_director* director = director_get(user_index);
	//return director->inhibits_input();
}

void __cdecl director_initialize()
{
	INVOKE(0x00591C40, director_initialize);

	//director_globals = (s_director_globals*)g_director_globals_allocator.allocate(sizeof(s_director_globals), "director globals");
	//director_camera_scripted = (bool*)g_director_camera_scripted_allocator.allocate(sizeof(bool), "director scripting");
	//*director_camera_scripted = false;
	//scripted_camera_initialize();
}

void __cdecl director_initialize_for_new_map()
{
	INVOKE(0x00591CE0, director_initialize_for_new_map);

	//static bool initialize_from_tags = true;
	//if (initialize_from_tags)
	//{
	//	camera_globals_initialize_from_tags();
	//	initialize_from_tags = false;
	//}
	//
	//*director_camera_scripted = false;
	//for (int32 i = 0; i < k_number_of_users; i++)
	//{
	//	// $TODO set director
	//}
	//director_reset();
}

void __cdecl director_initialize_for_saved_game(int32 flags)
{
	INVOKE(0x00591DA0, director_initialize_for_saved_game, flags);
}

void __cdecl director_notify_map_reset()
{
	INVOKE(0x00591F20, director_notify_map_reset);
}

//.text:00591F30 ; c_game_team director_player_get_team_index(int32)

void __cdecl director_render()
{
	//INVOKE(0x00591F80, director_render);

	// if (game_is_playback())
	// skipping for now since we haven't implemented saved film support *yet*

	if (!player_control_get_machinima_camera_debug())
	{
		return;
	}

	int32 active_output_user = player_mapping_first_active_output_user();
	const s_observer_result* camera = observer_try_and_get_camera(active_output_user);
	if (camera)
	{
		c_rasterizer_draw_string draw_string;
		c_font_cache_mt_safe font_cache;

		real_euler_angles2d facing{};
		euler_angles2d_from_vector3d(&facing, &camera->forward);
		if (facing.yaw < 0.0f)
			facing.yaw += TWO_PI;

		const char* control_mode = "normal";
		if (player_control_get_machinima_camera_use_old_controls())
			control_mode = "pan-cam";

		c_static_string<256> rasterizer_string;
		rasterizer_string.print("%.3f %.3f %.3f / %.2f %.2f / %.3f [%s]",
			camera->position.x,
			camera->position.y,
			camera->position.z,
			RAD * facing.yaw,
			RAD * facing.pitch,
			g_director_camera_speed_scale,
			control_mode
		);

		rectangle2d bounds{};
		interface_get_current_display_settings(NULL, NULL, NULL, &bounds);
		bounds.y0 += 80;

		draw_string.set_font(_large_body_text_font);
		draw_string.set_style(_text_style_plain);
		draw_string.set_justification(_text_justification_left);
		draw_string.set_bounds(&bounds);
		draw_string.set_color(global_real_argb_white);
		draw_string.draw(&font_cache, rasterizer_string.get_string());
	}
}

void __cdecl director_reset()
{
	INVOKE(0x00592210, director_reset);
}

void __cdecl director_reset_user_game_camera(int32 user_index)
{
	INVOKE(0x005922F0, director_reset_user_game_camera, user_index);
}

void __cdecl director_script_camera(bool scripted)
{
	//INVOKE(0x00592320, director_script_camera, scripted);

	if (*director_camera_scripted != scripted)
	{
		*director_camera_scripted = scripted;

		for (int32 user_index = first_output_user(); user_index != NONE; user_index = next_output_user(user_index))
		{
			if (scripted)
			{
				director_set_camera_mode(user_index, _camera_mode_scripted);
			}
			else
			{
				director_set_mode(user_index, choose_appropriate_director(user_index));
			}
		}
	}
}

void __cdecl director_set_camera_mode(int32 user_index, e_camera_mode camera_mode)
{
	//INVOKE(0x005923C0, director_set_camera_mode, user_index, camera_mode);

	director_get(user_index)->set_camera_mode(camera_mode, 0.0f);
}

void __cdecl director_set_camera_third_person(int32 user_index, bool camera_third_person)
{
	INVOKE(0x00592400, director_set_camera_third_person, user_index, camera_third_person);

	//director_set_camera_mode(user_index, camera_third_person ? _camera_mode_following : _camera_mode_first_person);
}

void __cdecl director_set_fade_timer(real32 fade_timer)
{
	INVOKE(0x00592440, director_set_fade_timer, fade_timer);

	//director_globals->fade_timer = fade_timer;
	//director_globals->fade_maximum = fade_timer;
}

void __cdecl director_set_mode(int32 user_index, e_director_mode director_mode)
{
	//INVOKE(0x00592480, director_set_mode, user_index, director_mode);

	switch (director_mode)
	{
	case _director_mode_game:
	{
		((c_game_director*)director_get(user_index))->constructor(user_index);
	}
	break;
	case _director_mode_saved_film:
	{
		((c_saved_film_director*)director_get(user_index))->constructor(user_index);
	}
	break;
	case _director_mode_observer:
	{
		((c_observer_director*)director_get(user_index))->constructor(user_index);
	}
	break;
	case _director_mode_debug:
	{
		((c_debug_director*)director_get(user_index))->constructor(user_index);
	}
	break;
	case _director_mode_editor:
	{
		((c_editor_director*)director_get(user_index))->constructor(user_index);
	}
	break;
	}

	director_get_info(user_index)->mode = director_mode;
}

void __cdecl director_setup_flying_camera_at_scenario_point(int32 user_index, int32 camera_point_index)
{
	INVOKE(0x00592620, director_setup_flying_camera_at_scenario_point, user_index, camera_point_index);
}

void __cdecl director_update(real32 dt)
{
	//INVOKE(0x005926C0, director_update, dt);

	if (!g_director_use_dt)
	{
		dt = 0.016666668f;
	}

	real32 timestep = fminf(0.06666667f, dt);

	//collision_log_begin_period(6);

	director_globals->dtime = timestep;

	real32 fade_timer = director_globals->fade_timer;
	if (fade_timer <= 0.0f)
	{
		if (fade_timer < 0.0f)
			director_globals->fade_timer = fminf(fade_timer + timestep, 0.0f);
	}
	else
	{
		director_globals->fade_timer = fmaxf(0.0f, fade_timer - timestep);
	}

	for (int32 user_index = first_output_user(); user_index != NONE; user_index = next_output_user(user_index))
	{
		if (player_mapping_output_user_is_active(user_index))
		{
			survival_mode_update_flying_camera(user_index);
			control_toggling_of_debug_directors(user_index);
			director_get(user_index)->update(timestep);
		}
	}

	//collision_log_end_period();
}

//.text:00592780 ; void __cdecl c_camera::enable_movement(bool)
//.text:00592790 ; void __cdecl c_camera::enable_orientation(bool)
//.text:005927A0 ; void __cdecl c_camera::enable_roll(bool)

bool c_director::force_set_camera_mode(e_camera_mode camera_mode, real32 interpolation_time)
{
	//return INVOKE_CLASS_MEMBER(0x005927B0, c_director, force_set_camera_mode, camera_mode, interpolation_time);

	return c_director::set_camera_mode_internal(camera_mode, interpolation_time, true);
}

c_camera* c_director::get_camera()
{
	//return DECLFUNC(0x00592830, c_camera*, __thiscall, c_director*)(this);

	if (!m_camera_storage[0])
	{
		return NULL;
	}

	return (c_camera*)&m_camera_storage;
}

const c_camera* c_director::get_camera() const
{
	//return DECLFUNC(0x00592840, c_camera*, __thiscall, const c_director*)(this);

	if (!m_camera_storage[0])
	{
		return NULL;
	}

	return (c_camera*)&m_camera_storage;
}

const s_observer_command* c_director::get_last_observer_command() const
{
	return INVOKE_CLASS_MEMBER(0x00592850, c_director, get_last_observer_command);

	//return &m_last_observer_command;
}

e_director_perspective c_director::get_perspective() const
{
	//return INVOKE_CLASS_MEMBER(0x00592860, c_director, get_perspective);

	if (!game_in_progress())
	{
		return _director_perspective_neutral;
	}

	e_director_perspective perspective = get_camera()->get_perspective();
	if (!perspective)
	{
		perspective = e_director_perspective(m_change_camera_pause > 0.0f);
	}

	if (game_is_ui_shell())
	{
		return _director_perspective_neutral;
	}

	return perspective;
}

//.text:005928B0 ; e_director_perspective __cdecl c_null_camera::get_perspective() const
//.text:005928C0 ; void __cdecl c_director::get_position(real_point3d*)
//.text:00592920 ; 
//.text:00592930 ; 
//.text:00592940 ; e_camera_mode __cdecl c_null_camera::get_type() const
//.text:00592950 ; int32 __cdecl c_director::get_user_index() const

int32 c_director::get_watched_player() const
{
	//return INVOKE_CLASS_MEMBER(0x00592960, c_director, get_watched_player);

	return m_watched_player_index;
}

//.text:00592970 ; void __cdecl c_director::handle_deleted_object(int32)
//.text:00592980 ; void __cdecl c_director::handle_deleted_player(int32)

void __cdecl hs_director_set_camera_mode(int32 user_index, int32 camera_mode)
{
	INVOKE(0x005929B0, hs_director_set_camera_mode, user_index, camera_mode);
}

void __cdecl hs_director_set_camera_target(int32 user_index, int32 object_index)
{
	INVOKE(0x005929F0, hs_director_set_camera_target, user_index, object_index);
}

bool c_director::in_free_camera_mode() const
{
	//return INVOKE_CLASS_MEMBER(0x00592A20, c_director, in_free_camera_mode);

	if (!m_camera_storage[0])
	{
		return false;
	}

	e_camera_mode camera_mode = get_camera()->get_type();
	if (camera_mode == _camera_mode_flying || camera_mode == _camera_mode_scripted)
	{
		return true;
	}

	if (camera_mode != _camera_mode_authored)
	{
		return false;
	}

	return get_camera()->get_target() == NONE;
}

//.text:00592A60 ; bool c_director::inhibits_facing() const
//.text:00592A70 ; bool c_director::inhibits_input() const

int32 c_director::player_get_next_player_with_a_unit(int32 avoid_player_index, int32 starting_player_index, int32 always_player_index, int32 search_direction, bool match_team, bool match_alive_unit, bool allow_wrapping, int32* out_unit_index)
{
	return INVOKE_CLASS_MEMBER(0x00592A80, c_director, player_get_next_player_with_a_unit, avoid_player_index, starting_player_index, always_player_index, search_direction, match_team, match_alive_unit, allow_wrapping, out_unit_index);
}

void __cdecl c_director::player_set_desired_respawn_player(int32 player_index, int32 desired_respawn_player_index)
{
	INVOKE(0x00592C10, c_director::player_set_desired_respawn_player, player_index, desired_respawn_player_index);
}

void c_director::player_switch_gui_button_pressed()
{
	INVOKE_CLASS_MEMBER(0x00592C80, c_director, player_switch_gui_button_pressed);
}

bool __cdecl c_director::player_switch_gui_create(int32 user_index)
{
	return INVOKE(0x00592CC0, c_director::player_switch_gui_create, user_index);
}

class c_observer_camera_list_screen* __cdecl try_and_get_camera_list_screen_for_window(e_window_index window_index)
{
	return INVOKE(0x00ABBAF0, try_and_get_camera_list_screen_for_window, window_index);
}

void c_director::player_switch_gui_create_or_update()
{
	//INVOKE_CLASS_MEMBER(0x00592D80, c_director, player_switch_gui_create_or_update);

	if (game_is_multiplayer() || !game_is_lost())
	{
		e_controller_index controller_index = controller_index_from_user_index(m_user_index);
		if (controller_index != k_no_controller)
		{
			e_window_index game_render_window = controller_get_game_render_window(controller_index);
			if (try_and_get_camera_list_screen_for_window(game_render_window))
			{
				if (m_player_switch_gui_activated)
				{
					c_director::player_switch_gui_update();
				}
				else
				{
					m_player_switch_gui_activated = c_director::player_switch_gui_set_visible(m_user_index);
				}
			}
			else
			{
				c_director::player_switch_gui_create(m_user_index);
				m_player_switch_gui_activated = false;
			}
		}
	}
	else
	{
		c_director::player_switch_gui_destroy();
	}
}

bool c_director::player_switch_gui_destroy()
{
	return INVOKE_CLASS_MEMBER(0x00592D90, c_director, player_switch_gui_destroy);
}

bool __cdecl c_director::player_switch_gui_set_visible(int32 user_index)
{
	return INVOKE(0x00592DD0, c_director::player_switch_gui_set_visible, user_index);
}

void c_director::player_switch_gui_update()
{
	INVOKE_CLASS_MEMBER(0x00592E30, c_director, player_switch_gui_update);
}

bool c_director::player_switch_update()
{
	return INVOKE_CLASS_MEMBER(0x00592F90, c_director, player_switch_update);
}

//.text:00593150 ; 
//.text:00593190 ; 
//.text:005931D0 ; 

bool c_director::set_camera_mode(e_camera_mode camera_mode, real32 transition_time)
{
	//return INVOKE_CLASS_MEMBER(0x005931F0, c_director, set_camera_mode, camera_mode, transition_time);

	return c_director::set_camera_mode_internal(camera_mode, transition_time, false);
}

bool c_director::set_camera_mode_internal(e_camera_mode camera_mode, real32 transition_time, bool force_update)
{
	//return INVOKE_CLASS_MEMBER(0x00593210, c_director, set_camera_mode_internal, camera_mode, transition_time, force_update);

	if (!can_use_camera_mode(camera_mode))
	{
		return false;
	}

	e_camera_mode current_camera_mode = get_camera()->get_type();

	bool result = camera_mode != current_camera_mode;
	if (result || force_update)
	{
		switch (camera_mode)
		{
		case _camera_mode_following:
		{
			((c_following_camera*)get_camera())->constructor(dead_or_alive_unit_from_user(m_user_index));
		}
		break;
		case _camera_mode_orbiting:
		{
			((c_orbiting_camera*)get_camera())->constructor(dead_or_alive_unit_from_user(m_user_index));
		}
		break;
		case _camera_mode_flying:
		{
			((c_flying_camera*)get_camera())->constructor(m_user_index);
		}
		break;
		case _camera_mode_first_person:
		{
			((c_first_person_camera*)get_camera())->constructor(dead_or_alive_unit_from_user(m_user_index));
		}
		break;
		case _camera_mode_dead:
		{
			((c_dead_camera*)get_camera())->constructor(m_user_index);
		}
		break;
		case _camera_mode_static:
		{
			((c_static_camera*)get_camera())->constructor(m_user_index);
		}
		break;
		case _camera_mode_scripted:
		{
			((c_scripted_camera*)get_camera())->constructor();
		}
		break;
		case _camera_mode_authored:
		{
			((c_authored_camera*)get_camera())->constructor(m_user_index);
		}
		break;
		}
		m_change_camera_pause = transition_time;
	}

	e_director_perspective perspective = get_perspective();
	s_director_globals* director_globals = director_globals_get();
	if (director_globals->director_info[m_user_index].perspective != perspective ||
		director_globals->director_info[m_user_index].camera_mode != camera_mode)
	{
		director_globals->director_info[m_user_index].perspective = perspective;
		director_globals->director_info[m_user_index].camera_mode = camera_mode;
		first_person_weapon_perspective_changed(m_user_index);
	}

	return result || force_update;
}

//.text:005934A0 ; void c_camera::set_forward(const real_vector3d*)
//.text:005934B0 ; void c_camera::set_position(const real_point3d*)
//.text:005934C0 ; void c_camera::set_roll(real32)

void c_director::set_watched_player(int32 player_index)
{
	INVOKE_CLASS_MEMBER(0x005934D0, c_director, set_watched_player, player_index);
}

//.text:00593520 ; bool c_director::should_draw_hud() const
//.text:00593530 ; bool c_director::should_draw_hud_saved_film() const
//.text:00593540 ; void c_director::update(real32)
//.text:00593770 ; void c_null_camera::update(int32, real32, s_observer_command*)

void c_director::update_perspective()
{
	INVOKE_CLASS_MEMBER(0x00593780, c_director, update_perspective);

	//director_globals->director_info[m_user_index].perspective = c_director::get_perspective();
}

//.text:005937F0 ; void update_vtables()

//.text:007215C0 ; c_game_director::c_game_director(int32)
//.text:007215F0 ; c_game_director::c_game_director()
//.text:00721610 ; e_director_perspective director_game_camera_deterministic(int32, real_point3d*, real_vector3d*)
//.text:00721660 ; bool c_game_director::dead_camera_should_switch_to_orbiting(int32, int32)
//.text:00721910 ; e_director_mode c_game_director::get_type() const
//.text:00721920 ; bool c_game_director::inhibits_facing() const
//.text:00721970 ; bool c_game_director::inhibits_input() const
//.text:00721980 ; bool c_game_director::should_draw_hud() const
//.text:00721990 ; bool c_game_director::should_draw_hud_saved_film() const
//.text:007219A0 ; void c_game_director::update(real32)

//.text:00725A80 ; c_camera::c_camera()

bool __cdecl camera_input_inhibited(e_controller_index controller_index)
{
	return INVOKE(0x00725AA0, camera_input_inhibited, controller_index);
}

//.text:00725B90 ; c_camera::vftable13
//.text:00725BA0 ; int32 __cdecl c_camera::get_target() const
//.text:00725BB0 ; void __cdecl c_camera::handle_deleted_object(int32)
//.text:00725BD0 ; void __cdecl c_camera::handle_deleted_player(int32)
//.text:00725BE0 ; void __cdecl c_camera::post_update(s_observer_command*)
//.text:00725C10 ; void __cdecl c_camera::pre_update()
//.text:00725C20 ; void __cdecl c_flags_no_init<c_camera::e_base_camera_flags, int32, 1>::set(c_camera::e_base_camera_flags, bool)
//.text:00725C50 ; void __cdecl c_camera::set_next_move_instantly()
//.text:00725C60 ; void __cdecl c_camera::set_target(int32)
//.text:00725C70 ; bool __cdecl c_flags_no_init<c_camera::e_base_camera_flags, int32, 1>::test(c_camera::e_base_camera_flags) const
//.text:00725C90 ; void camera_globals_initialize_from_tags()
//.text:007260D0 ; c_debug_director::c_debug_director(int32)

const char* director_mode_get_name(e_director_mode director_mode)
{
	if (director_mode < _director_mode_game || director_mode >= k_number_of_director_modes)
	{
		return "<invalid 'director_mode'>";
	}

	return k_director_mode_names[director_mode];
}

e_director_mode director_mode_from_string(const char* str)
{
	e_director_mode director_mode = e_director_mode(-1);
	for (int32 i = _camera_mode_following; i < k_number_of_director_modes; i++)
	{
		if (csstricmp(str, k_director_mode_names[i]) != 0)
		{
			continue;
		}

		director_mode = e_director_mode(i);
	}

	return director_mode;
}

s_director_info* director_get_info(int32 user_index)
{
	if (!director_globals_get())
	{
		return NULL;
	}

	return &director_globals_get()->director_info[user_index];
}

void director_set_perspective(int32 user_index, e_director_perspective perspective)
{
	s_director_globals* director_globals = director_globals_get();
	if (director_globals->director_info[user_index].perspective != perspective)
	{
		director_globals->director_info[user_index].perspective = perspective;
		first_person_weapon_perspective_changed(user_index);
	}
}

void director_toggle(int32 user_index, e_director_mode director_mode)
{
	static e_director_mode previous_mode = {};

	if (director_get_info(user_index)->mode == director_mode)
	{
		director_mode = previous_mode;
	}

	director_set_mode(user_index, director_mode);
}

void director_toggle_perspective(int32 user_index, e_director_perspective perspective)
{
	static e_director_perspective previous_mode = {};

	if (director_get_info(user_index)->perspective == perspective)
	{
		perspective = previous_mode;
	}

	director_set_perspective(user_index, perspective);
}

void director_toggle_camera(int32 user_index, e_camera_mode camera_mode)
{
	static e_camera_mode previous_mode = {};

	if (director_get_info(user_index)->camera_mode == camera_mode)
	{
		camera_mode = previous_mode;
	}

	director_get(user_index)->set_camera_mode(camera_mode, 0.0f);
}

void __cdecl director_set_flying_camera_direct(int32 user_index, const real_point3d* position, const real_vector3d* forward, const real_vector3d* up)
{
	if (user_index == NONE)
	{
		return;
	}

	ASSERT(VALID_INDEX(user_index, k_number_of_users));

	c_director* director = director_get(user_index);
	director->set_camera_mode_internal(_camera_mode_flying, 0.0f, false);

	c_flying_camera* flying_camera = static_cast<c_flying_camera*>(director->get_camera());
	flying_camera->set_position(position);
	flying_camera->set_forward(forward);

	real_vector3d up_from_forward{};
	generate_up_vector3d(forward, &up_from_forward);
	real32 roll = angle_between_vectors3d(up, &up_from_forward);
	real_vector3d product3d;
	cross_product3d(up, &up_from_forward, &product3d);
	if (dot_product3d(&product3d, forward) > 0.0f)
	{
		roll = -roll;
	}

	flying_camera->set_roll(roll);
}

const char* scenario_get_name()
{
	if (global_scenario_index == NONE)
	{
		return tag_name_strip_path(main_game_globals.game_loaded_scenario_path);
	}

	if (const char* name = tag_get_name(global_scenario_index))
	{
		return tag_name_strip_path(name);
	}

	return tag_name_strip_path(main_game_globals.game_loaded_scenario_path);
}

void director_save_camera_named(const char* name)
{
	if (!global_scenario_try_and_get())
	{
		return;
	}

	if (!players_globals)
	{
		return;
	}

	c_static_string<256> filename;
	const char* root = "";
	const char* scenario_name = scenario_get_name();
	filename.print("%s%s_%s.txt", root, name, scenario_name);

	FILE* file;
	fopen_s(&file, filename.get_string(), "w");
	if (file)
	{
		const s_observer_result* camera = NULL;
		for (int32 user_index = first_output_user(); user_index != NONE; user_index = next_output_user(user_index))
		{
			camera = observer_try_and_get_camera(user_index);
			if (camera)
			{
				break;
			}
		}

		if (camera)
		{
			fprintf(file, "%f %f %f\n", camera->position.x, camera->position.y, camera->position.z);
			fprintf(file, "%f %f %f\n", camera->forward.i, camera->forward.j, camera->forward.k);
			fprintf(file, "%f %f %f\n", camera->up.i, camera->up.j, camera->up.k);
			fprintf(file, "%f\n", camera->horizontal_field_of_view);
			console_printf("%s written successfully", filename.get_string());
		}

		fclose(file);
	}
}

void director_load_camera_named(const char* name)
{
	if (!global_scenario_try_and_get())
	{
		console_printf("no scenario loaded");
		return;
	}

	if (!players_globals)
	{
		return;
	}

	c_static_string<256> filename;
	const char* root = "";
	const char* scenario_name = scenario_get_name();
	filename.print("%s%s_%s.txt", root, name, scenario_name);

	s_file_reference info;
	file_reference_create_from_path(&info, filename.get_string(), false);
	if (!file_exists(&info))
	{
		console_printf("'%s' doesn't exist", filename.get_string());
		return;
	}

	FILE* file;
	fopen_s(&file, filename.get_string(), "r");
	if (!file)
	{
		c_console::write_line("saved camera '%s' couldn't be opened", filename.get_string());
		return;
	}

	real_point3d position{};
	real_vector3d forward{};
	real_vector3d up{};
	real32 field_of_view;

	fscanf_s(file, "%f %f %f\n", &position.x, &position.y, &position.z);
	fscanf_s(file, "%f %f %f\n", &forward.i, &forward.j, &forward.k);
	fscanf_s(file, "%f %f %f\n", &up.i, &up.j, &up.k);
	fscanf_s(file, "%f\n", &field_of_view);

	fclose(file);

	// $TODO actually validate `position`, `forward`, `up` and `field_of_view`
	if (false)
	{
		c_console::write_line("saved camera '%s' isn't valid", filename.get_string());
		return;
	}

	int32 user_index = player_mapping_first_active_output_user();
	if (user_index != k_number_of_users)
	{
		director_set_flying_camera_direct(user_index, &position, &forward, &up);
	}
	else
	{
		c_console::write_line("no active user to set saved camera '%s'", filename.get_string());
	}
}

void director_save_camera()
{
	director_save_camera_named(k_camera_save_filename);
}

void director_load_camera()
{
	director_load_camera_named(k_camera_save_filename);
}

void director_debug_camera(bool render)
{
	int32 user_index = player_mapping_first_active_output_user();
	if (VALID_INDEX(user_index, k_number_of_users))
	{
		if (render)
		{
			director_set_mode(user_index, _director_mode_debug);
		}
		else
		{
			director_set_mode(user_index, choose_appropriate_director(user_index));
		}
	}

	g_debug_observer_render = render;
}

void survival_mode_update_flying_camera(int32 user_index)
{
	if (!survival_mode_allow_flying_camera)
	{
		return;
	}

	int32 player_index = player_mapping_get_player_by_output_user(user_index);
	if (player_index == NONE || !game_is_survival())
	{
		return;
	}

	player_datum* player = DATUM_GET(player_data, player_datum, player_index);

	c_director* director = director_get(user_index);
	e_director_mode director_mode = director->get_type();

	if (player->dead_timer <= 0)
	{
		if (director_mode == _director_mode_observer)
		{
			director_set_mode(user_index, _director_mode_game);

			if (player_mapping_get_input_controller(player_index) != NONE)
			{
				e_controller_index controller_index = player_mapping_get_input_controller(player_index);
				input_abstraction_latch_all_buttons(controller_index);
			}
		}
	}
	else if (director_mode == _director_mode_game)
	{
		if (player->dead_timer > game_seconds_to_ticks_real(g_camera_globals.survival_switch_time))
		{
			director_set_mode(user_index, _director_mode_observer);
		}
	}
}

void control_toggling_of_debug_directors(int32 user_index)
{
	c_director* director = director_get(user_index);
	e_director_mode director_mode = director->get_type();

	if (director_mode == _director_mode_editor || input_key_frames_down(_key_backspace, _input_type_game) != 1)
	{
		return;
	}

	if (director_mode == _director_mode_debug)
	{
		if (static_cast<c_debug_director*>(director)->finished_cycle())
		{
			console_printf("exiting debug director");
			director_set_mode(user_index, choose_appropriate_director(user_index));
		}

	}
	else
	{
		input_suppress();
		console_printf("entering debug director");
		director_set_mode(user_index, _director_mode_debug);
	}
}

