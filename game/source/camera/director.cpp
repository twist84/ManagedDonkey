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

bool g_director_use_dt = true;
bool survival_mode_allow_flying_camera = true;

char const* const k_camera_save_filename = "camera";
char const* k_director_mode_names[k_number_of_director_modes]
{
	"game",
	"saved_film",
	"observer",
	"debug",
	"unused",
	"editor"
};

s_director_globals* director_globals_get()
{
	if (!get_tls())
		return nullptr;

	TLS_DATA_GET_VALUE_REFERENCE(director_globals);
	return (director_globals && director_globals->directors[0][0]) ? director_globals : nullptr;
}

//.text:005914B0 ; c_director::c_director(e_output_user_index)
//.text:00591540 ; c_director::c_director()
//.text:00591550 ; c_null_camera::c_null_camera()
//.text:005915A0 ; byte(&__cdecl c_static_array<byte[0x160], 4>::operator[]<e_output_user_index>(e_output_user_index))[0x160]
//.text:005915C0 ; s_director_info& __cdecl c_static_array<s_director_info, 4>::operator[]<e_output_user_index>(e_output_user_index)

e_director_mode __cdecl choose_appropriate_director(e_output_user_index output_user_index)
{
	return INVOKE(0x005916B0, choose_appropriate_director, output_user_index);
}

long __cdecl dead_or_alive_unit_from_output_user(e_output_user_index output_user_index)
{
	return INVOKE(0x005916F0, dead_or_alive_unit_from_output_user, output_user_index);
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

//e_director_perspective __cdecl director_game_desired_perspective(long unit_index, e_seat_state* seat_state)
e_director_perspective __cdecl director_game_desired_perspective(long unit_index, long* seat_state)
{
	return INVOKE(0x005917E0, director_game_desired_perspective, unit_index, seat_state);
}

void __cdecl director_game_tick()
{
	//INVOKE(0x00591980, director_game_tick);

	scripted_camera_game_tick();
}

c_director* __cdecl director_get(e_output_user_index output_user_index)
{
	//return INVOKE(0x00591990, director_get, output_user_index);

	if (!director_globals_get())
		return nullptr;

	return (c_director*)&director_globals_get()->directors[output_user_index];
}

bool __cdecl director_get_camera_third_person(e_output_user_index output_user_index)
{
	//return INVOKE(0x005919C0, director_get_camera_third_person, output_user_index);

	return director_get(output_user_index)->get_camera()->get_type() == _camera_mode_orbiting;
}

s_cluster_reference __cdecl director_get_deterministic_scripted_camera_cluster_reference()
{
	return INVOKE(0x005919F0, director_get_deterministic_scripted_camera_cluster_reference);
}

real __cdecl director_get_fade_timer()
{
	//return INVOKE(0x00591A20, director_get_fade_timer);

	TLS_DATA_GET_VALUE_REFERENCE(director_globals);
	return director_globals->fade_timer5B4;
}

e_director_perspective __cdecl director_get_perspective(e_output_user_index output_user_index)
{
	return INVOKE(0x00591A40, director_get_perspective, output_user_index);

	c_director* director = director_get(output_user_index);
	if (!director)
		return _director_perspective_3;

	return director->get_perspective();
}

void __cdecl director_get_position(e_output_user_index output_user_index, real_point3d* position)
{
	INVOKE(0x00591A70, director_get_position, output_user_index, position);
}

void __cdecl director_handle_deleted_object(long object_index)
{
	INVOKE(0x00591AE0, director_handle_deleted_object, object_index);

	//for (e_output_user_index user_index = first_output_user(); user_index != k_output_user_none; user_index = next_output_user(user_index))
	//	director_get(user_index)->handle_deleted_object(object_index);
}

void __cdecl director_handle_deleted_player(long player_index)
{
	INVOKE(0x00591B30, director_handle_deleted_player, player_index);

	//for (e_output_user_index user_index = first_output_user(); user_index != k_output_user_none; user_index = next_output_user(user_index))
	//	director_get(user_index)->handle_deleted_player(player_index);
}

bool __cdecl director_in_scripted_camera()
{
	INVOKE(0x00591B80, director_in_scripted_camera);

	if (!get_tls())
		return false;

	TLS_DATA_GET_VALUE_REFERENCE(director_camera_scripted);
	return director_camera_scripted ? *director_camera_scripted : false;
}

bool __cdecl director_in_unit_perspective(e_output_user_index output_user_index)
{
	return INVOKE(0x00591BA0, director_in_unit_perspective, output_user_index);
}

bool __cdecl director_inhibited_facing(e_output_user_index output_user_index)
{
	return INVOKE(0x00591BE0, director_inhibited_facing, output_user_index);
}

bool __cdecl director_inhibited_input(e_output_user_index output_user_index)
{
	return INVOKE(0x00591C10, director_inhibited_input, output_user_index);
}

void __cdecl director_initialize()
{
	INVOKE(0x00591C40, director_initialize);
}

void __cdecl director_initialize_for_new_map()
{
	INVOKE(0x00591CE0, director_initialize_for_new_map);
}

void __cdecl director_initialize_for_saved_game(long flags)
{
	INVOKE(0x00591DA0, director_initialize_for_saved_game, flags);
}

void __cdecl director_notify_map_reset()
{
	INVOKE(0x00591F20, director_notify_map_reset);
}

//.text:00591F30 ; c_game_team director_player_get_team_index(long)

void __cdecl director_render()
{
	//INVOKE(0x00591F80, director_render);

	// if (game_is_playback())
	// skipping for now since we haven't implemented saved film support *yet*

	if (player_control_get_machinima_camera_debug())
	{
		e_output_user_index active_output_user = player_mapping_first_active_output_user();
		s_observer_result const* camera = observer_try_and_get_camera(active_output_user);
		if (camera)
		{
			c_rasterizer_draw_string rasterizer_draw_string;
			c_font_cache_mt_safe font_cache;

			euler_angles2d facing{};
			euler_angles2d_from_vector3d(&facing, &camera->forward);
			if (facing.yaw < 0.0f)
				facing.yaw += TWO_PI;

			char const* control_mode = "normal";
			if (player_control_get_machinima_camera_use_old_controls())
				control_mode = "pan-cam";

			c_static_string<256> rasterizer_string;
			rasterizer_string.print("%.3f %.3f %.3f / %.2f %.2f / %.3f [%s]",
				camera->focus_point.x,
				camera->focus_point.y,
				camera->focus_point.z,
				RAD * facing.yaw,
				RAD * facing.pitch,
				g_director_camera_speed_scale,
				control_mode
			);

			short_rectangle2d bounds{};
			interface_get_current_display_settings(nullptr, nullptr, nullptr, &bounds);
			bounds.y0 += 80;

			rasterizer_draw_string.set_font(4);
			rasterizer_draw_string.set_style(-1);
			rasterizer_draw_string.set_justification(0);
			rasterizer_draw_string.set_bounds(&bounds);
			rasterizer_draw_string.set_color(global_real_argb_white);
			rasterizer_draw_string.draw(&font_cache, rasterizer_string.get_string());
		}
	}
}

void __cdecl director_reset()
{
	INVOKE(0x00592210, director_reset);
}

void __cdecl director_reset_user_game_camera(e_output_user_index output_user_index)
{
	INVOKE(0x005922F0, director_reset_user_game_camera, output_user_index);
}

void __cdecl director_script_camera(bool scripted)
{
	//INVOKE(0x00592320, director_script_camera, scripted);

	TLS_DATA_GET_VALUE_REFERENCE(director_camera_scripted);

	if (*director_camera_scripted != scripted)
	{
		*director_camera_scripted = scripted;

		for (e_output_user_index user_index = first_output_user(); user_index != k_output_user_none; user_index = next_output_user(user_index))
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

void __cdecl director_set_camera_mode(e_output_user_index output_user_index, e_camera_mode camera_mode)
{
	//INVOKE(0x005923C0, director_set_camera_mode, output_user_index, camera_mode);

	director_get(output_user_index)->set_camera_mode(camera_mode, 0.0f);
}

void __cdecl director_set_camera_third_person(e_output_user_index output_user_index, bool camera_third_person)
{
	INVOKE(0x00592400, director_set_camera_third_person, output_user_index, camera_third_person);

	//director_set_camera_mode(output_user_index, camera_third_person ? _camera_mode_following : _camera_mode_first_person);
}

void __cdecl director_set_fade_timer(real fade_timer)
{
	INVOKE(0x00592440, director_set_fade_timer, fade_timer);

	//TLS_DATA_GET_VALUE_REFERENCE(director_globals);
	//director_globals->fade_timer5B4 = fade_timer;
	//director_globals->fade_timer5B8 = fade_timer;
}

void __cdecl director_set_mode(e_output_user_index output_user_index, e_director_mode director_mode)
{
	//INVOKE(0x00592480, director_set_mode, output_user_index, director_mode);

	switch (director_mode)
	{
	case _director_mode_game:
		static_cast<c_game_director*>(director_get(output_user_index))->constructor(output_user_index);
		break;
	case _director_mode_saved_film:
		static_cast<c_saved_film_director*>(director_get(output_user_index))->constructor(output_user_index);
		break;
	case _director_mode_observer:
		static_cast<c_observer_director*>(director_get(output_user_index))->constructor(output_user_index);
		break;
	case _director_mode_debug:
		static_cast<c_debug_director*>(director_get(output_user_index))->constructor(output_user_index);
		break;
	case _director_mode_editor:
		static_cast<c_editor_director*>(director_get(output_user_index))->constructor(output_user_index);
		break;
	}

	director_get_info(output_user_index)->director_mode = director_mode;
}

void __cdecl director_setup_flying_camera_at_scenario_point(e_output_user_index output_user_index, long cutscene_camera_point)
{
	INVOKE(0x00592620, director_setup_flying_camera_at_scenario_point, output_user_index, cutscene_camera_point);
}

void __cdecl director_update(real world_seconds_elapsed)
{
	//INVOKE(0x005926C0, director_update, world_seconds_elapsed);

	TLS_DATA_GET_VALUE_REFERENCE(director_globals);

	if (!g_director_use_dt)
		world_seconds_elapsed = 0.016666668f;

	real timestep = fminf(0.06666667f, world_seconds_elapsed);

	//collision_log_begin_period(6);

	director_globals->timestep = timestep;

	real fade_timer5B4 = director_globals->fade_timer5B4;
	if (fade_timer5B4 <= 0.0f)
	{
		if (fade_timer5B4 < 0.0f)
			director_globals->fade_timer5B4 = fminf(fade_timer5B4 + timestep, 0.0f);
	}
	else
	{
		director_globals->fade_timer5B4 = fmaxf(0.0f, fade_timer5B4 - timestep);
	}

	for (e_output_user_index user_index = first_output_user(); user_index != k_output_user_none; user_index = next_output_user(user_index))
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
//.text:005927B0 ; bool __cdecl c_director::force_set_camera_mode(e_camera_mode, real)

c_camera* c_director::get_camera()
{
	//return DECLFUNC(0x00592830, c_camera*, __thiscall, c_director*)(this);

	if (!m_camera[0])
		return nullptr;

	return (c_camera*)&m_camera;
}

c_camera const* c_director::get_camera() const
{
	//return DECLFUNC(0x00592840, c_camera*, __thiscall, c_director const*)(this);

	if (!m_camera[0])
		return nullptr;

	return (c_camera*)&m_camera;
}

s_observer_command const* c_director::get_last_observer_command() const
{
	return DECLFUNC(0x00592850, s_observer_command const*, __thiscall, c_director const*)(this);

	//return &m_observer_command;
}

e_director_perspective c_director::get_perspective() const
{
	//return DECLFUNC(0x00592860, e_director_perspective, __thiscall, c_director const*)(this);

	if (!game_in_progress())
		return _director_perspective_3;
	e_director_perspective director_perspective = get_camera()->get_perspective();
	if (!director_perspective)
		director_perspective = e_director_perspective(m_transition_time > 0.0f);
	if (game_is_ui_shell())
		return _director_perspective_3;

	return director_perspective;
}

//.text:005928B0 ; e_director_perspective __cdecl c_null_camera::get_perspective() const
//.text:005928C0 ; void __cdecl c_director::get_position(real_point3d*)
//.text:00592920 ; 
//.text:00592930 ; 
//.text:00592940 ; e_camera_mode __cdecl c_null_camera::get_type() const
//.text:00592950 ; e_output_user_index __cdecl c_director::get_output_user_index() const
//.text:00592960 ; long __cdecl c_director::get_watched_player() const
//.text:00592970 ; void __cdecl c_director::handle_deleted_object(long)
//.text:00592980 ; void __cdecl c_director::handle_deleted_player(long)
//.text:005929B0 ; void __cdecl hs_director_set_camera_mode(long, long)
//.text:005929F0 ; void __cdecl hs_director_set_camera_target(long, long)

bool c_director::in_free_camera_mode() const
{
	//return DECLFUNC(0x00592A20, bool, __thiscall, c_director const*)(this);

	if (!m_camera[0])
		return false;

	e_camera_mode camera_mode = get_camera()->get_type();
	if (camera_mode == _camera_mode_flying || camera_mode == _camera_mode_scripted)
		return true;

	if (camera_mode == _camera_mode_authored)
		return get_camera()->get_target() == NONE;

	return false;
}

//.text:00592A60 ; bool __cdecl c_director::inhibits_facing() const
//.text:00592A70 ; bool __cdecl c_director::inhibits_input() const
//.text:00592A80 ; long __cdecl c_director::player_get_next_player_with_a_unit(long, long, long, long, bool, bool, bool, long*)
//.text:00592C10 ; 
//.text:00592C80 ; void __cdecl c_director::player_switch_gui_button_pressed()
//.text:00592CC0 ; 
//.text:00592D80 ; void __cdecl c_director::player_switch_gui_create_or_update()
//.text:00592D90 ; bool __cdecl c_director::player_switch_gui_destroy()
//.text:00592DD0 ; bool __cdecl c_director::player_switch_gui_set_visible(e_output_user_index)
//.text:00592E30 ; 
//.text:00592F90 ; bool __cdecl c_director::player_switch_update()
//.text:005931CC ; 

bool c_director::set_camera_mode(e_camera_mode camera_mode, real transition_time)
{
	//return DECLFUNC(0x005931F0, bool, __thiscall, c_director*, e_camera_mode, real)(this, camera_mode, transition_time);

	return set_camera_mode_internal(camera_mode, transition_time, false);
}

bool c_director::set_camera_mode_internal(e_camera_mode camera_mode, real transition_time, bool force_update)
{
	//return DECLFUNC(0x00593210, bool, __thiscall, c_director*, e_camera_mode, real, bool)(this, camera_mode, transition_time, force_update);

	if (!can_use_camera_mode(camera_mode))
		return false;

	e_camera_mode current_camera_mode = get_camera()->get_type();

	bool result = camera_mode != current_camera_mode;
	if (result || force_update)
	{
		switch (camera_mode)
		{
		case _camera_mode_following:
			static_cast<c_following_camera*>(get_camera())->constructor(dead_or_alive_unit_from_output_user(m_output_user_index));
			break;
		case _camera_mode_orbiting:
			static_cast<c_orbiting_camera*>(get_camera())->constructor(dead_or_alive_unit_from_output_user(m_output_user_index));
			break;
		case _camera_mode_flying:
			static_cast<c_flying_camera*>(get_camera())->constructor(m_output_user_index);
			break;
		case _camera_mode_first_person:
			static_cast<c_first_person_camera*>(get_camera())->constructor(dead_or_alive_unit_from_output_user(m_output_user_index));
			break;
		case _camera_mode_dead:
			static_cast<c_dead_camera*>(get_camera())->constructor(m_output_user_index);
			break;
		case _camera_mode_static:
			static_cast<c_static_camera*>(get_camera())->constructor(m_output_user_index);
			break;
		case _camera_mode_scripted:
			static_cast<c_scripted_camera*>(get_camera())->constructor();
			break;
		case _camera_mode_authored:
			static_cast<c_authored_camera*>(get_camera())->constructor(m_output_user_index);
			break;
		}
		m_transition_time = transition_time;
	}

	e_director_perspective director_perspective = get_perspective();
	s_director_globals* director_globals = director_globals_get();
	if (director_globals->infos[m_output_user_index].director_perspective != director_perspective ||
		director_globals->infos[m_output_user_index].camera_mode != camera_mode)
	{
		director_globals->infos[m_output_user_index].director_perspective = director_perspective;
		director_globals->infos[m_output_user_index].camera_mode = camera_mode;
		first_person_weapon_perspective_changed(m_output_user_index);
	}

	return result || force_update;
}

//.text:005934A0 ; void __cdecl c_camera::set_forward(vector3d const*)
//.text:005934B0 ; void __cdecl c_camera::set_position(real_point3d const*)
//.text:005934C0 ; void __cdecl c_camera::set_roll(real)
//.text:005934D0 ; void __cdecl c_director::set_watched_player(long)
//.text:00593520 ; bool __cdecl c_director::should_draw_hud() const
//.text:00593530 ; bool __cdecl c_director::should_draw_hud_saved_film() const
//.text:00593540 ; void __cdecl c_director::update(real)
//.text:00593770 ; void __cdecl c_null_camera::update(long, real, s_observer_command*)
//.text:00593780 ; void __cdecl c_director::update_perspective()
//.text:005937F0 ; void __cdecl update_vtables()

//.text:007215C0 ; c_game_director::c_game_director(e_output_user_index)
//.text:007215F0 ; c_game_director::c_game_director()
//.text:00721610 ; e_director_perspective __cdecl director_game_camera_deterministic(long, real_point3d*, vector3d*)
//.text:00721660 ; bool __cdecl c_game_director::dead_camera_should_switch_to_orbiting(long, long)
//.text:00721910 ; e_director_mode __cdecl c_game_director::get_type() const
//.text:00721920 ; bool __cdecl c_game_director::inhibits_facing() const
//.text:00721970 ; bool __cdecl c_game_director::inhibits_input() const
//.text:00721980 ; bool __cdecl c_game_director::should_draw_hud() const
//.text:00721990 ; bool __cdecl c_game_director::should_draw_hud_saved_film() const
//.text:007219A0 ; void __cdecl c_game_director::update(real)

//.text:00725A80 ; c_camera::c_camera()

bool __cdecl camera_input_inhibited(e_controller_index controller_index)
{
	return INVOKE(0x00725AA0, camera_input_inhibited, controller_index);
}

//.text:00725B90 ; c_camera::vftable13
//.text:00725BA0 ; long __cdecl c_camera::get_target() const
//.text:00725BB0 ; void __cdecl c_camera::handle_deleted_object(long)
//.text:00725BD0 ; void __cdecl c_camera::handle_deleted_player(long)
//.text:00725BE0 ; void __cdecl c_camera::post_update(s_observer_command*)
//.text:00725C10 ; void __cdecl c_camera::pre_update()
//.text:00725C20 ; void __cdecl c_flags_no_init<c_camera::e_base_camera_flags, long, 1>::set(c_camera::e_base_camera_flags, bool)
//.text:00725C50 ; void __cdecl c_camera::set_next_move_instantly()
//.text:00725C60 ; void __cdecl c_camera::set_target(long)
//.text:00725C70 ; bool __cdecl c_flags_no_init<c_camera::e_base_camera_flags, long, 1>::test(c_camera::e_base_camera_flags) const
//.text:00725C90 ; void camera_globals_initialize_from_tags()
//.text:007260D0 ; c_debug_director::c_debug_director(long)

char const* director_mode_get_name(e_director_mode director_mode)
{
	if (director_mode < _director_mode_game || director_mode >= k_number_of_director_modes)
		return "<invalid 'director_mode'>";

	return k_director_mode_names[director_mode];
}

e_director_mode director_mode_from_string(char const* str)
{
	e_director_mode director_mode = e_director_mode(-1);
	for (long i = _camera_mode_following; i < k_number_of_director_modes; i++)
	{
		if (csstricmp(str, k_director_mode_names[i]) != 0)
			continue;

		director_mode = e_director_mode(i);
	}

	return director_mode;
}

s_director_info* director_get_info(e_output_user_index output_user_index)
{
	if (!director_globals_get())
		return nullptr;

	return &director_globals_get()->infos[output_user_index];
}

void director_set_perspective(e_output_user_index output_user_index, e_director_perspective director_perspective)
{
	s_director_globals* director_globals = director_globals_get();
	if (director_globals->infos[output_user_index].director_perspective != director_perspective)
	{
		director_globals->infos[output_user_index].director_perspective = director_perspective;
		first_person_weapon_perspective_changed(output_user_index);
	}
}

void director_toggle(e_output_user_index output_user_index, e_director_mode director_mode)
{
	static e_director_mode previous_mode = {};

	if (director_get_info(output_user_index)->director_mode == director_mode)
		director_mode = previous_mode;

	director_set_mode(output_user_index, director_mode);
}

void director_toggle_perspective(e_output_user_index output_user_index, e_director_perspective director_perspective)
{
	static e_director_perspective previous_mode = {};

	if (director_get_info(output_user_index)->director_perspective == director_perspective)
		director_perspective = previous_mode;

	director_set_perspective(output_user_index, director_perspective);
}

void director_toggle_camera(e_output_user_index output_user_index, e_camera_mode camera_mode)
{
	static e_camera_mode previous_mode = {};

	if (director_get_info(output_user_index)->camera_mode == camera_mode)
		camera_mode = previous_mode;

	director_get(output_user_index)->set_camera_mode(camera_mode, 0.0f);
}

void __cdecl director_set_flying_camera_direct(e_output_user_index output_user_index, real_point3d const* position, vector3d const* forward, vector3d const* up)
{
	if (output_user_index == NONE)
		return;

	TLS_DATA_GET_VALUE_REFERENCE(director_globals);
	ASSERT(VALID_INDEX(output_user_index, k_number_of_output_users));

	c_director* director = director_get(output_user_index);
	director->set_camera_mode_internal(_camera_mode_flying, 0.0f, false);

	c_flying_camera* flying_camera = static_cast<c_flying_camera*>(director->get_camera());
	flying_camera->set_position(position);
	flying_camera->set_forward(forward);

	vector3d up_from_forward{};
	generate_up_vector3d(forward, &up_from_forward);
	real roll = angle_between_vectors3d(up, &up_from_forward);
	vector3d product3d;
	cross_product3d(up, &up_from_forward, &product3d);
	if (dot_product3d(&product3d, forward) > 0.0f)
		roll = -roll;

	flying_camera->set_roll(roll);
}

char const* scenario_get_name()
{
	if (global_scenario_index == NONE)
		return tag_name_strip_path(main_game_globals.game_loaded_scenario_path);

	if (char const* name = tag_get_name(global_scenario_index))
		return tag_name_strip_path(name);

	return tag_name_strip_path(main_game_globals.game_loaded_scenario_path);
}

void director_save_camera_named(char const* name)
{
	if (!global_scenario_try_and_get())
		return;

	TLS_DATA_GET_VALUE_REFERENCE(players_globals);
	if (!players_globals)
		return;

	c_static_string<256> filename;
	char const* root = "";
	char const* scenario_name = scenario_get_name();
	filename.print("%s%s_%s.txt", root, name, scenario_name);

	FILE* file;
	fopen_s(&file, filename.get_string(), "w");
	if (file)
	{
		s_observer_result const* camera = nullptr;
		for (e_output_user_index user_index = first_output_user(); user_index != k_output_user_none; user_index = next_output_user(user_index))
		{
			camera = observer_try_and_get_camera(user_index);
			if (camera)
				break;
		}

		if (camera)
		{
			fprintf(file, "%f %f %f\n", camera->focus_point.x, camera->focus_point.y, camera->focus_point.z);
			fprintf(file, "%f %f %f\n", camera->forward.i, camera->forward.j, camera->forward.k);
			fprintf(file, "%f %f %f\n", camera->up.i, camera->up.j, camera->up.k);
			fprintf(file, "%f\n", camera->horizontal_field_of_view);
			console_printf("%s written successfully", filename.get_string());
		}

		fclose(file);
	}
}

void director_load_camera_named(char const* name)
{
	if (!global_scenario_try_and_get())
	{
		console_printf("no scenario loaded");
		return;
	}

	TLS_DATA_GET_VALUE_REFERENCE(players_globals);
	if (!players_globals)
		return;

	c_static_string<256> filename;
	char const* root = "";
	char const* scenario_name = scenario_get_name();
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
	vector3d forward{};
	vector3d up{};
	real field_of_view;

	fscanf_s(file, "%f %f %f\n", &position.x, &position.y, &position.z);
	fscanf_s(file, "%f %f %f\n", &forward.i, &forward.j, &forward.k);
	fscanf_s(file, "%f %f %f\n", &up.i, &up.j, &up.k);
	fscanf_s(file, "%f\n", &field_of_view);

	fclose(file);

	// #TODO: actually validate `position`, `forward`, `up` and `field_of_view`
	if (false)
	{
		c_console::write_line("saved camera '%s' isn't valid", filename.get_string());
		return;
	}

	e_output_user_index user_index = player_mapping_first_active_output_user();
	if (user_index != k_number_of_output_users)
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
	e_output_user_index user_index = player_mapping_first_active_output_user();
	if (VALID_INDEX(user_index, k_number_of_output_users))
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

void survival_mode_update_flying_camera(e_output_user_index output_user_index)
{
	if (survival_mode_allow_flying_camera)
	{
		long player_index = player_mapping_get_player_by_output_user(output_user_index);
		if (player_index != NONE && game_is_survival())
		{
			TLS_DATA_GET_VALUE_REFERENCE(player_data);
			player_datum* player = &player_data[player_index];

			c_director* director = director_get(output_user_index);
			e_director_mode director_mode = director->get_type();

			if (player->dead_timer <= 0)
			{
				if (director_mode == _director_mode_observer)
				{
					director_set_mode(output_user_index, _director_mode_game);

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
					director_set_mode(output_user_index, _director_mode_observer);
			}
		}
	}
}

void control_toggling_of_debug_directors(e_output_user_index output_user_index)
{
	c_director* director = director_get(output_user_index);
	e_director_mode director_mode = director->get_type();

	if (director_mode != _director_mode_editor && input_key_frames_down(_key_code_backspace, _input_type_game) == 1)
	{
		if (director_mode == _director_mode_debug)
		{
			if (static_cast<c_debug_director*>(director)->finished_cycle())
			{
				console_printf("exiting debug director");
				director_set_mode(output_user_index, choose_appropriate_director(output_user_index));
			}

		}
		else
		{
			input_suppress();
			console_printf("entering debug director");
			director_set_mode(output_user_index, _director_mode_debug);
		}
	}
}

