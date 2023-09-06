#pragma once

#include "cache/restricted_memory.hpp"
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
#include "gpu_particle/beam_gpu.hpp"
#include "gpu_particle/contrail_gpu.hpp"
#include "gpu_particle/light_volume_gpu.hpp"
#include "gpu_particle/particle_block.hpp"
#include "effects/contrails.hpp"
#include "effects/effects.hpp"
#include "interface/user_interface_objectives.hpp"
#include "main/global_preferences.hpp"
#include "main/main_time.hpp"
#include "memory/data.hpp"
#include "memory/hashtable.hpp"
#include "physics/physics_constants.hpp"
#include "scenario/scenario_interpolators.hpp"

struct simulation_gamestate_entity_datum : s_datum_header
{
	word __unknown2;
	dword __unknown4;
	dword __unknown8;
	dword __unknownC;
};
static_assert(sizeof(simulation_gamestate_entity_datum) == 0x10);

struct s_breakable_surface_globals
{
	byte __data[0x3CE18];
};
static_assert(sizeof(s_breakable_surface_globals) == 0x3CE18);

struct breakable_surface_set_broken_event_datum : s_datum_header
{
	byte __data[0x462];
};
static_assert(sizeof(breakable_surface_set_broken_event_datum) == 0x464);

struct hs_stack_frame
{
	word stack_offset;
	long return_value;
	long tracking_index;

	// 0: _hs_thread_type_script
	// 1: [global initialize]
	// 2: [console command]
	// 3: script?
	// 4: script?
	char type;

	byte_flags __flagsD;
	byte_flags __flagsE;
	byte __unknownF;
	dword __unknown10;
};
static_assert(sizeof(hs_stack_frame) == 0x14);

struct hs_thread : s_datum_header
{
	short script_index;
	short previous_script_index;
	long sleep_until;
	long sleep_time;
	hs_stack_frame stack_pointer;
	byte stack_data[0x500];
};
static_assert(sizeof(hs_thread) == 0x524);

struct s_hs_runtime_globals
{
	bool initialized;
	bool require_gc;
	bool require_object_list_gc;
	bool globals_initializing;
	long executing_thread_index;
};
static_assert(sizeof(s_hs_runtime_globals) == 0x8);

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

struct s_effect_counts
{
	byte __data[0x18];
};
static_assert(sizeof(s_effect_counts) == 0x18);

struct effect_geometry_sample_datum : s_datum_header
{
	byte __data[0x26];
};
static_assert(sizeof(effect_geometry_sample_datum) == 0x28);

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

struct game_looping_sound_datum : s_datum_header
{
	byte __data[0x1E];
};
static_assert(sizeof(game_looping_sound_datum) == 0x20);

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

struct s_seam_mapping
{
	struct s_seam_cluster_mapping
	{
		real_point3d cluster_center;
		short cluster_indices[2];
		short cluster_mapping_indices[2];
	};
	static_assert(sizeof(s_seam_cluster_mapping) == 0x14);

	char structure_bsp_indices[2];
	short cluster_count;
	c_static_array<s_seam_cluster_mapping, 32> clusters;
};
static_assert(sizeof(s_seam_mapping) == 0x284);

using c_active_seam_flags = c_static_flags<128>;

struct s_structure_seam_globals
{
	c_static_array<c_static_flags<512>, 16> flags;
	c_static_array<s_seam_mapping, 128> seam_mappings;
	c_active_seam_flags active_seams_mask;
	dword_flags connected_bsps_mask;
};
static_assert(sizeof(s_structure_seam_globals) == 0x14614);

struct s_campaign_metagame_runtime_globals
{
	byte __data[0x1A158];
};
static_assert(sizeof(s_campaign_metagame_runtime_globals) == 0x1A158);

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
	c_static_flags<128> flags;
};
static_assert(sizeof(s_scenario_soft_ceilings_globals) == 0x10);

struct s_game_sound_player_effects_globals
{
	byte __data[0x28];
};
static_assert(sizeof(s_game_sound_player_effects_globals) == 0x28);

struct s_havok_proxy_datum : s_datum_header
{
	struct hkRigidBody* rigid_body;
	real_matrix4x3 matrix;
	long object_index;
	dword __time40;
};
static_assert(sizeof(s_havok_proxy_datum) == 0x44);

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

struct recorded_animation_datum : s_datum_header
{
	byte __data[0xA2];
};
static_assert(sizeof(recorded_animation_datum) == 0xA4);

struct s_game_save_globals
{
	byte __data[0x18];
};
static_assert(sizeof(s_game_save_globals) == 0x18);

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

struct c_decal_system : s_datum_header
{
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
static_assert(sizeof(c_decal_system) == 0x358);

struct s_decal_counts
{
	byte __data[0x20];
};
static_assert(sizeof(s_decal_counts) == 0x20);

struct c_decal : s_datum_header
{
	byte __data[0x12E];
};
static_assert(sizeof(c_decal) == 0x130);

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

struct c_impact : s_datum_header
{
	byte __unknown2[0xB2];
};
static_assert(sizeof(c_impact) == 0xB4);

struct impact_array_datum : s_datum_header
{
	short impact_indexes_count;
	long impact_indexes[33];
};
static_assert(sizeof(impact_array_datum) == 0x88);

struct object_list_header_datum : s_datum_header
{
	short __unknown2;
	short reference_count;
	short count;
	long reference_index;
};
static_assert(sizeof(object_list_header_datum) == 0xC);

struct object_list_datum : s_datum_header
{
	long object_index;
	long reference_index;
};
static_assert(sizeof(object_list_datum) == 0xC);

struct scripted_camera_globals
{
	byte __unknown0[0xF0];
};
static_assert(sizeof(scripted_camera_globals) == 0xF0);

struct c_particle_system : s_datum_header
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
static_assert(sizeof(c_particle_system) == 0x58);

struct c_particle_location : s_datum_header
{
	byte __data[0x5E];
};
static_assert(sizeof(c_particle_location) == 0x60);

struct c_light_volume_location : s_datum_header
{
	byte __data[0x2A];
};
static_assert(sizeof(c_light_volume_location) == 0x2C);

struct c_light_volume : s_datum_header
{
	byte __data[0x32];
};
static_assert(sizeof(c_light_volume) == 0x34);

struct c_light_volume_system : s_datum_header
{
	byte __data[0x2A];
};
static_assert(sizeof(c_light_volume_system) == 0x2C);

struct c_beam_system : s_datum_header
{
	byte __data[0x32];
};
static_assert(sizeof(c_beam_system) == 0x34);

struct c_beam : s_datum_header
{
	byte __data[0x2A];
};
static_assert(sizeof(c_beam) == 0x2C);

struct c_beam_location : s_datum_header
{
	byte __data[0x2E];
};
static_assert(sizeof(c_beam_location) == 0x30);

struct hue_saturation_control
{
	dword graphics_override;
	real saturation;
	real color;
	real __unknownC;
	real __unknown10;
};
static_assert(sizeof(hue_saturation_control) == 0x14);

struct s_ragdoll_datum : s_datum_header
{
	byte __data[0x12E];
};
static_assert(sizeof(s_ragdoll_datum) == 0x130);

struct c_particle_emitter : s_datum_header
{
	byte __data[0x8E];
};
static_assert(sizeof(c_particle_emitter) == 0x90);

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

struct s_interaction_event
{
	byte __data[0x1400];
};
static_assert(sizeof(s_interaction_event) == 0x1400);

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

struct s_depth_of_field
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
static_assert(sizeof(s_depth_of_field) == 0x40);

struct cached_object_render_state_datum : s_datum_header
{
	byte __data[0x4D6];
};
static_assert(sizeof(cached_object_render_state_datum) == 0x4D8);

enum e_implicit_transparent_type
{
	_implicit_transparent_type_cylinder = 0,
	_implicit_transparent_type_sphere,
	_implicit_transparent_type_box,

	k_implicit_transparent_type_count
};

struct s_rasterizer_implicit_cylinder_object
{
	real width;
};
static_assert(sizeof(s_rasterizer_implicit_cylinder_object) == sizeof(real));

struct s_rasterizer_implicit_sphere_object
{
	real width;
	real absolute_positive_height;
	real absolute_negative_height;
};
static_assert(sizeof(s_rasterizer_implicit_sphere_object) == sizeof(real) * 3);

struct s_rasterizer_implicit_box_object
{
	real width;
	real length;
	real absolute_positive_height;
	real absolute_negative_height;
};
static_assert(sizeof(s_rasterizer_implicit_box_object) == sizeof(real) * 4);

struct s_rasterizer_implicit_object
{
	c_enum<e_implicit_transparent_type, long, _implicit_transparent_type_cylinder, k_implicit_transparent_type_count> transparent_type;
	long render_method_index;
	real_argb_color color;
	real_matrix4x3 transform;

	union
	{
		s_rasterizer_implicit_cylinder_object cylinder;
		s_rasterizer_implicit_sphere_object sphere;
		s_rasterizer_implicit_box_object box;
	};
};
static_assert(sizeof(s_rasterizer_implicit_object) == 0x5C);

struct s_implicit_vertex
{
	real_point3d position;
	real_point2d texcoord;
};
static_assert(sizeof(s_implicit_vertex) == 0x14);

struct rasterizer_vertex_implicit
{
	s_implicit_vertex bottom;
	s_implicit_vertex top;
};
static_assert(sizeof(rasterizer_vertex_implicit) == 0x28);

struct s_rasterizer_implicit_geometry_globals
{
	long implicit_object_count;
	s_rasterizer_implicit_object implicit_objects[64];
	rasterizer_vertex_implicit implicit_vertex_data[64];
};
static_assert(sizeof(s_rasterizer_implicit_geometry_globals) == 0x2104);

//void initialize_circle_strip()
//{
//	TLS_DATA_GET_VALUE_REFERENCE(g_rasterizer_implicit_geometry_globals);
//
//	for (long i = 0; i < NUMBEROF(g_rasterizer_implicit_geometry_globals->implicit_vertex_data); i++)
//	{
//		rasterizer_vertex_implicit* vertex_data = &g_rasterizer_implicit_geometry_globals->implicit_vertex_data[i];
//
//		real angle_scaling_factor = i * 63.0f;
//		real circle_x_shift = sinf(angle_scaling_factor * TWO_PI);
//		real circle_y_shift = cosf(angle_scaling_factor * TWO_PI);
//
// 
//		// MS23
//
//		vertex_data->bottom.position.x = circle_x_shift;
//		vertex_data->bottom.position.y = circle_y_shift;
//		vertex_data->bottom.position.z = 0.0f;
//		vertex_data->bottom.texcoord.x = angle_scaling_factor;
//		vertex_data->bottom.texcoord.y = 0.0f;
//
//		vertex_data->top.position.x = circle_x_shift;
//		vertex_data->top.position.y = circle_y_shift;
//		vertex_data->top.position.z = 1.0f;
//		vertex_data->top.texcoord.x = angle_scaling_factor;
//		vertex_data->top.texcoord.y = 1.0f;
//
// 
//		// H3EK
//		for (long j = 0; j < 2; j += 2)
//		{
//			vertex_data->bottom.position.x = circle_x_shift;
//			vertex_data->bottom.position.y = circle_y_shift;
//			vertex_data->bottom.position.z = j ? 1.0f : 0.0f;
//			vertex_data->bottom.texcoord.x = angle_scaling_factor;
//			vertex_data->bottom.texcoord.y = j ? 1.0f : 0.0f;
//
//			vertex_data->top.position.x = circle_x_shift;
//			vertex_data->top.position.y = circle_y_shift;
//			vertex_data->top.position.z = j == -1 ? 0.0f : 1.0f;
//			vertex_data->top.texcoord.x = angle_scaling_factor;
//			vertex_data->top.texcoord.y = j == -1 ? 0.0f : 1.0f;
//		}
//	}
//}

struct s_render_object_globals
{
	byte __data[0x3C040];
};
static_assert(sizeof(s_render_object_globals) == 0x3C040);

struct shield_render_cache_message : s_datum_header
{
	byte __data[0x12];
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

struct chud_widget_datum : s_datum_header
{
	byte __data[0x16];
};
static_assert(sizeof(chud_widget_datum) == 0x18);

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

struct device_group_datum : s_datum_header
{
	dword __unknown4;
	dword __unknown8;
	dword __unknownC;
};
static_assert(sizeof(device_group_datum) == 0x10);

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

struct s_object_early_mover
{
	byte __data[0x130];
};
static_assert(sizeof(s_object_early_mover) == 0x130);

struct s_object_early_movers_globals
{
	s_object_early_mover early_movers[32];
	long object_indices[32];
	long object_index_count;
	bool map_initialized;
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

struct light_datum : s_datum_header
{
	byte __data[0xE2];
};
static_assert(sizeof(light_datum) == 0xE4);

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

struct widget_datum : s_datum_header
{
	dword __unknown4;
	dword __unknown8;
};
static_assert(sizeof(widget_datum) == 0xC);

struct s_recycling_volumes
{
	byte __data[0x148];
};
static_assert(sizeof(s_recycling_volumes) == 0x148);

struct recycling_group_datum : s_datum_header
{
	byte __data[0x12];
};
static_assert(sizeof(recycling_group_datum) == 0x14);

struct muffin_datum : s_datum_header
{
	byte __data[0x188E];
};
static_assert(sizeof(muffin_datum) == 0x1890);

struct c_leaf_system : s_datum_header
{
	byte __data[0x94A];
};
static_assert(sizeof(c_leaf_system) == 0x94C);

struct antenna_datum : s_datum_header
{
	byte __data[0x62];
};
static_assert(sizeof(antenna_datum) == 0x64);

struct cloth_datum : s_datum_header
{
	byte __data[0x1702];
};
static_assert(sizeof(cloth_datum) == 0x1704);

struct actor_datum : s_datum_header
{
	byte __data[0xA96];
};
static_assert(sizeof(actor_datum) == 0xA98);

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

struct ai_globals_type
{
	bool enable;
	byte __unknown1;
	byte_flags flags;
	byte __unknown3;
	byte __unknown4;
	bool fast_and_dumb;
	byte __data6[22];
	real unknown1C;
	bool dialogue_enabled;
	byte __data21[31];
	word __unknown40;
	word __unknown42;
	byte __unknown44[0x380];
	byte __data3C4[0x2C4];
};
static_assert(sizeof(ai_globals_type) == 0x688);

struct ai_player_state
{
	byte __data[0xB0];
};
static_assert(sizeof(ai_player_state) == 0xB0);

struct vocalization_records : s_datum_header
{
	byte __data[0x5A];
};
static_assert(sizeof(vocalization_records) == 0x5C);

struct vocalization_timers
{
	byte __data[0xFB8];
};
static_assert(sizeof(vocalization_timers) == 0xFB8);

struct command_script_datum : s_datum_header
{
	byte __data[0x186];
};
static_assert(sizeof(command_script_datum) == 0x188);

struct s_objective : s_datum_header
{
	dword __unknown4;
	dword __unknown8;
};
static_assert(sizeof(s_objective) == 0xC);

struct s_task_record
{
	byte __data[0x61A80];
};
static_assert(sizeof(s_task_record) == 0x61A80);

struct squad_datum : s_datum_header
{
	byte __data[0xEA];
};
static_assert(sizeof(squad_datum) == 0xEC);

struct squad_group_datum : s_datum_header
{
	byte __data[0x22];
};
static_assert(sizeof(squad_group_datum) == 0x24);

struct swarm_datum : s_datum_header
{
	byte __data[0x32];
};
static_assert(sizeof(swarm_datum) == 0x34);

struct s_swarm_spawner
{
	byte __data[0x258];
};
static_assert(sizeof(s_swarm_spawner) == 0x258);

struct s_spawner_globals
{
	short __unknown0;
};
static_assert(sizeof(s_spawner_globals) == 0x2);

struct dynamic_firing_set_datum : s_datum_header
{
	byte __data[0x582];
};
static_assert(sizeof(dynamic_firing_set_datum) == 0x584);

struct prop_ref_datum : s_datum_header
{
	byte __data[0x3A];
};
static_assert(sizeof(prop_ref_datum) == 0x3C);

struct prop_datum : s_datum_header
{
	byte __data[0xC2];
};
static_assert(sizeof(prop_datum) == 0xC4);

struct tracking_datum : s_datum_header
{
	byte __data[0xFE];
};
static_assert(sizeof(tracking_datum) == 0x100);

struct joint_state_datum : s_datum_header
{
	byte __data[0xCA];
};
static_assert(sizeof(joint_state_datum) == 0xCC);

struct clump_datum : s_datum_header
{
	byte __data[0x106];
};
static_assert(sizeof(clump_datum) == 0x108);

struct squad_patrol_datum : s_datum_header
{
	byte __data[0x6C2];
};
static_assert(sizeof(squad_patrol_datum) == 0x6C4);

struct flock_datum : s_datum_header
{
	byte __data[0x4A];
};
static_assert(sizeof(flock_datum) == 0x4C);

struct formation_datum : s_datum_header
{
	byte __data[0x292];
};
static_assert(sizeof(formation_datum) == 0x294);

struct s_vision_mode_state
{
	byte __data[0xF0];
};
static_assert(sizeof(s_vision_mode_state) == 0xF0);

struct cluster_partition
{
	void* first_data;
	void* data_reference;
	void* cluster_first_data_references;
};

struct s_thread_local_storage
{
	void* __unknown0;
	void* __unknown4;

	//  name: "sim. gamestate entities"
	// count: 2048
	//  size: 0x10
	c_smart_data_array<simulation_gamestate_entity_datum> simulation_gamestate_entity_data;

	// name: "gamestate timing samples"
	// type: "global"
	// size: 0x14
	s_game_tick_time_samples* g_main_gamestate_timing_data;

	// name: "render timing samples"
	// type: "global"
	// size: 0x14
	s_game_tick_time_samples* g_main_render_timing_data;

	// name: "main_timing"
	// type: "globals"
	// size: 0x40
	s_main_time_globals* g_main_time_globals;

	// size: 0x84030
	// This is interacted with in the same manner as `g_font_cache_globals`,
	// using `c_global_preferences_scope_lock`
	s_global_preferences_internals_type* g_global_preferences;

	void* __unknown1C;

	// from assert
	long g_registered_thread_index;

	void* __unknown24;
	void* __unknown28;
	void* __unknown2C;
	void* __unknown30;

	// name: "random math"
	// type: "globals"
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
	c_smart_data_array<player_datum> player_data;

	// name: "players globals"
	// size: 0x234
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
	c_smart_data_array<breakable_surface_set_broken_event_datum> g_breakable_surface_set_broken_event_data;

	// name: "player mapping globals"
	// size: 0xE8
	s_player_mapping_globals* player_mapping_globals;

	// name: "director globals"
	// size: 0x5C0
	s_director_globals* director_globals;

	// name: "director scripting"
	// size: 0x1
	bool* director_camera_scripted;

	//  name: "det hs thread"
	// count: 320
	//  size: 0x524
	c_smart_data_array<hs_thread> hs_thread_deterministic_data;

	// name: "hs runtime globals"
	// size: 0x8
	s_hs_runtime_globals* hs_runtime_globals;

	//  name: "hs globals"
	// count: 3072
	//  size: 0x8
	c_smart_data_array<hs_global_data> hs_global_data;

	//  name: "hs dist. globals"
	// count: 512
	//  size: 0x2C
	c_smart_data_array<hs_distributed_global_data> hs_distributed_global_data;

	//  name: "tracking hs thread"
	// count: 320
	//  size: 0xC
	c_smart_data_array<hs_thread_tracking_data> hs_thread_tracking_data;

	//  name: "non-det hs thread"
	// count: 4
	//  size: 0x524
	c_smart_data_array<hs_thread> hs_thread_non_deterministic_data;

	// from assert
	void* g_restricted_address[k_total_restricted_memory_regions];

	// from assert
	bool g_restricted_alias[k_total_restricted_memory_regions];

	void* __unknown9C;

	//  name: "effect"
	// count: 640
	//  size: 0xA0
	c_smart_data_array<effect_datum> effect_data;

	//  name: "effect event"
	// count: 640
	//  size: 0x14
	c_smart_data_array<effect_event_datum> event_data;

	//  name: "effect location"
	// count: 1152
	//  size: 0x40
	c_smart_data_array<effect_location_datum> effect_location_data;

	// name: "effect counts"
	// size: 0x18
	s_effect_counts* g_effect_counts;

	//  name: "effect geometry sample"
	// count: 128
	//  size: 0x28
	c_smart_data_array<effect_geometry_sample_datum> effect_geometry_sample_data;

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
	c_smart_data_array<game_looping_sound_datum> game_looping_sound_data;

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
	c_static_array<c_static_flags<1024>, 16>* g_active_portal_bitvectors;
	static_assert(sizeof(*g_active_portal_bitvectors) == 0x800);

	// name: "campaign meta-game globals"
	// size: 0x1A158
	s_campaign_metagame_runtime_globals* g_campaign_metagame_globals;

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

	// The size marked as 0x14 in our research database
	// This is interacted with in the same manner as `g_global_preferences`,
	// using `c_font_cache_scope_lock`
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
	c_smart_data_array<s_havok_proxy_datum> g_havok_proxy_data;

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
	c_smart_data_array<recorded_animation_datum> animation_threads;

	// name: "game save globals"
	// size: 0x18
	s_game_save_globals* g_game_save_globals;

	//  name: "screen_effect"
	// count: 64
	//  size: 0x3C
	c_smart_data_array<screen_effect_datum> screen_effect_data;

	// name: "player effects"
	// size: 0x3A0
	s_player_effect_globals* player_effect_globals;

	// name: "scenario interpolator globals"
	// size: 0x204
	s_scenario_interpolator_globals* scenario_interpolator_globals;

	// name: "survival mode globals"
	// size: 0x7E0
	s_survival_mode_globals* g_survival_mode_globals;

	// name: "player training globals"
	// type: "globals"
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
	c_smart_data_array<c_decal_system> decal_system;

	// name: "decal counts"
	// size: 0x20
	s_decal_counts* decal_counts;

	//  name: "decal"
	// count: 856
	//  size: 0x130
	c_smart_data_array<c_decal> decal_data;

	// name: "decal messaging queue"
	// size: 0x824
	s_decal_message_queue* g_decal_message_queue;

	// name: "impact globals"
	// size: 0x8C
	s_impact_globals* g_impact_globals;

	//  name: "impacts"
	// count: 32
	//  size: 0xB4
	c_smart_data_array<c_impact> g_impact_data;

	//  name: "impact arrarys"
	// count: 32
	//  size: 0x88
	c_smart_data_array<impact_array_datum> g_impact_array_data;

	//  name: "object list header"
	// count: 48
	//  size: 0xC
	c_smart_data_array<object_list_header_datum> object_list_header_data;

	//  name: "list object"
	// count: 128
	//  size: 0xC
	c_smart_data_array<object_list_datum> object_list_data;

	// name: "scripted camera globals"
	// size: 0xF0
	scripted_camera_globals* scripted_camera_globals;

	//  name: "particles"
	// count: 512
	//  size: 0x7C
	data_array_base* particles;

	//  name: "particle_system"
	// count: 320
	//  size: 0x58
	c_smart_data_array<c_particle_system> particle_system;

	//  name: "contrail_system"
	// count: 160
	//  size: 0x30
	c_smart_data_array<c_contrail_system> contrail_system;

	//  name: "contrail"
	// count: 160
	//  size: 0x4C
	c_smart_data_array<c_contrail> contrail;

	//  name: "contrail"
	// count: 160
	//  size: 0x60
	c_smart_data_array<c_contrail_location> contrail_location;

	//  name: "contrail_profile"
	// count: 1
	//  size: 0x60
	c_smart_data_array<c_contrail_profile> contrail_profile;

	//  name: "particle_location"
	// count: 576
	//  size: 0x60
	c_smart_data_array<c_particle_location> particle_location;

	//  name: "light_volume_system"
	// count: 160
	//  size: 0x2C
	c_smart_data_array<c_light_volume_system> light_volume_system;

	//  name: "light_volume"
	// count: 160
	//  size: 0x2C
	c_smart_data_array<c_light_volume_location> light_volume_location;

	//  name: "light_volume"
	// count: 160
	//  size: 0x34
	c_smart_data_array<c_light_volume> light_volume;

	//  name: "beam_system"
	// count: 160
	//  size: 0x2C
	c_smart_data_array<c_beam_system> beam_system;

	//  name: "beam_system"
	// count: 160
	//  size: 0x34
	c_smart_data_array<c_beam> beam;

	//  name: "beam_system"
	// count: 160
	//  size: 0x30
	c_smart_data_array<c_beam_location> beam_location;

	// from assert
	bool g_havok_memory_allocator_locked;

	//  name: "ragdolls"
	// count: 5
	//  size: 0x130
	c_smart_data_array<s_ragdoll_datum> g_ragdoll_data;

	//  name: "particle_emitter"
	// count: 1088
	//  size: 0x90
	c_smart_data_array<c_particle_emitter> particle_emitter;

	// name: "rasterizer game states"
	// size: 0x208
	rasterizer_game_states* rasterizer_game_states;

	// name: "hue saturation control"
	// size: 0x14
	hue_saturation_control* hue_saturation_control;

	void* __unknown3C4;

	// name: "scripted exposure globals"
	// size: 0x34
	scripted_exposure_globals* scripted_exposure_globals;

	// name: "render hud globals"
	// size: 0x480
	render_hud_globals* render_hud_globals;

	// name: "interaction ripples"
	// size: 0x1400
	s_interaction_event* g_water_interaction_events;

	// name: "render texture globals"
	// size: 0x6C
	render_texture_globals* render_texture_globals;

	// name: "render game globals"
	// size: 0xD80
	render_game_globals* render_game_globals;

	// name: "DOF globals"
	// size: 0x40
	s_depth_of_field* depth_of_field_game_globals;

	//  name: "cached object render states"
	// count: 384
	//  size: 0x4D8
	c_smart_data_array<cached_object_render_state_datum> cached_object_render_states;

	//  name: "c_particle_emitter_gpu"
	// count: 1088
	//  size: 0x18
	// c_particle_emitter_gpu::x_data_array
	c_smart_data_array<c_particle_emitter_gpu> g_particle_emitter_gpu_data_array;

	//  name: "c_particle_emitter_gpu::s_row"
	// count: 448
	//  size: 0x14
	// c_particle_emitter_gpu::s_row::x_data_array
	c_smart_data_array<c_particle_emitter_gpu::s_row> g_particle_emitter_gpu_row_data_array;

	// name: "c_particle_emitter_gpu"
	// type: "x_gamestate_storage"
	// size: (16 * 448 * rasterizer_vertex_get_stride_from_declaration(c_rasterizer_vertex_buffer::get_usage_from_declaration(16)) + 0x7F) & ~0x7F
	// c_particle_emitter_gpu::x_gamestate_storage
	void* g_particle_emitter_gpu_gamestate_storage;

	//  name: "c_beam_gpu"
	// count: 160
	//  size: 0x10
	// c_beam_gpu::x_data_array
	c_smart_data_array<c_beam_gpu> g_beam_gpu_data_array;

	//  name: "c_beam_gpu::s_row"
	// count: 160
	//  size: 0x14
	// c_beam_gpu::s_row::x_data_array
	c_smart_data_array<c_beam_gpu::s_row> g_beam_gpu_row_data_array;

	// name: "c_particle_emitter_gpu"
	// type: "x_gamestate_storage"
	// size: (16 * 160 * rasterizer_vertex_get_stride_from_declaration(c_rasterizer_vertex_buffer::get_usage_from_declaration(29)) + 0x7F) & ~0x7F
	// c_beam_gpu::x_gamestate_storage
	void* g_beam_gpu_gamestate_storage;

	//  name: "c_beam_gpu::s_row"
	// count: 160
	//  size: 0x14
	// c_contrail_gpu::s_row::x_data_array;
	c_smart_data_array<c_contrail_gpu::s_row> g_contrail_gpu_row_data_array;

	//  name: "c_contrail_gpu"
	// count: 160
	//  size: 0x10
	// c_contrail_gpu::x_data_array;
	c_smart_data_array<c_contrail_gpu> g_contrail_gpu_data_array;

	// name: "c_particle_emitter_gpu"
	// type: "x_gamestate_storage"
	// size: (16 * 160 * rasterizer_vertex_get_stride_from_declaration(c_rasterizer_vertex_buffer::get_usage_from_declaration(29)) + 0x7F) & ~0x7F
	// c_contrail_gpu::x_gamestate_storage;
	void* g_contrail_gpu_gamestate_storage;

	// c_light_volume_gpu::x_data_array;
	// count: 160
	//  size: 0x10
	c_smart_data_array<c_light_volume_gpu> g_light_volume_gpu_data_array;

	// c_light_volume_gpu::s_row::x_data_array;
	// count: 160
	//  size: 0xC
	c_smart_data_array<c_light_volume_gpu::s_row> g_light_volume_gpu_row_data_array;

	// name: "c_particle_emitter_gpu"
	// type: "x_gamestate_storage"
	// size: (16 * 160 * rasterizer_vertex_get_stride_from_declaration(c_rasterizer_vertex_buffer::get_usage_from_declaration(19)) + 0x7F) & ~0x7F
	// c_light_volume_gpu::x_gamestate_storage;
	void* g_light_volume_gpu_gamestate_storage;

	// name: "rasterizer"
	// type: "implicit geometry data"
	// size: 0x2104
	s_rasterizer_implicit_geometry_globals* g_rasterizer_implicit_geometry_globals;

	// name: "render object"
	// type: "globals"
	// size: 0x3C040
	s_render_object_globals* render_object_globals;

	// name: "shield_render_cache_message"
	// count: 8
	//  size: 0x14
	c_smart_data_array<shield_render_cache_message> shield_render_cache_message;

	// name: "chud"
	// type: "persistent user data"
	// size: 0xFA40
	chud_persistent_user_data* chud_persistent_user_data;

	// name: "chud"
	// type: "persistent user data"
	// size: 0x470
	chud_persistent_global_data* chud_persistent_global_data;

	// name: "chud widgets"
	// count: 128
	//  size: 0x18
	// c_chud_manager::x_user_widget_data
	c_smart_data_array<chud_widget_datum> g_chud_manager_user_widget_data[4];

	// name: "fp orientations"
	// size: 0x12C00
	s_first_person_orientations* first_person_weapon_orientations;

	// name: "fp weapons"
	// size: 0x14000
	first_person_weapon* first_person_weapons;

	// name: "chud"
	// type: "cortana effect"
	// size: 0x10
	s_cortana_globals* g_cortana_globals;

	// name: "campaign objectives"
	// size: 0x14
	s_current_objective_state* g_objectives;

	//  name: "object"
	// count: 2048
	//  size: 0x18
	c_smart_data_array<object_header_datum> object_header_data;

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

	// name: "OBJ: Render Data"
	// type: "Render"
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
	c_smart_data_array<device_group_datum> device_groups_data;

	// name: "object scripting"
	// size: 0x304
	object_scripting* object_scripting;

	// name: "object_broadphase"
	// type: "global"
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
	c_static_flags<256>* g_trigger_activations;
	//object_activation_regions* g_object_activation_regions_data;

	//  name: "lights"
	// count: 400
	//  size: 0xE4
	c_smart_data_array<light_datum> light_data;

	// name: "lights globals"
	// size: 0x40
	lights_game_globals_definition* lights_game_globals;

	// name: "light"
	// size: 0xC
	cluster_partition light_cluster_partition;

	// name: "nondet_light_data"
	// type: "render lights"
	// size: 0x2580
	s_nondeterministic_light_data* g_nondeterministic_light_data;

	//  name: "widget"
	// count: 64
	//  size: 0xC
	c_smart_data_array<widget_datum> widget_data;

	// name: "recycling_volumes"
	// size: 0x148
	s_recycling_volumes* recycling_volumes;

	//  name: "recycling_group"
	// count: 128
	//  size: 0x14
	c_smart_data_array<recycling_group_datum> recycling_groups;

	//  name: "muffin"
	// count: 10
	//  size: 0x1890
	c_smart_data_array<muffin_datum> muffin_data;

	//  name: "leaf system"
	// count: 32
	//  size: 0x94C
	c_smart_data_array<c_leaf_system> leaf_system_data;

	//  name: "antenna_datum"
	// count: 12
	//  size: 0x64
	c_smart_data_array<antenna_datum> antenna_data;

	//  name: "cloth"
	// count: 8
	//  size: 0x1704
	c_smart_data_array<cloth_datum> cloth_data;


	// perhaps some or all of the following tls members are part saber memory allocation code,
	// and perhaps and fmod related memory allocations are using a saber provided memory allocator?

	// unknown saber/fmod memory allocation related
	long __unknown4CC;

	void* __unknown4D0;

	// saber strings? memory allocation related, a structure with a `char*` as the first struct member?
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

	// unknown saber/fmod memory allocation related
	long __unknown4FC;

	void* __unknown500;
	void* __unknown504;
	void* __unknown508;
	void* __unknown50C;
	void* __unknown510;
	void* __unknown514;
	void* __unknown518;


	//  name: "actor"
	// count: 256
	//  size: 0xA98
	c_smart_data_array<actor_datum> actor_data;

	//             name:  "actor firing-position owners"
	//   user_data_size: 4
	//     bucket_count: 0x400
	// maximum_elements: 0x100
	s_hash_table* g_actor_firing_position_hash;

	// name: "ai_reference_frame"
	// size: 0x4B0
	ai_reference_frame* ai_reference_frame;

	// name: "ai globals"
	// size: 0x688
	ai_globals_type* ai_globals;

	// name: "ai player state globals"
	// size: 0xB0
	ai_player_state* g_ai_players;

	//  name: "vocalization records"
	// count: 15
	//  size: 0x5C
	c_smart_data_array<vocalization_records> vocalization_record_data;

	// name: "vocalization timers"
	// size: 0xFB8
	vocalization_timers* vocalization_timers;

	//  name: "command scripts"
	// count: 40
	//  size: 0x188
	c_smart_data_array<command_script_datum> command_script_data;

	//  name: "objectives"
	// count: 125
	//  size: 0xC
	c_smart_data_array<s_objective> objectives_data;

	// name: "task records"
	// size: 0x61A80
	s_task_record* task_data;

	//  name: "squad"
	// count: 512
	//  size: 0xEC
	c_smart_data_array<squad_datum> squad_data;

	//  name: "squad group"
	// count: 130
	//  size: 0x24
	c_smart_data_array<squad_group_datum> squad_group_data;

	// name: "swarm"
	// count: 32
	//  size: 0x34
	c_smart_data_array<swarm_datum> swarm_data;

	// name: "swarm_spawner"
	// size: 0x258
	s_swarm_spawner* g_swarm_spawners;

	// name: "spawner_globals"
	// size: 0x2
	s_spawner_globals* spawner_globals;

	//  name: "dynamic firing points"
	// count: 15
	//  size: 0x584
	c_smart_data_array<dynamic_firing_set_datum> dynamic_firing_set_data;

	//  name: "prop_ref"
	// count: 1024
	//  size: 0x3C
	c_smart_data_array<prop_ref_datum> prop_ref_data;

	//  name: "prop"
	// count: 256
	//  size: 0xC4
	c_smart_data_array<prop_datum> prop_data;

	//  name: "tracking"
	// count: 100
	//  size: 0x100
	c_smart_data_array<tracking_datum> tracking_data;

	//  name: "joint state"
	// count: 20
	//  size: 0xCC
	c_smart_data_array<joint_state_datum> g_joint_data;

	//  name: "clump"
	// count: 20
	//  size: 0x108
	c_smart_data_array<clump_datum> clump_data;

	//  name: "squad_patrol"
	// count: 16
	//  size: 0x6C4
	c_smart_data_array<squad_patrol_datum> squad_patrol_data;

	//  name: "flocks"
	// count: 10
	//  size: 0x4C
	c_smart_data_array<flock_datum> flocks_data;

	//  name: "formations"
	// count: 16
	//  size: 0x294
	c_smart_data_array<formation_datum> formation_data;

	// name: "vision mode globals"
	// size: 0xF0
	s_vision_mode_state* g_vision_mode_state;

	void* __unknown580;
};
static_assert(sizeof(s_thread_local_storage) == 0x584);

#define TLS_DATA_GET_VALUE_REFERENCE(NAME) decltype(get_tls()->NAME)& NAME = get_tls()->NAME

extern s_thread_local_storage* get_tls();

