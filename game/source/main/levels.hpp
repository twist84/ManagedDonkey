#pragma once

struct s_blf_chunk_campaign;
struct s_blf_chunk_scenario;

struct s_level_datum;

extern void __cdecl levels_add_campaign(s_blf_chunk_campaign const* campaign, bool byte_swap, char const* maps_path, bool is_dlc);
extern void __cdecl levels_add_map_from_scripting(long map_id, char const* scenario_path);
extern void __cdecl levels_add_fake_map_from_scripting(char const* scenario_path);
extern s_level_datum const* __cdecl levels_add_level(s_blf_chunk_scenario const* scenario, bool byte_swap, char const* maps_path, bool is_dlc);
extern bool __cdecl levels_map_id_is_fake(long map_id);