#pragma once

#include "memory/data.hpp"
#include "multithreading/synchronized_value.hpp"
#include "shell/shell.hpp"
#include "tag_files/files_windows.hpp"

enum
{
	k_max_campaign_insertion_points = 9
};

enum e_campaign_flags
{
	_campaign_from_dlc_bit = 0,
	_campaign_unlockable_bit,

	k_number_of_campaign_flags
};

struct s_campaign_datum :
	s_datum_header
{
	c_flags<e_campaign_flags, word, 2> flags;
	long campaign_id;
	wchar_t name[64];
	wchar_t description[128];
	long map_ids[64];
};
static_assert(sizeof(s_campaign_datum) == 0x288);

struct s_level_datum :
	s_datum_header
{
	c_flags<e_level_flags, word, k_number_of_level_flags> flags;
	long map_id;
	wchar_t name[32];
	wchar_t description[128];
	char scenario_file[256];
	char image_file_base[256];
	long presence_context_id;
	long sort_order;
	char minimum_desired_players;
	char maximum_desired_players;
	char maximum_teams[11];
	bool allows_saved_films;
	bool allows_survival;
	char playable_character;
};
static_assert(sizeof(s_level_datum) == 0x360);

struct s_level_insertion_datum :
	s_datum_header
{
	short insertion_point_count;
	long map_id;
	wchar_t insertion_point_names[k_max_campaign_insertion_points][32];
	wchar_t insertion_point_descriptions[k_max_campaign_insertion_points][128];
	byte insertion_point_initial_zone_set[k_max_campaign_insertion_points];

	// odst
	long return_from_map_ids[k_max_campaign_insertion_points];
	long survival_presence_context_ids[k_max_campaign_insertion_points];
	byte_flags __flagsB9C[k_max_campaign_insertion_points];
};
static_assert(sizeof(s_level_insertion_datum) == 0xBA8);

struct s_level_globals
{
	c_smart_data_array<s_campaign_datum> campaigns;
	c_smart_data_array<s_level_datum> campaign_levels;
	c_smart_data_array<s_level_insertion_datum> campaign_insertions;
	c_smart_data_array<s_level_datum> multiplayer_levels;
	s_level_datum main_menu;
	long enumeration_task;
	s_find_file_data find_files_data;
	c_synchronized_long finished;
	bool need_to_enumerate_dvd;
	bool need_to_enumerate_dlc;
	bool initialized;
	byte preorder_unlock_controller_mask;
	dword checksum;
};
static_assert(sizeof(s_level_globals) == 0xA1C);

extern s_level_globals& g_level_globals;

enum e_async_completion;

struct s_blf_chunk_campaign;
struct s_blf_chunk_scenario;
struct s_configuration_enumeration_task;

struct s_async_task;

extern void __cdecl levels_add_campaign_from_configuration_file(s_blf_chunk_campaign const* campaign_data, bool must_byte_swap, wchar_t const* source_directory_path, bool dlc_content);
extern void __cdecl levels_add_map_from_scripting(long map_id, char const* scenario_path);
extern void __cdecl levels_add_fake_map_from_scripting(char const* scenario_path);
extern void __cdecl levels_add_multiplayer_map_from_scripting(long map_id, char const* scenario_path);
extern void __cdecl levels_add_fake_multiplayer_map_from_scripting(char const* scenario_path);
extern void __cdecl levels_add_level_from_configuration_file(s_blf_chunk_scenario const* scenario, bool must_byte_swap, wchar_t const* source_directory_path, bool dlc_content);
extern bool __cdecl levels_begin_dlc_enumeration();
extern bool __cdecl levels_begin_dvd_enumeration();
extern void __cdecl levels_dispose();
extern void __cdecl levels_dispose_from_old_map();
extern e_async_completion __cdecl levels_dlc_enumeration_callback(s_async_task* work);
extern e_async_completion __cdecl levels_dvd_enumeration_callback(s_async_task* work);
extern bool __cdecl levels_enumeration_in_progress();
extern dword __cdecl levels_get_available_map_mask();
extern long __cdecl levels_get_campaign_count();
extern long __cdecl levels_get_campaign_id_from_path(char const* path);
extern long __cdecl levels_get_campaign_level_count(long campaign_id);
extern long __cdecl levels_get_campaign_level_index(long campaign_id, long map_id);
extern long __cdecl levels_get_campaign_map_by_display_name(wchar_t* display_name);
extern void __cdecl levels_get_campaign_map_ids(long campaign_id, long* out_map_ids, long* in_out_count);
extern long __cdecl levels_get_campaign_next_map_id(long campaign_id, long map_id);
extern long __cdecl levels_get_default_multiplayer_map_id();
extern dword __cdecl levels_get_checksum();
extern long __cdecl levels_get_multiplayer_map_by_display_name(wchar_t const* display_name);
extern void __cdecl levels_get_multiplayer_map_ids(long* out_map_ids, long* in_out_count);
extern bool __cdecl levels_get_multiplayer_map_is_allowed(long map_id);
extern bool __cdecl levels_find_path(s_data_array* data, long map_id, char* path, long maximum_characters);
extern char* __cdecl levels_get_path(long campaign_id, long map_id, char* path, long maximum_characters);
extern void __cdecl levels_initialize();
extern void __cdecl levels_initialize_for_new_map();
extern bool __cdecl levels_map_id_is_fake(long map_id);
extern void __cdecl levels_open_dlc(char const* scenario_path, bool blocking);
extern bool __cdecl levels_path_is_dlc(char const* scenario_path);
extern void __cdecl levels_process_campaign_configuration_file(s_file_reference* file, wchar_t const* source_directory_path, bool dlc_content);
extern void __cdecl levels_process_level_configuration_file(s_file_reference* file, wchar_t const* source_directory_path, bool dlc_content);
extern void __cdecl levels_remove_dlc();
extern bool __cdecl levels_try_and_get_by_map_id(s_data_array* data, long map_id, s_level_datum* level);
extern bool __cdecl levels_try_and_get_campaign_insertion(long map_id, s_level_insertion_datum* insertion);
extern bool __cdecl levels_try_and_get_campaign_map(long map_id, s_level_datum* level);
extern bool __cdecl levels_try_and_get_main_menu_map(s_level_datum* level);
extern bool __cdecl levels_try_and_get_multiplayer_map(long map_id, s_level_datum* level);
extern void __cdecl levels_update();
extern void __cdecl sub_54CBA0();
extern void __cdecl sub_54CC00();

extern void levels_find_campaign_chunk(s_file_reference* file, char* const file_buffer, s_blf_chunk_campaign const** out_campaign, bool* must_byte_swap);
extern void levels_find_scenario_chunk(s_file_reference* file, char* const file_buffer, s_blf_chunk_scenario const** out_scenario, bool* must_byte_swap);

