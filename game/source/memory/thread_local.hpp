#pragma once

#include "ai/ai.hpp"
#include "ai/cl_engine.hpp"
#include "bink/bink_playback.hpp"
#include "cache/restricted_memory.hpp"
#include "camera/camera.hpp"
#include "camera/camera_scripting.hpp"
#include "camera/director.hpp"
#include "camera/observer.hpp"
#include "cutscene/cinematics.hpp"
#include "cutscene/recorded_animations.hpp"
#include "devices/devices.hpp"
#include "effects/decals.hpp"
#include "effects/effects.hpp"
#include "game/campaign_metagame.hpp"
#include "game/game.hpp"
#include "game/game_allegiance.hpp"
#include "game/game_save.hpp"
#include "game/player_control.hpp"
#include "game/player_rumble.hpp"
#include "game/player_training.hpp"
#include "gpu_particle/beam_gpu.hpp"
#include "gpu_particle/contrail_gpu.hpp"
#include "gpu_particle/light_volume_gpu.hpp"
#include "gpu_particle/particle_block.hpp"
#include "hs/hs_runtime.hpp"
#include "hs/object_lists.hpp"
#include "interface/chud/chud.hpp"
#include "interface/chud/cortana_effect.hpp"
#include "interface/first_person_weapons.hpp"
#include "interface/user_interface_objectives.hpp"
#include "main/global_preferences.hpp"
#include "main/main_time.hpp"
#include "memory/data.hpp"
#include "memory/hashtable.hpp"
#include "memory/memory_pool.hpp"
#include "multithreading/message_queue.hpp"
#include "multithreading/threads.hpp"
#include "objects/objects.hpp"
#include "objects/widgets/widgets.hpp"
#include "physics/breakable_surfaces.hpp"
#include "physics/havok.hpp"
#include "physics/havok_proxies.hpp"
#include "physics/impacts.hpp"
#include "physics/physics_constants.hpp"
#include "physics/ragdolls.hpp"
#include "rasterizer/rasterizer_hue_saturation.hpp"
#include "rasterizer/rasterizer_implicit_geometry.hpp"
#include "rasterizer/rasterizer_main.hpp"
#include "render/camera_fx_settings.hpp"
#include "render/depth_of_field.hpp"
#include "render/render_game_state.hpp"
#include "render/render_objects.hpp"
#include "render/render_objects_static_lighting.hpp"
#include "render/render_shield_cache.hpp"
#include "render/render_water.hpp"
#include "render/views/hud_camera_view.hpp"
#include "render/views/render_texture_camera_view.hpp"
#include "scenario/scenario_interpolators.hpp"
#include "scenario/scenario_kill_trigger_volumes.hpp"
#include "scenario/scenario_soft_ceilings.hpp"
#include "simulation/simulation_gamestate_entities.hpp"
#include "sky_atm/atmosphere.hpp"
#include "sound/game_sound.hpp"
#include "sound/game_sound_deterministic.hpp"
#include "sound/game_sound_player_effects.hpp"
#include "sound/sound_classes.hpp"
#include "structures/cluster_partitions.hpp"
#include "structures/structure_seams.hpp"

template<int32 index, void(__cdecl* tls_pre_overwrite_fixup_callback)(void*) = nullptr, void(__cdecl* tls_post_copy_fixup_callback)(void*) = nullptr, void(__cdecl* tls_update_callback)(void*) = nullptr>
struct t_restricted_allocation_manager :
	public c_allocation_base
{
public:
	//t_restricted_allocation_manager();
	//void* reserve_memory(const char* name, const char* type, unsigned int allocation, int32 alignment_bits);
	bool valid() const
	{
		return m_member_index != NONE && m_thread_id != NONE;
	}

protected:
	int32 m_member_index;
	int32 m_thread_id;
};
COMPILE_ASSERT(sizeof(t_restricted_allocation_manager<NONE>) == 0xC);

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

	// size: 0x84018
	// This is interacted with in the same manner as `g_font_cache_globals`,
	// using `c_global_preferences_scope_lock`
	s_global_preferences_internals_type* g_global_preferences;

	bool g_thread_assert_triggered;
	int32 g_registered_thread_index;
	s_thread_assert_arguments g_thread_assert_arguments;

	// name: "random math"
	// type: "globals"
	// size: 0x4
	uns32* g_deterministic_random_seed_ptr;

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
	players_global_data* players_globals;

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
	c_smart_data_array<hs_global_runtime> hs_global_data;

	//  name: "hs dist. globals"
	// count: 512
	//  size: 0x2C
	c_smart_data_array<s_hs_distributed_global_data> hs_distributed_global_data;

	//  name: "tracking hs thread"
	// count: 320
	//  size: 0xC
	c_smart_data_array<s_hs_thread_tracking_data> hs_thread_tracking_data;

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
	c_smart_data_array<event_datum> event_data;

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
	COMPILE_ASSERT(sizeof(*g_effect_message_queue) == 0x17084);

	// name: "effect lightprobes"
	// size: 0xFE00, sizeof(s_effect_lightprobe) * 128
	c_static_array<s_effect_lightprobe, 128>* effect_lightprobe_data;

	// name: "havok gamestate"
	// size: 0x8
	s_havok_gamestate* g_havok_game_state;

	// from assert
	int32 havok_style_fpu_exceptions_count;

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
	COMPILE_ASSERT(sizeof(*g_active_portal_bitvectors) == 0x800);

	// name: "campaign meta-game globals"
	// size: 0x1A158
	s_campaign_metagame_runtime_globals* g_campaign_metagame_runtime_globals;

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
	c_atmosphere_fog_interface* g_atmosphere_fog_globals;

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

	/* \\Saved Games\\blamlib\\Replays\\ */
	wchar_t blamlib_replays_path[260];

	// name: "cinematic new globals"
	// size: 0x2808
	s_cinematic_new_globals_definition* cinematic_new_globals;

	// name: "cinematic globals"
	// size: 0x3C
	s_cinematic_globals_definition* cinematic_globals;

	// name: "cinematic light globals"
	// size: 0xB2C8
	s_cinematic_light_globals* cinematic_light_globals;

	// name: "physics constants"
	// size: 0x20
	s_physics_constants* g_physics_constants;

	//  name: "recorded animations"
	// count: 1
	//  size: 0xA4
	c_smart_data_array<animation_thread> animation_threads;

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
	// size: 0x20, sizeof(s_decal_counts) * 2
	s_decal_counts* decal_counts;

	//  name: "decal"
	// count: 856
	//  size: 0x130
	// c_decal::x_data_array
	c_smart_data_array<c_decal> g_decal_data_array;

	// name: "decal messaging queue"
	// size: 0x824
	s_decal_message_queue* g_decal_message_queue;

	// name: "impact globals"
	// size: 0x8C
	impact_globals* g_impact_globals;

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
	s_rasterizer_game_states* g_rasterizer_game_states;

	// name: "hue saturation control"
	// size: 0x14
	// c_hue_saturation_control::x_in_gamestate
	c_hue_saturation_control* g_hue_saturation_control_in_gamestate;

	void* __unknown3C4;

	// name: "scripted exposure globals"
	// size: 0x34
	s_scripted_exposure* scripted_exposure_game_globals;

	// name: "render hud globals"
	// size: 0x480
	s_hud_camera_globals* g_hud_camera_globals;

	// name: "interaction ripples"
	// size: 0x1400, sizeof(s_new_interaction_event) * 128
	s_new_interaction_event* g_water_new_interaction_events;

	// name: "render texture globals"
	// size: 0x6C
	s_render_texture_camera_globals* g_render_texture_camera_globals;

	// name: "render game globals"
	// size: 0xD80
	s_render_game_state* render_game_globals;

	// name: "DOF globals"
	// size: 0x40
	s_depth_of_field* depth_of_field_game_globals;

	//  name: "cached object render states"
	// count: 384
	//  size: 0x4D8
	c_smart_data_array<object_render_state> cached_object_render_states;

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
	// size: 0xFA40, sizeof(c_chud_persistent_user_data) * 4
	// c_chud_manager::x_persistent_user_data
	c_chud_persistent_user_data* g_chud_manager_persistent_user_data;

	// name: "chud"
	// type: "persistent user data"
	// size: 0x470
	// c_chud_manager::x_persistent_global_data
	c_chud_persistent_global_data* g_chud_manager_persistent_global_data;

	// name: "chud widgets"
	// count: 128
	//  size: 0x18
	// c_chud_manager::x_user_widget_data
	c_smart_data_array<s_chud_runtime_widget_datum> g_chud_manager_user_widget_data[4];

	// name: "fp orientations"
	// size: 0x12C00
	s_first_person_orientations* first_person_weapon_orientations;

	// name: "fp weapons"
	// size: 0x14000, k_number_of_users * sizeof(first_person_weapon)
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
	s_memory_pool* object_memory_pool;

	// name: "object name list"
	// size: 0x2000
	c_static_array<int32, 2048>* g_object_name_list;

	// name: "object messaging queue"
	// size: 0x4104
	t_message_queue<s_object_render_thread_message, 2048>* g_object_message_queue;
	COMPILE_ASSERT(sizeof(*g_object_message_queue) == 0x4104);

	// name: "collideable object"
	// size: 0x14
	cluster_partition collideable_object_cluster_partition;

	// name: "noncollideable object"
	// size: 0x14
	cluster_partition noncollideable_object_cluster_partition;

	// name: "OBJ: Render Data"
	// type: "Render"
	// size: 0x2000, sizeof(s_object_render_data) * 2048
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
	s_object_schedule_globals* g_object_schedule_globals;

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
	c_smart_data_array<s_muffin_datum> muffin_data;

	//  name: "leaf system"
	// count: 32
	//  size: 0x94C
	c_smart_data_array<s_leaf_system_datum> leaf_system_data;

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
	int32 __unknown4CC;

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
	int32 __unknown4FC;

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
	// maximum_reference_frames_per_map = 150
	// size: 0x4B0, maximum_reference_frames_per_map * sizeof(ai_reference_frame)
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
	c_smart_data_array<s_vocalization_record> vocalization_record_data;

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
	c_smart_data_array<s_objective_datum> objectives_data;

	// name: "task records"
	// size: 0x61A80, 6250 * sizeof(s_task_record)
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
	// size: 0x258, 5 * sizeof(s_swarm_spawner)
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
	c_smart_data_array<s_squad_patrol_datum> squad_patrol_data;

	//  name: "flocks"
	// count: 10
	//  size: 0x4C
	c_smart_data_array<flock_datum> flocks_data;

	//  name: "formations"
	// count: 16
	//  size: 0x294
	c_smart_data_array<formation_datum> formation_data;

	// name: "vision mode globals"
	// size: 0xF0, 4 * sizeof(s_vision_mode_state)
	s_vision_mode_state* g_vision_mode_state;

	void* __unknown580;
};
COMPILE_ASSERT(sizeof(s_thread_local_storage) == 0x584);

// $TODO move all `t_restricted_allocation_manager` declarations to the correct files

extern t_restricted_allocation_manager<k_game_state_update_region>& g_simulation_gamestate_entity_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_main_gamestate_timing_data_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_main_render_timing_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_main_time_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_deterministic_random_seed_ptr_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_game_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_player_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_players_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_local_game_engine_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_game_engine_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_game_time_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_breakable_surface_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_breakable_surface_set_broken_event_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_player_mapping_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_director_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_director_camera_scripted_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_hs_thread_deterministic_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_hs_thread_tracking_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_hs_thread_non_deterministic_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_hs_global_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_hs_distributed_global_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_hs_runtime_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_effect_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_event_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_effect_location_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_effect_lightprobe_slot_data_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_effect_lightprobe_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_effect_counts_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_effect_message_queue_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_havok_game_state_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_player_control_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_player_control_globals_deterministic_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_game_looping_sound_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_game_sound_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_game_sound_scripted_impulses_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_structure_seam_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_active_portal_bitvectors_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_campaign_metagame_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_observer_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_observer_gamestate_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_rumble_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_bink_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_sound_class_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_game_allegiance_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_atmosphere_fog_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_scenario_soft_ceilings_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_player_effects_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_havok_proxy_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_cinematic_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_cinematic_new_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_cinematic_light_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_physics_constants_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_animation_threads_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_game_save_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_screen_effect_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_player_effect_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_scenario_interpolator_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_survival_mode_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_player_training_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_scenario_kill_trigger_volumes_state_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_future_development_allocator;
extern t_restricted_allocation_manager<k_global_render_data_region>& g_decal_counts_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_decal_message_queue_allocator;
extern t_restricted_allocation_manager<k_global_render_data_region>& g_decal_system_allocator;
extern t_restricted_allocation_manager<k_global_render_data_region>& g_decal_array_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_impact_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_impact_array_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_impact_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_object_list_header_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_object_list_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_camera_script_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_particles_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_particle_system_data_array_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_contrail_system_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_contrail_location_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_contrail_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_contrail_profile_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_particle_location_data_array_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_light_volume_system_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_light_volume_location_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_light_volume_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_beam_system_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_beam_location_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_beam_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_ragdoll_data_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_particle_emitter_data_array_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_rasterizer_game_states_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_hue_saturation_control_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_scripted_exposure_game_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_hud_camera_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_water_interaction_event_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_render_texture_camera_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_render_game_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_depth_of_field_game_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_cached_object_render_states_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_particle_emitter_gpu_x_data_array_manager;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_particle_emitter_gpu_s_row_x_data_array_manager;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_particle_emitter_gpu_x_gamestate_storage_manager;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_beam_gpu_x_data_array_manager;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_beam_gpu_s_row_x_data_array_manager;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_beam_gpu_x_gamestate_storage_manager;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_contrail_gpu_x_data_array_manager;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_contrail_gpu_s_row_x_data_array_manager;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_contrail_gpu_x_gamestate_storage_manager;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_light_volume_gpu_x_data_array_manager;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_light_volume_gpu_s_row_x_data_array_manager;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_light_volume_gpu_x_gamestate_storage_manager;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_rasterizer_implicit_geometry_data_allocator;
extern t_restricted_allocation_manager<k_global_render_data_region>& g_render_object_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_shield_render_cache_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_user_widget_allocator_0;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_user_widget_allocator_1;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_user_widget_allocator_2;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_user_widget_allocator_3;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_chud_persistent_user_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_chud_persistent_global_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_first_person_weapons_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_first_person_weapon_orientations_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_cortana_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_campaign_objectives_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_object_message_queue_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_object_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_object_header_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_object_name_list_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_object_render_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_object_memory_pool_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_collideable_first_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_collideable_data_reference_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_collideable_cluster_reference_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_noncollideable_first_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_noncollideable_data_reference_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_noncollideable_cluster_reference_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_damage_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_object_placement_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_device_groups_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_object_scripting_state_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_object_broadphase_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_object_early_movers_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_object_schedule_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_object_activation_region_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_light_first_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_light_data_reference_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_light_cluster_reference_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_light_data_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_light_render_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_lights_game_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_widget_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_recycling_groups_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_recycling_volumes_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_muffin_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_leaf_system_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_antenna_data_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_cloth_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_actor_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_ai_reference_frame_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_actor_firing_position_hash_allocator;
extern t_restricted_allocation_manager<k_game_state_shared_region>& g_ai_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_ai_players_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_team_dialogue_state_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_vocalization_record_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_command_script_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_objectives_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_tasks_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_squad_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_squad_group_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_swarm_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_swarm_spawners_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_spawner_globals_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_dynamic_firing_set_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_prop_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_prop_ref_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_tracking_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_joint_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_clump_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_squad_patrol_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_flocks_data_allocator;
extern t_restricted_allocation_manager<k_game_state_update_region>& g_formation_data_allocator;
extern t_restricted_allocation_manager<k_game_state_render_region>& g_vision_mode_state_allocator;

// $TODO move each TLS declaration to its actual location
#define DECLARE_TLS_VALUE_REFERENCE(NAME) extern thread_local decltype(s_thread_local_storage::NAME)& NAME

DECLARE_TLS_VALUE_REFERENCE(simulation_gamestate_entity_data);
DECLARE_TLS_VALUE_REFERENCE(g_main_gamestate_timing_data);
DECLARE_TLS_VALUE_REFERENCE(g_main_render_timing_data);
DECLARE_TLS_VALUE_REFERENCE(g_main_time_globals);
DECLARE_TLS_VALUE_REFERENCE(g_global_preferences);
DECLARE_TLS_VALUE_REFERENCE(g_thread_assert_triggered);
DECLARE_TLS_VALUE_REFERENCE(g_registered_thread_index);
DECLARE_TLS_VALUE_REFERENCE(g_thread_assert_arguments);
DECLARE_TLS_VALUE_REFERENCE(g_deterministic_random_seed_ptr);
DECLARE_TLS_VALUE_REFERENCE(g_file_errors_suppressed);
DECLARE_TLS_VALUE_REFERENCE(game_globals);
DECLARE_TLS_VALUE_REFERENCE(player_data);
DECLARE_TLS_VALUE_REFERENCE(players_globals);
DECLARE_TLS_VALUE_REFERENCE(game_engine_globals);
DECLARE_TLS_VALUE_REFERENCE(local_game_engine_globals);
DECLARE_TLS_VALUE_REFERENCE(game_time_globals);
DECLARE_TLS_VALUE_REFERENCE(breakable_surface_globals);
DECLARE_TLS_VALUE_REFERENCE(g_breakable_surface_set_broken_event_data);
DECLARE_TLS_VALUE_REFERENCE(player_mapping_globals);
DECLARE_TLS_VALUE_REFERENCE(director_globals);
DECLARE_TLS_VALUE_REFERENCE(director_camera_scripted);
DECLARE_TLS_VALUE_REFERENCE(hs_thread_deterministic_data);
DECLARE_TLS_VALUE_REFERENCE(hs_runtime_globals);
DECLARE_TLS_VALUE_REFERENCE(hs_global_data);
DECLARE_TLS_VALUE_REFERENCE(hs_distributed_global_data);
DECLARE_TLS_VALUE_REFERENCE(hs_thread_tracking_data);
DECLARE_TLS_VALUE_REFERENCE(hs_thread_non_deterministic_data);
DECLARE_TLS_VALUE_REFERENCE(g_restricted_address);
DECLARE_TLS_VALUE_REFERENCE(g_restricted_alias);
DECLARE_TLS_VALUE_REFERENCE(effect_data);
DECLARE_TLS_VALUE_REFERENCE(event_data);
DECLARE_TLS_VALUE_REFERENCE(effect_location_data);
DECLARE_TLS_VALUE_REFERENCE(g_effect_counts);
DECLARE_TLS_VALUE_REFERENCE(effect_geometry_sample_data);
DECLARE_TLS_VALUE_REFERENCE(g_effect_message_queue);
DECLARE_TLS_VALUE_REFERENCE(effect_lightprobe_data);
DECLARE_TLS_VALUE_REFERENCE(g_havok_game_state);
DECLARE_TLS_VALUE_REFERENCE(havok_style_fpu_exceptions_count);
DECLARE_TLS_VALUE_REFERENCE(player_control_globals);
DECLARE_TLS_VALUE_REFERENCE(player_control_globals_deterministic);
DECLARE_TLS_VALUE_REFERENCE(game_looping_sound_data);
DECLARE_TLS_VALUE_REFERENCE(game_sound_globals);
DECLARE_TLS_VALUE_REFERENCE(game_sound_scripted_impulses);
DECLARE_TLS_VALUE_REFERENCE(g_structure_seam_globals);
DECLARE_TLS_VALUE_REFERENCE(g_active_portal_bitvectors);
DECLARE_TLS_VALUE_REFERENCE(g_campaign_metagame_runtime_globals);
DECLARE_TLS_VALUE_REFERENCE(observer_gamestate_globals);
DECLARE_TLS_VALUE_REFERENCE(g_observer_globals);
DECLARE_TLS_VALUE_REFERENCE(rumble_globals);
DECLARE_TLS_VALUE_REFERENCE(bink_game_state);
DECLARE_TLS_VALUE_REFERENCE(g_font_cache_globals);
DECLARE_TLS_VALUE_REFERENCE(sound_class_data);
DECLARE_TLS_VALUE_REFERENCE(game_allegiance_globals);
DECLARE_TLS_VALUE_REFERENCE(g_atmosphere_fog_globals);
DECLARE_TLS_VALUE_REFERENCE(g_scenario_soft_ceilings_globals);
DECLARE_TLS_VALUE_REFERENCE(g_game_sound_player_effects_globals);
DECLARE_TLS_VALUE_REFERENCE(g_havok_proxy_data);
DECLARE_TLS_VALUE_REFERENCE(blamlib_replays_path);
DECLARE_TLS_VALUE_REFERENCE(cinematic_new_globals);
DECLARE_TLS_VALUE_REFERENCE(cinematic_globals);
DECLARE_TLS_VALUE_REFERENCE(cinematic_light_globals);
DECLARE_TLS_VALUE_REFERENCE(g_physics_constants);
DECLARE_TLS_VALUE_REFERENCE(animation_threads);
DECLARE_TLS_VALUE_REFERENCE(g_game_save_globals);
DECLARE_TLS_VALUE_REFERENCE(screen_effect_data);
DECLARE_TLS_VALUE_REFERENCE(player_effect_globals);
DECLARE_TLS_VALUE_REFERENCE(scenario_interpolator_globals);
DECLARE_TLS_VALUE_REFERENCE(g_survival_mode_globals);
DECLARE_TLS_VALUE_REFERENCE(player_training_globals);
DECLARE_TLS_VALUE_REFERENCE(g_scenario_kill_trigger_volumes_state);
DECLARE_TLS_VALUE_REFERENCE(g_future_development_game_sound_deterministic_globals);
DECLARE_TLS_VALUE_REFERENCE(g_decal_system_data_array);
DECLARE_TLS_VALUE_REFERENCE(decal_counts);
DECLARE_TLS_VALUE_REFERENCE(g_decal_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_decal_message_queue);
DECLARE_TLS_VALUE_REFERENCE(g_impact_globals);
DECLARE_TLS_VALUE_REFERENCE(g_impact_data);
DECLARE_TLS_VALUE_REFERENCE(g_impact_array_data);
DECLARE_TLS_VALUE_REFERENCE(object_list_header_data);
DECLARE_TLS_VALUE_REFERENCE(object_list_data);
DECLARE_TLS_VALUE_REFERENCE(g_scripted_camera_globals);
DECLARE_TLS_VALUE_REFERENCE(g_particle_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_particle_system_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_contrail_system_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_contrail_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_contrail_location_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_contrail_profile_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_particle_location_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_light_volume_system_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_light_volume_location_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_light_volume_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_beam_system_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_beam_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_beam_location_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_havok_memory_allocator_locked);
DECLARE_TLS_VALUE_REFERENCE(g_ragdoll_data);
DECLARE_TLS_VALUE_REFERENCE(g_particle_emitter_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_rasterizer_game_states);
DECLARE_TLS_VALUE_REFERENCE(g_hue_saturation_control_in_gamestate);
DECLARE_TLS_VALUE_REFERENCE(scripted_exposure_game_globals);
DECLARE_TLS_VALUE_REFERENCE(g_hud_camera_globals);
DECLARE_TLS_VALUE_REFERENCE(g_water_new_interaction_events);
DECLARE_TLS_VALUE_REFERENCE(g_render_texture_camera_globals);
DECLARE_TLS_VALUE_REFERENCE(render_game_globals);
DECLARE_TLS_VALUE_REFERENCE(depth_of_field_game_globals);
DECLARE_TLS_VALUE_REFERENCE(cached_object_render_states);
DECLARE_TLS_VALUE_REFERENCE(g_particle_emitter_gpu_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_particle_emitter_gpu_row_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_particle_emitter_gpu_gamestate_storage);
DECLARE_TLS_VALUE_REFERENCE(g_beam_gpu_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_beam_gpu_row_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_beam_gpu_gamestate_storage);
DECLARE_TLS_VALUE_REFERENCE(g_contrail_gpu_row_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_contrail_gpu_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_contrail_gpu_gamestate_storage);
DECLARE_TLS_VALUE_REFERENCE(g_light_volume_gpu_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_light_volume_gpu_row_data_array);
DECLARE_TLS_VALUE_REFERENCE(g_light_volume_gpu_gamestate_storage);
DECLARE_TLS_VALUE_REFERENCE(g_rasterizer_implicit_geometry_globals);
DECLARE_TLS_VALUE_REFERENCE(g_render_object_globals_internal);
DECLARE_TLS_VALUE_REFERENCE(shield_render_cache_message);
//DECLARE_TLS_VALUE_REFERENCE(g_chud_manager_persistent_user_data);
//DECLARE_TLS_VALUE_REFERENCE(g_chud_manager_persistent_global_data);
//DECLARE_TLS_VALUE_REFERENCE(g_chud_manager_user_widget_data);
DECLARE_TLS_VALUE_REFERENCE(first_person_weapon_orientations);
DECLARE_TLS_VALUE_REFERENCE(first_person_weapons);
DECLARE_TLS_VALUE_REFERENCE(g_cortana_globals);
DECLARE_TLS_VALUE_REFERENCE(g_objectives);
DECLARE_TLS_VALUE_REFERENCE(object_header_data);
DECLARE_TLS_VALUE_REFERENCE(object_globals);
DECLARE_TLS_VALUE_REFERENCE(object_memory_pool);
DECLARE_TLS_VALUE_REFERENCE(g_object_name_list);
DECLARE_TLS_VALUE_REFERENCE(g_object_message_queue);
DECLARE_TLS_VALUE_REFERENCE(collideable_object_cluster_partition);
DECLARE_TLS_VALUE_REFERENCE(noncollideable_object_cluster_partition);
DECLARE_TLS_VALUE_REFERENCE(g_object_render_data);
DECLARE_TLS_VALUE_REFERENCE(damage_globals);
DECLARE_TLS_VALUE_REFERENCE(object_placement_globals);
DECLARE_TLS_VALUE_REFERENCE(device_groups_data);
DECLARE_TLS_VALUE_REFERENCE(g_object_scripting_state);
DECLARE_TLS_VALUE_REFERENCE(g_object_broadphase);
DECLARE_TLS_VALUE_REFERENCE(g_object_early_movers_globals);
DECLARE_TLS_VALUE_REFERENCE(g_object_schedule_globals);
DECLARE_TLS_VALUE_REFERENCE(g_object_activation_regions_data);
DECLARE_TLS_VALUE_REFERENCE(light_data);
DECLARE_TLS_VALUE_REFERENCE(lights_game_globals);
DECLARE_TLS_VALUE_REFERENCE(light_cluster_partition);
DECLARE_TLS_VALUE_REFERENCE(g_nondeterministic_light_data);
DECLARE_TLS_VALUE_REFERENCE(widget_data);
DECLARE_TLS_VALUE_REFERENCE(recycling_volumes);
DECLARE_TLS_VALUE_REFERENCE(recycling_groups);
DECLARE_TLS_VALUE_REFERENCE(muffin_data);
DECLARE_TLS_VALUE_REFERENCE(leaf_system_data);
DECLARE_TLS_VALUE_REFERENCE(antenna_data);
DECLARE_TLS_VALUE_REFERENCE(cloth_data);
DECLARE_TLS_VALUE_REFERENCE(actor_data);
DECLARE_TLS_VALUE_REFERENCE(g_actor_firing_position_hash);
DECLARE_TLS_VALUE_REFERENCE(g_ai_reference_frame_data);
DECLARE_TLS_VALUE_REFERENCE(ai_globals);
DECLARE_TLS_VALUE_REFERENCE(g_ai_players);
DECLARE_TLS_VALUE_REFERENCE(vocalization_record_data);
DECLARE_TLS_VALUE_REFERENCE(vocalization_timers);
DECLARE_TLS_VALUE_REFERENCE(command_script_data);
DECLARE_TLS_VALUE_REFERENCE(objectives_data);
DECLARE_TLS_VALUE_REFERENCE(tasks_data);
DECLARE_TLS_VALUE_REFERENCE(squad_data);
DECLARE_TLS_VALUE_REFERENCE(squad_group_data);
DECLARE_TLS_VALUE_REFERENCE(swarm_data);
DECLARE_TLS_VALUE_REFERENCE(g_swarm_spawners);
DECLARE_TLS_VALUE_REFERENCE(spawner_globals);
DECLARE_TLS_VALUE_REFERENCE(dynamic_firing_set_data);
DECLARE_TLS_VALUE_REFERENCE(prop_ref_data);
DECLARE_TLS_VALUE_REFERENCE(prop_data);
DECLARE_TLS_VALUE_REFERENCE(tracking_data);
DECLARE_TLS_VALUE_REFERENCE(g_joint_data);
DECLARE_TLS_VALUE_REFERENCE(clump_data);
DECLARE_TLS_VALUE_REFERENCE(squad_patrol_data);
DECLARE_TLS_VALUE_REFERENCE(flocks_data);
DECLARE_TLS_VALUE_REFERENCE(formation_data);
DECLARE_TLS_VALUE_REFERENCE(g_vision_mode_state);

#undef DECLARE_TLS_VALUE_REFERENCE

