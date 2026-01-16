#pragma once

#include "cseries/cseries.hpp"

enum e_sound_class
{
	_sound_class_projectile_impact,
	_sound_class_projectile_detonation,
	_sound_class_projectile_flyby,
	_sound_class_projectile_detonation_lod,
	_sound_class_weapon_fire,
	_sound_class_weapon_ready,
	_sound_class_weapon_reload,
	_sound_class_weapon_empty,
	_sound_class_weapon_charge,
	_sound_class_weapon_overheat,
	_sound_class_weapon_idle,
	_sound_class_weapon_melee,
	_sound_class_weapon_animation,
	_sound_class_object_impacts,
	_sound_class_particle_impacts,
	_sound_class_weapon_fire_lod,
	_sound_class_weapon_fire_lod_far,
	_sound_class_unused2_impacts,
	_sound_class_unit_footsteps,
	_sound_class_unit_dialog,
	_sound_class_unit_animation,
	_sound_class_unit_unused,
	_sound_class_vehicle_collision,
	_sound_class_vehicle_engine,
	_sound_class_vehicle_animation,
	_sound_class_vehicle_engine_lod,
	_sound_class_device_door,
	_sound_class_device_unused0,
	_sound_class_device_machinery,
	_sound_class_device_stationary,
	_sound_class_device_unused1,
	_sound_class_device_unused2,
	_sound_class_music,
	_sound_class_ambient_nature,
	_sound_class_ambient_machinery,
	_sound_class_ambient_stationary,
	_sound_class_huge_ass,
	_sound_class_object_looping,
	_sound_class_cinematic_music,
	_sound_class_player_armor,
	_sound_class_unknown_unused1,
	_sound_class_ambient_flock,
	_sound_class_no_pad,
	_sound_class_no_pad_stationary,
	_sound_class_arg,
	_sound_class_cortana_mission,
	_sound_class_cortana_gravemind_channel,
	_sound_class_mission_dialog,
	_sound_class_cinematic_dialog,
	_sound_class_scripted_cinematic_foley,
	_sound_class_hud,
	_sound_class_game_event,
	_sound_class_ui,
	_sound_class_test,
	_sound_class_multilingual_test,
	_sound_class_ambient_nature_details,
	_sound_class_ambient_machinery_details,
	_sound_class_inside_surround_tail,
	_sound_class_outside_surround_tail,
	_sound_class_vehicle_detonation,
	_sound_class_ambient_detonation,
	_sound_class_first_person_inside,
	_sound_class_first_person_outside,
	_sound_class_first_person_anywhere,
	_sound_class_ui_pda,

	k_sound_class_count
};

struct sound_class_datum
{
	byte __data[0x1144];
};
COMPILE_ASSERT(sizeof(sound_class_datum) == 0x1144);

extern const char* const sound_class_names[k_sound_class_count];

extern void __cdecl sound_classes_dispose();
extern void __cdecl sound_classes_dispose_from_old_map();
extern void __cdecl sound_classes_initialize();
extern void __cdecl sound_classes_initialize_for_new_map();

