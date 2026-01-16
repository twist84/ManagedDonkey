#pragma once

#include "memory/data.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files_windows.hpp"

struct s_async_task;
struct s_blf_chunk_campaign;
struct s_blf_chunk_scenario;
union s_configuration_enumeration_task;

struct s_campaign_datum :
	s_datum_header
{
	c_flags<e_campaign_flags, uns16, 2> flags;
	e_campaign_id campaign_id;
	wchar_t name[64];
	wchar_t description[128];
	e_map_id map_ids[64];
};
COMPILE_ASSERT(sizeof(s_campaign_datum) == 0x288);

struct s_level_datum :
	s_datum_header
{
	c_flags<e_level_flags, uns16, k_number_of_level_flags> flags;
	e_map_id map_id;
	wchar_t name[32];
	wchar_t description[128];
	char scenario_file[256];
	char image_file_base[256];
	int32 presence_context_id;
	int32 sort_order;
	int8 minimum_desired_players;
	int8 maximum_desired_players;
	int8 maximum_teams[11];
	bool allows_saved_films;
	bool allows_survival;
	int8 playable_character;
};
COMPILE_ASSERT(sizeof(s_level_datum) == 0x360);

struct s_level_insertion_datum :
	s_datum_header
{
	int16 insertion_point_count;
	e_map_id map_id;
	wchar_t insertion_point_names[k_max_campaign_insertion_points][32];
	wchar_t insertion_point_descriptions[k_max_campaign_insertion_points][128];
	uns8 insertion_point_initial_zone_set[k_max_campaign_insertion_points];

	// odst
	int32 return_from_map_ids[k_max_campaign_insertion_points];
	int32 survival_presence_context_ids[k_max_campaign_insertion_points];
	uns8 __flagsB9C[k_max_campaign_insertion_points];
};
COMPILE_ASSERT(sizeof(s_level_insertion_datum) == 0xBA8);

struct s_level_globals
{
	c_smart_data_array<s_campaign_datum> campaigns;
	c_smart_data_array<s_level_datum> campaign_levels;
	c_smart_data_array<s_level_insertion_datum> campaign_insertions;
	c_smart_data_array<s_level_datum> multiplayer_levels;
	s_level_datum main_menu;
	int32 enumeration_task;
	s_find_file_data find_files_data;
	c_synchronized_long finished;
	bool need_to_enumerate_dvd;
	bool need_to_enumerate_dlc;
	bool initialized;
	uns8 preorder_unlock_controller_mask;
	uns32 checksum;
};
COMPILE_ASSERT(sizeof(s_level_globals) == 0xA1C);

extern s_level_globals& g_level_globals;

extern void __cdecl levels_add_campaign_from_configuration_file(const s_blf_chunk_campaign* campaign_data, bool must_byte_swap, const wchar_t* source_directory_path, bool dlc_content);
extern void __cdecl levels_add_map_from_scripting(e_map_id map_id, const char* scenario_path);
extern void __cdecl levels_add_fake_map_from_scripting(const char* scenario_path);
extern void __cdecl levels_add_multiplayer_map_from_scripting(e_map_id map_id, const char* scenario_path);
extern void __cdecl levels_add_fake_multiplayer_map_from_scripting(const char* scenario_path);
extern void __cdecl levels_add_level_from_configuration_file(const s_blf_chunk_scenario* scenario, bool must_byte_swap, const wchar_t* source_directory_path, bool dlc_content);
extern bool __cdecl levels_begin_dlc_enumeration();
extern bool __cdecl levels_begin_dvd_enumeration();
extern void __cdecl levels_dispose();
extern void __cdecl levels_dispose_from_old_map();
extern e_async_completion __cdecl levels_dlc_enumeration_callback(s_async_task* work);
extern e_async_completion __cdecl levels_dvd_enumeration_callback(s_async_task* work);
extern wchar_t* __cdecl levels_get_active_session_map_name(wchar_t* map_name_buffer, int32 maximum_characters);
extern bool __cdecl levels_enumeration_in_progress();
extern uns32 __cdecl levels_get_available_map_mask();
extern int32 __cdecl levels_get_campaign_count();
extern e_campaign_id __cdecl levels_get_campaign_id_from_path(const char* path);
extern int32 __cdecl levels_get_campaign_level_count(e_campaign_id campaign_id);
extern int16 __cdecl levels_get_campaign_level_index(e_campaign_id campaign_id, e_map_id map_id);
extern e_map_id __cdecl levels_get_campaign_map_by_display_name(wchar_t* display_name);
extern void __cdecl levels_get_campaign_map_ids(e_campaign_id campaign_id, e_map_id* out_map_ids, int32* in_out_count);
extern e_map_id __cdecl levels_get_campaign_next_map_id(e_campaign_id campaign_id, e_map_id map_id);
extern e_map_id __cdecl levels_get_default_multiplayer_map_id();
extern uns32 __cdecl levels_get_checksum();
extern e_map_id __cdecl levels_get_multiplayer_map_by_display_name(const wchar_t* display_name);
extern void __cdecl levels_get_multiplayer_map_ids(e_map_id* out_map_ids, int32* in_out_count);
extern bool __cdecl levels_get_multiplayer_map_is_allowed(e_map_id map_id);
extern bool __cdecl levels_find_path(s_data_array* data, e_map_id map_id, char* path, int32 maximum_characters);
extern char* __cdecl levels_get_path(e_campaign_id campaign_id, e_map_id map_id, char* path, int32 maximum_characters);
extern void __cdecl levels_initialize();
extern void __cdecl levels_initialize_for_new_map();
extern bool __cdecl levels_map_id_is_fake(e_map_id map_id);
extern void __cdecl levels_open_dlc(const char* scenario_path, bool blocking);
extern bool __cdecl levels_path_is_dlc(const char* scenario_path);
extern void __cdecl levels_process_campaign_configuration_file(s_file_reference* file, const wchar_t* source_directory_path, bool dlc_content);
extern void __cdecl levels_process_level_configuration_file(s_file_reference* file, const wchar_t* source_directory_path, bool dlc_content);
extern void __cdecl levels_remove_dlc();
extern bool __cdecl levels_try_and_get_by_map_id(s_data_array* data, e_map_id map_id, s_level_datum* level);
extern bool __cdecl levels_try_and_get_campaign_insertion(e_map_id map_id, s_level_insertion_datum* insertion);
extern bool __cdecl levels_try_and_get_campaign_map(e_map_id map_id, s_level_datum* level);
extern bool __cdecl levels_try_and_get_main_menu_map(s_level_datum* level);
extern bool __cdecl levels_try_and_get_multiplayer_map(e_map_id map_id, s_level_datum* level);
extern void __cdecl levels_update();
extern void __cdecl sub_54CBA0();
extern void __cdecl sub_54CC00();

extern void levels_find_campaign_chunk(s_file_reference* file, char* const file_buffer, const s_blf_chunk_campaign** out_campaign, bool* must_byte_swap);
extern void levels_find_scenario_chunk(s_file_reference* file, char* const file_buffer, const s_blf_chunk_scenario** out_scenario, bool* must_byte_swap);

