#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_traits.hpp"
#include "saved_games/content/content_item_metadata.hpp"
#include "tag_files/files.hpp"
#include "text/unicode.hpp"

#pragma pack(push, 4)

struct game_engine_interface_state;
struct s_chud_navpoint;
struct s_netgame_goal_influencer;
struct s_game_engine_event_data;
struct s_multiplayer_runtime_globals_definition;
class c_bitstream;
struct s_file_reference;

class c_game_engine_base_variant
{
public:
	virtual int32 get_game_engine_name_string_id() const;
	virtual int32 get_game_engine_default_description_string_id() const;
	virtual void initialize();
	virtual void validate();
	virtual void encode(c_bitstream* packet) const;
	virtual void decode(c_bitstream* packet);
	//virtual void byteswap(); // MCC
	virtual bool can_add_to_recent_list() const;
	virtual int32 get_score_to_win_round() const;
	virtual int32 get_score_to_win_round_early() const; // halo online specific
	virtual bool can_be_cast_to(e_game_engine_type type, const void** pointer) const;
	virtual void custom_team_score_stats(e_game_team team_index, int32 old_score, int32 new_score) const;

	void encode_to_mcc(c_bitstream* packet) const;
	void decode_from_mcc(c_bitstream* packet);

	c_game_engine_base_variant* constructor()
	{
		return INVOKE_CLASS_MEMBER(0x00572B20, c_game_engine_base_variant, constructor);
	}

	void byteswap();

	void set(const c_game_engine_base_variant* variant, bool force);
	//void set(const s_game_engine_base_variant_definition* definition, e_game_engine_type game_engine_index);

	s_saved_game_item_metadata* get_metadata();
	const s_saved_game_item_metadata* get_metadata() const;

	void get_game_engine_name(c_static_wchar_string<1024>* game_engine_name) const;
	void get_game_engine_description(c_static_wchar_string<1024>* game_engine_description) const;

	const char* get_name() const;
	void set_name(const char* name);

	const char* get_description() const;
	void set_description(const char* description);

	c_game_engine_miscellaneous_options* get_miscellaneous_options_writeable();
	const c_game_engine_miscellaneous_options* get_miscellaneous_options() const;

	c_game_engine_respawn_options* get_respawn_options_writeable();
	const c_game_engine_respawn_options* get_respawn_options() const;

	c_game_engine_social_options* get_social_options_writeable();
	const c_game_engine_social_options* get_social_options() const;

	c_game_engine_map_override_options* get_map_override_options_writeable();
	const c_game_engine_map_override_options* get_map_override_options() const;

	bool get_built_in() const;
	void set_built_in(bool built_in);

	int16 get_team_scoring_method() const;
	void set_team_scoring_method(int16 team_scoring_method);

protected:
	uns32 m_checksum;

	char m_name[32];
	s_saved_game_item_metadata m_metadata;
	c_game_engine_miscellaneous_options m_miscellaneous_options;
	c_game_engine_respawn_options m_respawn_options;
	c_game_engine_social_options m_social_options;
	c_game_engine_map_override_options m_map_override_options;
	c_flags<e_base_variant_flags, uns16, k_base_variant_flags> m_flags;
	c_enum<e_team_scoring_method, int16, _team_scoring_method_first, k_number_of_team_scoring_methods> m_team_scoring_method;
};
COMPILE_ASSERT(sizeof(c_game_engine_base_variant) == 0x1D0);

struct s_game_engine_state_data
{
	uns16 initial_teams;
	uns16 valid_team_designators;
	uns16 valid_teams;
	uns16 active_teams;
	uns16 ever_active_teams;
	c_static_array<int16, 9> team_designator_to_team_index;
	c_static_array<int8, 8> team_lives_per_round;
	uns8 current_state;
	uns8 game_finished;
	int16 round_index;
	int16 round_timer;
	uns8 round_condition_flags;
	byte pad2B[0x1];
};
COMPILE_ASSERT(sizeof(s_game_engine_state_data) == 0x2C);

enum e_game_engine_kill_flags;
class c_game_engine
{
public:
	virtual int32 get_type() const;
	virtual int32 get_score_to_win_round() const;
	virtual int32 get_score_to_win_round_early() const; // halo online
	virtual void recompute_team_score(e_game_team game_team, int32 player_adjustment, e_team_scoring_method scoring_method) const;
	virtual void get_score_string(int32 score, c_static_wchar_string<256>* string) const;
	virtual void get_hud_interface_state(int32 player_index, game_engine_interface_state* interface_state) const;
	virtual bool initialize_for_new_map() const;
	virtual void dispose_from_old_map() const;
	virtual bool initialize_for_new_round() const;
	virtual void stats_reset_for_round_switch() const;
	virtual bool validate_team_designator_for_new_map(e_multiplayer_team_designator team_designator) const;
	virtual void player_added(int32 player_index) const;
	virtual void player_activated(int32 player_index) const;
	virtual void player_left(int32 player_index) const;
	virtual void player_rejoined(int32 player_index) const;
	virtual void player_changed_indices(int32 player_old_index, int32 player_new_index) const;
	virtual void player_changed_teams(int32 player_index) const;
	virtual void player_about_to_spawn(int32 player_index) const;
	virtual void player_just_spawned(int32 player_index) const;
	virtual void game_ending() const;
	virtual void game_starting() const;
	virtual void render(int32 player_index) const;
	virtual void render_debug(int32 player_index) const;
	virtual void submit_nav_points(int32 user_index, int32 named_player_index) const;
	virtual bool build_player_nav_point(int32 user_index, int32 target_player_index, int32 named_player_index, bool name_only, s_chud_navpoint* navpoint) const;
	virtual bool should_draw_nav_point(int32 user_index, int32 target_player_index) const;
	virtual void update() const;
	virtual void player_update(int32 player_index) const;
	virtual void apply_baseline_traits_for_player(int32 player_index, c_player_traits* traits) const;
	virtual void apply_game_engine_traits_for_player(int32 player_index, c_player_traits* traits) const;
	virtual void assemble_spawn_influencers_for_player(int32 player_index, s_netgame_goal_influencer* influencers, int32* in_out_count) const;
	virtual int32 compare_players(int32 player_index_a, int32 player_index_b) const;
	virtual int32 compare_teams(e_game_team team_a, e_game_team team_b) const;
	virtual bool allow_weapon_pickup(int32 player_index, int32 weapon_index) const;
	virtual bool should_auto_pickup_weapon(int32 player_index, int32 weapon_index) const;
	virtual void player_nearby_multiplayer_weapon(int32 player_index, int32 weapon_index) const;
	virtual int32 object_get_emblem_player(int32 object_index) const;
	virtual real32 compute_object_function(int32 object_index, int32 function) const;
	virtual void multiplayer_weapon_register(int32 weapon_index) const;
	virtual void multiplayer_weapon_deregister(int32 weapon_index) const;
	virtual void multiplayer_weapon_picked_up(int32 weapon_index, int32 unit_index) const;
	virtual void multiplayer_weapon_dropped(int32 weapon_index, int32 unit_index) const;
	virtual void handle_deleted_object(int32 object_index) const;
	virtual int32 get_time_left_in_ticks(int32 time_left, bool during_update, bool allow_overtime) const;
	virtual e_game_team get_defending_team_index() const;
	virtual e_multiplayer_team_designator get_player_team_designator(int32 player_index) const;
	virtual bool team_is_enemy(e_game_team our_team, e_game_team other_team) const;
	virtual void score_header_string(c_static_wchar_string<256>* text) const;
	virtual bool is_medal_allowed(e_game_results_medal medal) const;
	virtual void player_damaged_player(int32 killing_player_index, int32 killing_object_index, int32 dead_player_index, bool friendly_fire) const;
	virtual void player_killed_player(int32 killing_player_index, int32 killing_object_index, int32 dead_player_index, bool friendly_fire, c_flags<e_game_engine_kill_flags, int32, 24> special_kill_flags, int32 consecutive_kill_count) const;
	virtual void player_assisted_with_kill(int32 assisting_player_index) const;
	virtual int32 player_killed_player_get_kill_message(int32 killing_player_index, int32 dead_player_index, bool friendly_fire) const;
	virtual bool test_flag(int32 flag) const;
	virtual void adjust_object_gravity(int32 object_index, real_vector3d* in_out_gravity) const;
	virtual void prepare_for_new_state(int32 new_state) const;
	virtual bool should_end_round(int32* winner_index) const;
	virtual int32 get_player_state_index(int32 player_index, bool* state_is_overridable) const;
	virtual bool should_purge_multiplayer_item(int32 item_index) const;
	virtual void close_any_custom_ui(int32 user_index) const;
	virtual e_simulation_entity_type get_simulation_entity_type() const;
	virtual void promote_to_simulation_authority() const;
	virtual void recover_state_before_promotion() const;
	virtual void build_global_baseline(s_game_engine_state_data* state_data) const;
	virtual void build_global_update(uns32 update_mask, c_static_flags_no_init<64>* actual_update_mask, s_game_engine_state_data* state_data) const;
	virtual bool apply_global_update(uns32 update_mask, const s_game_engine_state_data* state_data) const;
	virtual void build_simulation_baseline(int32 state_data_size, void* state_data) const;
	virtual void build_simulation_update(c_static_flags_no_init<64>* update_mask, int32 state_data_size, void* state_data) const;
	virtual bool apply_simulation_update(c_static_flags_no_init<64>* update_mask, int32 state_data_size, const void* state_data) const;
	virtual void build_player_baseline(int16 player_absolute_index, int32 state_data_size, void* state_data) const;
	virtual void build_player_update(int16 player_absolute_index, c_static_flags_no_init<64>* update_mask, int32 state_data_size, void* state_data) const;
	virtual bool apply_player_update(int16 player_absolute_index, c_static_flags_no_init<64>* update_mask, int32 state_data_size, const void* state_data) const;
	virtual int32 parse_multiplayer_string_token(int32 player_index, const wchar_t* token, int32 token_length, const s_game_engine_event_data* event, wchar_t* destination, int32 available_characters_to_write) const;
	virtual bool handle_incoming_simulation_event(e_simulation_event_type type, const void* data) const;
	virtual bool enable_lead_change_messages() const;
	virtual bool enable_tied_leader_messages() const;
	virtual int32 get_message_chud_reference(s_multiplayer_runtime_globals_definition* runtime_data) const;
	virtual int32 get_message_chud_reference() const;
private:
	virtual void dump_settings_(s_file_reference* file) const;
public:
	virtual void emit_game_start_event(int32) const;

	void dump_player_trait_settings(const char* traits_name, const c_player_traits* traits, s_file_reference* file) const;
	void dump_settings(s_file_reference* file) const;
};

#pragma pack(pop)

