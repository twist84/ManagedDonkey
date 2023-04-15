#pragma once

#include "camera/observer.hpp"
#include "camera/camera.hpp"
#include "camera/director.hpp"
#include "game/game_engine.hpp"
#include "game/game_globals.hpp"
#include "game/player_control.hpp"
#include "game/player_mapping.hpp"
#include "game/players.hpp"
#include "game/game_time.hpp"
#include "game/survival_mode.hpp"
#include "effects/effects.hpp"
#include "interface/user_interface_objectives.hpp"
#include "main/global_preferences.hpp"
#include "main/main_time.hpp"
#include "physics/physics_constants.hpp"
#include "scenario/scenario_interpolators.hpp"

//struct simulation_gamestate_entity_datum : s_datum_header

struct s_breakable_surface_globals
{
	byte __data[0x3CE18];
};
static_assert(sizeof(s_breakable_surface_globals) == 0x3CE18);

//struct breakable_surface_set_broken_event_datum : s_datum_header

//struct hs_thread_deterministic_data : s_datum_header

struct hs_runtime
{
	dword __unknown0;
	dword __unknown4;
};
static_assert(sizeof(hs_runtime) == 0x8);

//struct hs_global_data : s_datum_header

//struct hs_distributed_global_data : s_datum_header

//struct hs_thread_tracking_data : s_datum_header

//struct hs_thread_non_deterministic_data : s_datum_header

//struct effect_datum : s_datum_header

//struct effect_event_datum : s_datum_header

//struct effect_location_datum : s_datum_header

struct s_effect_counts
{
	byte __data[0x18];
};
static_assert(sizeof(s_effect_counts) == 0x18);

//struct effect_geometry_sample_datum : s_datum_header

struct effect_messaging_queue
{
	byte __data[0x17084];
};
static_assert(sizeof(effect_messaging_queue) == 0x17084);

struct s_havok_gamestate
{
	dword __unknown0;
	dword __unknown4;
};
static_assert(sizeof(s_havok_gamestate) == 0x8);

struct s_player_control_globals_deterministic
{
	byte __data[0x80];
};
static_assert(sizeof(s_player_control_globals_deterministic) == 0x80);

//struct game_looping_sound_datum : s_datum_header

struct s_game_sound_globals
{
	byte __data[0x154];
};
static_assert(sizeof(s_game_sound_globals) == 0x154);

struct s_game_sound_impulse_datum
{
	byte __data[0x200];
};
static_assert(sizeof(s_game_sound_impulse_datum) == 0x200);

struct s_structure_seam_globals
{
	byte __data[0x14614];
};
static_assert(sizeof(s_structure_seam_globals) == 0x14614);

struct visibility_active_portals
{
	byte __data[0x800];
};
static_assert(sizeof(visibility_active_portals) == 0x800);

struct s_campaign_metagame_globals
{
	byte __data[0x1A158];
};
static_assert(sizeof(s_campaign_metagame_globals) == 0x1A158);

struct s_observer_gamestate_globals
{
	dword __unknown0;
	dword __unknown4;
	dword __unknown8;
};
static_assert(sizeof(s_observer_gamestate_globals) == 0xC);

struct rumble_global_data
{
	byte __data[0x22C];
};
static_assert(sizeof(rumble_global_data) == 0x22C);

struct s_bink_shared_game_state
{
	dword __unknown0;
	dword __unknown4;
};
static_assert(sizeof(s_bink_shared_game_state) == 0x8);

struct sound_class_datum
{
	byte __data[0x1144];
};
static_assert(sizeof(sound_class_datum) == 0x1144);

struct s_game_allegiance_globals
{
	byte __data[0x184];
};
static_assert(sizeof(s_game_allegiance_globals) == 0x184);

struct s_atmosphere_fog_globals
{
	byte __data[0x14];
};
static_assert(sizeof(s_atmosphere_fog_globals) == 0x14);

struct s_scenario_soft_ceilings_globals
{
	dword __unknown0;
	dword __unknown4;
	dword __unknown8;
	dword __unknownC;
};
static_assert(sizeof(s_scenario_soft_ceilings_globals) == 0x10);

struct s_game_sound_player_effects_globals
{
	byte __data[0x28];
};
static_assert(sizeof(s_game_sound_player_effects_globals) == 0x28);

struct cinematic_new_globals
{
	byte __data[0x3C];
};
static_assert(sizeof(cinematic_new_globals) == 0x3C);

struct cinematic_globals
{
	byte __unknown0[4];
	bool show_letterbox;
	bool start_stop;
	bool skip_start_stop;
	bool suppress_bsp_object_creation;
	byte __unknown8[24];
	dword subtitle_string_id;
	real subtitle_time_shown;
	real subtitle_time_shown2;
	byte __unknown2C[2];
	bool outro_start_stop;
	byte __unknown2F[10201];
};
static_assert(sizeof(cinematic_globals) == 0x2808);

struct cinematic_light_globals
{
	byte __data[0xB2C8];
};
static_assert(sizeof(cinematic_light_globals) == 0xB2C8);

//struct recorded_animation_datum : s_datum_header

struct s_game_save_globals
{
	byte __data[0x18];
};
static_assert(sizeof(s_game_save_globals) == 0x18);

//struct s_rasterizer_screen_effect : s_datum_header

struct s_player_effect_globals
{
	byte __data[0x3A0];
};
static_assert(sizeof(s_player_effect_globals) == 0x3A0);

struct s_player_training_globals
{
	byte __data[0x8E8];
};
static_assert(sizeof(s_player_training_globals) == 0x8E8);

struct s_scenario_kill_trigger_volumes_state
{
	byte __data[0x84];
};
static_assert(sizeof(s_scenario_kill_trigger_volumes_state) == 0x84);

struct s_game_sound_deterministic_globals
{
	byte __data[0x1300];
};
static_assert(sizeof(s_game_sound_deterministic_globals) == 0x1300);

//struct s_decal_system_datum : s_datum_header

struct s_decal_counts
{
	byte __data[0x20];
};
static_assert(sizeof(s_decal_counts) == 0x20);

struct decal
{
	byte __data[0x130];
};
static_assert(sizeof(decal) == 0x130);

struct s_decal_message_queue
{
	byte __data[0x824];
};
static_assert(sizeof(s_decal_message_queue) == 0x824);

struct s_impact_globals
{
	byte __unknown0[0x8C];
};
static_assert(sizeof(s_impact_globals) == 0x8C);

struct impacts
{
	byte __unknown0[0xB4];
};
static_assert(sizeof(impacts) == 0xB4);

struct impact_arrays
{
	byte __unknown0[0x88];
};
static_assert(sizeof(impact_arrays) == 0x88);

struct object_list_header
{
	dword __unknown0;
	dword __unknown4;
	dword __unknown8;
};
static_assert(sizeof(object_list_header) == 0xC);

struct list_object
{
	byte __data[0x8C];
};
static_assert(sizeof(list_object) == 0x8C);

struct scripted_camera_globals
{
	byte __unknown0[0xF0];
};
static_assert(sizeof(scripted_camera_globals) == 0xF0);

//struct s_particle_system_datum : s_datum_header

struct contrail_system
{
	byte __data[0x4C];
};
static_assert(sizeof(contrail_system) == 0x4C);

struct contrail
{
	byte __data[0x4C];
};
static_assert(sizeof(contrail) == 0x4C);

struct contrail_location
{
	byte __data[0x60];
};
static_assert(sizeof(contrail_location) == 0x60);

struct contrail_profile
{
	byte __data[0x4C];
};
static_assert(sizeof(contrail_profile) == 0x4C);

struct particle_location
{
	byte __data[0x60];
};
static_assert(sizeof(particle_location) == 0x60);

struct light_volume_location
{
	byte __data[0x2C];
};
static_assert(sizeof(light_volume_location) == 0x2C);

struct light_volume
{
	byte __data[0x34];
};
static_assert(sizeof(light_volume) == 0x34);

struct light_volume_system
{
	byte __data[0x2C];
};
static_assert(sizeof(light_volume_system) == 0x2C);

struct beam_system
{
	byte __data[0x34];
};
static_assert(sizeof(beam_system) == 0x34);

struct beam
{
	byte __data[0x2C];
};
static_assert(sizeof(beam) == 0x2C);

struct beam_location
{
	byte __data[0x30];
};
static_assert(sizeof(beam_location) == 0x30);

struct hue_saturation_control
{
	dword graphics_override;
	real saturation;
	real color;
	real __unknownC;
	real __unknown10;
};
static_assert(sizeof(hue_saturation_control) == 0x14);

struct ragdolls
{
	byte __data[0x130];
};
static_assert(sizeof(ragdolls) == 0x130);

struct particle_emitter
{
	byte __data[0x90];
};
static_assert(sizeof(particle_emitter) == 0x90);

struct rasterizer_game_states
{
	bool motion_blur;
	bool atmosphere_fog;
	bool patchy_fog;
	bool weather;
	bool cinematic_motion_blur;
	byte __unknown5[39];
	bool autoexposure;
	byte __unknown44[475];
};
static_assert(sizeof(rasterizer_game_states) == 0x208);

struct scripted_exposure_globals
{
	byte __data[0x34];
};
static_assert(sizeof(scripted_exposure_globals) == 0x34);

struct render_hud_globals
{
	byte __data[0x480];
};
static_assert(sizeof(render_hud_globals) == 0x480);

struct water_interaction_ripples
{
	byte __data[0x1400];
};
static_assert(sizeof(water_interaction_ripples) == 0x1400);

struct render_texture_globals
{
	bool camera_enable;
	bool camera_dynamic_lights_enable;
	short __unknown2;
	long camera_render_mode;
	long __unknown8;
	long __unknownC;
	long __unknown10;
	long __unknown14;
	long __unknown18;
	long camera_object_handle;
	long camera_marker_name;
	long __unknown24;
	real_point3d camera_position_world;
	real_point3d camera_target_object;
	byte __unknown40[0x1C];
	real camera_fov;
	real aspect_ratio;
	long camera_resolution_width;
	long camera_resolution_height;
};
static_assert(sizeof(render_texture_globals) == 0x6C);

struct render_game_globals
{
	byte __data[0xD80];
};
static_assert(sizeof(render_game_globals) == 0xD80);

struct depth_of_field_globals
{
	bool enable;
	byte __unknown1[0x3];
	real __unknown4;
	real __unknown8;
	real __unknownC;
	real intensity;
	real __unknown14;
	real __unknown18;
	real __unknown1C;
	real __unknown20;
	real __unknown24;
	real __unknown28;
	real __unknown2C;
	real __unknown30;
	real __unknown34;
	real __unknown38;
	real __unknown3C;
};
static_assert(sizeof(depth_of_field_globals) == 0x40);

struct cached_object_render_states
{
	byte __data[0x4D8];
};
static_assert(sizeof(cached_object_render_states) == 0x4D8);

struct particle_emitter_gpu_row
{
	byte __data[0x18];
};
static_assert(sizeof(particle_emitter_gpu_row) == 0x18);

struct particle_emitter_gpu_1
{
	byte __data[0x14];
};
static_assert(sizeof(particle_emitter_gpu_1) == 0x14);

struct beam_gpu
{
	dword __unknown0;
	dword __unknown4;
	dword __unknown8;
	dword __unknownC;
};
static_assert(sizeof(beam_gpu) == 0x10);

struct beam_gpu_row
{
	dword __unknown0;
	dword __unknown4;
	dword __unknown8;
};
static_assert(sizeof(beam_gpu_row) == 0xC);

struct contrail_gpu_row
{
	dword __unknown0;
	dword __unknown4;
	dword __unknown8;
	dword __unknownC;
};
static_assert(sizeof(contrail_gpu_row) == 0x10);

struct contrail_gpu
{
	dword __unknown0;
	dword __unknown4;
	dword __unknown8;
	dword __unknownC;
	dword __unknown10;
};
static_assert(sizeof(contrail_gpu) == 0x14);

struct light_volume_gpu
{
	dword __unknown0;
	dword __unknown4;
	dword __unknown8;
	dword __unknownC;
};
static_assert(sizeof(light_volume_gpu) == 0x10);

struct light_volume_gpu_row
{
	dword __unknown0;
	dword __unknown4;
	dword __unknown8;
};
static_assert(sizeof(light_volume_gpu_row) == 0xC);

struct s_render_object_globals
{
	byte __data[0x3C040];
};
static_assert(sizeof(s_render_object_globals) == 0x3C040);

struct shield_render_cache_message
{
	byte __data[0x14];
};
static_assert(sizeof(shield_render_cache_message) == 0x14);

struct chud_player_hud_elements
{
	byte __unknown0[0x2];
	byte crosshair;
	byte shield;
	byte grenades;
	byte messages;
	byte motion_sensor;
	byte spike_grenades;
	byte fire_grenades;
	byte compass;
	byte stamina;
	byte energy_meters;
	byte consumables;
};

struct chud_persistent_user_data
{
	byte __data[0x14D];
	chud_player_hud_elements player_hud;
	byte __unknown181[0x316];
};
static_assert(sizeof(chud_persistent_user_data) == 0x470);

struct chud_persistent_global_data
{
	byte __unknown0[0x14D];
	chud_player_hud_elements player_hud[4];
	byte __unknown181[0x273];
	byte bonus_round_show_timer;
	byte bonus_round_start_timer;
	byte __unknown3F6[2];
	long bonus_round_set_timer;
	long bonus_round_set_target_score;
	byte __unknown3FC[0xF640];
};
static_assert(sizeof(chud_persistent_global_data) == 0xFA40);

// struct chud_widget_datum : s_datum_header

struct s_first_person_orientations
{
	byte __data[0x12C00];
};
static_assert(sizeof(s_first_person_orientations) == 0x12C00);

struct first_person_weapon
{
	byte __data[0x14000];
};
static_assert(sizeof(first_person_weapon) == 0x14000);

struct s_cortana_globals
{
	dword cortana_effect_definition_index;
	dword __unknown4;
	dword __unknown8;
	dword __unknownC;
};
static_assert(sizeof(s_cortana_globals) == 0x10);

struct s_object_globals
{
	byte __data[0x6608];
};
static_assert(sizeof(s_object_globals) == 0x6608);

struct objects_memory_pool
{
	byte __data[0x44];
};
static_assert(sizeof(objects_memory_pool) == 0x44);

struct object_messaging_queue
{
	byte __data[0x4104];
};
static_assert(sizeof(object_messaging_queue) == 0x4104);

struct damage_globals
{
	byte __data[0x810];
};
static_assert(sizeof(damage_globals) == 0x810);

struct object_render_data
{
	byte __data[0x2000];
};
static_assert(sizeof(object_render_data) == 0x2000);

struct s_damage_globals
{
	byte __data[0x810];
};
static_assert(sizeof(s_damage_globals) == 0x810);

struct s_object_placement_globals
{
	byte __data[0x320];
};
static_assert(sizeof(s_object_placement_globals) == 0x320);

struct device_groups
{
	dword __unknown0;
	dword __unknown4;
	dword __unknown8;
	dword __unknownC;
};
static_assert(sizeof(device_groups) == 0x10);

struct object_scripting
{
	byte __data[0x304];
};
static_assert(sizeof(object_scripting) == 0x304);

struct s_object_broadphase
{
	byte __data[0x32450];
};
static_assert(sizeof(s_object_broadphase) == 0x32450);

struct s_object_early_movers_globals
{
	byte __data[0x2688];
};
static_assert(sizeof(s_object_early_movers_globals) == 0x2688);

struct s_object_scehdule_globals
{
	byte __data[0x27C];
};
static_assert(sizeof(s_object_scehdule_globals) == 0x27C);

struct object_activation_regions
{
	byte __data[0x28];
};
static_assert(sizeof(object_activation_regions) == 0x28);

struct lights
{
	byte __data[0xE4];
};
static_assert(sizeof(lights) == 0xE4);

struct lights_game_globals_definition
{
	byte __data[0x40];
};
static_assert(sizeof(lights_game_globals_definition) == 0x40);

struct s_nondeterministic_light_data
{
	byte __data[0x2580];
};
static_assert(sizeof(s_nondeterministic_light_data) == 0x2580);

struct widget
{
	dword __unknown0;
	dword __unknown4;
	dword __unknown8;
};
static_assert(sizeof(widget) == 0xC);

struct s_recycling_volumes
{
	byte __data[0x148];
};
static_assert(sizeof(s_recycling_volumes) == 0x148);

//struct recycling_group_datum : s_datum_header

struct muffin
{
	byte __data[0x1890];
};
static_assert(sizeof(muffin) == 0x1890);

struct leaf_system
{
	byte __data[0x94C];
};
static_assert(sizeof(leaf_system) == 0x94C);

struct antenna
{
	byte __data[0x64];
};
static_assert(sizeof(antenna) == 0x64);

struct cloth
{
	byte __data[0x1704];
};
static_assert(sizeof(cloth) == 0x1704);

struct actor
{
	byte __data[0xA98];
};
static_assert(sizeof(actor) == 0xA98);

struct actor_firing_position
{
	byte __data[0x400];
};
static_assert(sizeof(actor_firing_position) == 0x400);

struct ai_reference_frame
{
	byte __data[0x4B0];
};
static_assert(sizeof(ai_reference_frame) == 0x4B0);

struct ai_globals
{
	byte enable;
	byte __unknown1;
	short flags;
	byte __unknown4;
	byte fast_and_dumb;
	byte __unknown5[1666];
};
static_assert(sizeof(ai_globals) == 0x688);

struct ai_player_state
{
	byte __data[0xB0];
};
static_assert(sizeof(ai_player_state) == 0xB0);

struct vocalization_records
{
	byte __data[0x5C];
};
static_assert(sizeof(vocalization_records) == 0x5C);

struct vocalization_timers
{
	byte __data[0xFB8];
};
static_assert(sizeof(vocalization_timers) == 0xFB8);

struct command_scripts
{
	byte __data[0x188];
};
static_assert(sizeof(command_scripts) == 0x188);

struct objectives
{
	dword __unknown0;
	dword __unknown4;
	dword __unknown8;
};
static_assert(sizeof(objectives) == 0xC);

struct task_records
{
	byte __data[0x61A80];
};
static_assert(sizeof(task_records) == 0x61A80);

struct squad
{
	byte __data[0xEC];
};
static_assert(sizeof(squad) == 0xEC);

struct squad_group
{
	byte __data[0x24];
};
static_assert(sizeof(squad_group) == 0x24);

struct swarm
{
	byte __data[0x34];
};
static_assert(sizeof(swarm) == 0x34);

struct swarm_spawner
{
	byte __data[0x258];
};
static_assert(sizeof(swarm_spawner) == 0x258);

struct spawner_globals
{
	short __unknown0;
};
static_assert(sizeof(spawner_globals) == 0x2);

struct dynamic_firing_points
{
	byte __data[0x584];
};
static_assert(sizeof(dynamic_firing_points) == 0x584);

struct propref
{
	byte __data[0x3C];
};
static_assert(sizeof(propref) == 0x3C);

struct prop
{
	byte __data[0xC4];
};
static_assert(sizeof(prop) == 0xC4);

struct tracking
{
	byte __data[0x100];
};
static_assert(sizeof(tracking) == 0x100);

struct joint_state
{
	byte __data[0xCC];
};
static_assert(sizeof(joint_state) == 0xCC);

struct clump
{
	byte __data[0x108];
};
static_assert(sizeof(clump) == 0x108);

struct squad_patrol
{
	byte __data[0x6C4];
};
static_assert(sizeof(squad_patrol) == 0x6C4);

struct flocks
{
	byte __data[0x4C];
};
static_assert(sizeof(flocks) == 0x4C);

struct formations
{
	byte __data[0x294];
};
static_assert(sizeof(formations) == 0x294);

struct vision_mode
{
	byte __data[0xF0];
};
static_assert(sizeof(vision_mode) == 0xF0);

struct simulation_gamestate_entity_datum : s_datum_header
{
	word __unknown2;
	dword __unknown4;
	dword __unknown8;
	dword __unknownC;
};
static_assert(sizeof(simulation_gamestate_entity_datum) == 0x10);

struct breakable_surface_set_broken_event_datum : s_datum_header
{
	byte __data[0x462];
};
static_assert(sizeof(breakable_surface_set_broken_event_datum) == 0x464);

struct hs_thread_deterministic_data : s_datum_header
{
	byte __data[0x522];
};
static_assert(sizeof(hs_thread_deterministic_data) == 0x524);

struct hs_global_data : s_datum_header
{
	word __unknown2;
	dword __unknown4;
};
static_assert(sizeof(hs_global_data) == 0x8);

struct hs_distributed_global_data : s_datum_header
{
	byte __data[0x2A];
};
static_assert(sizeof(hs_distributed_global_data) == 0x2C);

struct hs_thread_tracking_data : s_datum_header
{
	word __unknown2;
	dword __unknown4;
	dword __unknown8;
};
static_assert(sizeof(hs_thread_tracking_data) == 0xC);

struct hs_thread_non_deterministic_data : s_datum_header
{
	byte __data[0x522];
};
static_assert(sizeof(hs_thread_non_deterministic_data) == 0x524);

struct effect_datum : s_datum_header
{
	byte __data[0x9E];
};
static_assert(sizeof(effect_datum) == 0xA0);

struct effect_event_datum : s_datum_header
{
	byte __data[0x12];
};
static_assert(sizeof(effect_event_datum) == 0x14);

struct effect_location_datum : s_datum_header
{
	byte __data[0x3E];
};
static_assert(sizeof(effect_location_datum) == 0x40);

struct effect_geometry_sample_datum : s_datum_header
{
	byte __data[0x26];
};
static_assert(sizeof(effect_geometry_sample_datum) == 0x28);

struct game_looping_sound_datum : s_datum_header
{
	byte __data[0x1E];
};
static_assert(sizeof(game_looping_sound_datum) == 0x20);

struct recorded_animation_datum : s_datum_header
{
	byte __data[0xA2];
};
static_assert(sizeof(recorded_animation_datum) == 0xA4);

struct screen_effect_datum : s_datum_header
{
	word field_2;
	dword tag_index;
	real seconds_active;
	vector3d position;
	dword object_index;
	dword field_1c;
	vector3d field_20;
	vector3d field_2c;
	dword field_38;
};
static_assert(sizeof(screen_effect_datum) == 0x3C);

struct s_decal_system_datum : s_datum_header
{
	short : 16;
	long tag_index;
	long __unknown8;
	long __unknownC;
	long __unknown10;
	long __unknown14;
	long __unknown18;
	long __unknown1C;
	long __unknown20;
	long __unknown24;
	long __unknown28;
	long __unknown2C;
	long __unknown30;
	long __unknown34;
	long __unknown38;
	long __unknown3C;
	long __unknown40;
	long __unknown44;
	long __unknown48;
	long __unknown4C;
	byte __unknown50[0x308];
};
static_assert(sizeof(s_decal_system_datum) == 0x358);

struct s_particle_system_datum : s_datum_header
{
	short particle_tag_index;
	long __unknown4;
	long __unknown8;
	long __unknownC;
	long __unknown10;
	long __unknown14;
	long __unknown18;
	long __unknown1C;
	long __unknown20;
	long __unknown24;
	long __unknown28;
	long __unknown2C;
	long __unknown30;
	long __unknown34;
	long __unknown38;
	long __unknown3C;
	long __unknown40;
	long __unknown44;
	long __unknown48;
	long __unknown4C;
	long __unknown50;
	long __unknown54;
};
static_assert(sizeof(s_particle_system_datum) == 0x58);

struct chud_widget_datum : s_datum_header
{
	byte __data[0x16];
};
static_assert(sizeof(chud_widget_datum) == 0x18);

struct cluster_partition
{
	void* first_data;
	void* data_reference;
	void* cluster_first_data_references;
};

struct recycling_group_datum : s_datum_header
{
	byte __data[0x12];
};
static_assert(sizeof(recycling_group_datum) == 0x14);

struct s_thread_local_storage
{
	void* __unknown0;
	void* __unknown4;

	//  name: "sim. gamestate entities"
	// count: 2048
	//  size: 0x10
	c_smart_data_array<simulation_gamestate_entity_datum>* simulation_gamestate_entity_data;

	// name: "gamestate timing samples", "global"
	// size: 0x14
	s_game_tick_time_samples* g_main_gamestate_timing_data;

	// name: "render timing samples", "global"
	// size: 0x14
	s_game_tick_time_samples* g_main_render_timing_data;

	// name: "main_timing", "globals"
	// size: 0x40
	s_main_time_globals* g_main_time_globals;

	// size: 0x84030
	s_global_preferences_internals_type* g_global_preferences;

	void* __unknown1C;

	// from assert
	long g_registered_thread_index;

	void* __unknown24;
	void* __unknown28;
	void* __unknown2C;
	void* __unknown30;

	// name: "random math", "globals"
	// size: 0x4
	dword* g_deterministic_random_seed_ptr;

	// used in `suppress_file_errors` and `file_error`
	bool g_file_errors_suppressed;

	// name: "game globals"
	// size: 0x25208
	game_globals_storage* game_globals;

	// name: "players"
	// count: 16
	//  size: 0x2F08
	c_smart_data_array<player_datum>* player_data;

	// name: "players globals"
	s_players_global_data* players_globals;

	// name: "game engine globals"
	// size: 0x15858
	s_game_engine_globals* game_engine_globals;

	// name: "local game engine globals"
	// size: 0xC4
	s_local_game_engine_globals* local_game_engine_globals;

	// name: "game time globals"
	// size: 0x2C
	game_time_globals_definition* game_time_globals;

	// name: "breakable surface breakable_surface_globals"
	// size: 0x3CE18
	s_breakable_surface_globals* breakable_surface_globals;

	//  name: "breakable surface set broken events"
	// count: 64
	//  size: 0x464
	c_smart_data_array<breakable_surface_set_broken_event_datum>* g_breakable_surface_set_broken_event_data;

	// name: "player mapping globals"
	// size: 0xE8
	s_player_mapping_globals* player_mapping_globals;

	// name: "director globals"
	// size: 0x5C0
	s_director_globals* director_globals;

	// name: "director scripting"
	// size: 0x1
	bool* director_camera_scripted;

	c_smart_data_array<hs_thread_deterministic_data>* hs_thread_deterministic_data;
	hs_runtime* hs_runtime;
	c_smart_data_array<hs_global_data>* hs_global_data;
	c_smart_data_array<hs_distributed_global_data>* hs_distributed_global_data;
	c_smart_data_array<hs_thread_tracking_data>* hs_thread_tracking_data;
	c_smart_data_array<hs_thread_non_deterministic_data>* hs_thread_non_deterministic_data;

	// from assert
	void* g_restricted_address[5];

	// from assert
	bool g_restricted_alias[5];

	void* __unknown9C;

	//  name: "effect"
	// count: 640
	//  size: 0xA0
	c_smart_data_array<effect_datum>* effect_data;

	//  name: "effect event"
	// count: 640
	//  size: 0x14
	c_smart_data_array<effect_event_datum>* event_data;

	//  name: "effect location"
	// count: 1152
	//  size: 0x40
	c_smart_data_array<effect_location_datum>* effect_location_data;

	// name: "effect counts"
	// size: 0x18
	s_effect_counts* g_effect_counts;

	//  name: "effect geometry sample"
	// count: 128
	//  size: 0x28
	c_smart_data_array<effect_geometry_sample_datum>* effect_geometry_sample_data;

	// name: "effect messaging queue"
	// size: 0x17084
	effect_messaging_queue* g_effect_message_queue;

	// name: "effect lightprobes"
	// size: 0xFE00, sizeof(s_effect_lightprobe) * 128
	c_static_array<s_effect_lightprobe, 128>* effect_lightprobe_data;

	// name: "havok gamestate"
	// size: 0x8
	s_havok_gamestate* g_havok_game_state;

	// from assert
	long havok_style_fpu_exceptions_count;

	// name: "player control globals"
	// size: 0x8B0
	s_player_control_globals* player_control_globals;

	// name: "player control globals deterministic"
	// size: 0x80
	s_player_control_globals_deterministic* player_control_globals_deterministic;

	//  name: "object looping sounds"
	// count: 1024
	//  size: 0x20
	c_smart_data_array<game_looping_sound_datum>* game_looping_sound_data;

	// name: "game sound globals"
	// size: 0x154
	s_game_sound_globals* game_sound_globals;

	// name: "game sound scripted impulses"
	// size: 0x200
	s_game_sound_impulse_datum* game_sound_scripted_impulses;

	// name: "s_structure_seam_globals"
	// size: 0x14614
	s_structure_seam_globals* g_structure_seam_globals;

	// name: "visibility active portals"
	// size: 0x800
	visibility_active_portals* visibility_active_portals;

	// name: "campaign meta-game globals"
	// size: 0x1A158
	s_campaign_metagame_globals* g_campaign_metagame_globals;

	// name: "observer gamestate globals"
	// size: 0xC
	s_observer_gamestate_globals* observer_gamestate_globals;

	// name: "observer globals"
	// size: 0xF28
	s_observer_globals* g_observer_globals;

	// name: "rumble"
	// size: 0x22C
	rumble_global_data* rumble_globals;

	// name: "bink game state"
	// size: 0x8
	s_bink_shared_game_state* bink_game_state;

	void* __unknownF4;

	struct s_font_cache_globals* g_font_cache_globals;

	// name: "sound classes"
	// size: 0x1144
	sound_class_datum* sound_class_data;

	// name: "game allegiance globals"
	// size: 0x184
	s_game_allegiance_globals* game_allegiance_globals;

	// name: "atmosphere fog globals"
	// size: 0x14
	s_atmosphere_fog_globals* g_atmosphere_fog_globals;

	// name: "soft surface globals"
	// size: 0x10
	s_scenario_soft_ceilings_globals* g_scenario_soft_ceilings_globals;

	// name: "game sound player effects globals"
	// size: 0x28
	s_game_sound_player_effects_globals* g_game_sound_player_effects_globals;

	//  name: "havok proxies"
	// count: 16
	//  size: 0x44
	void* g_havok_proxy_data; // s_data_array

	void* __unknown114;
	void* __unknown118;
	void* __unknown11C;
	void* __unknown120;
	void* __unknown124;
	void* __unknown128;
	void* __unknown12C;
	void* __unknown130;
	void* __unknown134;
	void* __unknown138;
	void* __unknown13C;
	void* __unknown140;
	void* __unknown144;
	void* __unknown148;
	void* __unknown14C;
	void* __unknown150;
	void* __unknown154;
	void* __unknown158;
	void* __unknown15C;
	void* __unknown160;
	void* __unknown164;
	void* __unknown168;
	void* __unknown16C;
	void* __unknown170;
	void* __unknown174;
	void* __unknown178;
	void* __unknown17C;
	void* __unknown180;
	void* __unknown184;
	void* __unknown188;
	void* __unknown18C;
	void* __unknown190;
	void* __unknown194;
	void* __unknown198;
	void* __unknown19C;
	void* __unknown1A0;
	void* __unknown1A4;
	void* __unknown1A8;
	void* __unknown1AC;
	void* __unknown1B0;
	void* __unknown1B4;
	void* __unknown1B8;
	void* __unknown1BC;
	void* __unknown1C0;
	void* __unknown1C4;
	void* __unknown1C8;
	void* __unknown1CC;
	void* __unknown1D0;
	void* __unknown1D4;
	void* __unknown1D8;
	void* __unknown1Dc;
	void* __unknown1E0;
	void* __unknown1E4;
	void* __unknown1E8;
	void* __unknown1EC;
	void* __unknown1F0;
	void* __unknown1F4;
	void* __unknown1F8;
	void* __unknown1FC;
	void* __unknown200;
	void* __unknown204;
	void* __unknown208;
	void* __unknown20C;
	void* __unknown210;
	void* __unknown214;
	void* __unknown218;
	void* __unknown21C;
	void* __unknown220;
	void* __unknown224;
	void* __unknown228;
	void* __unknown22C;
	void* __unknown230;
	void* __unknown234;
	void* __unknown238;
	void* __unknown23C;
	void* __unknown240;
	void* __unknown244;
	void* __unknown248;
	void* __unknown24C;
	void* __unknown250;
	void* __unknown254;
	void* __unknown258;
	void* __unknown25C;
	void* __unknown260;
	void* __unknown264;
	void* __unknown268;
	void* __unknown26C;
	void* __unknown270;
	void* __unknown274;
	void* __unknown278;
	void* __unknown27C;
	void* __unknown280;
	void* __unknown284;
	void* __unknown288;
	void* __unknown28C;
	void* __unknown290;
	void* __unknown294;
	void* __unknown298;
	void* __unknown29C;
	void* __unknown2A0;
	void* __unknown2A4;
	void* __unknown2A8;
	void* __unknown2AC;
	void* __unknown2B0;
	void* __unknown2B4;
	void* __unknown2B8;
	void* __unknown2BC;
	void* __unknown2C0;
	void* __unknown2C4;
	void* __unknown2C8;
	void* __unknown2CC;
	void* __unknown2D0;
	void* __unknown2D4;
	void* __unknown2D8;
	void* __unknown2DC;
	void* __unknown2E0;
	void* __unknown2E4;
	void* __unknown2E8;
	void* __unknown2EC;
	void* __unknown2F0;
	void* __unknown2F4;
	void* __unknown2F8;
	void* __unknown2FC;
	void* __unknown300;
	void* __unknown304;
	void* __unknown308;
	void* __unknown30C;
	void* __unknown310;
	void* __unknown314;
	void* __unknown318;
	void* __unknown31C;

	// name: "cinematic new globals"
	// size: 0x2808
	cinematic_new_globals* cinematic_new_globals;

	// name: "cinematic globals"
	// size: 0x3C
	cinematic_globals* cinematic_globals;

	// name: "cinematic light globals"
	// size: 0xB2C8
	cinematic_light_globals* cinematic_light_globals;

	// name: "physics constants"
	// size: 0x20
	s_physics_constants* g_physics_constants;

	//  name: "recorded animations"
	// count: 1
	//  size: 0xA4
	c_smart_data_array<recorded_animation_datum>* animation_threads;

	// name: "game save globals"
	// size: 0x18
	s_game_save_globals* g_game_save_globals;

	//  name: "screen_effect"
	// count: 64
	//  size: 0x3C
	c_smart_data_array<screen_effect_datum>* screen_effect_data;

	// name: "player effects"
	// size: 0x3A0
	s_player_effect_globals* player_effect_globals;

	// name: "scenario interpolator globals"
	// size: 0x204
	s_scenario_interpolator_globals* scenario_interpolator_globals;

	// name: "survival mode globals"
	// size: 0x7E0
	s_survival_mode_globals* g_survival_mode_globals;

	// name: "player training globals", "globals"
	// size: 0x8E8
	s_player_training_globals* player_training_globals;

	// name: "kill trigger volume state"
	// size: 0x84
	s_scenario_kill_trigger_volumes_state* g_scenario_kill_trigger_volumes_state;

	// name: "deterministic game sound globals"
	// size: 0x1300
	s_game_sound_deterministic_globals* g_future_development_game_sound_deterministic_globals;

	//  name: "decal_system"
	// count: 856
	//  size: 0x54
	c_smart_data_array<s_decal_system_datum>* decal_system;

	// name: "decal counts"
	// size: 0x20
	s_decal_counts* decal_counts;

	//  name: "decal"
	// count: 856
	//  size: 0x130
	decal* decal_data;

	// name: "decal messaging queue"
	// size: 0x824
	s_decal_message_queue* g_decal_message_queue;

	// name: "impact globals"
	// size: 0x8C
	s_impact_globals* g_impact_globals;

	//  name: "impacts"
	// count: 32
	//  size: 0xB4
	impacts* impacts;

	//  name: "impact arrarys"
	// count: 32
	//  size: 0x88
	impact_arrays* impact_arrays;

	//  name: "object list header"
	// count: 48
	//  size: 0xC
	object_list_header* object_list_header;

	//  name: "list object"
	// count: 128
	//  size: 0xC
	list_object* list_object;

	// name: "scripted camera globals"
	// size: 0xF0
	scripted_camera_globals* scripted_camera_globals;

	data_array_base* particles;
	c_smart_data_array<s_particle_system_datum>* particle_system;

	contrail_system* contrail_system;
	contrail* contrail;
	contrail_location* contrail_location;
	contrail_profile* contrail_profile;
	particle_location* particle_location;
	light_volume_system* light_volume_system;
	light_volume_location* light_volume_location;
	light_volume* light_volume;
	beam_system* beam_system;
	beam* beam;
	beam_location* beam_location;

	// from assert
	bool g_havok_memory_allocator_locked;

	ragdolls* ragdolls;
	particle_emitter* particle_emitter;
	rasterizer_game_states* rasterizer_game_states;
	hue_saturation_control* hue_saturation_control;
	void* __unknown3C4;
	scripted_exposure_globals* scripted_exposure_globals;
	render_hud_globals* render_hud_globals;
	water_interaction_ripples* water_interaction_ripples;
	render_texture_globals* render_texture_globals;
	render_game_globals* render_game_globals;
	depth_of_field_globals* depth_of_field_globals;
	cached_object_render_states* cached_object_render_states;
	particle_emitter_gpu_row* particle_emitter_gpu_row;
	particle_emitter_gpu_1* particle_emitter_gpu_1;
	void* particle_emitter_gpu_2;
	beam_gpu* beam_gpu;
	beam_gpu_row* beam_gpu_row;
	void* particle_emitter_gpu_3;
	contrail_gpu_row* contrail_gpu_row;
	contrail_gpu* contrail_gpu;
	void* particle_emitter_gpu_4;
	light_volume_gpu* light_volume_gpu;
	light_volume_gpu_row* light_volume_gpu_row;
	void* particle_emitter_gpu_5;
	void* rasterizer_implicit_geometry_data;
	s_render_object_globals* render_object_globals;
	shield_render_cache_message* shield_render_cache_message;
	chud_persistent_user_data* chud_persistent_user_data;
	chud_persistent_global_data* chud_persistent_global_data;

	// name: "chud widgets[0]"
	// name: "chud widgets[1]"
	// name: "chud widgets[2]"
	// name: "chud widgets[3]"
	c_static_array<c_smart_data_array<chud_widget_datum>*, 4> chud_widgets;

	// name: "fp orientations"
	// size: 0x12C00
	s_first_person_orientations* first_person_orientations;

	// name: "fp weapons"
	// size: 0x14000
	first_person_weapon* first_person_weapon;

	// name: "chud", "cortana effect"
	// size: 0x10
	s_cortana_globals* g_cortana_globals;

	// name: "campaign objectives"
	// size: 0x14
	s_current_objective_state* g_objectives;

	//  name: "object"
	// count: 2048
	//  size: 0x18
	//c_smart_data_array<Blam::Objects::ObjectHeader>* object_header_data;
	void* object_header_data;

	// name: "object globals"
	// size: 0x6608
	s_object_globals* object_globals;

	// name: "objects"
	// size: 0x180000
	// s_memory_pool* object_memory_pool
	objects_memory_pool* objects_memory_pool;

	// name: "object name list"
	// size: 0x2000
	c_static_array<long, 2048>* g_object_name_list;

	// name: "object messaging queue"
	// size: 0x4104
	// s_object_render_thread_message (*g_object_message_queue)[2048];
	object_messaging_queue* g_object_message_queue;

	// name: "collideable object"
	// size: 0x14
	cluster_partition collideable_object_cluster_partition;

	// name: "noncollideable object"
	// size: 0x14
	cluster_partition noncollideable_object_cluster_partition;

	// name: "OBJ: Render Data", "Render"
	// size: 0x2000
	object_render_data* object_render_data;

	// name: "damage globals"
	// size: 0x810
	s_damage_globals* damage_globals;

	// name: "object placement globals"
	// size: 0x320
	s_object_placement_globals* object_placement_globals;

	//  name: "device groups"
	// count: 1024
	//  size: 0x10
	device_groups* device_groups_data;

	// name: "object scripting"
	// size: 0x304
	object_scripting* object_scripting;

	// name: "object_broadphase", "global"
	// size: 0x32450
	s_object_broadphase* g_object_broadphase;

	// name: "object early movers"
	// size: 0x2688
	s_object_early_movers_globals* g_object_early_movers_globals;

	// name: "object schedule globals"
	// size: 0x27C
	s_object_scehdule_globals* g_object_scehdule_globals;

	//  name: "object activation regions"
	// count: 128
	//  size: 0x28
	object_activation_regions* g_object_activation_regions_data;

	//  name: "lights"
	// count: 400
	//  size: 0xE4
	lights* lights;

	// name: "lights globals"
	// size: 0x40
	lights_game_globals_definition* lights_game_globals;

	// name: "light"
	// size: 0xC
	cluster_partition light_cluster_partition;

	// name: "nondet_light_data", "render lights"
	// size: 0x2580
	s_nondeterministic_light_data* g_nondeterministic_light_data;

	widget* widget_data;

	// name: "recycling_volumes"
	// size: 0x148
	s_recycling_volumes* recycling_volumes;

	//  name: "recycling_group"
	// count: 128
	//  size: 0x14
	c_smart_data_array<recycling_group_datum>* recycling_groups;

	muffin* muffin_data;

	leaf_system* leaf_system_data;

	antenna* antenna_data;

	cloth* cloth_data;

	void* __unknown4CC;
	void* __unknown4D0;
	void* __unknown4D4;
	void* __unknown4D8;
	void* __unknown4DC;
	void* __unknown4E0;
	void* __unknown4E4;
	void* __unknown4E8;
	void* __unknown4EC;
	void* __unknown4F0;
	void* __unknown4F4;
	void* __unknown4F8;
	void* __unknown4FC;
	void* __unknown500;
	void* __unknown504;
	void* __unknown508;
	void* __unknown50C;
	void* __unknown510;
	void* __unknown514;
	void* __unknown518;

	actor* actor;
	actor_firing_position* actor_firing_position;
	ai_reference_frame* ai_reference_frame;
	ai_globals* ai_globals;
	ai_player_state* ai_player_state;
	vocalization_records* vocalization_records;
	vocalization_timers* vocalization_timers;
	command_scripts* command_scripts;
	objectives* objectives;
	task_records* task_records;
	squad* squad;
	squad_group* squad_group;
	swarm* swarm;
	swarm_spawner* swarm_spawner;
	spawner_globals* spawner_globals;
	dynamic_firing_points* dynamic_firing_points;
	propref* propref;
	prop* prop;
	tracking* tracking;
	joint_state* joint_state;
	clump* clump;
	squad_patrol* squad_patrol;
	flocks* flocks;
	formations* formations;

	// name: "vision mode globals"
	// size: 0xF0
	vision_mode* vision_mode;

	void* __unknown580;
};
static_assert(sizeof(s_thread_local_storage) == 0x584);

#define TLS_REFERENCE(NAME) decltype(get_tls()->NAME)& NAME = get_tls()->NAME

extern s_thread_local_storage* get_tls();
