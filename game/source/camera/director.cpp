#include "camera/director.hpp"

#include "cache/cache_files.hpp"
#include "camera/camera_globals.hpp"
#include "camera/debug_director.hpp"
#include "camera/editor_director.hpp"
#include "camera/game_director.hpp"
#include "camera/observer_director.hpp"
#include "camera/saved_film_director.hpp"
#include "cseries/cseries_console.hpp"
#include "interface/first_person_weapons.hpp"
#include "interface/interface_constants.hpp"
#include "game/game.hpp"
#include "game/players.hpp"
#include "main/console.hpp"
#include "main/main_game.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "scenario/scenario.hpp"
#include "text/draw_string.hpp"

HOOK_DECLARE(0x00591F80, director_render);

long __cdecl dead_or_alive_unit_from_user(long user_index)
{
	return INVOKE(0x005916F0, dead_or_alive_unit_from_user, user_index);
}

char const* k_director_mode_names[k_number_of_director_modes]
{
	"game",
	"saved_film",
	"observer",
	"debug",
	"unused",
	"editor"
};

const char* director_mode_get_name(e_director_mode director_mode)
{
	if (director_mode < _director_mode_game || director_mode >= k_number_of_director_modes)
		return "<invalid 'director_mode'>";

	return k_director_mode_names[director_mode];
}

e_director_mode director_mode_from_string(const char* str)
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

s_director_globals* director_globals_get()
{
	if (!get_tls())
		return nullptr;

	TLS_DATA_GET_VALUE_REFERENCE(director_globals);
	return (director_globals && director_globals->directors[0][0]) ? director_globals : nullptr;
}

e_director_perspective c_director::get_perspective()
{
	if (!game_in_progress())
		return _director_perspective_3;
	e_director_perspective director_perspective = (e_director_perspective)get_camera()->get_perspective();
	if (!director_perspective)
		director_perspective = e_director_perspective(m_transition_time > 0.0);
	if (game_is_ui_shell())
		return _director_perspective_3;

	return director_perspective;
}

bool c_director::set_camera_mode_internal(e_camera_mode camera_mode, real transition_time, bool force_update)
{
	if (!can_use_camera_mode(camera_mode))
		return false;

	e_camera_mode current_camera_mode = get_camera()->get_type();

	bool result = camera_mode != current_camera_mode;
	if (result || force_update)
	{
		switch (camera_mode)
		{
		case _camera_mode_following:
			get_camera<c_following_camera>()->constructor(dead_or_alive_unit_from_user(m_user_index));
			break;
		case _camera_mode_orbiting:
			get_camera<c_orbiting_camera>()->constructor(dead_or_alive_unit_from_user(m_user_index));
			break;
		case _camera_mode_flying:
			get_camera<c_flying_camera>()->constructor(m_user_index);
			break;
		case _camera_mode_first_person:
			get_camera<c_first_person_camera>()->constructor(dead_or_alive_unit_from_user(m_user_index));
			break;
		case _camera_mode_dead:
			get_camera<c_dead_camera>()->constructor(m_user_index);
			break;
		case _camera_mode_static:
			get_camera<c_static_camera>()->constructor(m_user_index);
			break;
		case _camera_mode_scripted:
			get_camera<c_scripted_camera>()->constructor();
			break;
		case _camera_mode_authored:
			get_camera<c_authored_camera>()->constructor(m_user_index);
			break;
		}
		m_transition_time = transition_time;
	}

	e_director_perspective director_perspective = get_perspective();
	s_director_globals* director_globals = director_globals_get();
	if (director_globals->infos[m_user_index].director_perspective != director_perspective ||
		director_globals->infos[m_user_index].camera_mode != camera_mode)
	{
		director_globals->infos[m_user_index].director_perspective = director_perspective;
		director_globals->infos[m_user_index].camera_mode = camera_mode;
		first_person_weapon_perspective_changed(m_user_index);
	}

	return result || force_update;
}

template<typename t_type>
t_type* director_get(long user_index)
{
	if (!director_globals_get())
		return nullptr;

	return (t_type*)&director_globals_get()->directors[user_index];
}

s_director_info* director_get_info(long user_index)
{
	if (!director_globals_get())
		return nullptr;

	return &director_globals_get()->infos[user_index];
}

e_director_perspective director_get_perspective(long user_index)
{
	c_director* director = director_get(user_index);
	if (!director)
		return _director_perspective_3;

	return director->get_perspective();
}

void director_set_perspective(long user_index, e_director_perspective director_perspective)
{
	s_director_globals* director_globals = director_globals_get();
	if (director_globals->infos[user_index].director_perspective != director_perspective)
	{
		director_globals->infos[user_index].director_perspective = director_perspective;
		first_person_weapon_perspective_changed(user_index);
	}
}

void director_set_mode(long user_index, e_director_mode director_mode)
{
	switch (director_mode)
	{
	case _director_mode_game:
		director_get<c_game_director>(user_index)->constructor(user_index);
		break;
	case _director_mode_saved_film:
		director_get<c_saved_film_director>(user_index)->constructor(user_index);
		break;
	case _director_mode_observer:
		director_get<c_observer_director>(user_index)->constructor(user_index);
		break;
	case _director_mode_debug:
		director_get<c_debug_director>(user_index)->constructor(user_index);
		break;
	case _director_mode_editor:
		director_get<c_editor_director>(user_index)->constructor(user_index);
		break;
	}

	director_get_info(user_index)->director_mode = director_mode;
}

bool director_get_camera_third_person(long user_index)
{
	return director_get(user_index)->get_camera()->get_type() == _camera_mode_orbiting;
}

bool director_in_scripted_camera()
{
	if (!get_tls())
		return false;

	TLS_DATA_GET_VALUE_REFERENCE(director_camera_scripted);
	return director_camera_scripted ? *director_camera_scripted : false;
}

void director_toggle(long user_index, e_director_mode director_mode)
{
	static e_director_mode previous_mode = {};

	if (director_get_info(user_index)->director_mode == director_mode)
		director_mode = previous_mode;

	director_set_mode(user_index, director_mode);
}

void director_toggle_perspective(long user_index, e_director_perspective director_perspective)
{
	static e_director_perspective previous_mode = {};

	if (director_get_info(user_index)->director_perspective == director_perspective)
		director_perspective = previous_mode;

	director_set_perspective(user_index, director_perspective);
}

void director_toggle_camera(long user_index, e_camera_mode camera_mode)
{
	static e_camera_mode previous_mode = {};

	if (director_get_info(user_index)->camera_mode == camera_mode)
		camera_mode = previous_mode;

	director_get(user_index)->set_camera_mode(camera_mode, 0.0f);
}

void __cdecl director_render()
{
	// if (game_is_playback())
	// skipping for now since we haven't implemented saved film support *yet*

	if (player_control_get_machinima_camera_debug())
	{
		long active_output_user = player_mapping_first_active_output_user();
		s_observer_result const* camera = observer_try_and_get_camera(active_output_user);
		if (camera)
		{
			c_rasterizer_draw_string rasterizer_draw_string;
			c_font_cache_mt_safe font_cache;

			euler_angles2d facing{};
			euler_angles2d_from_vector3d(&facing, &camera->forward);
			if (facing.yaw < 0.0f)
				facing.yaw += real(TWO_PI);

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

void __cdecl director_set_flying_camera_direct(long user_index, real_point3d const* position, vector3d const* forward, vector3d const* up)
{
	if (user_index == NONE)
		return;

	TLS_DATA_GET_VALUE_REFERENCE(director_globals);
	ASSERT(VALID_INDEX(user_index, 4));

	c_director* director = director_get(user_index);
	director->set_camera_mode_internal(_camera_mode_flying, 0.0f, false);

	c_flying_camera* flying_camera = director->get_camera<c_flying_camera>();
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

char const* const k_camera_save_filename = "camera";

void __cdecl director_save_camera_named(char const* name)
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
		for (long i = 0; i < 4; i++)
		{
			camera = observer_try_and_get_camera(i++);
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

void __cdecl director_load_camera_named(char const* name)
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

	long active_user = players_first_active_user();
	if (active_user == NONE)
	{
		c_console::write_line("no active user to set saved camera '%s'", filename.get_string());
		return;
	}

	director_set_flying_camera_direct(active_user, &position, &forward, &up);
}

void __cdecl director_save_camera()
{
	director_save_camera_named(k_camera_save_filename);
}

void __cdecl director_load_camera()
{
	director_load_camera_named(k_camera_save_filename);
}

