#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

extern bool& g_hf2p_first_run;

extern void __cdecl hf2p_handle_deleted_object(long object_index);
extern void __cdecl hf2p_initialize_for_new_map();
extern void __cdecl hf2p_initialize();
extern void __cdecl hf2p_game_initialize();
extern void __cdecl hf2p_scenario_tags_load_finished();
extern void __cdecl hf2p_scenario_load();
extern void __cdecl hf2p_game_dispose();
extern void __cdecl hf2p_dispose_from_old_map();
extern void __cdecl hf2p_game_update();
extern void __cdecl hf2p_idle();
extern void __cdecl hf2p_render();

struct s_s3d_player_armor_configuration_loadout;
struct s_s3d_player_weapon_configuration_loadout;
extern s_s3d_player_armor_configuration_loadout& get_armor_loadout();
extern s_s3d_player_weapon_configuration_loadout& get_weapon_loadout();
extern c_static_array<c_static_array<c_static_string<64>, 100>, k_armor_type_count>& get_armor_regions(e_player_model_choice player_model_choice = _player_model_choice_spartan, bool* cache_file_has_halo3_armors = nullptr);

