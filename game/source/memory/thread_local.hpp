#pragma once

#include "ai/ai.hpp"
#include "cache/restricted_memory.hpp"
#include "camera/observer.hpp"
#include "camera/camera.hpp"
#include "camera/camera_scripting.hpp"
#include "camera/director.hpp"
#include "devices/devices.hpp"
#include "game/game.hpp"
#include "gpu_particle/beam_gpu.hpp"
#include "gpu_particle/contrail_gpu.hpp"
#include "gpu_particle/light_volume_gpu.hpp"
#include "gpu_particle/particle_block.hpp"
#include "effects/effects.hpp"
#include "interface/user_interface_objectives.hpp"
#include "main/global_preferences.hpp"
#include "main/main_time.hpp"
#include "memory/data.hpp"
#include "memory/hashtable.hpp"
#include "multithreading/message_queue.hpp"
#include "objects/objects.hpp"
#include "objects/widgets/widgets.hpp"
#include "physics/breakable_surfaces.hpp"
#include "physics/havok.hpp"
#include "physics/impacts.hpp"
#include "physics/physics_constants.hpp"
#include "rasterizer/rasterizer_hue_saturation.hpp"
#include "render/render_objects_static_lighting.hpp"
#include "scenario/scenario_interpolators.hpp"
#include "simulation/simulation_gamestate_entities.hpp"
#include "structures/cluster_partitions.hpp"

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

struct hs_global_datum : s_datum_header
{
	word __unknown2;
	dword __unknown4;
};
static_assert(sizeof(hs_global_datum) == 0x8);

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

struct s_ragdoll_datum : s_datum_header
{
	byte __data[0x12E];
};
static_assert(sizeof(s_ragdoll_datum) == 0x130);

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

struct s_object_render_thread_message
{
	long object_index;
	short __unknown4;
	short __unknown6;
};
static_assert(sizeof(s_object_render_thread_message) == 0x8);

struct command_script_datum : s_datum_header
{
	byte __data[0x186];
};
static_assert(sizeof(command_script_datum) == 0x188);

struct s_thread_local_storage
{
	void* __unknown0;
	void* __unknown4;

	//  name: "sim. gamestate entities"
	// count: 2048
	//  size: 0x10
	c_smart_data_array<s_simulation_gamestate_entity> simulation_gamestate_entity_data;

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
	c_smart_data_array<s_breakable_surface_set_broken_event> g_breakable_surface_set_broken_event_data;

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
	c_smart_data_array<hs_global_datum> hs_global_data;

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
	t_message_queue<s_effect_message, 1024>* g_effect_message_queue;
	static_assert(sizeof(*g_effect_message_queue) == 0x17084);

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

	// there are potentially other locals in here
	void* __unknown114[131];

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
	c_smart_data_array<s_screen_effect_datum> screen_effect_data;

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
	// c_decal_system::x_data_array
	c_smart_data_array<c_decal_system> g_decal_system_data_array;

	// name: "decal counts"
	// size: 0x20
	s_decal_counts* decal_counts;

	//  name: "decal"
	// count: 856
	//  size: 0x130
	// c_decal::x_data_array
	c_smart_data_array<c_decal> g_decal_data_array;

	// name: "decal messaging queue"
	// size: 0x824
	c_decal_message_queue* g_decal_message_queue;

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
	s_scripted_camera_globals* g_scripted_camera_globals;

	//  name: "particles"
	// count: 512
	//  size: 0x7C
	// c_particle::x_data_array
	c_smart_data_array<c_particle>* g_particle_data_array;

	//  name: "particle_system"
	// count: 320
	//  size: 0x58
	// c_particle_system::x_data_array
	c_smart_data_array<c_particle_system> g_particle_system_data_array;

	//  name: "contrail_system"
	// count: 160
	//  size: 0x30
	// c_contrail_system::x_data_array
	c_smart_data_array<c_contrail_system> g_contrail_system_data_array;

	//  name: "contrail"
	// count: 160
	//  size: 0x4C
	// c_contrail::x_data_array
	c_smart_data_array<c_contrail> g_contrail_data_array;

	//  name: "contrail"
	// count: 160
	//  size: 0x60
	// c_contrail_location::x_data_array
	c_smart_data_array<c_contrail_location> g_contrail_location_data_array;

	//  name: "contrail_profile"
	// count: 1
	//  size: 0x60
	// c_contrail_profile::x_data_array
	c_smart_data_array<c_contrail_profile> g_contrail_profile_data_array;

	//  name: "particle_location"
	// count: 576
	//  size: 0x60
	// c_particle_location::x_data_array
	c_smart_data_array<c_particle_location> g_particle_location_data_array;

	//  name: "light_volume_system"
	// count: 160
	//  size: 0x2C
	// c_light_volume_system::x_data_array
	c_smart_data_array<c_light_volume_system> g_light_volume_system_data_array;

	//  name: "light_volume"
	// count: 160
	//  size: 0x2C
	// c_light_volume_location::x_data_array
	c_smart_data_array<c_light_volume_location> g_light_volume_location_data_array;

	//  name: "light_volume"
	// count: 160
	//  size: 0x34
	// c_light_volume::x_data_array
	c_smart_data_array<c_light_volume> g_light_volume_data_array;

	//  name: "beam_system"
	// count: 160
	//  size: 0x2C
	// c_beam_system::x_data_array
	c_smart_data_array<c_beam_system> g_beam_system_data_array;

	//  name: "beam"
	// count: 160
	//  size: 0x34
	// c_beam::x_data_array
	c_smart_data_array<c_beam> g_beam_data_array;

	//  name: "beam"
	// count: 160
	//  size: 0x30
	// c_beam_location::x_data_array
	c_smart_data_array<c_beam_location> g_beam_location_data_array;

	// from assert
	bool g_havok_memory_allocator_locked;

	//  name: "ragdolls"
	// count: 5
	//  size: 0x130
	c_smart_data_array<s_ragdoll_datum> g_ragdoll_data;

	//  name: "particle_emitter"
	// count: 1088
	//  size: 0x90
	// c_particle_emitter::x_data_array
	c_smart_data_array<c_particle_emitter> g_particle_emitter_data_array;

	// name: "rasterizer game states"
	// size: 0x208
	rasterizer_game_states* rasterizer_game_states;

	// name: "hue saturation control"
	// size: 0x14
	// c_hue_saturation_control::x_in_gamestate
	c_hue_saturation_control* hue_saturation_control;

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
	s_render_object_globals* g_render_object_globals_internal;

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
	t_message_queue<s_object_render_thread_message, 2048>* g_object_message_queue;
	static_assert(sizeof(*g_object_message_queue) == 0x4104);

	// name: "collideable object"
	// size: 0x14
	cluster_partition collideable_object_cluster_partition;

	// name: "noncollideable object"
	// size: 0x14
	cluster_partition noncollideable_object_cluster_partition;

	// name: "OBJ: Render Data"
	// type: "Render"
	// size: 0x2000
	s_object_render_data* g_object_render_data;

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
	s_object_scripting_state* g_object_scripting_state;

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
	c_smart_data_array<s_object_activation_region> g_object_activation_regions_data;

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
	c_smart_data_array<s_recycling_group> recycling_groups;

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
	ai_reference_frame* g_ai_reference_frame_data;

	// name: "ai globals"
	// size: 0x688
	ai_globals_type* ai_globals;

	// name: "ai player state globals"
	// size: 0xB0, 4 * sizeof(ai_player_state)
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
	s_task_record* tasks_data;

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

