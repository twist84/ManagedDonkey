#pragma once

#include "cseries/cseries.hpp"
#include "networking/session/network_session_parameters_generic.hpp"
#include "networking/logic/life_cycle/life_cycle_matchmaking_quality.hpp"
#include "networking/transport/transport_security.hpp"
#include "shell/shell.hpp"

struct s_network_session_matchmaking_hopper_category
{
	word category_identifier;
	short category_image_index;
	c_static_string<32> category_name;
};
static_assert(sizeof(s_network_session_matchmaking_hopper_category) == 0x24);

struct s_network_session_matchmaking_hopper_entry
{
	word hopper_identifier;
	word category_identifier;
	word presence_identifier;
	long hopper_image_index;
	bool is_hopper_visible;
	bool is_hopper_playable;
	bool is_hopper_ranked;
	bool has_xp_penalty;
	c_static_string<32> hopper_name;
};
static_assert(sizeof(s_network_session_matchmaking_hopper_entry) == 0x30);

struct s_network_session_matchmaking_hopper_list
{
	long hopper_load_status;
	long session_membership_version;
	dword visible_category_mask;
	long category_count;
	s_network_session_matchmaking_hopper_category categories[4];
	long hopper_count;
	s_network_session_matchmaking_hopper_entry hoppers[32];
};
static_assert(sizeof(s_network_session_matchmaking_hopper_list) == 0x6A4);

struct c_network_session_parameter_matchmaking_hopper_list :
	public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<s_network_session_matchmaking_hopper_list>
{
};
static_assert(sizeof(c_network_session_parameter_matchmaking_hopper_list) == 0xD78);

struct s_network_session_matchmaking_hopper
{
	word hopper_identifier;
	char xlast_index;
	bool is_ranked;
	bool veto_allowed;
	bool rematch_allowed;
	bool team_game;
	long draw_probability;
	long beta;
	long tau;
	long experience_base_increment;
	long experience_penalty_decrement;
	long require_hosts_on_multiple_teams;
	long selection_time;
	long max_party_size;
	long rematch_timer;
	long repeated_opponents_to_consider_for_penalty;
	long repeated_opponents_experience_threshold;
	long repeated_opponents_skill_throttle_start;
	long repeated_opponents_skill_throttle_stop;
	long pre_match_voice;
	long in_match_voice;
	long post_match_voice;
	bool restrict_open_channel;
};
static_assert(sizeof(s_network_session_matchmaking_hopper) == 0x4C);

struct c_network_session_parameter_matchmaking_hopper :
	public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<s_network_session_matchmaking_hopper>
{
};
static_assert(sizeof(c_network_session_parameter_matchmaking_hopper) == 0xC8);

struct s_replicated_life_cycle_matchmaking_progress_searching_for_match
{
	long search_stage;
	long searching_skill_min;
	long searching_skill_max;
	long contacted_session_count;
	long potential_session_count;
	long search_queries_submitted;
};
static_assert(sizeof(s_replicated_life_cycle_matchmaking_progress_searching_for_match) == 0x18);

struct s_replicated_life_cycle_matchmaking_progress_assembling_match
{
	long give_up_timer_seconds;
	long give_up_timer_maximum_seconds;
	long qos_listener_data_replies_sent;
};
static_assert(sizeof(s_replicated_life_cycle_matchmaking_progress_assembling_match) == 0xC);

struct s_replicated_life_cycle_matchmaking_progress_configuring_match
{
	long state;
	long map_load_progress_percentage;
	long countdown_timer;
	long current_veto_count;
	long max_veto_count;
	long game_map_index;
	bool load_failure_encountered;
};
static_assert(sizeof(s_replicated_life_cycle_matchmaking_progress_configuring_match) == 0x1C);

struct s_replicated_life_cycle_progress_post_match
{
	bool countdown_running;
	long countdown;
};
static_assert(sizeof(s_replicated_life_cycle_progress_post_match) == 0x8);

struct s_replicated_life_cycle_matchmaking_progress
{
	c_enum<e_life_cycle_matchmaking_progress_type, long, _life_cycle_matchmaking_progress_type_unknown0, k_life_cycle_matchmaking_progress_type_count> progress_type;

	union
	{
		s_replicated_life_cycle_matchmaking_progress_searching_for_match searching_for_match;
		s_replicated_life_cycle_matchmaking_progress_assembling_match assembling_match;
		s_replicated_life_cycle_matchmaking_progress_configuring_match configuring_match;
		s_replicated_life_cycle_progress_post_match post_match;
	};
};
static_assert(sizeof(s_replicated_life_cycle_matchmaking_progress) == 0x20);

struct s_replicated_life_cycle_matchmaking_progress_search_criteria
{
	long party_skill_range_min;
	long party_skill_range_max;
	long searching_skill_range_min;
	long searching_skill_range_max;
	long searching_party_size_min;
	long searching_party_size_max;
	bool search_party_big_and_mixed;
	long hopper_required_player_count;
	long hopper_preferred_player_count;
};
static_assert(sizeof(s_replicated_life_cycle_matchmaking_progress_search_criteria) == 0x24);

struct s_network_session_parameter_matchmaking_rematch_team_data
{
	long host_peer_index;
	dword rematch_peer_mask;
	bool join_data_valid;
	s_transport_session_description host_description;
};
static_assert(sizeof(s_network_session_parameter_matchmaking_rematch_team_data) == 0x3C);

struct s_network_session_parameter_matchmaking_rematch_data
{
	dword team_rematch_mask;
	c_static_array<s_network_session_parameter_matchmaking_rematch_team_data, 16> teams;
};
static_assert(sizeof(s_network_session_parameter_matchmaking_rematch_data) == 0x3C4);

struct c_network_session_parameter_matchmaking_rematch_data :
	public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<s_network_session_parameter_matchmaking_rematch_data>
{
};
static_assert(sizeof(c_network_session_parameter_matchmaking_rematch_data) == 0x7B8);

struct s_matchmaking_single_hopper_statistics
{
	long total_online_population;
	long current_hopper_population;
};
static_assert(sizeof(s_matchmaking_single_hopper_statistics) == 0x8);

struct c_network_session_parameter_matchmaking_hopper_statistics :
	public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<s_matchmaking_single_hopper_statistics>
{
};
static_assert(sizeof(c_network_session_parameter_matchmaking_hopper_statistics) == 0x40);

struct s_network_session_parameter_matchmaking_messaging
{
	long type;
	long display_cookie;
	long player_sequence_number;
};
static_assert(sizeof(s_network_session_parameter_matchmaking_messaging) == 0xC);

struct c_network_session_parameter_matchmaking_messaging :
	public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<s_network_session_parameter_matchmaking_messaging>
{
};
static_assert(sizeof(c_network_session_parameter_matchmaking_messaging) == 0x48);
