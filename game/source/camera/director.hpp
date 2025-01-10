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

enum e_director_perspective
{
	_director_perspective_first_person = 0,
	_director_perspective_third_person,
	_director_perspective_scripted,
	_director_perspective_neutral,

	k_number_of_director_perspectives,
};

struct c_director
{
public:
	virtual e_director_mode get_type() const;
	virtual void update(real dt);
	virtual bool should_draw_hud();
	virtual bool should_draw_hud_saved_film();
	virtual bool inhibits_facing();
	virtual bool inhibits_input();
	virtual void handle_deleted_player(long player_index);
	virtual void handle_deleted_object(long object_index);
	virtual bool can_use_camera_mode(e_camera_mode camera_mode);
	//virtual void select_fallback_target(); // c_observer_director, c_saved_film_director

	bool in_free_camera_mode() const;
	c_camera* get_camera();
	c_camera const* get_camera() const;
	s_observer_command const* get_last_observer_command() const;
	e_director_perspective get_perspective() const;
	bool set_camera_mode_internal(e_camera_mode camera_mode, real transition_time, bool force_update);
	bool set_camera_mode(e_camera_mode camera_mode, real transition_time);

//protected:
	byte m_camera_storage[0x4C];
	s_observer_command m_last_observer_command;
	real m_change_camera_pause;
	long m_user_index;
	long m_watched_player_index;
	bool m_player_switch_gui_activated;
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
	c_static_array<byte[0x160], 4> directors;
	c_static_array<s_director_info, 4> director_info;
	real dtime;
	real fade_timer;
	real fade_maximum;
	bool debug_force_scripted_camera_disable;
};
static_assert(sizeof(s_director_globals) == 0x5C0);

struct s_observer_gamestate_globals
{
	long active_structure_bsp_index_mask;
	s_cluster_reference cluster_references[4];
};
static_assert(sizeof(s_observer_gamestate_globals) == 0xC);

enum e_controller_index;

extern e_director_mode __cdecl choose_appropriate_director(long user_index);
extern long __cdecl dead_or_alive_unit_from_user(long user_index);
extern void __cdecl director_dispose();
extern void __cdecl director_dispose_from_old_map();
extern void __cdecl director_fix_vtables_for_saved_game();
extern e_director_perspective __cdecl director_game_desired_perspective(long unit_index, long* seat_state);
extern void __cdecl director_game_tick();
extern c_director* __cdecl director_get(long user_index);
extern bool __cdecl director_get_camera_third_person(long user_index);
extern s_cluster_reference __cdecl director_get_deterministic_scripted_camera_cluster_reference();
extern real __cdecl director_get_fade_timer();
extern e_director_perspective __cdecl director_get_perspective(long user_index);
extern void __cdecl director_get_position(long user_index, real_point3d* position);
extern void __cdecl director_handle_deleted_object(long object_index);
extern void __cdecl director_handle_deleted_player(long player_index);
extern bool __cdecl director_in_scripted_camera();
extern bool __cdecl director_in_unit_perspective(long user_index);
extern bool __cdecl director_inhibited_facing(long user_index);
extern bool __cdecl director_inhibited_input(long user_index);
extern void __cdecl director_initialize();
extern void __cdecl director_initialize_for_new_map();
extern void __cdecl director_initialize_for_saved_game(long flags);
extern void __cdecl director_notify_map_reset();
extern void __cdecl director_render();
extern void __cdecl director_reset();
extern void __cdecl director_reset_user_game_camera(long user_index);
extern void __cdecl director_script_camera(bool scripted);
extern void __cdecl director_set_camera_mode(long user_index, e_camera_mode camera_mode);
extern void __cdecl director_set_camera_third_person(long user_index, bool camera_third_person);
extern void __cdecl director_set_fade_timer(real fade_timer);
extern void __cdecl director_set_mode(long user_index, e_director_mode director_mode);
extern void __cdecl director_setup_flying_camera_at_scenario_point(long user_index, long camera_point_index);
extern bool __cdecl camera_input_inhibited(e_controller_index controller_index);
extern void __cdecl director_update(real dt);

extern char const* director_mode_get_name(e_director_mode director_mode);
extern e_director_mode director_mode_from_string(char const* str);
extern s_director_info* director_get_info(long user_index);
extern void director_set_perspective(long user_index, e_director_perspective director_perspective);
extern void director_toggle(long user_index, e_director_mode director_mode);
extern void director_toggle_perspective(long user_index, e_director_perspective director_perspective);
extern void director_toggle_camera(long user_index, e_camera_mode camera_mode);
extern void director_set_flying_camera_direct(long user_index, real_point3d const* position, real_vector3d const* forward, real_vector3d const* up);
extern void director_save_camera_named(char const* name);
extern void director_load_camera_named(char const* name);
extern void director_save_camera();
extern void director_load_camera();
extern void director_debug_camera(bool render);
extern void survival_mode_update_flying_camera(long user_index);
extern void control_toggling_of_debug_directors(long user_index);

extern char const* const k_camera_save_filename;
extern char const* k_director_mode_names[k_number_of_director_modes];

