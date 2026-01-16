#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "networking/session/network_session_parameters_matchmaking.hpp"
#include "tag_files/files_windows.hpp"

int32 const k_hopper_maximum_hopper_count = 32;

struct s_game_hopper_picked_game_collection;
class c_network_session_membership;
class c_game_variant;
class c_map_variant;

struct s_hopper_universal_configuration
{
	c_static_string<k_tag_string_length> hopper_name;
	s_network_http_request_hash game_set_hash;
	uns16 hopper_identifier;
	int32 hopper_category;
	c_enum<e_hopper_type, int32, _hopper_type_unranked_ffa, k_hopper_type_count> hopper_type;
	int32 image_index;
	int32 xlast_index;
	uns8 rich_presence_id;
	byte __data49[0x7];
	s_file_last_modification_date start_time;
	s_file_last_modification_date end_time;
	int32 hopper_regions;
	int32 minimum_base_xp;
	int32 maximum_base_xp;
	int32 minimum_games_played;
	int32 maximum_games_played;
	int32 minimum_party_size;
	int32 maximum_party_size;
	int32 hopper_access_bit;
	int32 account_type_access;
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
	int32 stats_write;
	int32 language_filter;
	int32 country_code_filter;
	int32 gamerzone_filter;
	int32 quitter_filter_percentage;
	int32 quitter_filter_maximum_party_size;
	int32 rematch_countdown_timer;
	int32 rematch_group_formation;
	int32 repeated_opponents_to_consider_for_penalty;
	int32 repeated_opponents_experience_threshold;
	int32 repeated_opponents_skill_throttle_start;
	int32 repeated_opponents_skill_throttle_stop;
	int32 maximum_total_matchmaking_seconds;
	int32 gather_start_game_early_seconds;
	int32 gather_give_up_seconds;
	uns8 chance_of_gathering[16];
	int32 experience_points_per_win;
	int32 experience_penalty_per_drop;
	real32 minimum_mu_per_level[49];
	int8 maximum_skill_level_match_delta[50];
	real32 trueskill_sigma_multiplier;
	real32 trueskill_beta_performance_variation;
	real32 trueskill_tau_dynamics_factor;
	bool trueskill_adjust_tau_with_update_weight;
	int32 trueskill_draw_probability;
	int32 trueskill_hillclimb_w0;
	int32 trueskill_hillclimb_w50;
	int32 trueskill_hillclimb_w100;
	int32 trueskill_hillclimb_w150;
	int32 skill_update_weight_s0;
	int32 skill_update_weight_s10;
	int32 skill_update_weight_s20;
	int32 skill_update_weight_s30;
	int32 skill_update_weight_s40;
	int32 skill_update_weight_s50;
	int32 quality_update_weight_q0;
	int32 quality_update_weight_q25;
	int32 quality_update_weight_q50;
	int32 quality_update_weight_q75;
	int32 quality_update_weight_q100;
	int32 pre_match_voice_configuration;
	int32 in_match_voice_configuration;
	int32 post_match_voice_configuration;
	bool restrict_open_channel;
};
COMPILE_ASSERT(sizeof(s_hopper_universal_configuration) == 0x240);

struct s_ffa_hopper_configuration
{
	int32 minimum_player_count;
	int32 maximum_player_count;
};
COMPILE_ASSERT(sizeof(s_ffa_hopper_configuration) == 0x8);

struct s_ranked_teams_hopper_configuration
{
	int32 team_count;
	int32 minimum_team_size;
	int32 maximum_team_size;
	int32 maximum_team_imbalance;
	int32 big_squad_size_threshold;
	int32 maximum_big_squad_imbalance;
	bool enable_big_squad_mixed_skill_restrictions;
};
COMPILE_ASSERT(sizeof(s_ranked_teams_hopper_configuration) == 0x1C);

struct s_unranked_teams_hopper_configuration
{
	int32 team_count;
	int32 minimum_team_size;
	int32 maximum_team_size;
	bool allow_uneven_teams;
	bool allow_parties_to_split;
};
COMPILE_ASSERT(sizeof(s_unranked_teams_hopper_configuration) == 0x10);

struct s_hopper_type_configuration
{
	union
	{
		s_ffa_hopper_configuration ffa;
		s_ranked_teams_hopper_configuration ranked_teams;
		s_unranked_teams_hopper_configuration unranked_teams;

		byte storage[0x20];
	};
};

class c_hopper_configuration
{
public:
	s_hopper_universal_configuration m_universal;
	s_hopper_type_configuration m_type;
};
COMPILE_ASSERT(sizeof(c_hopper_configuration) == 0x260);

struct s_hopper_configuration_table
{
	int32 hopper_category_count;
	c_static_array<s_game_hopper_custom_category, 4> hopper_category;

	int32 hopper_configuration_count;
	c_static_array<c_hopper_configuration, k_hopper_maximum_hopper_count> hopper_configurations;
};
COMPILE_ASSERT(sizeof(s_hopper_configuration_table) == 0x4C98);

struct s_game_hopper_description
{
	uns16 hopper_identifier;
	int32 hopper_description_type;
	c_static_string<k_tag_long_string_length> hopper_description;
};
COMPILE_ASSERT(sizeof(s_game_hopper_description) == 0x108);

struct s_game_hopper_description_table
{
	int32 hopper_description_count;
	c_static_array<s_game_hopper_description, 64> hopper_descriptions;
};
COMPILE_ASSERT(sizeof(s_game_hopper_description_table) == 0x4204);

struct s_game_set_entry
{
	int32 game_entry_weight;
	int32 minimum_player_count;
	bool skip_after_veto;
	bool optional;
	e_map_id map_id;

	c_static_string<k_tag_string_length> game_variant_file_name;
	s_network_http_request_hash game_variant_hash;

	c_static_string<k_tag_string_length> map_variant_file_name;
	s_network_http_request_hash map_variant_hash;
};
COMPILE_ASSERT(sizeof(s_game_set_entry) == 0x78);

struct s_game_set
{
	int32 game_entry_count;
	c_static_array<s_game_set_entry, 128> entries;
};
COMPILE_ASSERT(sizeof(s_game_set) == 0x3C04);

struct multiplayer_file_load
{
	e_hopper_load_status get_load_status();

	bool is_valid;
	bool had_load_failure;
	int32 request_cookie;
	s_network_http_request_hash configuration_hash;
	uns32 time_of_last_load;
	uns32 retry_interval;
};
COMPILE_ASSERT(sizeof(multiplayer_file_load) == 0x24);

struct multiplayer_hopper_check
{
	uns32 player_mask;
	uns32 guest_allowed_mask;
	uns32 player_stats_pending_mask;
	uns32 ban_mask;
	uns32 hopper_id_mask;
	e_session_game_start_error game_start_error;
	uns32 required_region_mask;
	uns32 language_filter_mask;
	uns32 country_code_filter_mask;
	uns32 gamerzone_filter_mask;
	uns32 games_played_too_low_mask;
	uns32 games_played_too_high_mask;
	uns32 experience_too_low;
	uns32 experience_too_high;
	uns32 access_mask;
	uns32 paid_hopper_mask;
	uns32 required_maps_mask;
};
COMPILE_ASSERT(sizeof(multiplayer_hopper_check) == 0x44);

extern void __cdecl initialize_fake_hopper(s_hopper_configuration_table* configuration, s_game_hopper_description_table* description, s_game_set* game_set_a, s_game_set* game_set_b);
extern e_hopper_load_status __cdecl multiplayer_game_hopper_catalog_load_status();
extern e_session_game_start_error __cdecl multiplayer_game_hopper_check_required_files(bool check_hopper, bool valid_hopper_identifier);
extern void __cdecl multiplayer_game_hopper_dispose();
extern e_hopper_load_status __cdecl multiplayer_game_hopper_game_set_load_status();
extern e_hopper_load_status __cdecl multiplayer_game_hopper_game_variant_load_status();
extern const s_game_hopper_custom_category* __cdecl multiplayer_game_hopper_get_category_from_index(int32 category_index);
extern const c_game_variant* __cdecl multiplayer_game_hopper_get_current_game_variant();
extern uns16 __cdecl multiplayer_game_hopper_get_current_hopper_identifier();
extern const c_map_variant* __cdecl multiplayer_game_hopper_get_current_map_variant();
extern const utf8* __cdecl multiplayer_game_hopper_get_description(uns16 hopper_identifier);
extern uns16 __cdecl multiplayer_game_hopper_get_hopper_identifier(int32 hopper_index);
extern void __cdecl multiplayer_game_hopper_get_players_status(const c_hopper_configuration* hopper, const c_network_session_membership* session_membership, multiplayer_hopper_check* check);
extern void __cdecl multiplayer_game_hopper_initialize();
extern bool __cdecl multiplayer_game_hopper_is_hopper_visible(uns16 hopper_identifier, const c_network_session_membership* session_membership);
extern e_hopper_load_status __cdecl multiplayer_game_hopper_map_variant_load_status();
extern int32 __cdecl multiplayer_game_hopper_pack_game_variant(void* buffer, int32 buffer_size, const c_game_variant* game_variant);
extern int32 __cdecl multiplayer_game_hopper_pack_map_variant(void* buffer, int32 buffer_size, const c_map_variant* map_variant);
extern void __cdecl multiplayer_game_hopper_request_game_variant(uns16 hopper_identifier, const char* variant_name, const s_network_http_request_hash* hash);
extern void __cdecl multiplayer_game_hopper_request_map_variant(uns16 hopper_identifier, const char* variant_name, const s_network_http_request_hash* hash);
extern bool __cdecl multiplayer_game_hopper_set_active_hopper_and_request_game_set(uns16 hopper_identifier);
extern void __cdecl multiplayer_game_hopper_set_game_for_current_hopper(int32 game_index);
extern bool __cdecl multiplayer_game_hopper_unpack_game_set(const void* buffer, int32 bytes_read, s_game_set* game_set);
extern bool __cdecl multiplayer_game_hopper_unpack_game_variant(const void* buffer, int32 bytes_read, c_game_variant* game_variant);
extern bool __cdecl multiplayer_game_hopper_unpack_hopper_description(const void* buffer, int32 bytes_read, s_game_hopper_description_table* description);
extern bool __cdecl multiplayer_game_hopper_unpack_hopper_file(const void* buffer, int32 bytes_read, s_hopper_configuration_table* configuration);
extern bool __cdecl multiplayer_game_hopper_unpack_map_variant(const void* buffer, int32 bytes_read, c_map_variant* map_variant);
extern void __cdecl multiplayer_game_hopper_update();
extern void __cdecl multiplayer_game_hopper_update_in_matchmaking(bool is_leader);
extern const c_hopper_configuration* __cdecl multiplayer_game_hoppers_get_current_hopper_configuration();
extern c_hopper_configuration* __cdecl multiplayer_game_hoppers_get_hopper_configuration(uns16 hopper_identifier);
extern bool __cdecl multiplayer_game_hoppers_pick_random_game_collection(int32 player_count, int32 valid_map_mask, s_game_hopper_picked_game_collection* game_collection_out);
extern e_session_game_start_error __cdecl multiplayer_game_is_playable(uns16 hopper_identifier, bool is_matchmaking, bool check_hopper, const c_network_session_membership* session_membership, uns16* out_player_error_mask);
extern const char* __cdecl multiplayer_game_start_error_to_string(e_session_game_start_error error);

extern void __cdecl network_build_game_variant(const char* file_path);
extern void __cdecl network_load_and_use_game_variant_file(const char* file_path);
extern void __cdecl network_verify_game_variant_file(const char* file_path);
extern void __cdecl network_load_and_use_packed_game_variant_file(const char* file_path);
extern void __cdecl network_verify_packed_game_variant_file(const char* file_path);

extern void __cdecl network_build_map_variant(const char* file_path);
extern void __cdecl network_load_and_use_map_variant_file(const char* file_path);
extern void __cdecl network_verify_map_variant_file(const char* file_path);
extern void __cdecl network_load_and_use_packed_map_variant_file(const char* file_path);
extern void __cdecl network_verify_packed_map_variant_file(const char* file_path);

