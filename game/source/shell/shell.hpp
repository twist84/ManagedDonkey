#pragma once

enum e_game_mode
{
	_game_mode_none = 0,
	_game_mode_campaign,
	_game_mode_multiplayer,
	_game_mode_mainmenu,
	_game_mode_shared,

	k_game_mode_count
};

enum e_campaign_difficulty_level
{
	_campaign_difficulty_level_easy = 0,
	_campaign_difficulty_level_normal,
	_campaign_difficulty_level_heroic,
	_campaign_difficulty_level_legendary,

	k_campaign_difficulty_levels_count
};

// e_game_skulls
enum e_campaign_skulls_primary
{
	_campaign_skull_iron = 0,
	_campaign_skull_black_eye,
	_campaign_skull_tough_luck,
	_campaign_skull_catch,
	_campaign_skull_fog,
	_campaign_skull_famine,
	_campaign_skull_thunderstorm,
	_campaign_skull_tilt,
	_campaign_skull_mythic,

	k_campaign_skull_primary_count
};

enum e_campaign_skulls_secondary
{
	_campaign_skull_assassin = 0,
	_campaign_skull_blind,
	_campaign_skull_superman,
	_campaign_skull_birthday_party,
	_campaign_skull_daddy,
	_campaign_skull_third_person,
	_campaign_skull_directors_cut,

	k_campaign_skull_secondary_count
};

// TODO: find a home
enum e_game_simulation_type
{
	_game_simulation_none = 0,
	_game_simulation_local,
	_game_simulation_synchronous_client,
	_game_simulation_synchronous_server,
	_game_simulation_distributed_client,
	_game_simulation_distributed_server,

	k_game_simulation_count
};

// TODO: find a home
enum e_game_playback_type
{
	_game_playback_none = 0,
	_game_playback_local,
	_game_playback_network_server,
	_game_playback_network_client,

	k_game_playback_count
};

enum e_content_item_type
{

};

enum e_player_model_choice
{

};

enum e_controller_index
{
	_controller_index0 = 0,
	_controller_index1,
	_controller_index2,
	_controller_index3,

	k_number_of_controllers,

	k_any_controller = 0xFF,
	k_no_controller = 0xFFFFFFFF,
};

enum e_window_index
{
	_window_index0 = 0,
	_window_index1,
	_window_index2,
	_window_index3,
	_window_index4,

	k_number_of_windows
};
