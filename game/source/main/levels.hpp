#pragma once

struct s_blf_chunk_campaign;
struct s_blf_chunk_scenario;

struct s_level_datum;

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

extern long __cdecl levels_dvd_enumeration_callback2(void* userdata);
extern long __cdecl levels_dvd_enumeration_callback(s_levels_dvd_enumeration_callback_data* userdata);

extern bool __cdecl levels_map_id_is_fake(long map_id);

extern void __cdecl levels_process_level_configuration_file(s_file_reference* file, wchar_t const* maps_path, bool unused);extern void levels_find_campaign_chunk(s_file_reference* file, char* const file_buffer, s_blf_chunk_campaign const** out_campaign, bool* must_byte_swap);

extern void levels_find_campaign_chunk(s_file_reference* file, char* const file_buffer, s_blf_chunk_campaign const** out_campaign, bool* must_byte_swap);
extern void levels_find_scenario_chunk(s_file_reference* file, char* const file_buffer, s_blf_chunk_scenario const** out_scenario, bool* must_byte_swap);