#pragma once

#include "memory/data.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files_windows.hpp"

enum
{
	k_max_campaign_insertion_points = 9
};

struct s_campaign_datum :
	s_datum_header
{
	word_flags flags;
	long campaign_id;
	wchar_t name[64];
	wchar_t description[128];
	long map_ids[64];
};
static_assert(sizeof(s_campaign_datum) == 0x288);

enum e_level_flags
{
	k_level_flags
};

struct s_level_datum :
	s_datum_header
{
	//c_flags<e_level_flags, word, k_level_flags> flags;
	word_flags flags;
	long map_id;
	wchar_t name[32];
	wchar_t description[128];
	char scenario_path[256];
	char image_file_base[256];
	long presence_context_id;
	long sort_order;
	char multiplayer_minimum_desired_players;
	char multiplayer_maximum_desired_players;
	char engine_maximum_teams[11];
	bool allows_saved_films;
	bool allows_survival;
	char playable_character;
};
static_assert(sizeof(s_level_datum) == 0x360);

struct s_level_insertion_datum :
	s_datum_header
{
	short next_insertion_index;
	long map_id;
	wchar_t names[k_max_campaign_insertion_points][32];
	wchar_t descriptions[k_max_campaign_insertion_points][128];
	byte zone_sets[k_max_campaign_insertion_points];
	long __unknownB54[k_max_campaign_insertion_points];
	long __unknownB78[k_max_campaign_insertion_points];
	byte_flags __flagsB9C[k_max_campaign_insertion_points];
};
static_assert(sizeof(s_level_insertion_datum) == 0xBA8);

struct s_level_globals
{
	c_smart_data_array<s_campaign_datum> campaigns;
	c_smart_data_array<s_level_datum> campaign_levels;
	c_smart_data_array<s_level_insertion_datum> campaign_insertions;
	c_smart_data_array<s_level_datum> multiplayer_levels;
	s_level_datum mainmenu_level;

	dword enumeration_task;
	s_find_file_data enumeration_task_data;
	c_synchronized_long enumeration_result;
	bool __unknownA14;
	bool __unknownA15;

	bool initialized;
	byte controller_mask;

	// checksum = fast_checksum(header.hash, sizeof(s_network_http_request_hash), fast_checksum_new());
	dword checksum;
};
static_assert(sizeof(s_level_globals) == 0xA1C);

extern s_level_globals& g_level_globals;

struct s_blf_chunk_campaign;
struct s_blf_chunk_scenario;
struct s_configuration_enumeration_task;

extern void __cdecl levels_add_campaign(s_blf_chunk_campaign const* campaign, bool byte_swap, wchar_t const* maps_path, bool is_dlc);
extern void __cdecl levels_add_map_from_scripting(long map_id, char const* scenario_path);
extern void __cdecl levels_add_fake_map_from_scripting(char const* scenario_path);
extern void __cdecl levels_add_multiplayer_map_from_scripting(long map_id, char const* scenario_path);
extern void __cdecl levels_add_fake_multiplayer_map_from_scripting(char const* scenario_path);
extern void __cdecl levels_add_level(s_blf_chunk_scenario const* scenario, bool byte_swap, wchar_t const* maps_path, bool is_dlc);
extern bool __cdecl levels_begin_dvd_enumeration();

extern long __cdecl levels_dvd_enumeration_callback2(void* callback_data);
extern long __cdecl levels_dvd_enumeration_callback(s_configuration_enumeration_task* task_data);

extern long levels_get_default_multiplayer_map_id();
extern long __cdecl levels_get_multiplayer_map_by_display_name(wchar_t const* display_name);
extern char* __cdecl levels_get_path(long campaign_id, long map_id, char* path, long maximum_characters);
extern bool __cdecl levels_map_id_is_fake(long map_id);
extern void __cdecl levels_open_dlc(char const* scenario_path, bool a2);
extern bool __cdecl levels_path_is_dlc(const char* scenario_path);

extern void __cdecl levels_process_campaign_configuration_file(s_file_reference* file, wchar_t const* maps_path, bool is_dlc);
extern void __cdecl levels_process_level_configuration_file(s_file_reference* file, wchar_t const* maps_path, bool is_dlc);
extern bool __cdecl levels_try_and_get_main_menu_map(s_level_datum* level);
extern bool __cdecl levels_try_and_get_multiplayer_map(long map_id, s_level_datum* level);


extern void levels_find_campaign_chunk(s_file_reference* file, char* const file_buffer, s_blf_chunk_campaign const** out_campaign, bool* must_byte_swap);
extern void levels_find_scenario_chunk(s_file_reference* file, char* const file_buffer, s_blf_chunk_scenario const** out_scenario, bool* must_byte_swap);