#include "sound/sound_classes.hpp"

void __cdecl sound_classes_dispose()
{
	INVOKE(0x00661290, sound_classes_dispose);
}

void __cdecl sound_classes_dispose_from_old_map()
{
	INVOKE(0x006612B0, sound_classes_dispose_from_old_map);
}

void __cdecl sound_classes_initialize()
{
	INVOKE(0x006612C0, sound_classes_initialize);
}

void __cdecl sound_classes_initialize_for_new_map()
{
	INVOKE(0x00661330, sound_classes_initialize_for_new_map);
}

char const* const sound_class_names[k_sound_class_count]
{
	"projectile_impact",
	"projectile_detonation",
	"projectile_flyby",
	"projectile_detonation_lod",
	"weapon_fire",
	"weapon_ready",
	"weapon_reload",
	"weapon_empty",
	"weapon_charge",
	"weapon_overheat",
	"weapon_idle",
	"weapon_melee",
	"weapon_animation",
	"object_impacts",
	"particle_impacts",
	"weapon_fire_lod",
	"weapon_fire_lod_far",
	"unused2_impacts!",
	"unit_footsteps",
	"unit_dialog",
	"unit_animation",
	"unit_unused!",
	"vehicle_collision",
	"vehicle_engine",
	"vehicle_animation",
	"vehicle_engine_lod",
	"device_door",
	"device_unused0!",
	"device_machinery",
	"device_stationary",
	"device_unused1!",
	"device_unused2!",
	"music",
	"ambient_nature",
	"ambient_machinery",
	"ambient_stationary",
	"huge_ass",
	"object_looping",
	"cinematic_music",
	"player_armor",
	"unknown_unused1!",
	"ambient_flock",
	"no_pad",
	"no_pad_stationary",
	"arg",
	"cortana_mission",
	"cortana_gravemind_channel",
	"mission_dialog",
	"cinematic_dialog",
	"scripted_cinematic_foley",
	"hud",
	"game_event",
	"ui",
	"test",
	"multilingual_test",
	"ambient_nature_details",
	"ambient_machinery_details",
	"inside_surround_tail",
	"outside_surround_tail",
	"vehicle_detonation",
	"ambient_detonation",
	"first_person_inside",
	"first_person_outside",
	"first_person_anywhere",
	"ui_pda"
};

