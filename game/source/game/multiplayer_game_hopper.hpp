#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "networking/session/network_session_parameters_matchmaking.hpp"
#include "shell/shell.hpp"
#include "tag_files/files_windows.hpp"

long const k_hopper_maximum_hopper_count = 32;

struct s_game_hopper_picked_game_collection;
struct c_network_session_membership;
struct c_game_variant;
struct c_map_variant;

enum e_hopper_type
{
	_hopper_type_ffa_unranked = 0,
	_hopper_type_ffa_ranked,
	_hopper_type_unranked,
	_hopper_type_ranked,

	k_hopper_type_count
};

struct c_hopper_configuration
{
	c_static_string<k_tag_string_length> hopper_name;
	s_network_http_request_hash game_set_hash;
	word hopper_identifier;
	long hopper_category;
	c_enum<e_hopper_type, long, _hopper_type_ffa_unranked, k_hopper_type_count> hopper_type;
	long image_index;
	long xlast_index;
	byte rich_presence_id;
	byte __data49[0x7];
	s_file_last_modification_date start_time;
	s_file_last_modification_date end_time;
	long hopper_regions;
	long minimum_base_xp;
	long maximum_base_xp;
	long minimum_games_played;
	long maximum_games_played;
	long minimum_party_size;
	long maximum_party_size;
	long hopper_access_bit;
	long account_type_access;
	bool require_all_party_members_meet_games_played_requirements;
	bool require_all_party_members_meet_experience_requirements;
	bool require_all_party_members_meet_access_requirements;
	bool require_all_party_members_meet_live_account_access_requirements;
	bool hide_hopper_from_games_played_restricted_players;
	bool hide_hopper_from_experience_restricted_players;
	bool hide_hopper_from_access_restricted_players;
	bool hide_hopper_from_live_account_access_restricted_players;
	bool hide_hopper_due_to_time_restriction;
	bool requires_all_downloadable_maps;
	bool veto_enabled;
	bool guests_allowed;
	bool require_hosts_on_multiple_teams;
	long stats_write;
	long language_filter;
	long country_code_filter;
	long gamerzone_filter;
	long quitter_filter_percentage;
	long quitter_filter_maximum_party_size;
	long rematch_countdown_timer;
	long rematch_group_formation;
	long repeated_opponents_to_consider_for_penalty;
	long repeated_opponents_experience_threshold;
	long repeated_opponents_skill_throttle_start;
	long repeated_opponents_skill_throttle_stop;
	long maximum_total_matchmaking_seconds;
	long gather_start_game_early_seconds;
	long gather_give_up_seconds;
	byte chance_of_gathering[16];
	long experience_points_per_win;
	long experience_penalty_per_drop;
	real minimum_mu_per_level[49];
	char maximum_skill_level_match_delta[50];
	real trueskill_sigma_multiplier;
	real trueskill_beta_performance_variation;
	real trueskill_tau_dynamics_factor;
	bool trueskill_adjust_tau_with_update_weight;
	long trueskill_draw_probability;
	long trueskill_hillclimb_w0;
	long trueskill_hillclimb_w50;
	long trueskill_hillclimb_w100;
	long trueskill_hillclimb_w150;
	long skill_update_weight_s0;
	long skill_update_weight_s10;
	long skill_update_weight_s20;
	long skill_update_weight_s30;
	long skill_update_weight_s40;
	long skill_update_weight_s50;
	long quality_update_weight_q0;
	long quality_update_weight_q25;
	long quality_update_weight_q50;
	long quality_update_weight_q75;
	long quality_update_weight_q100;
	long pre_match_voice;
	long in_match_voice;
	long post_match_voice;
	bool restrict_open_channel;
	byte __align23D[3];

	union
	{
		// hopper_type == _hopper_type_ffa_unranked 
		// hopper_type == _hopper_type_ffa_ranked
		struct
		{
			// ffa->minimum_player_count >= 2 && ffa->minimum_player_count <= 16
			long minimum_player_count;

			// ffa->maximum_player_count >= 2 && ffa->maximum_player_count <= 16
			long maximum_player_count;
		} ffa;

		// hopper_type == _hopper_type_unranked
		struct
		{
			// unranked_teams->team_count >= 2 && unranked_teams->team_count <= 8
			long team_count;

			// unranked_teams->minimum_team_size >= 1 && unranked_teams->minimum_team_size <= 8
			long minimum_team_size;

			// unranked_teams->maximum_team_size >= 1 && unranked_teams->minimum_team_size <= 8
			long maximum_team_size;

			bool allow_uneven_teams;
			bool allow_parties_to_split;
		} unranked_teams;

		// hopper_type == _hopper_type_ranked
		struct
		{
			// ranked_teams->team_count >= 2 && ranked_teams->team_count <= 8
			long team_count;

			// ranked_teams->minimum_team_size >= 1 && ranked_teams->minimum_team_size <= 8
			long minimum_team_size;

			// ranked_teams->maximum_team_size >= 1 && ranked_teams->maximum_team_size <= 8
			long maximum_team_size;

			long maximum_team_imbalance;

			// ranked_teams->big_squad_size_threshold >= 1 && ranked_teams->big_squad_size_threshold <= 16
			long big_squad_size_threshold;

			// ranked_teams->maximum_big_squad_imbalance >= 0 && ranked_teams->maximum_big_squad_imbalance <= 6
			long maximum_big_squad_imbalance;

			bool enable_big_squad_mixed_skill_restrictions;
		} ranked_teams;

		byte hopper_type_storage[32];
	};
};
static_assert(sizeof(c_hopper_configuration) == 0x260);

struct s_hopper_configuration_table
{
	long hopper_category_count;
	c_static_array<s_game_hopper_custom_category, 4> hopper_category;

	long hopper_configuration_count;
	c_static_array<c_hopper_configuration, k_hopper_maximum_hopper_count> hopper_configurations;
};
static_assert(sizeof(s_hopper_configuration_table) == 0x4C98);

struct s_game_hopper_description
{
	word hopper_identifier;
	long hopper_description_type;
	c_static_string<k_tag_long_string_length> hopper_description;
};
static_assert(sizeof(s_game_hopper_description) == 0x108);

struct s_game_hopper_description_table
{
	long hopper_description_count;
	c_static_array<s_game_hopper_description, 64> hopper_descriptions;
};
static_assert(sizeof(s_game_hopper_description_table) == 0x4204);

struct s_game_set_entry
{
	long game_entry_weight;
	long minimum_player_count;
	bool skip_after_veto;
	bool optional;
	long map_id;

	c_static_string<k_tag_string_length> game_variant_file_name;
	s_network_http_request_hash game_variant_hash;

	c_static_string<k_tag_string_length> map_variant_file_name;
	s_network_http_request_hash map_variant_hash;
};
static_assert(sizeof(s_game_set_entry) == 0x78);

struct s_game_set
{
	long game_entry_count;
	c_static_array<s_game_set_entry, 128> entries;
};
static_assert(sizeof(s_game_set) == 0x3C04);

enum e_hopper_load_status
{
	_hopper_load_none = 0,
	_hopper_load_pending,
	_hopper_load_complete,
	_hopper_load_failed, // is this correct?

	k_hopper_load_count
};

struct multiplayer_file_load
{
	e_hopper_load_status get_load_status();

	bool __unknown0;
	bool __unknown1;
	byte __data2[0x2];
	long request_cookie;
	s_network_http_request_hash hash;
	long __time1C;
	long __time20;
};
static_assert(sizeof(multiplayer_file_load) == 0x24);

extern void __cdecl initialize_fake_hopper(s_hopper_configuration_table* configuration, s_game_hopper_description_table* description, s_game_set* game_set_a, s_game_set* game_set_b);
extern e_hopper_load_status __cdecl multiplayer_game_hopper_catalog_load_status();
extern e_session_game_start_error __cdecl multiplayer_game_hopper_check_required_files(bool check_hopper, bool valid_hopper_identifier);
extern void __cdecl multiplayer_game_hopper_dispose();
extern e_hopper_load_status __cdecl multiplayer_game_hopper_game_set_load_status();
extern e_hopper_load_status __cdecl multiplayer_game_hopper_game_variant_load_status();
extern s_game_hopper_custom_category const* __cdecl multiplayer_game_hopper_get_category_from_index(long category_index);
extern c_game_variant const* __cdecl multiplayer_game_hopper_get_current_game_variant();
extern word __cdecl multiplayer_game_hopper_get_current_hopper_identifier();
extern c_map_variant const* __cdecl multiplayer_game_hopper_get_current_map_variant();
extern utf8 const* __cdecl multiplayer_game_hopper_get_description(word hopper_identifier);
extern word __cdecl multiplayer_game_hopper_get_hopper_identifier(long hopper_index);
extern void __cdecl multiplayer_game_hopper_initialize();
extern bool __cdecl multiplayer_game_hopper_is_hopper_visible(word hopper_identifier, c_network_session_membership const* session_membership);
extern e_hopper_load_status __cdecl multiplayer_game_hopper_map_variant_load_status();
extern long __cdecl multiplayer_game_hopper_pack_game_variant(void* buffer, long buffer_size, c_game_variant const* game_variant);
extern long __cdecl multiplayer_game_hopper_pack_map_variant(void* buffer, long buffer_size, c_map_variant const* map_variant);
extern void __cdecl multiplayer_game_hopper_request_game_variant(word hopper_identifier, char const* variant_name, s_network_http_request_hash const* hash);
extern void __cdecl multiplayer_game_hopper_request_map_variant(word hopper_identifier, char const* variant_name, s_network_http_request_hash const* hash);
extern bool __cdecl multiplayer_game_hopper_unpack_game_set(void const* buffer, long bytes_read, s_game_set* game_set);
extern void __cdecl multiplayer_game_hopper_update_in_matchmaking(bool is_leader);
extern bool __cdecl multiplayer_game_hopper_set_active_hopper_and_request_game_set(word hopper_identifier);
extern bool __cdecl multiplayer_game_hopper_unpack_game_variant(void const* buffer, long bytes_read, c_game_variant* game_variant);
extern bool __cdecl multiplayer_game_hopper_unpack_hopper_description(void const* buffer, long bytes_read, s_game_hopper_description_table* description);
extern bool __cdecl multiplayer_game_hopper_unpack_hopper_file(void const* buffer, long bytes_read, s_hopper_configuration_table* configuration);
extern bool __cdecl multiplayer_game_hopper_unpack_map_variant(void const* buffer, long bytes_read, c_map_variant* map_variant);
extern void __cdecl multiplayer_game_hopper_update();
extern c_hopper_configuration const* __cdecl multiplayer_game_hoppers_get_current_hopper_configuration();
extern c_hopper_configuration* __cdecl multiplayer_game_hoppers_get_hopper_configuration(word hopper_identifier);
extern bool __cdecl multiplayer_game_hoppers_pick_random_game_collection(long player_count, long valid_map_mask, s_game_hopper_picked_game_collection* game_collection_out);
extern e_session_game_start_error __cdecl multiplayer_game_is_playable(word hopper_identifier, bool is_matchmaking, bool check_hopper, c_network_session_membership const* session_membership, word* out_player_error_mask);
extern char const* __cdecl multiplayer_game_start_error_to_string(e_session_game_start_error error);

extern void __cdecl network_build_game_variant(char const* filename);
extern void __cdecl network_load_and_use_game_variant_file(char const* filename);
extern void __cdecl network_verify_game_variant_file(char const* filename);
extern void __cdecl network_load_and_use_packed_game_variant_file(char const* filename);
extern void __cdecl network_verify_packed_game_variant_file(char const* filename);

extern void __cdecl network_build_map_variant(char const* filename);
extern void __cdecl network_load_and_use_map_variant_file(char const* filename);
extern void __cdecl network_verify_map_variant_file(char const* filename);
extern void __cdecl network_load_and_use_packed_map_variant_file(char const* filename);
extern void __cdecl network_verify_packed_map_variant_file(char const* filename);

