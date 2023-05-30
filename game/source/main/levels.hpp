#pragma once

struct s_level_datum
{
	unsigned short flags;
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

struct s_blf_chunk_campaign;
struct s_blf_chunk_scenario;

struct s_file_reference;

struct s_find_file_data;
struct s_levels_dvd_enumeration_callback_data
{
	long enumeration_index;
	s_find_file_data* find_file_data;
};

extern void __cdecl levels_add_campaign(s_blf_chunk_campaign const* campaign, bool byte_swap, wchar_t const* maps_path, bool is_dlc);
extern void __cdecl levels_add_map_from_scripting(long map_id, char const* scenario_path);
extern void __cdecl levels_add_fake_map_from_scripting(char const* scenario_path);
extern void __cdecl levels_add_level(s_blf_chunk_scenario const* scenario, bool byte_swap, wchar_t const* maps_path, bool is_dlc);
extern bool __cdecl levels_begin_dvd_enumeration();

extern long __cdecl levels_dvd_enumeration_callback2(void* callback_data);
extern long __cdecl levels_dvd_enumeration_callback(s_levels_dvd_enumeration_callback_data* callback_data);

extern long levels_get_default_multiplayer_map_id();
extern long __cdecl levels_get_multiplayer_map_by_display_name(wchar_t const* display_name);
extern char* __cdecl levels_get_path(long campaign_id, long map_id, char* path, long maximum_characters);
extern bool __cdecl levels_map_id_is_fake(long map_id);

extern void __cdecl levels_process_campaign_configuration_file(s_file_reference* file, wchar_t const* maps_path, bool is_dlc);
extern void __cdecl levels_process_level_configuration_file(s_file_reference* file, wchar_t const* maps_path, bool is_dlc);
extern bool __cdecl levels_try_and_get_multiplayer_map(long map_id, s_level_datum* level);


extern void levels_find_campaign_chunk(s_file_reference* file, char* const file_buffer, s_blf_chunk_campaign const** out_campaign, bool* must_byte_swap);
extern void levels_find_scenario_chunk(s_file_reference* file, char* const file_buffer, s_blf_chunk_scenario const** out_scenario, bool* must_byte_swap);