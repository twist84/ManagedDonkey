#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_traits.hpp"
#include "saved_games/saved_game_files.hpp"
#include "tag_files/files.hpp"
#include "text/unicode.hpp"

#pragma pack(push, 4)

enum e_multiplayer_team;
enum e_team_scoring_method;
enum e_multiplayer_team_designator;
enum e_game_team;
enum e_multiplayer_team_designator;
enum e_game_engine_kill_flags;
enum e_simulation_entity_type;
enum e_simulation_event_type;

struct game_engine_interface_state;
struct s_chud_navpoint;
struct s_netgame_goal_influencer;
struct s_game_engine_event_data;
struct s_multiplayer_runtime_globals_definition;
struct c_bitstream;
struct s_file_reference;

enum e_game_engine_type
{
	_game_engine_type_none = 0,
	_game_engine_type_ctf,
	_game_engine_type_slayer,
	_game_engine_type_oddball,
	_game_engine_type_king,
	_game_engine_type_sandbox,
	_game_engine_type_vip,
	_game_engine_type_juggernaut,
	_game_engine_type_territories,
	_game_engine_type_assault,
	_game_engine_type_infection,

	k_game_engine_type_count,
	k_game_engine_type_default = _game_engine_type_none
};

struct c_game_engine_base_variant
{
public:
	virtual long get_game_engine_name_string_id() const;
	virtual long get_game_engine_default_description_string_id() const;
	virtual void initialize();
	virtual void validate();
	virtual void encode(c_bitstream* packet) const;
	virtual void decode(c_bitstream* packet);
	//virtual void byteswap(); // MCC
	virtual bool can_add_to_recent_list() const;
	virtual long get_score_to_win_round() const;
	virtual long get_score_unknown() const; // halo online specific
	virtual bool can_be_cast_to(e_game_engine_type game_engine_index, void const**) const;
	virtual void custom_team_score_stats(long team_index, long, long) const;

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	c_game_engine_base_variant* constructor()
	{
		return DECLFUNC(0x00572B20, c_game_engine_base_variant*, __thiscall, c_game_engine_base_variant*)(this);
	}

	void byteswap();

	void set(c_game_engine_base_variant const* variant, bool force);
	//void set(s_game_engine_base_variant_definition const* definition, bool force);

	void get_game_engine_name(c_static_wchar_string<1024>* game_engine_name) const;
	void get_game_engine_description(c_static_wchar_string<1024>* game_engine_description) const;

	char const* get_name() const;
	void set_name(char const* name);

	char const* get_description() const;
	void set_description(char const* description);

	c_game_engine_miscellaneous_options* get_miscellaneous_options_writeable();
	c_game_engine_miscellaneous_options const* get_miscellaneous_options() const;

	c_game_engine_respawn_options* get_respawn_options_writeable();
	c_game_engine_respawn_options const* get_respawn_options() const;

	c_game_engine_social_options* get_social_options_writeable();
	c_game_engine_social_options const* get_social_options() const;

	c_game_engine_map_override_options* get_map_override_options_writeable();
	c_game_engine_map_override_options const* get_map_override_options() const;

	bool get_built_in() const;
	void set_built_in(bool built_in);

	short get_team_scoring_method() const;
	void set_team_scoring_method(short team_scoring_method);

protected:
	dword m_checksum;

	char m_name[32];
	s_content_item_metadata m_metadata;
	c_game_engine_miscellaneous_options m_miscellaneous_options;
	c_game_engine_respawn_options m_respawn_options;
	c_game_engine_social_options m_social_options;
	c_game_engine_map_override_options m_map_override_options;
	c_flags<e_base_variant_flags, word, k_base_variant_flags> m_flags;
	short m_team_scoring_method;
};
static_assert(sizeof(c_game_engine_base_variant) == 0x1D0);

struct s_game_engine_state_data
{
	word_flags initial_teams;
	word_flags valid_designators;
	word_flags valid_teams;
	word_flags active_teams;
	word game_simulation;
	c_static_array<short, 9> team_designator_to_team_index;
	c_static_array<byte, 8> team_lives_per_round;
	byte current_state;
	bool game_finished;
	word round_index;
	word round_timer;
	byte_flags round_condition_flags;
	byte pad2B[1];
};
static_assert(sizeof(s_game_engine_state_data) == 0x2C);

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
	virtual void adjust_object_gravity(long, vector3d*) const;
	virtual void prepare_for_new_state(long) const;
	virtual bool should_end_round(long*) const;
	virtual long get_player_state_index(long, bool*) const;
	virtual bool should_purge_multiplayer_item(long) const;

	// function in the same place as `close_any_custom_ui` from Reach
	virtual void close_any_ui(e_output_user_index output_user_index) const;

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
private:
	virtual void dump_settings_(s_file_reference*) const;
public:
	virtual void emit_game_start_event(long) const;

	void dump_player_trait_settings(char const* traits_name, c_player_traits const* traits, s_file_reference* file) const;
	void dump_settings(s_file_reference* file) const;
};

#pragma pack(pop)

