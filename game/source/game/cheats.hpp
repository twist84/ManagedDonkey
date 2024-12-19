#pragma once

#include "cseries/cseries.hpp"
#include "input/input_windows.hpp"
#include "objects/objects.hpp"

struct cheat_globals
{
	bool deathless_player;
	bool valhalla;
	bool jetpack;
	bool infinite_ammo;
	bool bump_possession;
	bool super_jump;
	bool reflexive_damage_effects;
	bool medusa;
	bool omnipotent;
	bool controller_enabled;
	bool bottomless_clip;
	bool chevy;
};
static_assert(sizeof(cheat_globals) == 0xC);

extern cheat_globals cheat;
extern char cheat_strings[k_controller_button_count][200];

struct s_game_input_state;
struct s_tag_reference;

extern void __cdecl cheat_active_camouflage(bool enable);
extern void __cdecl cheat_active_camouflage_by_player(long player_index, bool enable);
extern void __cdecl cheat_all_chars();
extern void __cdecl cheat_all_powerups();
extern void __cdecl cheat_all_vehicles();
extern void __cdecl cheat_all_weapons();
extern bool __cdecl cheat_drop_effect(tag group_tag, char const* effect_name, long effect_index, real_point3d* position, real_vector3d* forward);
extern bool __cdecl cheat_drop_object(tag group_tag, char const* tag_name, tag expected_group_tag, long object_definition_index, long variant_name, long shader, real_point3d const* position, real_vector3d const* forward, s_model_customization_region_permutation const* permutations, long permutation_count);
extern long __cdecl cheat_drop_tag(tag group_tag, char const* tag_name, char const* variant_name, s_model_customization_region_permutation const* permutations, long permutation_count);
extern void __cdecl cheat_drop_tag_in_main_event_loop(long tag_index, long variant_name, s_model_customization_region_permutation const* permutations, long permutation_count);
extern void __cdecl cheat_drop_tag_name(char const* tag_name);
extern void __cdecl cheat_drop_tag_name_with_permutation_hs(char const* tag_name, char const* permutation_info);
extern void __cdecl cheat_drop_tag_name_with_variant_and_permutations(char const* tag_name, char const* variant_name, s_model_customization_region_permutation const* permutations, long permutation_count);
extern void __cdecl cheat_drop_tag_name_with_variant_hs(char const* tag_name, char const* variant_name);
extern void __cdecl cheat_drop_tag_safe_hs(long tag_index);
extern void __cdecl cheat_get_droppable_tag_types(tag* const out_droppable_tag_types, long* out_droppable_tag_type_count);
extern long __cdecl cheat_get_region_and_permutation_array_from_string(char const* permutation_info, s_model_customization_region_permutation* permutations, long maximum_permutations);
extern long __cdecl cheat_get_tag_definition(tag group_tag, char const* tag_name);
extern bool __cdecl cheat_get_teleport_to_camera_information(long* unit_index, real_point3d* position);
extern void __cdecl cheat_objects(s_tag_reference* references, short reference_count);
extern long __cdecl cheat_player_index();
extern void __cdecl cheat_spawn_warthog();
extern void __cdecl cheat_teleport_to_camera();
extern void __cdecl cheats_dispose();
extern void __cdecl cheats_dispose_from_old_map();
extern void __cdecl cheats_initialize();
extern void __cdecl cheats_initialize_for_new_map();
extern void __cdecl cheats_load();
extern bool __cdecl cheats_process_gamepad(long controller_index, s_game_input_state const* input_state);

