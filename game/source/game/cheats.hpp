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
extern void __cdecl cheat_active_camouflage_by_user(int32 player_index, bool enable);
extern void __cdecl cheat_all_chars();
extern void __cdecl cheat_all_powerups();
extern void __cdecl cheat_all_vehicles();
extern void __cdecl cheat_all_weapons();
extern bool __cdecl cheat_drop_effect(tag group_tag, const char* effect_name, int32 effect_index, real_point3d* position, real_vector3d* forward);
extern bool __cdecl cheat_drop_object(tag group_tag, const char* tag_name, tag expected_group_tag, int32 object_definition_index, int32 variant_name, int32 shader, const real_point3d* position, const real_vector3d* forward, const s_model_customization_region_permutation* permutations, int32 permutation_count);
extern int32 __cdecl cheat_drop_tag(tag group_tag, const char* tag_name, const char* variant_name, const s_model_customization_region_permutation* permutations, int32 permutation_count);
extern void __cdecl cheat_drop_tag_in_main_event_loop(int32 tag_index, int32 variant_name, const s_model_customization_region_permutation* permutations, int32 permutation_count);
extern void __cdecl cheat_drop_tag_name(const char* tag_name);
extern void __cdecl cheat_drop_tag_name_with_permutation_hs(const char* tag_name, const char* permutation_info);
extern void __cdecl cheat_drop_tag_name_with_variant_and_permutations(const char* tag_name, const char* variant_name, const s_model_customization_region_permutation* permutations, int32 permutation_count);
extern void __cdecl cheat_drop_tag_name_with_variant_hs(const char* tag_name, const char* variant_name);
extern void __cdecl cheat_drop_tag_safe(int32 tag_index);
extern void __cdecl cheat_get_droppable_tag_types(tag* const out_droppable_tag_types, int32* out_droppable_tag_type_count);
extern int32 __cdecl cheat_get_region_and_permutation_array_from_string(const char* permutation_info, s_model_customization_region_permutation* permutations, int32 maximum_permutations);
extern int32 __cdecl cheat_get_tag_definition(tag group_tag, const char* tag_name);
extern void __cdecl cheat_objects(s_tag_reference* references, int16 reference_count);
extern int32 __cdecl cheat_player_index();
extern void __cdecl cheat_spawn_warthog();
extern void __cdecl cheat_teleport_to_camera();
extern void __cdecl cheats_dispose();
extern void __cdecl cheats_dispose_from_old_map();
extern void __cdecl cheats_initialize();
extern void __cdecl cheats_initialize_for_new_map();
extern void __cdecl cheats_load();
extern bool __cdecl cheats_process_gamepad(int32 controller_index, const s_game_input_state* input_state);

