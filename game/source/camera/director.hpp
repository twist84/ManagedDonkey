#pragma once

#include "cseries/cseries.hpp"
#include "camera/camera.hpp"
#include "camera/dead_camera.hpp"
#include "camera/first_person_camera.hpp"
#include "camera/flying_camera.hpp"
#include "camera/following_camera.hpp"
#include "camera/observer.hpp"
#include "camera/orbiting_camera.hpp"
#include "camera/static_camera.hpp"
#include "memory/data.hpp"

enum e_director_mode
{
	_director_mode_game = 0,
	_director_mode_saved_film,
	_director_mode_observer,
	_director_mode_debug,
	_director_mode_unused,
	_director_mode_editor,

	k_number_of_director_modes
};

enum e_controller_index;
enum e_output_user_index;

enum e_director_perspective
{
	_director_perspective_first_person = 0,
	_director_perspective_third_person,
	_director_perspective_scripted,

	// c_null_camera, (c_authored_camera default)
	_director_perspective_3,

	k_number_of_director_perspectives,
};

struct c_director
{
	virtual e_director_mode get_type() const;
	virtual void update(real);
	virtual bool should_draw_hud();
	virtual bool should_draw_hud_saved_film();
	virtual bool inhibits_facing();
	virtual bool inhibits_input();
	virtual void handle_deleted_player(long);
	virtual void handle_deleted_object(long);
	virtual bool can_use_camera_mode(e_camera_mode);
	//virtual void select_fallback_target(); // c_observer_director, c_saved_film_director

	bool in_free_camera_mode() const;
	c_camera* get_camera();
	c_camera const* get_camera() const;
	s_observer_command const* get_last_observer_command() const;
	e_director_perspective get_perspective() const;
	bool set_camera_mode_internal(e_camera_mode camera_mode, real transition_time, bool force_update);
	bool set_camera_mode(e_camera_mode camera_mode, real transition_time);

	byte m_camera[0x4C];

	s_observer_command m_observer_command;
	real m_transition_time;
	e_output_user_index m_output_user_index;
	long m_user_index;
	bool __unknown148;

	byte pad[3];
};
static_assert(sizeof(c_director) == 0x14C);

struct s_director_info
{
	e_director_mode director_mode;
	e_director_perspective director_perspective;
	e_camera_mode camera_mode;
};
static_assert(sizeof(s_director_info) == 0xC);

struct s_director_globals
{
	// c_static_array<byte[0x160], 4> directors;
	byte directors[4][0x160];

	s_director_info infos[4];
	real timestep;
	real fade_timer5B4; // in?
	real fade_timer5B8; // out?
	bool debug_force_scripted_camera_disable;
};
static_assert(sizeof(s_director_globals) == 0x5C0);

struct s_observer_gamestate_globals
{
	long __unknown0;
	short __unknown4;
	short __unknown6;
	short __unknown8;
	short __unknownA;
};
static_assert(sizeof(s_observer_gamestate_globals) == 0xC);

extern e_director_mode __cdecl choose_appropriate_director(e_output_user_index output_user_index);
extern long __cdecl dead_or_alive_unit_from_output_user(e_output_user_index output_user_index);
extern void __cdecl director_dispose();
extern void __cdecl director_dispose_from_old_map();
extern void __cdecl director_fix_vtables_for_saved_game();
extern e_director_perspective __cdecl director_game_desired_perspective(long unit_index, long* seat_state);
extern void __cdecl director_game_tick();
extern c_director* __cdecl director_get(e_output_user_index output_user_index);
extern bool __cdecl director_get_camera_third_person(e_output_user_index output_user_index);
extern s_cluster_reference __cdecl director_get_deterministic_scripted_camera_cluster_reference();
extern real __cdecl director_get_fade_timer();
extern e_director_perspective __cdecl director_get_perspective(e_output_user_index output_user_index);
extern void __cdecl director_get_position(e_output_user_index output_user_index, real_point3d* position);
extern void __cdecl director_handle_deleted_object(long object_index);
extern void __cdecl director_handle_deleted_player(long player_index);
extern bool __cdecl director_in_scripted_camera();
extern bool __cdecl director_in_unit_perspective(e_output_user_index output_user_index);
extern bool __cdecl director_inhibited_facing(e_output_user_index output_user_index);
extern bool __cdecl director_inhibited_input(e_output_user_index output_user_index);
extern void __cdecl director_initialize();
extern void __cdecl director_initialize_for_new_map();
extern void __cdecl director_initialize_for_saved_game(long flags);
extern void __cdecl director_notify_map_reset();
extern void __cdecl director_render();
extern void __cdecl director_reset();
extern void __cdecl director_reset_user_game_camera(e_output_user_index output_user_index);
extern void __cdecl director_script_camera(bool scripted);
extern void __cdecl director_set_camera_mode(e_output_user_index output_user_index, e_camera_mode camera_mode);
extern void __cdecl director_set_camera_third_person(e_output_user_index output_user_index, bool camera_third_person);
extern void __cdecl director_set_fade_timer(real fade_timer);
extern void __cdecl director_set_mode(e_output_user_index output_user_index, e_director_mode director_mode);
extern void __cdecl director_setup_flying_camera_at_scenario_point(e_output_user_index output_user_index, long cutscene_camera_point);
extern bool __cdecl camera_input_inhibited(e_controller_index controller_index);
extern void __cdecl director_update(real world_seconds_elapsed);

extern char const* director_mode_get_name(e_director_mode director_mode);
extern e_director_mode director_mode_from_string(char const* str);
extern s_director_info* director_get_info(e_output_user_index output_user_index);
extern void director_set_perspective(e_output_user_index output_user_index, e_director_perspective director_perspective);
extern void director_toggle(e_output_user_index output_user_index, e_director_mode director_mode);
extern void director_toggle_perspective(e_output_user_index output_user_index, e_director_perspective director_perspective);
extern void director_toggle_camera(e_output_user_index output_user_index, e_camera_mode camera_mode);
extern void director_set_flying_camera_direct(e_output_user_index output_user_index, real_point3d const* position, vector3d const* forward, vector3d const* up);
extern void director_save_camera_named(char const* name);
extern void director_load_camera_named(char const* name);
extern void director_save_camera();
extern void director_load_camera();
extern void director_debug_camera(bool render);
extern void survival_mode_update_flying_camera(e_output_user_index output_user_index);
extern void control_toggling_of_debug_directors(e_output_user_index output_user_index);

extern char const* const k_camera_save_filename;
extern char const* k_director_mode_names[k_number_of_director_modes];

