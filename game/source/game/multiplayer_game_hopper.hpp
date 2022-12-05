#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "shell/shell.hpp"
#include "tag_files/files_windows.hpp"

struct s_game_hopper_picked_game_collection;
struct c_network_session_membership;

struct c_hopper_configuration
{
	c_static_string<32> hopper_name;
	s_network_http_request_hash game_set_hash;
	word hopper_identifier;
	long hopper_category;
	long hopper_type;
	long image_index;
	long xlast_index;
	byte rich_presence_id[8];
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
	bool require_all_party_members_meet_base_xp_requirements;
	bool require_all_party_members_meet_access_requirements;
	bool require_all_party_members_meet_live_account_access_requirements;
	bool hide_hopper_from_games_played_restricted_players;
	bool hide_hopper_from_xp_restricted_players;
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
	long minimum_mu_per_level[49];
	char maximum_skill_level_match_delta[50];
	long trueskill_sigma_multiplier;
	long trueskill_beta_performance_variation;
	long trueskill_tau_dynamics_factor;
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
	byte __data[32];
};
static_assert(sizeof(c_hopper_configuration) == 0x260);

extern void __cdecl multiplayer_game_hopper_update();
extern c_hopper_configuration* __cdecl multiplayer_game_hoppers_get_current_hopper_configuration();
extern c_hopper_configuration* __cdecl multiplayer_game_hoppers_get_hopper_configuration(word hopper_identifier);
extern bool __cdecl multiplayer_game_hoppers_pick_random_game_collection(long player_count, long valid_map_mask, s_game_hopper_picked_game_collection* game_collection_out);
extern e_session_game_start_error __cdecl multiplayer_game_is_playable(word hopper_identifier, bool is_matchmaking, bool check_hopper, c_network_session_membership const* session_membership, word* out_player_error_mask);
extern e_session_game_start_error __cdecl multiplayer_game_hopper_check_required_files(bool check_hopper, bool valid_hopper_identifier);

