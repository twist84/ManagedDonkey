#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_traits.hpp"
#include "tag_files/files.hpp"

enum e_multiplayer_team;
enum e_team_scoring_method;
struct game_engine_interface_state;
enum e_multiplayer_team_designator;
struct s_chud_navpoint;
struct s_netgame_goal_influencer;
enum e_game_team;
enum e_multiplayer_team_designator;
enum e_game_engine_kill_flags;
enum e_simulation_entity_type;
struct s_game_engine_state_data;
struct s_game_engine_event_data;
enum e_simulation_event_type;
struct s_multiplayer_runtime_globals_definition;

struct c_game_engine
{
public:
	virtual long get_type() const;
	virtual long get_score_to_win_round() const;

	// some new score saber added for halo online
	virtual long get_score_unknown() const;

	virtual void recompute_team_score(e_multiplayer_team, long, e_team_scoring_method) const;
	virtual void get_score_string(long, wchar_t(&)[256]) const;
	virtual void get_hud_interface_state(long, game_engine_interface_state*) const;
	virtual bool initialize_for_new_map() const;
	virtual void dispose_from_old_map() const;
	virtual bool initialize_for_new_round() const;
	virtual void stats_reset_for_round_switch() const;
	virtual bool validate_team_designator_for_new_map(e_multiplayer_team_designator) const;
	virtual void player_added(long) const;
	virtual void player_activated(long) const;
	virtual void player_left(long) const;
	virtual void player_rejoined(long) const;
	virtual void player_changed_indices(long, long) const;
	virtual void player_changed_teams(long) const;
	virtual void player_about_to_spawn(long) const;
	virtual void player_just_spawned(long) const;
	virtual void game_ending() const;
	virtual void game_starting() const;
	virtual void render(long) const;
	virtual void render_debug(long) const;
	virtual void submit_nav_points(long, long) const;
	virtual bool build_player_nav_point(long, long, long, bool, s_chud_navpoint*) const;
	virtual bool should_draw_nav_point(long, long) const;
	virtual void update() const;
	virtual void player_update(long) const;
	virtual void assemble_baseline_traits_for_player(long, c_player_traits*) const;
	virtual void apply_game_engine_traits_for_player(long, c_player_traits*) const;
	virtual void assemble_spawn_influencers_for_player(long, s_netgame_goal_influencer*, long*) const;
	virtual long compare_players(long, long) const;
	virtual long compare_teams(e_game_team, e_game_team) const;
	virtual bool allow_weapon_pickup(long, long) const;
	virtual bool should_auto_pickup_weapon(long, long) const;
	virtual void player_nearby_multiplayer_weapon(long, long) const;
	virtual long object_get_emblem_player(long) const;
	virtual real compute_object_function(long, long) const;
	virtual void multiplayer_weapon_register(long) const;
	virtual void multiplayer_weapon_deregister(long) const;
	virtual void multiplayer_weapon_picked_up(long, long) const;
	virtual void multiplayer_weapon_dropped(long, long) const;
	virtual void handle_deleted_object(long) const;
	virtual long get_time_left_in_ticks(long, bool, bool) const;
	virtual e_game_team get_defending_team_index() const;
	virtual e_multiplayer_team_designator get_player_team_designator(long) const;
	virtual bool team_is_enemy(e_game_team, e_game_team) const;
	virtual void score_header_string(wchar_t(&)[256]) const;
	virtual bool is_medal_allowed(long) const;
	virtual void player_damaged_player(long, long, long, bool) const;
	virtual void player_killed_player(long, long, long, bool, c_flags<e_game_engine_kill_flags, long, 24>, long) const;
	virtual void player_assisted_with_kill(long) const;
	virtual long player_killed_player_get_kill_message(long, long, bool) const;
	virtual bool test_flag(long) const;
	virtual void adjust_object_gravity(long, real_vector3d*) const;
	virtual void prepare_for_new_state(long) const;
	virtual bool should_end_round(long*) const;
	virtual long get_player_state_index(long, bool*) const;
	virtual bool should_purge_multiplayer_item(long) const;

	// unknown function, also exists in halo 3 mcc
	virtual void* function59(void*, ...);

	virtual e_simulation_entity_type get_simulation_entity_type() const;
	virtual void promote_to_simulation_authority() const;
	virtual void recover_state_before_promotion() const;
	virtual void build_global_baseline(s_game_engine_state_data*) const;
	virtual void build_global_update(dword, dword*, s_game_engine_state_data*) const;
	virtual bool apply_global_update(dword, s_game_engine_state_data const*) const;
	virtual void build_simulation_baseline(long, void*) const;
	virtual void build_simulation_update(dword*, long, void*) const;
	virtual bool apply_simulation_update(dword, long, void const*) const;
	virtual void build_player_baseline(short, long, void*) const;
	virtual void build_player_update(short, dword*, long, void*) const;
	virtual bool apply_player_update(short, dword, long, void const*) const;
	virtual long parse_multiplayer_string_token(long, wchar_t const*, long, s_game_engine_event_data const*, wchar_t*, long) const;
	virtual bool handle_incoming_simulation_event(e_simulation_event_type, void const*) const;
	virtual bool enable_lead_change_messages() const;
	virtual bool enable_tied_leader_messages() const;
	virtual long get_message_chud_reference(s_multiplayer_runtime_globals_definition*) const;
	virtual long get_message_chud_reference() const;
	virtual void dump_settings(s_file_reference*) const;
	virtual void emit_game_start_event(long) const;
};
