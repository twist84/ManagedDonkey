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

class c_director
{
public:
	void __thiscall player_switch_gui_create_or_update_();

public:
	virtual e_director_mode get_type() const;
	virtual void update(real32 dt);
	virtual bool should_draw_hud();
	virtual bool should_draw_hud_saved_film();
	virtual bool inhibits_facing();
	virtual bool inhibits_input();
	virtual void handle_deleted_player(int32 player_index);
	virtual void handle_deleted_object(int32 object_index);
	virtual bool can_use_camera_mode(e_camera_mode camera_mode);
	//virtual void select_fallback_target(); // c_observer_director, c_saved_film_director

	bool in_free_camera_mode() const;
	bool force_set_camera_mode(e_camera_mode camera_mode, real32 interpolation_time);
	c_camera* get_camera();
	const c_camera* get_camera() const;
	const s_observer_command* get_last_observer_command() const;
	e_director_perspective get_perspective() const;
	int32 get_watched_player() const;

	int32 player_get_next_player_with_a_unit(int32 avoid_player_index, int32 starting_player_index, int32 always_player_index, int32 search_direction, bool match_team, bool match_alive_unit, bool allow_wrapping, int32* out_unit_index);
	static void __cdecl player_set_desired_respawn_player(int32 player_index, int32 desired_respawn_player_index);
	void player_switch_gui_button_pressed();
	static bool __cdecl player_switch_gui_create(int32 user_index);
	void player_switch_gui_create_or_update();
	bool player_switch_gui_destroy();
	static bool __cdecl player_switch_gui_set_visible(int32 user_index);
	void player_switch_gui_update();
	bool player_switch_update();

	bool set_camera_mode(e_camera_mode camera_mode, real32 transition_time);
	bool set_camera_mode_internal(e_camera_mode camera_mode, real32 transition_time, bool force_update);
	void set_watched_player(int32 player_index);
	void update_perspective();

//protected:
	byte m_camera_storage[0x4C];
	s_observer_command m_last_observer_command;
	real32 m_change_camera_pause;
	int32 m_user_index;
	int32 m_watched_player_index;
	bool m_player_switch_gui_activated;
	byte pad[0x3];
};
static_assert(sizeof(c_director) == 0x14C);

struct s_director_info
{
	e_director_mode mode;
	e_director_perspective perspective;
	e_camera_mode camera_mode;
};
static_assert(sizeof(s_director_info) == 0xC);

struct s_director_globals
{
	c_static_array<byte[0x160], 4> directors;
	c_static_array<s_director_info, 4> director_info;
	real32 dtime;
	real32 fade_timer;
	real32 fade_maximum;
	bool debug_force_scripted_camera_disable;
};
static_assert(sizeof(s_director_globals) == 0x5C0);

struct s_observer_gamestate_globals
{
	int32 active_structure_bsp_index_mask;
	s_cluster_reference cluster_references[4];
};
static_assert(sizeof(s_observer_gamestate_globals) == 0xC);

extern e_director_mode __cdecl choose_appropriate_director(int32 user_index);
extern int32 __cdecl dead_or_alive_unit_from_user(int32 user_index);
extern void __cdecl director_dispose();
extern void __cdecl director_dispose_from_old_map();
extern void __cdecl director_fix_vtables_for_saved_game();
extern e_director_perspective __cdecl director_game_desired_perspective(int32 unit_index, int32* seat_state);
extern void __cdecl director_game_tick();
extern c_director* __cdecl director_get(int32 user_index);
extern bool __cdecl director_get_camera_third_person(int32 user_index);
extern s_cluster_reference __cdecl director_get_deterministic_scripted_camera_cluster_reference();
extern real32 __cdecl director_get_fade_timer();
extern e_director_perspective __cdecl director_get_perspective(int32 user_index);
extern void __cdecl director_get_position(int32 user_index, real_point3d* position);
extern void __cdecl director_handle_deleted_object(int32 object_index);
extern void __cdecl director_handle_deleted_player(int32 player_index);
extern bool __cdecl director_in_scripted_camera();
extern bool __cdecl director_in_unit_perspective(int32 user_index);
extern bool __cdecl director_inhibited_facing(int32 user_index);
extern bool __cdecl director_inhibited_input(int32 user_index);
extern void __cdecl director_initialize();
extern void __cdecl director_initialize_for_new_map();
extern void __cdecl director_initialize_for_saved_game(int32 flags);
extern void __cdecl director_notify_map_reset();
extern void __cdecl director_render();
extern void __cdecl director_reset();
extern void __cdecl director_reset_user_game_camera(int32 user_index);
extern void __cdecl director_script_camera(bool scripted);
extern void __cdecl director_set_camera_mode(int32 user_index, e_camera_mode camera_mode);
extern void __cdecl director_set_camera_third_person(int32 user_index, bool camera_third_person);
extern void __cdecl director_set_fade_timer(real32 fade_timer);
extern void __cdecl director_set_mode(int32 user_index, e_director_mode director_mode);
extern void __cdecl director_setup_flying_camera_at_scenario_point(int32 user_index, int32 camera_point_index);
extern bool __cdecl camera_input_inhibited(e_controller_index controller_index);
extern void __cdecl director_update(real32 dt);
extern void __cdecl hs_director_set_camera_mode(int32 user_index, int32 camera_mode);
extern void __cdecl hs_director_set_camera_target(int32 user_index, int32 object_index);

extern const char* director_mode_get_name(e_director_mode director_mode);
extern e_director_mode director_mode_from_string(const char* str);
extern s_director_info* director_get_info(int32 user_index);
extern void director_set_perspective(int32 user_index, e_director_perspective perspective);
extern void director_toggle(int32 user_index, e_director_mode director_mode);
extern void director_toggle_perspective(int32 user_index, e_director_perspective perspective);
extern void director_toggle_camera(int32 user_index, e_camera_mode camera_mode);
extern void director_set_flying_camera_direct(int32 user_index, const real_point3d* position, const real_vector3d* forward, const real_vector3d* up);
extern void director_save_camera_named(const char* name);
extern void director_load_camera_named(const char* name);
extern void director_save_camera();
extern void director_load_camera();
extern void director_debug_camera(bool render);
extern void survival_mode_update_flying_camera(int32 user_index);
extern void control_toggling_of_debug_directors(int32 user_index);

extern const char* const k_camera_save_filename;
extern const char* k_director_mode_names[k_number_of_director_modes];

