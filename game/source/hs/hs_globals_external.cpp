#include "hs/hs_globals_external.hpp"

#include "hs/hs_runtime.hpp"
#include "rasterizer/rasterizer_synchronization.hpp"

struct hs_global_external_release
{
	int16 type; // e_hs_type
	void* value;
};
static_assert(sizeof(hs_global_external_release) == 0x8);

enum : int16
{
	k_maximum_number_of_ms23_hs_globals = 1315
};

//REFERENCE_DECLARE(0x0167B188, const int16, k_hs_external_global_count);
//REFERENCE_DECLARE_ARRAY(0x018EF450, hs_global_external_release*, hs_external_globals_release, k_maximum_number_of_ms23_hs_globals);

#define EXTERNAL_GLOBAL_DECLARE(NAME, TYPE, VALUE, ...) \
static hs_global_external NAME##_definition \
{ \
	.name = #NAME, \
	.type = TYPE, \
	.pointer = &VALUE \
}

#define EXTERNAL_GLOBAL_DECLARE2(NAME, TYPE, VALUE_ADDRESS, ...) \
static hs_global_external NAME##_definition = \
{ \
	.name = #NAME, \
	.type = TYPE, \
	.pointer = (void*)VALUE_ADDRESS \
}

#pragma region HS_EXTERNAL_GLOBALS_DEFINITIONS

EXTERNAL_GLOBAL_DECLARE2(
	unknown0,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown1,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown2,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown3,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown4,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown5,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown6,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown7,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown8,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown9,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknownA,
	_hs_type_string_id,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknownB,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknownC,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknownD,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknownE,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknownF,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown10,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown11,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown12,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_controller_latency,
	_hs_type_boolean,
	0x02444006, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_physical_memory,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	terminal_render,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	events_debug_spam_render,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	console_dump_to_debug_display,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	camera_fov,
	_hs_type_real,
	0x018ECE00, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	camera_yaw_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	camera_pitch_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	camera_forward_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	camera_side_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	camera_up_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	flying_camera_maximum_boost_speed,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	flying_camera_movement_delay,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	flying_camera_has_collision,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	flying_camera_use_old_controls,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	editor_director_mouse_wheel_speed_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	async_display_statistics,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	suppress_upload_debug,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	lightmap_pointsample,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_no_frustum_clip,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_camera_projection,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_bink,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	game_paused,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	game_speed,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown2B,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown2C,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown2D,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown2E,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown2F,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown30,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown31,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown32,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown33,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown34,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown35,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown36,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown37,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown38,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown39,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown3A,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown3B,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown3C,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown3D,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown3E,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown3F,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown40,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown41,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown42,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown43,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown44,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown45,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown46,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown47,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown48,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	fake_precache_percentage,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	force_aligned_utility_drive,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_object_garbage_collection,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_object_dump_log,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	allow_all_sounds_on_player,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	disable_player_rotation,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	player_rotation_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_player_respawn,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	g_synchronous_client_maximum_catchup_chunk_size,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	g_editor_maximum_edited_object_speed,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	g_editor_edited_object_spring_constant,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	g_editor_maximum_rotation_speed,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	chud_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	chud_debug_grid,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	chud_debug_messages,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	chud_cortana_debug,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	chud_debug_crosshair,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	chud_debug_metagame,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_unit_all_animations,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_unit_animations,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_unit_illumination,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_unit_active_camo_frequency_modulator,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_unit_active_camo_frequency_modulator_bias,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown60,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown61,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown62,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown63,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown64,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown65,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown66,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown67,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown68,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown69,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown6A,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown6B,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown6C,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown6D,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown6E,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown6F,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown70,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown71,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown72,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown73,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown74,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown75,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	effects_enable,
	_hs_type_boolean,
	0x0243FB60, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown77,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown78,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown79,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown7A,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown7B,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown7C,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown7D,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown7E,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	sound_global_room_gain,
	_hs_type_real,
	0x0189D5B8, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	sound_direct_path_gain,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_pc_sound,
	_hs_type_boolean,
	0x02497D26, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown82,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown83,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown84,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown85,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown86,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown87,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown88,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown89,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown8A,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown8B,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown8C,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown8D,
	_hs_type_boolean,
	0x0238E86F, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown8E,
	_hs_type_boolean,
	0x0238E86E, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown8F,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown90,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown91,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown92,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown93,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown94,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown95,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown96,
	_hs_type_boolean,
	0x02444020, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown97,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown98,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown99,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown9A,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown9B,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown9C,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown9D,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_log_time,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_collision_tolerance,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_debug_mode,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_thread_count,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_environment_type,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_shape_radius,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_jumping_beans,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_disable_deactivation,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_weld_environment,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_batch_add_entities_disabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_shape_cache,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_shape_cache_debug,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_enable_back_stepping,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_enable_sweep_shapes,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_display_stats,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_initialize_profiling,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	impacts_debug,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	proxies_debug,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_damage_debug,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_play_impact_effects,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_play_biped_impact_effects,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	physics_debug,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_cleanup_inactive_agents,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	havok_memory_always_system,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	disable_expensive_physics_rebuilding,
	_hs_type_boolean,
	0x04FDE15C, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	minimum_havok_object_acceleration,
	_hs_type_real,
	0x01947890, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	minimum_havok_biped_object_acceleration,
	_hs_type_real,
	0x01947894, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_object_scheduler,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_cache_state,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_environment,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_objects,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	visibility_debug_use_old_sphere_test,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_lightmap_shadows,
	_hs_type_boolean,
	0x0191342C, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_lightmap_shadows_apply,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_lights,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_water_tessellated,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_water_wireframe,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_water_interaction,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_water,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_water_ripple_cutoff_distance,
	_hs_type_real,
	0x019146E0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_first_person,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_mode,
	_hs_type_short_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_safe_frame_bounds,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_colorbars,
	_hs_type_short_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_aspect_ratio_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_force_4x3_aspect_ratio,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_transparents,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_slow_transparents,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_transparents,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_transparent_cull,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_transparent_cull_flip,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_transparent_sort_method,
	_hs_type_short_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_lens_flares,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_instanced_geometry,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_sky,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_lens_flares,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_decorators,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	light_decorators,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_decorator_bounds,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_decorator_spheres,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_decorator_bsp_test_offset_scale_parameter,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknownDB,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknownDC,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknownDD,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknownDE,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknownDF,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknownE0,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknownE1,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknownE2,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknownE3,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknownE4,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_save_surface,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_disable_screen_effects_not_first_person,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_screen_flashes,
	_hs_type_boolean,
	0x018BF5ED, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	texture_camera_near_plane,
	_hs_type_real,
	0x019186E0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	texture_camera_exposure,
	_hs_type_real,
	0x019186E4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	texture_camera_illum_scale,
	_hs_type_real,
	0x019186E8, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_near_clip_distance,
	_hs_type_real,
	0x0191068C, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_far_clip_distance,
	_hs_type_real,
	0x01910690, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_exposure_stops,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	display_exposure,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_HDR_target_stops,
	_hs_type_real,
	0x019134C0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_surface_LDR_mode,
	_hs_type_long_integer,
	0x019100CC, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_surface_HDR_mode,
	_hs_type_long_integer,
	0x019100D0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_first_person_fov_scale,
	_hs_type_real,
	0x01913434, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	rasterizer_triliner_threshold,
	_hs_type_long_integer,
	0x019100D4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	rasterizer_present_immediate_threshold,
	_hs_type_long_integer,
	0x019100D8, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	tiling,
	_hs_type_long_integer,
	0x050DCA34, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_screen_res,
	_hs_type_long_integer,
	0x050DCA3C, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_force_tiling,
	_hs_type_boolean,
	0x050DB0FA, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_tiling_resolve_fragment_index,
	_hs_type_long_integer,
	0x050DCA20, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_tiling_viewport_offset_x,
	_hs_type_real,
	0x050DCA24, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_tiling_viewport_offset_y,
	_hs_type_real,
	0x050DCA28, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_true_gamma,
	_hs_type_boolean,
	0x019134BC, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_shadow_bounds,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_shadow_bounds_solid,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_shadow_opaque,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_shadow_screenspace,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_shadow_histencil,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_shadow_hires,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_shadow_objectspace_stencil_clip,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_shadow_force_fancy,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_shadow_force_old,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_postprocess,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_accum,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_bloom_source,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_persist,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_bloom,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_bling,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_downsample,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_show_alpha,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_postprocess_exposure,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_accum_filter,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_tone_curve,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_tone_curve_white,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_exposure_lock,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_postprocess_hue,
	_hs_type_real,
	0x050FB650, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_postprocess_saturation,
	_hs_type_real,
	0x050FB654, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_postprocess_red_filter,
	_hs_type_real,
	0x050FB658, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_postprocess_green_filter,
	_hs_type_real,
	0x050FB65C, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_postprocess_blue_filter,
	_hs_type_real,
	0x050FB660, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_screenspace_center,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_bounce_light_intensity,
	_hs_type_real,
	0x018F2FFC, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_bounce_light_only,
	_hs_type_boolean,
	0x04B1D840, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_disable_prt,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	force_render_lightmap_mesh,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	screenshot_anisotropic_level,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	screenshot_gamma,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_light_intensity,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_light_clip_planes,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_light_opaque,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	cubemap_debug,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_cloth,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_antennas,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_leaf_systems,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_object_lighting,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_object_lighting_refresh,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_use_cholocate_mountain,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_object_lighting_use_scenery_probe,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_object_lighting_use_device_probe,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_object_lighting_use_air_probe,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_show_air_probes,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_infinite_framerate,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_toggle_default_lightmaps_texaccum,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_toggle_default_static_lighting,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_toggle_default_dynamic_lighting,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_toggle_default_sfx,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_depth_render,
	_hs_type_long_integer,
	0x050FB3FC, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_depth_render_scale_r,
	_hs_type_real,
	0x019147BC, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_depth_render_scale_g,
	_hs_type_real,
	0x019147C0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_depth_render_scale_b,
	_hs_type_real,
	0x019147C4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_show_4x3_bounds,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_weather_bounds,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_cinematic_clip,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_buffer_gamma,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_screen_gamma,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_buffer_gamma_curve,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_color_balance_red,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_color_balance_green,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_color_balance_blue,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_black_level_red,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_black_level_green,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_black_level_blue,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_volume_samples,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	decal_create,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	decal_frame_advance,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	decal_render,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	decal_render_debug,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	decal_render_latest_debug,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	decal_cull,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	decal_fade,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	decal_dump,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	decal_z_bias,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	decal_slope_z_bias,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown14C,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown14D,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown14E,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown14F,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown150,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown151,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown152,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown153,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown154,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown155,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown156,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown157,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown158,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	effect_priority_cutoff,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	weather_occlusion_max_height,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_method_debug,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_viewport_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_light_probes,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	effect_property_culling,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	contrail_create,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	contrail_pulse,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	contrail_frame_advance,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	contrail_submit,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	contrail_dump,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	light_volume_create,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	light_volume_frame_advance,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	light_volume_submit,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	light_volume_dump,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	beam_create,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	beam_frame_advance,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	beam_submit,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	beam_dump,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_inactive_objects,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_pvs,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_pvs_render_all,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_pvs_activation,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	pvs_building_disabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_pvs_editor_mode,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_default_lighting,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	visibility_debug_portals,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	visibility_debug_audio_clusters,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	visibility_debug_visible_clusters,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	visibility_debug_portals_structure_bsp_index,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	visibility_debug_portals_cluster_index,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	error_geometry_draw_names,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	error_geometry_tangent_space,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	error_geometry_lightmap_lights,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_position_velocity,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_origin,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_root_node,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_bounding_spheres,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_attached_bounding_spheres,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_dynamic_render_bounding_spheres,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_render_models,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_collision_models,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_early_movers,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_contact_points,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_constraints,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_vehicle_physics,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_mass,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_physics_models,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_expensive_physics,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_water_physics,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	water_physics_velocity_minimum,
	_hs_type_real,
	0x018B5414, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	water_physics_velocity_maximum,
	_hs_type_real,
	0x018B5418, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_names,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_names_full,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_indices,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_functions,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_functions_all,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_model_targets,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_pathfinding,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_profile_times,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_node_bounds,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_unit_vectors,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_unit_seats,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_unit_mouth_apeture,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_unit_firing,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_unit_lipsync,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_unit_lipsync_verbose,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_unit_emotion,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_unit_acceleration,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_unit_camera,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_biped_autoaim_pills,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_biped_melee_in_range,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_physics_control_node,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_ground_plane,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_movement_mode,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_unit_pathfinding_surface,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_devices,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_machines,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_garbage,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_type_mask,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_sound_spheres,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_active_nodes,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_animation_times,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_animation,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_spawn_timers,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_freeze_ragdolls,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_disable_relaxation,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_sentinel_physics_ignore_lag,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_ignore_node_masks,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_force_awake,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_disable_node_animation,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_dump_memory_stats,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_object,
	_hs_type_object,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_by_index,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_player_only,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_vehicle_suspension,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_skeletons,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_cluster_counts,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_cluster_count_threshold,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_model_nodes,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_model_point_counts,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_model_vertex_counts,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_model_names,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_model_triangle_counts,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_model_collision_vertex_counts,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_model_collision_surface_counts,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_model_texture_usage,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_model_geometry_usage,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_model_cost,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_model_markers,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_model_no_geometry,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_model_skinning_disable,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_damage,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_player_damage,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_damage_verbose,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_damage_radius,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE(
	hs_verbose,
	_hs_type_boolean,
	hs_verbose,
);
EXTERNAL_GLOBAL_DECLARE2(
	breakpoints_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_trigger_volumes,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_trigger_volume_triangulation,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_point_physics,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	water_physics_debug,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_water_proxy,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_spray,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_features,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_phantom_bsp,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_lightmaps,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_geometry_sampling,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flags,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_structure,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_water,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_instanced_geometry,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_objects,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_objects_bipeds,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_objects_giants,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_objects_effect_scenery,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_objects_vehicles,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_objects_weapons,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_objects_equipment,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_objects_terminals,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_objects_projectiles,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_objects_scenery,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_objects_machines,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_objects_controls,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_objects_sound_scenery,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_objects_crates,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_objects_creatures,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_ignore_child_objects,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_ignore_nonpathfindable_objects,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_ignore_cinematic_objects,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_ignore_dead_bipeds,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_ignore_passthrough_bipeds,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_front_facing_surfaces,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_back_facing_surfaces,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_ignore_two_sided_surfaces,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_ignore_invisible_surfaces,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_ignore_breakable_surfaces,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_allow_early_out,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_flag_try_to_keep_location_valid,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_repeat,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_point_x,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_point_y,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_point_z,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_vector_i,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_vector_j,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_vector_k,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_length,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_width,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_height,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	collision_debug_ignore_object_index,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_obstacle_path,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_obstacle_path_on_failure,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_obstacle_path_start_point_x,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_obstacle_path_start_point_y,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_obstacle_path_goal_point_x,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_obstacle_path_goal_point_y,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	suppress_pathfinding_generation,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_pathfinding_generation_xbox,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_generate_flood_sector_wrl,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_pathfinding_generate_stats,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_zone_set_critical_portals,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_camera,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_tangent_space,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_player,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_player_control_autoaim_always_active,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_complexity,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_water,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_invisible,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_cluster_skies,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_slip_surfaces,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_soft_ceilings,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_soft_ceilings_biped,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_soft_ceilings_vehicle,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_soft_ceilings_huge_vehicle,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_soft_ceilings_camera,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_soft_ceilings_test_observer,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	soft_ceilings_disable,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_soft_kill,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_seam_edges,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_seams,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_seam_triangles,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_automatic,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_unique_colors,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_instanced_geometry,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_instanced_geometry_bounding_spheres,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_instanced_geometry_names,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_instanced_geometry_vertex_counts,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_instanced_geometry_collision_geometry,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_surface_references,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_structure_markers,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_bsp,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_plane_index,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_surface_index,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_input,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_leaf0_index,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_leaf1_index,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_leaf_connection_index,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_cluster_index,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_first_person_weapons,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_first_person_models,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	breakable_surfaces,
	_hs_type_boolean,
	0x0189F9BC, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_lights,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_light_passes,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_biped_landing,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_biped_throttle,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_biped_relaxation_pose,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_biped_node_velocities,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_collision_skip_instanced_geometry,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_collision_skip_objects,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_collision_skip_vectors,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_collision_object_payload_collision,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_material_effects,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_material_default_effects,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	player_training_debug,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	player_training_disable,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	game_engine_debug_statborg,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	jaime_control_hack,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	bertone_control_hack,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	motor_system_debug,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_profile_disable,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_profile_random,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_show,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_show_stats,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_show_actors,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_show_swarms,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_show_paths,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_show_line_of_sight,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_show_prop_types,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_show_sound_distance,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_all_actors,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_inactive_actors,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_lineoffire_crouching,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_lineoffire,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_lineofsight,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_ballistic_lineoffire,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_vision_cones,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_current_state,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_detailed_state,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_props,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_props_web,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_props_no_friends,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_props_unreachable,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_props_unopposable,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_props_stimulus,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_props_dialogue,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_props_salience,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_props_update,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_idle_look,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_support_surfaces,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_recent_damage,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_current_damage,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_threats,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_emotions,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_audibility,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_aiming_vectors,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_secondary_looking,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_targets,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_targets_all,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_targets_last_visible,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_states,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_vitality,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_evaluations,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_evaluations_detailed,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_evaluations_text,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_evaluations_shading,
	_hs_type_short_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_evaluations_shading_type,
	_hs_type_short_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_pursuit,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_shooting,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_trigger,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_projectile_aiming,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_aiming_validity,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_speech,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_leadership,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_status_flags,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_goal_state,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_behavior_debug,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_active_camo,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_vehicle_attachment,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_vehicle_reservations,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_actor_blinddeaf,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_morphing,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_look_orders,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_character_names,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_behavior_failure,
	_hs_type_ai_behavior,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_dialogue,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_dialogue_queue,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_dialogue_records,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_dialogue_player_weights,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_dialogue_test_mode,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_dialogue_datamine_enable,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_teams,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_player_ratings,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_spatial_effects,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_firing_positions,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_firing_position_statistics,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_firing_position_obstacles,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_mission_critical,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_gun_positions,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_aiming_positions,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_burst_geometry,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_vehicle_avoidance,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_vehicles_enterable,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_melee_check,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_dialogue_variants,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_grenades,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_danger_zones,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_control,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_activation,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_text,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_selected_only,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_destination,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_raw,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_current,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_failed,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_smoothed,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_avoided,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_error_thresholds,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_avoidance_segment,
	_hs_type_short_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_avoidance_obstacles,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_avoidance_search,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_nodes,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_nodes_all,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_nodes_polygons,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_nodes_costs,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_nodes_closest,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_paths_distance,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_player_aiming_blocked,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_squad_patrol_state,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_deceleration_obstacles,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_recent_obstacles,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_combat_range,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_dynamic_firing_positions,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_clumps,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_clump_props,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_clump_props_all,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_clump_dialogue,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_sectors,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_sector_bsps,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_giant_sector_bsps,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_sector_link_errors,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_intersection_links,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_non_walkable_sectors,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_threshold_links,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_sector_geometry_errors,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_pathfinding_generation_verbose,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_sectors_range_max,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_sectors_range_min,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_link_specific,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_links,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_user_hints,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_area_flight_hints,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_hints,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_hints_detailed,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_object_hints,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_object_hints_all,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_object_properties,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_hints_movement,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_orders_print_entries,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_orders_print_entries_verbose,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_orders,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_suppress_combat,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_squad_patrol,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_formations,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_objectives,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_strength,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_squad_fronts,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_squad_fronts_detailed,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_ai_iterator,
	_hs_type_ai,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_child_stack,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_behavior_stack,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_behavior_stack_all,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_stimuli,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_combat_status,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_decisions,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_decisions_all,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_command_scripts,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_script_data,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_hide_actor_errors,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_tracking_data,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_perception_data,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_combat_status,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_tracked_props,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_tracked_props_all,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_vignettes,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_joint_behaviors,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_swarm,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_flocks,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_vehicle_interest,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_player_battle_vector,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_player_needs_vehicle,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_prop_refresh,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_all_disposable,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_current_squad,
	_hs_type_ai,
	0x05840678, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_current_actor,
	_hs_type_ai,
	0x05840674, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_vehicle_turns,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_discarded_firing_positions,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_firing_positions_all,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_firing_position_info,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_inspect_avoidance_failure,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_render_action_selection_failure,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_combat_status_asleep,
	_hs_type_short_integer,
	0x019922D0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_combat_status_idle,
	_hs_type_short_integer,
	0x019922D2, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_combat_status_alert,
	_hs_type_short_integer,
	0x019922D4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_combat_status_active,
	_hs_type_short_integer,
	0x019922D6, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_combat_status_uninspected,
	_hs_type_short_integer,
	0x019922D8, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_combat_status_definite,
	_hs_type_short_integer,
	0x019922DA, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_combat_status_certain,
	_hs_type_short_integer,
	0x019922DC, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_combat_status_visible,
	_hs_type_short_integer,
	0x019922DE, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_combat_status_clear_los,
	_hs_type_short_integer,
	0x019922E0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_combat_status_dangerous,
	_hs_type_short_integer,
	0x019922E2, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_task_status_never,
	_hs_type_short_integer,
	0x019922D0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_task_status_occupied,
	_hs_type_short_integer,
	0x019922D2, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_task_status_empty,
	_hs_type_short_integer,
	0x019922D4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_task_status_inactive,
	_hs_type_short_integer,
	0x019922D6, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_task_status_exhausted,
	_hs_type_short_integer,
	0x019922D8, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_preference,
	_hs_type_short_integer,
	0x019922D0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_avoidance,
	_hs_type_short_integer,
	0x019922D2, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_sum,
	_hs_type_short_integer,
	0x019922D4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_pathfinding,
	_hs_type_short_integer,
	0x019922D0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_preferred_group,
	_hs_type_short_integer,
	0x019922D2, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_pursuit_walkdistance,
	_hs_type_short_integer,
	0x019922D4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_pursuit_targetdistance,
	_hs_type_short_integer,
	0x019922D6, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_pursuit_targethint,
	_hs_type_short_integer,
	0x019922D8, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_pursuit_visible,
	_hs_type_short_integer,
	0x019922DA, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_pursuit_examined_us,
	_hs_type_short_integer,
	0x019922DC, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_pursuit_examined_total,
	_hs_type_short_integer,
	0x019922DE, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_pursuit_available,
	_hs_type_short_integer,
	0x019922E0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_panic_walkdistance,
	_hs_type_short_integer,
	0x019922E2, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_panic_targetdistance,
	_hs_type_short_integer,
	0x019922E4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_panic_closetotarget,
	_hs_type_short_integer,
	0x019922E6, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_guard_walkdistance,
	_hs_type_short_integer,
	0x019922E8, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_attack_weaponrange,
	_hs_type_short_integer,
	0x019922EA, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_attack_idealrange,
	_hs_type_short_integer,
	0x019922EC, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_attack_visible,
	_hs_type_short_integer,
	0x019922EE, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_attack_dangerousenemy,
	_hs_type_short_integer,
	0x019922F0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_combatmove_walkdistance,
	_hs_type_short_integer,
	0x019922F2, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_combatmove_lineoffire,
	_hs_type_short_integer,
	0x019922F4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_hide_cover,
	_hs_type_short_integer,
	0x019922F6, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_hide_exposed,
	_hs_type_short_integer,
	0x019922F8, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_uncover_pre_evaluate,
	_hs_type_short_integer,
	0x019922FA, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_uncover_visible,
	_hs_type_short_integer,
	0x019922FC, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_uncover_blocked,
	_hs_type_short_integer,
	0x019922FE, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_previously_discarded,
	_hs_type_short_integer,
	0x01992300, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_danger_zone,
	_hs_type_short_integer,
	0x01992302, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_move_into_danger_zone,
	_hs_type_short_integer,
	0x01992304, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_3d_path_available,
	_hs_type_short_integer,
	0x01992306, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_point_avoidance,
	_hs_type_short_integer,
	0x01992308, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_point_preference,
	_hs_type_short_integer,
	0x0199230A, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_directional_driving,
	_hs_type_short_integer,
	0x0199230C, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_favor_former_firing_position,
	_hs_type_short_integer,
	0x0199230E, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_hide_pre_evaluation,
	_hs_type_short_integer,
	0x01992310, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_pursuit,
	_hs_type_short_integer,
	0x01992312, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_pursuit_area_discarded,
	_hs_type_short_integer,
	0x01992314, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_flag_preferences,
	_hs_type_short_integer,
	0x01992316, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_perch_preferences,
	_hs_type_short_integer,
	0x01992318, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_combatmove_lineoffire_occluded,
	_hs_type_short_integer,
	0x0199231A, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_attack_same_frame_of_reference,
	_hs_type_short_integer,
	0x0199231C, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_wall_leanable,
	_hs_type_short_integer,
	0x0199231E, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_cover_near_friends,
	_hs_type_short_integer,
	0x01992320, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_combat_move_near_follow_unit,
	_hs_type_short_integer,
	0x01992322, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_goal_preferences,
	_hs_type_short_integer,
	0x01992324, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_hint_plane,
	_hs_type_short_integer,
	0x01992326, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_postsearch_prefer_original,
	_hs_type_short_integer,
	0x01992328, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_leadership,
	_hs_type_short_integer,
	0x0199232A, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_flee_to_leader,
	_hs_type_short_integer,
	0x0199232C, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_goal_points_only,
	_hs_type_short_integer,
	0x0199232E, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_attack_leader_distance,
	_hs_type_short_integer,
	0x01992330, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_too_far_from_leader,
	_hs_type_short_integer,
	0x01992332, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_guard_preference,
	_hs_type_short_integer,
	0x01992334, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_guard_wall_preference,
	_hs_type_short_integer,
	0x01992336, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_obstacle,
	_hs_type_short_integer,
	0x01992338, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_facing,
	_hs_type_short_integer,
	0x0199233A, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_evaluator_hide_equipment,
	_hs_type_short_integer,
	0x0199233C, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_berserk,
	_hs_type_short_integer,
	0x019922D0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_surprise_front,
	_hs_type_short_integer,
	0x019922D2, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_surprise_back,
	_hs_type_short_integer,
	0x019922D4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_evade_left,
	_hs_type_short_integer,
	0x019922D6, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_evade_right,
	_hs_type_short_integer,
	0x019922D8, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_dive_forward,
	_hs_type_short_integer,
	0x019922DA, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_dive_back,
	_hs_type_short_integer,
	0x019922DC, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_dive_left,
	_hs_type_short_integer,
	0x019922DE, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_dive_right,
	_hs_type_short_integer,
	0x019922E0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_advance,
	_hs_type_short_integer,
	0x019922E2, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_cheer,
	_hs_type_short_integer,
	0x019922E4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_fallback,
	_hs_type_short_integer,
	0x019922E6, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_hold,
	_hs_type_short_integer,
	0x019922E8, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_point,
	_hs_type_short_integer,
	0x019922EA, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_posing,
	_hs_type_short_integer,
	0x019922EC, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_shakefist,
	_hs_type_short_integer,
	0x019922EE, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_signal_attack,
	_hs_type_short_integer,
	0x019922F0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_signal_move,
	_hs_type_short_integer,
	0x019922F2, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_taunt,
	_hs_type_short_integer,
	0x019922F4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_warn,
	_hs_type_short_integer,
	0x019922F6, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_action_wave,
	_hs_type_short_integer,
	0x019922F8, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_none,
	_hs_type_short_integer,
	0x019922D0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_patrol,
	_hs_type_short_integer,
	0x019922D2, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_stand,
	_hs_type_short_integer,
	0x019922D4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_crouch,
	_hs_type_short_integer,
	0x019922D6, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_stand_drawn,
	_hs_type_short_integer,
	0x019922D8, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_crouch_drawn,
	_hs_type_short_integer,
	0x019922DA, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_corner,
	_hs_type_short_integer,
	0x019922DC, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_corner_open,
	_hs_type_short_integer,
	0x019922DE, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_bunker,
	_hs_type_short_integer,
	0x019922E0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_bunker_open,
	_hs_type_short_integer,
	0x019922E2, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_combat,
	_hs_type_short_integer,
	0x019922E4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_backup,
	_hs_type_short_integer,
	0x019922E6, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_guard,
	_hs_type_short_integer,
	0x019922E8, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_guard_crouch,
	_hs_type_short_integer,
	0x019922EA, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_guard_wall,
	_hs_type_short_integer,
	0x019922EC, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_typing,
	_hs_type_short_integer,
	0x019922EE, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_kneel,
	_hs_type_short_integer,
	0x019922F0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_gaze,
	_hs_type_short_integer,
	0x019922F2, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_poke,
	_hs_type_short_integer,
	0x019922F4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_sniff,
	_hs_type_short_integer,
	0x019922F6, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_track,
	_hs_type_short_integer,
	0x019922F8, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_watch,
	_hs_type_short_integer,
	0x019922FA, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_examine,
	_hs_type_short_integer,
	0x019922FC, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_sleep,
	_hs_type_short_integer,
	0x019922FE, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_at_ease,
	_hs_type_short_integer,
	0x01992300, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_cower,
	_hs_type_short_integer,
	0x01992302, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_tai_chi,
	_hs_type_short_integer,
	0x01992304, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_pee,
	_hs_type_short_integer,
	0x01992306, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_doze,
	_hs_type_short_integer,
	0x01992308, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_eat,
	_hs_type_short_integer,
	0x0199230A, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_medic,
	_hs_type_short_integer,
	0x0199230C, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_work,
	_hs_type_short_integer,
	0x0199230E, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_cheering,
	_hs_type_short_integer,
	0x01992310, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_injured,
	_hs_type_short_integer,
	0x01992312, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_activity_captured,
	_hs_type_short_integer,
	0x01992314, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	morph_disallowed,
	_hs_type_boolean,
	0x05840694, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	morph_time_ranged_tank,
	_hs_type_real,
	0x01993070, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	morph_time_ranged_stealth,
	_hs_type_real,
	0x01993074, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	morph_time_tank_ranged,
	_hs_type_real,
	0x01993078, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	morph_time_tank_stealth,
	_hs_type_real,
	0x01993080, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	morph_time_stealth_ranged,
	_hs_type_real,
	0x01993084, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	morph_time_stealth_tank,
	_hs_type_real,
	0x01993088, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	morph_form_ranged,
	_hs_type_short_integer,
	0x01993090, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	morph_form_tank,
	_hs_type_short_integer,
	0x01993092, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	morph_form_stealth,
	_hs_type_short_integer,
	0x01993094, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_movement_patrol,
	_hs_type_short_integer,
	0x01992374, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_movement_asleep,
	_hs_type_short_integer,
	0x01992376, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_movement_combat,
	_hs_type_short_integer,
	0x01992378, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_movement_flee,
	_hs_type_short_integer,
	0x0199237A, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_movement_flaming,
	_hs_type_short_integer,
	0x0199237C, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_movement_stunned,
	_hs_type_short_integer,
	0x0199237E, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_movement_berserk,
	_hs_type_short_integer,
	0x01992380, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_print_major_upgrade,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_print_evaluation_statistics,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_print_communication,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_print_communication_player,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_print_vocalizations,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_print_placement,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_print_speech,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_print_allegiance,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_print_lost_speech,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_print_migration,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_print_scripting,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_print_disposal,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_print_killing_sprees,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_naimad_spew,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_maxd_spew,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_fast_los,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_evaluate_all_positions,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_path,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_path_start_freeze,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_path_end_freeze,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_path_flood,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_path_maximum_radius,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_path_attractor,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_path_attractor_radius,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_path_attractor_weight,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_path_accept_radius,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_path_radius,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_path_destructible,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_path_giant,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_ballistic_lineoffire_freeze,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_path_naive_estimate,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_blind,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_deaf,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_invisible_player,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_ignore_player,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_force_all_active,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_path_disable_smoothing,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ai_debug_path_disable_obstacle_avoidance,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_bitstream_debug,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_bitstream_display_errors,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_bitstream_capture_structure,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_never_timeout,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_use_local_time,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_traffic_warnings,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_traffic_print,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_messages_print,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_replication_requests_print,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_packet_print_mask,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_experimental,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_rate_unlimited,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_rate_server,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_rate_client,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_window_unlimited,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_window_size,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_bandwidth_unlimited,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_bandwidth_per_channel,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown3CC,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown3CD,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown3CE,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown3CF,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown3D0,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown3D1,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_maximum_machine_count,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_maximum_player_count,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_debug_random_seeds,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_allow_out_of_sync,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_distributed_always,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_distributed_never,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_matchmaking_force_gather,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_matchmaking_force_search,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_matchmaking_fail_arbitration,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_connectivity_model_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_nat_override,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_matchmaking_nat_check_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_matchmaking_hopper_id_adjustment,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_matchmaking_use_last_map_and_game,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_matchmaking_allow_early_start,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_matchmaking_skip_host_migration,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_matchmaking_force_disband,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_enable_host_migration_loop,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_matchmaking_fake_progress,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_matchmaking_force_no_joining,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_matchmaking_allow_idle_controllers,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_simulation_set_stream_bandwidth,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_set_channel_disconnect_interval,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_enable_block_detection,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_override_base_xp,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_override_ranked_games_played,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_matchmaking_mask_maps,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_status_memory,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_status_link,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_status_sim,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_status_channels,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_status_connections,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_status_message_queues,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_status_observer,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_status_sessions,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_status_leaderboard,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_status_leaderboard_mask,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_test,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_test_rate,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_test_update_server,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_test_update_client,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_test_replication_scheduler,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_test_debug_spheres,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_test_matchmaking_playlist_index,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_voice_diagnostics,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_http_failure_ratio,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_show_network_quality,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_fake_network_quality,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	sim_status_world,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	sim_status_views,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	sim_entity_validate,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	sim_disable_aim_assist,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	sim_bandwidth_eater,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_player_teleport,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_players,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_player_input,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_survival_mode,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	display_rumble_status_lines,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_pc_joystick,
	_hs_type_boolean,
	0x0189D580, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	texture_cache_show_mipmap_bias,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	texture_cache_graph,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	texture_cache_list,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	texture_cache_force_low_detail,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	texture_cache_force_medium_detail,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	texture_cache_force_high_detail,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	texture_cache_status,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	texture_cache_usage,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	texture_cache_block_warning,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	texture_cache_lod_bias,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	texture_cache_dynamic_low_detail_texture,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_low_res_textures,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	geometry_cache_graph,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	geometry_cache_list,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	geometry_cache_status,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	geometry_cache_block_warning,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	geometry_cache_never_block,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	geometry_cache_debug_display,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	director_camera_switch_fast,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	director_camera_switch_disable,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	director_camera_speed_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	director_disable_first_person,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	director_use_dt,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	observer_collision_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	observer_collision_anticipation_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	observer_collision_water_flags,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	g_observer_wave_height,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_recording,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_recording_newlines,
	_hs_type_short_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	cinematic_letterbox_style,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE(
	run_game_scripts,
	_hs_type_boolean,
	g_run_game_scripts,
);
EXTERNAL_GLOBAL_DECLARE2(
	vehicle_status_display,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	vehicle_disable_suspension_animations,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	vehicle_disable_acceleration_screens,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	biped_meter_display,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	display_verbose_disk_usage,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	display_disk_usage,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	default_scenario_ai_type,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_menu_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	catch_exceptions,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_first_person_hide_base,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_first_person_hide_movement,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_first_person_hide_jitter,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_first_person_hide_overlay,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_first_person_hide_pitch_turn,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_first_person_hide_ammo,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_first_person_hide_ik,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	global_playtest_mode,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	g_override_logon_task,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	g_logon_task_override_result_code,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ui_time_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ui_display_memory,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ui_memory_verify,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	xov_display_memory,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	gui_debug_text_bounds_global,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	gui_debug_bitmap_bounds_global,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	gui_debug_model_bounds_global,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	gui_debug_list_item_bounds_global,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	gui_debug_list_bounds_global,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	gui_debug_group_bounds_global,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	gui_debug_screen_bounds_global,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_comment_flags,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_comment_flags_text,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_comment_flags_look_at,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_controller_flag_drop,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	sapien_keyboard_toggle_for_camera_movement,
	_hs_type_boolean,
	0x050D9FB2, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	override_player_representation_index,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	e3,
	_hs_type_boolean,
	0x0471AFF5, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_tag_dependencies,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	disable_network_hopper_download,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	disable_network_configuration_download,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	check_system_heap,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	data_mine_player_update_interval,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	data_mine_mp_player_update_interval,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	data_mine_debug_menu_interval,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	data_mine_spam_enabled,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	webstats_file_zip_writes_per_frame,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_projectiles,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_damage_effects,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_damage_effect_obstacles,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	force_player_walking,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unit_animation_report_missing_animations,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	font_cache_status,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	font_cache_debug_texture,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	font_cache_debug_graph,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	font_cache_debug_list,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	lruv_lirp_enabled,
	_hs_type_boolean,
	0x0190A974, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	halt_on_stack_overflow,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	disable_progress_screen,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_thread_enable,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	character_force_physics,
	_hs_type_enum_character_physics_override,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_new_ik_method,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	animation_throttle_dampening_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	animation_blend_change_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	biped_fitting_enable,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	biped_fitting_root_offset_enable,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	biped_pivot_enable,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	biped_pivot_allow_player,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	giant_hunt_player,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	giant_hunting_min_radius,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	giant_hunting_max_radius,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	giant_hunting_throttle_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	giant_weapon_wait_time,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	giant_weapon_trigger_time,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	giant_ik,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	giant_foot_ik,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	giant_ankle_ik,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	giant_elevation_control,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	giant_force_buckle,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	giant_force_crouch,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	giant_force_death,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	giant_buckle_rotation,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_giant_feet,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_giant_buckle,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_xsync_timings,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	allow_restricted_tag_groups_to_load,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	xsync_restricted_tag_groups,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_cache_build_resources,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	xma_compression_level_default,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_console_window,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	display_colors_in_banded_gamma,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	use_tool_command_legacy,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	maximum_tool_command_history,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	disable_unit_aim_screens,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	disable_unit_look_screens,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	disable_unit_eye_tracking,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_tag_resource_xsync,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	dont_recompile_shaders,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	use_temp_directory_for_files,
	_hs_type_boolean,
	0x0238EF60, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	scenario_load_all_tags,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	synchronization_debug,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	profiler_pulse_rates,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	profiler_collection_interval,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_scenery,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	disable_switch_zone_sets,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	facial_animation_testing_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	profiler_datamine_uploads_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_object_recycling,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_sound_over_network,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	lsp_allow_lsp_connections,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	lsp_allow_raw_connections,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	lsp_service_id_override,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown49B,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown49C,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown49D,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown49E,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown49F,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown4A0,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown4A1,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown4A2,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown4A3,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown4A4,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	aiming_interpolation_stop_delta,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	aiming_interpolation_start_delta,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	aiming_interpolation_rate,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	airborne_arc_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	airborne_descent_test_duration,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	airborne_descent_test_count,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_amortized_prediction,
	_hs_type_boolean,
	0x018BAC61, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	amortized_prediction_object_batch_size,
	_hs_type_long_integer,
	0x018BAC9C, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_tag_resource_prediction,
	_hs_type_boolean,
	0x018BAC5C, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_entire_pvs_prediction,
	_hs_type_boolean,
	0x018BAC5F, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_cluster_objects_prediction,
	_hs_type_boolean,
	0x018BAC60, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	disable_main_loop_throttle,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	force_unit_walking,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	leap_force_start_rotation,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	leap_force_end_rotation,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	leap_force_flight_start_rotation,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	leap_force_flight_end_rotation,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	leap_flight_path_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	leap_departure_power,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	leap_departure_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	leap_arrival_power,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	leap_arrival_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	leap_twist_power,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	leap_cannonball_power,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	leap_cannonball_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	leap_idle_power,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	leap_idle_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	leap_overlay_power,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	leap_overlay_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	leap_interpolation_limit,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	biped_fake_soft_landing,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	biped_fake_hard_landing,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	biped_soft_landing_recovery_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	biped_hard_landing_recovery_scale,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	biped_landing_absorbtion,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_player_network_aiming,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	aim_assist_disable_target_lead_vector,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_tag_edit_sync,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	instance_default_fade_start_pixels,
	_hs_type_long_integer,
	0x018BF6B4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	instance_default_fade_end_pixels,
	_hs_type_long_integer,
	0x018BF6B8, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_dont_flash_low_res_textures,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_pendulum,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ui_alpha,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ui_alpha_lockdown,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ui_alpha_eula_accepted,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ui_alpha_custom_games_enabled,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_config_client_badness_rating_threshold_override,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_config_disable_bad_client_anticheating_override,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_config_disable_bad_connectivity_anticheating_override,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_config_disable_bad_bandwidth_anticheating_override,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_config_maximum_multiplayer_split_screen_override,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_config_crash_handling_minidump_type_override,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	net_config_crash_handling_ui_display_override,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	online_files_slowdown,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_trace_main_events,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	force_xsync_memory_buyback,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	bitmaps_trim_unused_pixels,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	bitmaps_interleave_compressed_bitmaps,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	ignore_predefined_performance_throttles,
	_hs_type_boolean,
	0x05115AB4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_first_person_prediction,
	_hs_type_boolean,
	0x018BAC5D, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_structure_prediction,
	_hs_type_boolean,
	0x018BAC5E, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_structure_audibility,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_sound_transmission,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	minidump_force_regular_minidump_with_ui,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	giant_custom_anim_recovery_time,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	facial_animation_enable_lipsync,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	facial_animation_enable_gestures,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	facial_animation_enable_noise,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_alpha_to_coverage,
	_hs_type_boolean,
	0x050DB0FB, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	rasterizer_disable_vsync,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	scale_ui_to_maintain_aspect_ratio,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	maximum_aspect_ratio_scale_percentage,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	enable_sound_transmission,
	_hs_type_boolean,
	0x0189D5C0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	disable_audibility_generation,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	motion_blur_expected_dt,
	_hs_type_real,
	0x01914BA4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	motion_blur_max_x,
	_hs_type_real,
	0x01914B90, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	motion_blur_max_y,
	_hs_type_real,
	0x01914B94, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	motion_blur_scale_x,
	_hs_type_real,
	0x01914B98, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	motion_blur_scale_y,
	_hs_type_real,
	0x01914B9C, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	motion_blur_center_falloff,
	_hs_type_real,
	0x01914BA0, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unicode_warn_on_truncation,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_determinism_version,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_determinism_compatible_version,
	_hs_type_long_integer,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	error_geometry_environment_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	error_geometry_lightmap_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	error_geometry_seam_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	error_geometry_instance_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	error_geometry_object_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_unit_melee,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	force_buffer_2_frames,
	_hs_type_boolean,
	0x0471AFF7, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	disable_render_state_cache_optimization,
	_hs_type_boolean,
	0x05106FAD, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	utility_drive_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_objects_root_node_print,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE(
	enable_better_cpu_gpu_sync,
	_hs_type_boolean,
	g_enable_better_cpu_gpu_sync_hs_setting,
);
EXTERNAL_GLOBAL_DECLARE2(
	require_secure_cache_files,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	debug_aim_assist_targets_enabled,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	motion_blur_max_viewport_count,
	_hs_type_long_integer,
	0x01914BA8, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	cinematic_prediction_enable,
	_hs_type_boolean,
	0x018BAC62, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	render_debug_cortana_ticks,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	skies_delete_on_zone_set_switch_enable,
	_hs_type_boolean,
	0x05287B90, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	reduce_widescreen_fov_during_cinematics,
	_hs_type_boolean,
	0x01918050, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown50A,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown50B,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown50C,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown50D,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown50E,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown50F,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown510,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown511,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown512,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown513,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown514,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown515,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown516,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown517,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown518,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown519,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown51A,
	_hs_type_boolean,
	0x024E00A4, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown51B,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown51C,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown51D,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	load_time_multiplier,
	_hs_type_real,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown51F,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown520,
	_hs_type_boolean,
	0x0526A086, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown521,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);
EXTERNAL_GLOBAL_DECLARE2(
	unknown522,
	_hs_type_boolean,
	NULL, // $TODO write the global chuckle nuts
);

#pragma endregion // HS_EXTERNAL_GLOBALS_DEFINITIONS

static hs_global_external* const hs_external_globals[]
{
	&unknown0_definition,
	&unknown1_definition,
	&unknown2_definition,
	&unknown3_definition,
	&unknown4_definition,
	&unknown5_definition,
	&unknown6_definition,
	&unknown7_definition,
	&unknown8_definition,
	&unknown9_definition,
	&unknownA_definition,
	&unknownB_definition,
	&unknownC_definition,
	&unknownD_definition,
	&unknownE_definition,
	&unknownF_definition,
	&unknown10_definition,
	&unknown11_definition,
	&unknown12_definition,
	&debug_controller_latency_definition,
	&debug_physical_memory_definition,
	&terminal_render_definition,
	&events_debug_spam_render_definition,
	&console_dump_to_debug_display_definition,
	&camera_fov_definition,
	&camera_yaw_scale_definition,
	&camera_pitch_scale_definition,
	&camera_forward_scale_definition,
	&camera_side_scale_definition,
	&camera_up_scale_definition,
	&flying_camera_maximum_boost_speed_definition,
	&flying_camera_movement_delay_definition,
	&flying_camera_has_collision_definition,
	&flying_camera_use_old_controls_definition,
	&editor_director_mouse_wheel_speed_enabled_definition,
	&async_display_statistics_definition,
	&suppress_upload_debug_definition,
	&lightmap_pointsample_definition,
	&debug_no_frustum_clip_definition,
	&debug_camera_projection_definition,
	&debug_bink_definition,
	&game_paused_definition,
	&game_speed_definition,
	&unknown2B_definition,
	&unknown2C_definition,
	&unknown2D_definition,
	&unknown2E_definition,
	&unknown2F_definition,
	&unknown30_definition,
	&unknown31_definition,
	&unknown32_definition,
	&unknown33_definition,
	&unknown34_definition,
	&unknown35_definition,
	&unknown36_definition,
	&unknown37_definition,
	&unknown38_definition,
	&unknown39_definition,
	&unknown3A_definition,
	&unknown3B_definition,
	&unknown3C_definition,
	&unknown3D_definition,
	&unknown3E_definition,
	&unknown3F_definition,
	&unknown40_definition,
	&unknown41_definition,
	&unknown42_definition,
	&unknown43_definition,
	&unknown44_definition,
	&unknown45_definition,
	&unknown46_definition,
	&unknown47_definition,
	&unknown48_definition,
	&fake_precache_percentage_definition,
	&force_aligned_utility_drive_definition,
	&debug_object_garbage_collection_definition,
	&debug_object_dump_log_definition,
	&allow_all_sounds_on_player_definition,
	&disable_player_rotation_definition,
	&player_rotation_scale_definition,
	&debug_player_respawn_definition,
	&g_synchronous_client_maximum_catchup_chunk_size_definition,
	&g_editor_maximum_edited_object_speed_definition,
	&g_editor_edited_object_spring_constant_definition,
	&g_editor_maximum_rotation_speed_definition,
	&chud_enabled_definition,
	&chud_debug_grid_definition,
	&chud_debug_messages_definition,
	&chud_cortana_debug_definition,
	&chud_debug_crosshair_definition,
	&chud_debug_metagame_definition,
	&debug_unit_all_animations_definition,
	&debug_unit_animations_definition,
	&debug_unit_illumination_definition,
	&debug_unit_active_camo_frequency_modulator_definition,
	&debug_unit_active_camo_frequency_modulator_bias_definition,
	&unknown60_definition,
	&unknown61_definition,
	&unknown62_definition,
	&unknown63_definition,
	&unknown64_definition,
	&unknown65_definition,
	&unknown66_definition,
	&unknown67_definition,
	&unknown68_definition,
	&unknown69_definition,
	&unknown6A_definition,
	&unknown6B_definition,
	&unknown6C_definition,
	&unknown6D_definition,
	&unknown6E_definition,
	&unknown6F_definition,
	&unknown70_definition,
	&unknown71_definition,
	&unknown72_definition,
	&unknown73_definition,
	&unknown74_definition,
	&unknown75_definition,
	&effects_enable_definition,
	&unknown77_definition,
	&unknown78_definition,
	&unknown79_definition,
	&unknown7A_definition,
	&unknown7B_definition,
	&unknown7C_definition,
	&unknown7D_definition,
	&unknown7E_definition,
	&sound_global_room_gain_definition,
	&sound_direct_path_gain_definition,
	&enable_pc_sound_definition,
	&unknown82_definition,
	&unknown83_definition,
	&unknown84_definition,
	&unknown85_definition,
	&unknown86_definition,
	&unknown87_definition,
	&unknown88_definition,
	&unknown89_definition,
	&unknown8A_definition,
	&unknown8B_definition,
	&unknown8C_definition,
	&unknown8D_definition,
	&unknown8E_definition,
	&unknown8F_definition,
	&unknown90_definition,
	&unknown91_definition,
	&unknown92_definition,
	&unknown93_definition,
	&unknown94_definition,
	&unknown95_definition,
	&unknown96_definition,
	&unknown97_definition,
	&unknown98_definition,
	&unknown99_definition,
	&unknown9A_definition,
	&unknown9B_definition,
	&unknown9C_definition,
	&unknown9D_definition,
	&collision_log_time_definition,
	&havok_collision_tolerance_definition,
	&havok_debug_mode_definition,
	&havok_thread_count_definition,
	&havok_environment_type_definition,
	&havok_shape_radius_definition,
	&havok_jumping_beans_definition,
	&havok_disable_deactivation_definition,
	&havok_weld_environment_definition,
	&havok_batch_add_entities_disabled_definition,
	&havok_shape_cache_definition,
	&havok_shape_cache_debug_definition,
	&havok_enable_back_stepping_definition,
	&havok_enable_sweep_shapes_definition,
	&havok_display_stats_definition,
	&havok_initialize_profiling_definition,
	&impacts_debug_definition,
	&proxies_debug_definition,
	&collision_damage_debug_definition,
	&havok_play_impact_effects_definition,
	&havok_play_biped_impact_effects_definition,
	&physics_debug_definition,
	&havok_cleanup_inactive_agents_definition,
	&havok_memory_always_system_definition,
	&disable_expensive_physics_rebuilding_definition,
	&minimum_havok_object_acceleration_definition,
	&minimum_havok_biped_object_acceleration_definition,
	&debug_object_scheduler_definition,
	&render_debug_cache_state_definition,
	&render_environment_definition,
	&render_objects_definition,
	&visibility_debug_use_old_sphere_test_definition,
	&render_lightmap_shadows_definition,
	&render_lightmap_shadows_apply_definition,
	&render_lights_definition,
	&render_water_tessellated_definition,
	&render_water_wireframe_definition,
	&render_water_interaction_definition,
	&render_water_definition,
	&render_water_ripple_cutoff_distance_definition,
	&render_first_person_definition,
	&render_debug_mode_definition,
	&render_debug_safe_frame_bounds_definition,
	&render_debug_colorbars_definition,
	&render_debug_aspect_ratio_scale_definition,
	&render_debug_force_4x3_aspect_ratio_definition,
	&render_debug_transparents_definition,
	&render_debug_slow_transparents_definition,
	&render_transparents_definition,
	&render_debug_transparent_cull_definition,
	&render_debug_transparent_cull_flip_definition,
	&render_debug_transparent_sort_method_definition,
	&render_debug_lens_flares_definition,
	&render_instanced_geometry_definition,
	&render_sky_definition,
	&render_lens_flares_definition,
	&render_decorators_definition,
	&light_decorators_definition,
	&render_decorator_bounds_definition,
	&render_decorator_spheres_definition,
	&render_decorator_bsp_test_offset_scale_parameter_definition,
	&unknownDB_definition,
	&unknownDC_definition,
	&unknownDD_definition,
	&unknownDE_definition,
	&unknownDF_definition,
	&unknownE0_definition,
	&unknownE1_definition,
	&unknownE2_definition,
	&unknownE3_definition,
	&unknownE4_definition,
	&render_debug_save_surface_definition,
	&render_disable_screen_effects_not_first_person_definition,
	&render_screen_flashes_definition,
	&texture_camera_near_plane_definition,
	&texture_camera_exposure_definition,
	&texture_camera_illum_scale_definition,
	&render_near_clip_distance_definition,
	&render_far_clip_distance_definition,
	&render_exposure_stops_definition,
	&display_exposure_definition,
	&render_HDR_target_stops_definition,
	&render_surface_LDR_mode_definition,
	&render_surface_HDR_mode_definition,
	&render_first_person_fov_scale_definition,
	&rasterizer_triliner_threshold_definition,
	&rasterizer_present_immediate_threshold_definition,
	&tiling_definition,
	&render_screen_res_definition,
	&render_force_tiling_definition,
	&render_tiling_resolve_fragment_index_definition,
	&render_tiling_viewport_offset_x_definition,
	&render_tiling_viewport_offset_y_definition,
	&render_true_gamma_definition,
	&render_shadow_bounds_definition,
	&render_shadow_bounds_solid_definition,
	&render_shadow_opaque_definition,
	&render_shadow_screenspace_definition,
	&render_shadow_histencil_definition,
	&render_shadow_hires_definition,
	&render_shadow_objectspace_stencil_clip_definition,
	&render_shadow_force_fancy_definition,
	&render_shadow_force_old_definition,
	&render_postprocess_definition,
	&render_accum_definition,
	&render_bloom_source_definition,
	&render_persist_definition,
	&render_bloom_definition,
	&render_bling_definition,
	&render_downsample_definition,
	&render_show_alpha_definition,
	&render_postprocess_exposure_definition,
	&render_accum_filter_definition,
	&render_tone_curve_definition,
	&render_tone_curve_white_definition,
	&render_exposure_lock_definition,
	&render_postprocess_hue_definition,
	&render_postprocess_saturation_definition,
	&render_postprocess_red_filter_definition,
	&render_postprocess_green_filter_definition,
	&render_postprocess_blue_filter_definition,
	&render_screenspace_center_definition,
	&render_bounce_light_intensity_definition,
	&render_bounce_light_only_definition,
	&render_disable_prt_definition,
	&force_render_lightmap_mesh_definition,
	&screenshot_anisotropic_level_definition,
	&screenshot_gamma_definition,
	&render_light_intensity_definition,
	&render_light_clip_planes_definition,
	&render_light_opaque_definition,
	&cubemap_debug_definition,
	&render_debug_cloth_definition,
	&render_debug_antennas_definition,
	&render_debug_leaf_systems_definition,
	&render_debug_object_lighting_definition,
	&render_debug_object_lighting_refresh_definition,
	&render_debug_use_cholocate_mountain_definition,
	&render_debug_object_lighting_use_scenery_probe_definition,
	&render_debug_object_lighting_use_device_probe_definition,
	&render_debug_object_lighting_use_air_probe_definition,
	&render_debug_show_air_probes_definition,
	&render_debug_infinite_framerate_definition,
	&render_debug_toggle_default_lightmaps_texaccum_definition,
	&render_debug_toggle_default_static_lighting_definition,
	&render_debug_toggle_default_dynamic_lighting_definition,
	&render_debug_toggle_default_sfx_definition,
	&render_debug_depth_render_definition,
	&render_debug_depth_render_scale_r_definition,
	&render_debug_depth_render_scale_g_definition,
	&render_debug_depth_render_scale_b_definition,
	&render_debug_show_4x3_bounds_definition,
	&render_weather_bounds_definition,
	&render_debug_cinematic_clip_definition,
	&render_buffer_gamma_definition,
	&render_screen_gamma_definition,
	&render_buffer_gamma_curve_definition,
	&render_color_balance_red_definition,
	&render_color_balance_green_definition,
	&render_color_balance_blue_definition,
	&render_black_level_red_definition,
	&render_black_level_green_definition,
	&render_black_level_blue_definition,
	&debug_volume_samples_definition,
	&decal_create_definition,
	&decal_frame_advance_definition,
	&decal_render_definition,
	&decal_render_debug_definition,
	&decal_render_latest_debug_definition,
	&decal_cull_definition,
	&decal_fade_definition,
	&decal_dump_definition,
	&decal_z_bias_definition,
	&decal_slope_z_bias_definition,
	&unknown14C_definition,
	&unknown14D_definition,
	&unknown14E_definition,
	&unknown14F_definition,
	&unknown150_definition,
	&unknown151_definition,
	&unknown152_definition,
	&unknown153_definition,
	&unknown154_definition,
	&unknown155_definition,
	&unknown156_definition,
	&unknown157_definition,
	&unknown158_definition,
	&effect_priority_cutoff_definition,
	&weather_occlusion_max_height_definition,
	&render_method_debug_definition,
	&render_debug_viewport_scale_definition,
	&render_debug_light_probes_definition,
	&effect_property_culling_definition,
	&contrail_create_definition,
	&contrail_pulse_definition,
	&contrail_frame_advance_definition,
	&contrail_submit_definition,
	&contrail_dump_definition,
	&light_volume_create_definition,
	&light_volume_frame_advance_definition,
	&light_volume_submit_definition,
	&light_volume_dump_definition,
	&beam_create_definition,
	&beam_frame_advance_definition,
	&beam_submit_definition,
	&beam_dump_definition,
	&debug_inactive_objects_definition,
	&debug_pvs_definition,
	&debug_pvs_render_all_definition,
	&debug_pvs_activation_definition,
	&pvs_building_disabled_definition,
	&debug_pvs_editor_mode_definition,
	&render_default_lighting_definition,
	&visibility_debug_portals_definition,
	&visibility_debug_audio_clusters_definition,
	&visibility_debug_visible_clusters_definition,
	&visibility_debug_portals_structure_bsp_index_definition,
	&visibility_debug_portals_cluster_index_definition,
	&error_geometry_draw_names_definition,
	&error_geometry_tangent_space_definition,
	&error_geometry_lightmap_lights_definition,
	&debug_objects_definition,
	&debug_objects_position_velocity_definition,
	&debug_objects_origin_definition,
	&debug_objects_root_node_definition,
	&debug_objects_bounding_spheres_definition,
	&debug_objects_attached_bounding_spheres_definition,
	&debug_objects_dynamic_render_bounding_spheres_definition,
	&debug_objects_render_models_definition,
	&debug_objects_collision_models_definition,
	&debug_objects_early_movers_definition,
	&debug_objects_contact_points_definition,
	&debug_objects_constraints_definition,
	&debug_objects_vehicle_physics_definition,
	&debug_objects_mass_definition,
	&debug_objects_physics_models_definition,
	&debug_objects_expensive_physics_definition,
	&debug_objects_water_physics_definition,
	&water_physics_velocity_minimum_definition,
	&water_physics_velocity_maximum_definition,
	&debug_objects_names_definition,
	&debug_objects_names_full_definition,
	&debug_objects_indices_definition,
	&debug_objects_functions_definition,
	&debug_objects_functions_all_definition,
	&debug_objects_model_targets_definition,
	&debug_objects_pathfinding_definition,
	&debug_objects_profile_times_definition,
	&debug_objects_node_bounds_definition,
	&debug_objects_unit_vectors_definition,
	&debug_objects_unit_seats_definition,
	&debug_objects_unit_mouth_apeture_definition,
	&debug_objects_unit_firing_definition,
	&debug_objects_unit_lipsync_definition,
	&debug_objects_unit_lipsync_verbose_definition,
	&debug_objects_unit_emotion_definition,
	&debug_objects_unit_acceleration_definition,
	&debug_objects_unit_camera_definition,
	&debug_objects_biped_autoaim_pills_definition,
	&debug_objects_biped_melee_in_range_definition,
	&debug_objects_physics_control_node_definition,
	&debug_objects_ground_plane_definition,
	&debug_objects_movement_mode_definition,
	&debug_objects_unit_pathfinding_surface_definition,
	&debug_objects_devices_definition,
	&debug_objects_machines_definition,
	&debug_objects_garbage_definition,
	&debug_objects_type_mask_definition,
	&debug_objects_sound_spheres_definition,
	&debug_objects_active_nodes_definition,
	&debug_objects_animation_times_definition,
	&debug_objects_animation_definition,
	&debug_objects_spawn_timers_definition,
	&debug_objects_freeze_ragdolls_definition,
	&debug_objects_disable_relaxation_definition,
	&debug_objects_sentinel_physics_ignore_lag_definition,
	&debug_objects_ignore_node_masks_definition,
	&debug_objects_force_awake_definition,
	&debug_objects_disable_node_animation_definition,
	&debug_objects_dump_memory_stats_definition,
	&debug_objects_object_definition,
	&debug_objects_by_index_definition,
	&debug_objects_player_only_definition,
	&debug_objects_vehicle_suspension_definition,
	&debug_objects_skeletons_definition,
	&debug_objects_cluster_counts_definition,
	&debug_objects_cluster_count_threshold_definition,
	&render_model_nodes_definition,
	&render_model_point_counts_definition,
	&render_model_vertex_counts_definition,
	&render_model_names_definition,
	&render_model_triangle_counts_definition,
	&render_model_collision_vertex_counts_definition,
	&render_model_collision_surface_counts_definition,
	&render_model_texture_usage_definition,
	&render_model_geometry_usage_definition,
	&render_model_cost_definition,
	&render_model_markers_definition,
	&render_model_no_geometry_definition,
	&render_model_skinning_disable_definition,
	&debug_damage_definition,
	&debug_player_damage_definition,
	&debug_damage_verbose_definition,
	&debug_damage_radius_definition,
	&hs_verbose_definition,
	&breakpoints_enabled_definition,
	&debug_trigger_volumes_definition,
	&debug_trigger_volume_triangulation_definition,
	&debug_point_physics_definition,
	&water_physics_debug_definition,
	&collision_debug_definition,
	&collision_debug_water_proxy_definition,
	&collision_debug_spray_definition,
	&collision_debug_features_definition,
	&collision_debug_phantom_bsp_definition,
	&collision_debug_lightmaps_definition,
	&collision_debug_geometry_sampling_definition,
	&collision_debug_flags_definition,
	&collision_debug_flag_structure_definition,
	&collision_debug_flag_water_definition,
	&collision_debug_flag_instanced_geometry_definition,
	&collision_debug_flag_objects_definition,
	&collision_debug_flag_objects_bipeds_definition,
	&collision_debug_flag_objects_giants_definition,
	&collision_debug_flag_objects_effect_scenery_definition,
	&collision_debug_flag_objects_vehicles_definition,
	&collision_debug_flag_objects_weapons_definition,
	&collision_debug_flag_objects_equipment_definition,
	&collision_debug_flag_objects_terminals_definition,
	&collision_debug_flag_objects_projectiles_definition,
	&collision_debug_flag_objects_scenery_definition,
	&collision_debug_flag_objects_machines_definition,
	&collision_debug_flag_objects_controls_definition,
	&collision_debug_flag_objects_sound_scenery_definition,
	&collision_debug_flag_objects_crates_definition,
	&collision_debug_flag_objects_creatures_definition,
	&collision_debug_flag_ignore_child_objects_definition,
	&collision_debug_flag_ignore_nonpathfindable_objects_definition,
	&collision_debug_flag_ignore_cinematic_objects_definition,
	&collision_debug_flag_ignore_dead_bipeds_definition,
	&collision_debug_flag_ignore_passthrough_bipeds_definition,
	&collision_debug_flag_front_facing_surfaces_definition,
	&collision_debug_flag_back_facing_surfaces_definition,
	&collision_debug_flag_ignore_two_sided_surfaces_definition,
	&collision_debug_flag_ignore_invisible_surfaces_definition,
	&collision_debug_flag_ignore_breakable_surfaces_definition,
	&collision_debug_flag_allow_early_out_definition,
	&collision_debug_flag_try_to_keep_location_valid_definition,
	&collision_debug_repeat_definition,
	&collision_debug_point_x_definition,
	&collision_debug_point_y_definition,
	&collision_debug_point_z_definition,
	&collision_debug_vector_i_definition,
	&collision_debug_vector_j_definition,
	&collision_debug_vector_k_definition,
	&collision_debug_length_definition,
	&collision_debug_width_definition,
	&collision_debug_height_definition,
	&collision_debug_ignore_object_index_definition,
	&debug_obstacle_path_definition,
	&debug_obstacle_path_on_failure_definition,
	&debug_obstacle_path_start_point_x_definition,
	&debug_obstacle_path_start_point_y_definition,
	&debug_obstacle_path_goal_point_x_definition,
	&debug_obstacle_path_goal_point_y_definition,
	&suppress_pathfinding_generation_definition,
	&enable_pathfinding_generation_xbox_definition,
	&ai_generate_flood_sector_wrl_definition,
	&ai_pathfinding_generate_stats_definition,
	&debug_zone_set_critical_portals_definition,
	&debug_camera_definition,
	&debug_tangent_space_definition,
	&debug_player_definition,
	&debug_player_control_autoaim_always_active_definition,
	&debug_structure_definition,
	&debug_structure_complexity_definition,
	&debug_structure_water_definition,
	&debug_structure_invisible_definition,
	&debug_structure_cluster_skies_definition,
	&debug_structure_slip_surfaces_definition,
	&debug_structure_soft_ceilings_definition,
	&debug_structure_soft_ceilings_biped_definition,
	&debug_structure_soft_ceilings_vehicle_definition,
	&debug_structure_soft_ceilings_huge_vehicle_definition,
	&debug_structure_soft_ceilings_camera_definition,
	&debug_structure_soft_ceilings_test_observer_definition,
	&soft_ceilings_disable_definition,
	&debug_structure_soft_kill_definition,
	&debug_structure_seam_edges_definition,
	&debug_structure_seams_definition,
	&debug_structure_seam_triangles_definition,
	&debug_structure_automatic_definition,
	&debug_structure_unique_colors_definition,
	&debug_instanced_geometry_definition,
	&debug_instanced_geometry_bounding_spheres_definition,
	&debug_instanced_geometry_names_definition,
	&debug_instanced_geometry_vertex_counts_definition,
	&debug_instanced_geometry_collision_geometry_definition,
	&debug_structure_surface_references_definition,
	&debug_structure_markers_definition,
	&debug_bsp_definition,
	&debug_plane_index_definition,
	&debug_surface_index_definition,
	&debug_input_definition,
	&debug_leaf0_index_definition,
	&debug_leaf1_index_definition,
	&debug_leaf_connection_index_definition,
	&debug_cluster_index_definition,
	&debug_first_person_weapons_definition,
	&debug_first_person_models_definition,
	&breakable_surfaces_definition,
	&debug_lights_definition,
	&debug_light_passes_definition,
	&debug_biped_landing_definition,
	&debug_biped_throttle_definition,
	&debug_biped_relaxation_pose_definition,
	&debug_biped_node_velocities_definition,
	&debug_collision_skip_instanced_geometry_definition,
	&debug_collision_skip_objects_definition,
	&debug_collision_skip_vectors_definition,
	&debug_collision_object_payload_collision_definition,
	&debug_material_effects_definition,
	&debug_material_default_effects_definition,
	&player_training_debug_definition,
	&player_training_disable_definition,
	&game_engine_debug_statborg_definition,
	&jaime_control_hack_definition,
	&bertone_control_hack_definition,
	&motor_system_debug_definition,
	&ai_profile_disable_definition,
	&ai_profile_random_definition,
	&ai_show_definition,
	&ai_show_stats_definition,
	&ai_show_actors_definition,
	&ai_show_swarms_definition,
	&ai_show_paths_definition,
	&ai_show_line_of_sight_definition,
	&ai_show_prop_types_definition,
	&ai_show_sound_distance_definition,
	&ai_render_definition,
	&ai_render_all_actors_definition,
	&ai_render_inactive_actors_definition,
	&ai_render_lineoffire_crouching_definition,
	&ai_render_lineoffire_definition,
	&ai_render_lineofsight_definition,
	&ai_render_ballistic_lineoffire_definition,
	&ai_render_vision_cones_definition,
	&ai_render_current_state_definition,
	&ai_render_detailed_state_definition,
	&ai_render_props_definition,
	&ai_render_props_web_definition,
	&ai_render_props_no_friends_definition,
	&ai_render_props_unreachable_definition,
	&ai_render_props_unopposable_definition,
	&ai_render_props_stimulus_definition,
	&ai_render_props_dialogue_definition,
	&ai_render_props_salience_definition,
	&ai_render_props_update_definition,
	&ai_render_idle_look_definition,
	&ai_render_support_surfaces_definition,
	&ai_render_recent_damage_definition,
	&ai_render_current_damage_definition,
	&ai_render_threats_definition,
	&ai_render_emotions_definition,
	&ai_render_audibility_definition,
	&ai_render_aiming_vectors_definition,
	&ai_render_secondary_looking_definition,
	&ai_render_targets_definition,
	&ai_render_targets_all_definition,
	&ai_render_targets_last_visible_definition,
	&ai_render_states_definition,
	&ai_render_vitality_definition,
	&ai_render_evaluations_definition,
	&ai_render_evaluations_detailed_definition,
	&ai_render_evaluations_text_definition,
	&ai_render_evaluations_shading_definition,
	&ai_render_evaluations_shading_type_definition,
	&ai_render_pursuit_definition,
	&ai_render_shooting_definition,
	&ai_render_trigger_definition,
	&ai_render_projectile_aiming_definition,
	&ai_render_aiming_validity_definition,
	&ai_render_speech_definition,
	&ai_render_leadership_definition,
	&ai_render_status_flags_definition,
	&ai_render_goal_state_definition,
	&ai_render_behavior_debug_definition,
	&ai_render_active_camo_definition,
	&ai_render_vehicle_attachment_definition,
	&ai_render_vehicle_reservations_definition,
	&ai_render_actor_blinddeaf_definition,
	&ai_render_morphing_definition,
	&ai_render_look_orders_definition,
	&ai_render_character_names_definition,
	&ai_render_behavior_failure_definition,
	&ai_render_dialogue_definition,
	&ai_render_dialogue_queue_definition,
	&ai_render_dialogue_records_definition,
	&ai_render_dialogue_player_weights_definition,
	&ai_dialogue_test_mode_definition,
	&ai_dialogue_datamine_enable_definition,
	&ai_render_teams_definition,
	&ai_render_player_ratings_definition,
	&ai_render_spatial_effects_definition,
	&ai_render_firing_positions_definition,
	&ai_render_firing_position_statistics_definition,
	&ai_render_firing_position_obstacles_definition,
	&ai_render_mission_critical_definition,
	&ai_render_gun_positions_definition,
	&ai_render_aiming_positions_definition,
	&ai_render_burst_geometry_definition,
	&ai_render_vehicle_avoidance_definition,
	&ai_render_vehicles_enterable_definition,
	&ai_render_melee_check_definition,
	&ai_render_dialogue_variants_definition,
	&ai_render_grenades_definition,
	&ai_render_danger_zones_definition,
	&ai_render_control_definition,
	&ai_render_activation_definition,
	&ai_render_paths_definition,
	&ai_render_paths_text_definition,
	&ai_render_paths_selected_only_definition,
	&ai_render_paths_destination_definition,
	&ai_render_paths_raw_definition,
	&ai_render_paths_current_definition,
	&ai_render_paths_failed_definition,
	&ai_render_paths_smoothed_definition,
	&ai_render_paths_avoided_definition,
	&ai_render_paths_error_thresholds_definition,
	&ai_render_paths_avoidance_segment_definition,
	&ai_render_paths_avoidance_obstacles_definition,
	&ai_render_paths_avoidance_search_definition,
	&ai_render_paths_nodes_definition,
	&ai_render_paths_nodes_all_definition,
	&ai_render_paths_nodes_polygons_definition,
	&ai_render_paths_nodes_costs_definition,
	&ai_render_paths_nodes_closest_definition,
	&ai_render_paths_distance_definition,
	&ai_render_player_aiming_blocked_definition,
	&ai_render_squad_patrol_state_definition,
	&ai_render_deceleration_obstacles_definition,
	&ai_render_recent_obstacles_definition,
	&ai_render_combat_range_definition,
	&ai_render_dynamic_firing_positions_definition,
	&ai_render_clumps_definition,
	&ai_render_clump_props_definition,
	&ai_render_clump_props_all_definition,
	&ai_render_clump_dialogue_definition,
	&ai_render_sectors_definition,
	&ai_render_sector_bsps_definition,
	&ai_render_giant_sector_bsps_definition,
	&ai_render_sector_link_errors_definition,
	&ai_render_intersection_links_definition,
	&ai_render_non_walkable_sectors_definition,
	&ai_render_threshold_links_definition,
	&ai_render_sector_geometry_errors_definition,
	&ai_pathfinding_generation_verbose_definition,
	&ai_render_sectors_range_max_definition,
	&ai_render_sectors_range_min_definition,
	&ai_render_link_specific_definition,
	&ai_render_links_definition,
	&ai_render_user_hints_definition,
	&ai_render_area_flight_hints_definition,
	&ai_render_hints_definition,
	&ai_render_hints_detailed_definition,
	&ai_render_object_hints_definition,
	&ai_render_object_hints_all_definition,
	&ai_render_object_properties_definition,
	&ai_render_hints_movement_definition,
	&ai_orders_print_entries_definition,
	&ai_orders_print_entries_verbose_definition,
	&ai_render_orders_definition,
	&ai_render_suppress_combat_definition,
	&ai_render_squad_patrol_definition,
	&ai_render_formations_definition,
	&ai_render_objectives_definition,
	&ai_render_strength_definition,
	&ai_render_squad_fronts_definition,
	&ai_render_squad_fronts_detailed_definition,
	&ai_render_ai_iterator_definition,
	&ai_render_child_stack_definition,
	&ai_render_behavior_stack_definition,
	&ai_render_behavior_stack_all_definition,
	&ai_render_stimuli_definition,
	&ai_render_combat_status_definition,
	&ai_render_decisions_definition,
	&ai_render_decisions_all_definition,
	&ai_render_command_scripts_definition,
	&ai_render_script_data_definition,
	&ai_hide_actor_errors_definition,
	&ai_debug_tracking_data_definition,
	&ai_debug_perception_data_definition,
	&ai_debug_combat_status_definition,
	&ai_render_tracked_props_definition,
	&ai_render_tracked_props_all_definition,
	&ai_debug_vignettes_definition,
	&ai_render_joint_behaviors_definition,
	&ai_render_swarm_definition,
	&ai_render_flocks_definition,
	&ai_render_vehicle_interest_definition,
	&ai_render_player_battle_vector_definition,
	&ai_render_player_needs_vehicle_definition,
	&ai_debug_prop_refresh_definition,
	&ai_debug_all_disposable_definition,
	&ai_current_squad_definition,
	&ai_current_actor_definition,
	&ai_render_vehicle_turns_definition,
	&ai_render_discarded_firing_positions_definition,
	&ai_render_firing_positions_all_definition,
	&ai_render_firing_position_info_definition,
	&ai_inspect_avoidance_failure_definition,
	&ai_render_action_selection_failure_definition,
	&ai_combat_status_asleep_definition,
	&ai_combat_status_idle_definition,
	&ai_combat_status_alert_definition,
	&ai_combat_status_active_definition,
	&ai_combat_status_uninspected_definition,
	&ai_combat_status_definite_definition,
	&ai_combat_status_certain_definition,
	&ai_combat_status_visible_definition,
	&ai_combat_status_clear_los_definition,
	&ai_combat_status_dangerous_definition,
	&ai_task_status_never_definition,
	&ai_task_status_occupied_definition,
	&ai_task_status_empty_definition,
	&ai_task_status_inactive_definition,
	&ai_task_status_exhausted_definition,
	&ai_evaluator_preference_definition,
	&ai_evaluator_avoidance_definition,
	&ai_evaluator_sum_definition,
	&ai_evaluator_pathfinding_definition,
	&ai_evaluator_preferred_group_definition,
	&ai_evaluator_pursuit_walkdistance_definition,
	&ai_evaluator_pursuit_targetdistance_definition,
	&ai_evaluator_pursuit_targethint_definition,
	&ai_evaluator_pursuit_visible_definition,
	&ai_evaluator_pursuit_examined_us_definition,
	&ai_evaluator_pursuit_examined_total_definition,
	&ai_evaluator_pursuit_available_definition,
	&ai_evaluator_panic_walkdistance_definition,
	&ai_evaluator_panic_targetdistance_definition,
	&ai_evaluator_panic_closetotarget_definition,
	&ai_evaluator_guard_walkdistance_definition,
	&ai_evaluator_attack_weaponrange_definition,
	&ai_evaluator_attack_idealrange_definition,
	&ai_evaluator_attack_visible_definition,
	&ai_evaluator_attack_dangerousenemy_definition,
	&ai_evaluator_combatmove_walkdistance_definition,
	&ai_evaluator_combatmove_lineoffire_definition,
	&ai_evaluator_hide_cover_definition,
	&ai_evaluator_hide_exposed_definition,
	&ai_evaluator_uncover_pre_evaluate_definition,
	&ai_evaluator_uncover_visible_definition,
	&ai_evaluator_uncover_blocked_definition,
	&ai_evaluator_previously_discarded_definition,
	&ai_evaluator_danger_zone_definition,
	&ai_evaluator_move_into_danger_zone_definition,
	&ai_evaluator_3d_path_available_definition,
	&ai_evaluator_point_avoidance_definition,
	&ai_evaluator_point_preference_definition,
	&ai_evaluator_directional_driving_definition,
	&ai_evaluator_favor_former_firing_position_definition,
	&ai_evaluator_hide_pre_evaluation_definition,
	&ai_evaluator_pursuit_definition,
	&ai_evaluator_pursuit_area_discarded_definition,
	&ai_evaluator_flag_preferences_definition,
	&ai_evaluator_perch_preferences_definition,
	&ai_evaluator_combatmove_lineoffire_occluded_definition,
	&ai_evaluator_attack_same_frame_of_reference_definition,
	&ai_evaluator_wall_leanable_definition,
	&ai_evaluator_cover_near_friends_definition,
	&ai_evaluator_combat_move_near_follow_unit_definition,
	&ai_evaluator_goal_preferences_definition,
	&ai_evaluator_hint_plane_definition,
	&ai_evaluator_postsearch_prefer_original_definition,
	&ai_evaluator_leadership_definition,
	&ai_evaluator_flee_to_leader_definition,
	&ai_evaluator_goal_points_only_definition,
	&ai_evaluator_attack_leader_distance_definition,
	&ai_evaluator_too_far_from_leader_definition,
	&ai_evaluator_guard_preference_definition,
	&ai_evaluator_guard_wall_preference_definition,
	&ai_evaluator_obstacle_definition,
	&ai_evaluator_facing_definition,
	&ai_evaluator_hide_equipment_definition,
	&ai_action_berserk_definition,
	&ai_action_surprise_front_definition,
	&ai_action_surprise_back_definition,
	&ai_action_evade_left_definition,
	&ai_action_evade_right_definition,
	&ai_action_dive_forward_definition,
	&ai_action_dive_back_definition,
	&ai_action_dive_left_definition,
	&ai_action_dive_right_definition,
	&ai_action_advance_definition,
	&ai_action_cheer_definition,
	&ai_action_fallback_definition,
	&ai_action_hold_definition,
	&ai_action_point_definition,
	&ai_action_posing_definition,
	&ai_action_shakefist_definition,
	&ai_action_signal_attack_definition,
	&ai_action_signal_move_definition,
	&ai_action_taunt_definition,
	&ai_action_warn_definition,
	&ai_action_wave_definition,
	&ai_activity_none_definition,
	&ai_activity_patrol_definition,
	&ai_activity_stand_definition,
	&ai_activity_crouch_definition,
	&ai_activity_stand_drawn_definition,
	&ai_activity_crouch_drawn_definition,
	&ai_activity_corner_definition,
	&ai_activity_corner_open_definition,
	&ai_activity_bunker_definition,
	&ai_activity_bunker_open_definition,
	&ai_activity_combat_definition,
	&ai_activity_backup_definition,
	&ai_activity_guard_definition,
	&ai_activity_guard_crouch_definition,
	&ai_activity_guard_wall_definition,
	&ai_activity_typing_definition,
	&ai_activity_kneel_definition,
	&ai_activity_gaze_definition,
	&ai_activity_poke_definition,
	&ai_activity_sniff_definition,
	&ai_activity_track_definition,
	&ai_activity_watch_definition,
	&ai_activity_examine_definition,
	&ai_activity_sleep_definition,
	&ai_activity_at_ease_definition,
	&ai_activity_cower_definition,
	&ai_activity_tai_chi_definition,
	&ai_activity_pee_definition,
	&ai_activity_doze_definition,
	&ai_activity_eat_definition,
	&ai_activity_medic_definition,
	&ai_activity_work_definition,
	&ai_activity_cheering_definition,
	&ai_activity_injured_definition,
	&ai_activity_captured_definition,
	&morph_disallowed_definition,
	&morph_time_ranged_tank_definition,
	&morph_time_ranged_stealth_definition,
	&morph_time_tank_ranged_definition,
	&morph_time_tank_stealth_definition,
	&morph_time_stealth_ranged_definition,
	&morph_time_stealth_tank_definition,
	&morph_form_ranged_definition,
	&morph_form_tank_definition,
	&morph_form_stealth_definition,
	&ai_movement_patrol_definition,
	&ai_movement_asleep_definition,
	&ai_movement_combat_definition,
	&ai_movement_flee_definition,
	&ai_movement_flaming_definition,
	&ai_movement_stunned_definition,
	&ai_movement_berserk_definition,
	&ai_print_major_upgrade_definition,
	&ai_print_evaluation_statistics_definition,
	&ai_print_communication_definition,
	&ai_print_communication_player_definition,
	&ai_print_vocalizations_definition,
	&ai_print_placement_definition,
	&ai_print_speech_definition,
	&ai_print_allegiance_definition,
	&ai_print_lost_speech_definition,
	&ai_print_migration_definition,
	&ai_print_scripting_definition,
	&ai_print_disposal_definition,
	&ai_print_killing_sprees_definition,
	&ai_naimad_spew_definition,
	&ai_maxd_spew_definition,
	&ai_debug_fast_los_definition,
	&ai_debug_evaluate_all_positions_definition,
	&ai_debug_path_definition,
	&ai_debug_path_start_freeze_definition,
	&ai_debug_path_end_freeze_definition,
	&ai_debug_path_flood_definition,
	&ai_debug_path_maximum_radius_definition,
	&ai_debug_path_attractor_definition,
	&ai_debug_path_attractor_radius_definition,
	&ai_debug_path_attractor_weight_definition,
	&ai_debug_path_accept_radius_definition,
	&ai_debug_path_radius_definition,
	&ai_debug_path_destructible_definition,
	&ai_debug_path_giant_definition,
	&ai_debug_ballistic_lineoffire_freeze_definition,
	&ai_debug_path_naive_estimate_definition,
	&ai_debug_blind_definition,
	&ai_debug_deaf_definition,
	&ai_debug_invisible_player_definition,
	&ai_debug_ignore_player_definition,
	&ai_debug_force_all_active_definition,
	&ai_debug_path_disable_smoothing_definition,
	&ai_debug_path_disable_obstacle_avoidance_definition,
	&net_bitstream_debug_definition,
	&net_bitstream_display_errors_definition,
	&net_bitstream_capture_structure_definition,
	&net_never_timeout_definition,
	&net_use_local_time_definition,
	&net_traffic_warnings_definition,
	&net_traffic_print_definition,
	&net_messages_print_definition,
	&net_replication_requests_print_definition,
	&net_packet_print_mask_definition,
	&net_experimental_definition,
	&net_rate_unlimited_definition,
	&net_rate_server_definition,
	&net_rate_client_definition,
	&net_window_unlimited_definition,
	&net_window_size_definition,
	&net_bandwidth_unlimited_definition,
	&net_bandwidth_per_channel_definition,
	&unknown3CC_definition,
	&unknown3CD_definition,
	&unknown3CE_definition,
	&unknown3CF_definition,
	&unknown3D0_definition,
	&unknown3D1_definition,
	&net_maximum_machine_count_definition,
	&net_maximum_player_count_definition,
	&net_debug_random_seeds_definition,
	&net_allow_out_of_sync_definition,
	&net_distributed_always_definition,
	&net_distributed_never_definition,
	&net_matchmaking_force_gather_definition,
	&net_matchmaking_force_search_definition,
	&net_matchmaking_fail_arbitration_definition,
	&net_connectivity_model_enabled_definition,
	&net_nat_override_definition,
	&net_matchmaking_nat_check_enabled_definition,
	&net_matchmaking_hopper_id_adjustment_definition,
	&net_matchmaking_use_last_map_and_game_definition,
	&net_matchmaking_allow_early_start_definition,
	&net_matchmaking_skip_host_migration_definition,
	&net_matchmaking_force_disband_definition,
	&net_enable_host_migration_loop_definition,
	&net_matchmaking_fake_progress_definition,
	&net_matchmaking_force_no_joining_definition,
	&net_matchmaking_allow_idle_controllers_definition,
	&net_simulation_set_stream_bandwidth_definition,
	&net_set_channel_disconnect_interval_definition,
	&net_enable_block_detection_definition,
	&net_override_base_xp_definition,
	&net_override_ranked_games_played_definition,
	&net_matchmaking_mask_maps_definition,
	&net_status_memory_definition,
	&net_status_link_definition,
	&net_status_sim_definition,
	&net_status_channels_definition,
	&net_status_connections_definition,
	&net_status_message_queues_definition,
	&net_status_observer_definition,
	&net_status_sessions_definition,
	&net_status_leaderboard_definition,
	&net_status_leaderboard_mask_definition,
	&net_test_definition,
	&net_test_rate_definition,
	&net_test_update_server_definition,
	&net_test_update_client_definition,
	&net_test_replication_scheduler_definition,
	&net_test_debug_spheres_definition,
	&net_test_matchmaking_playlist_index_definition,
	&net_voice_diagnostics_definition,
	&net_http_failure_ratio_definition,
	&net_show_network_quality_definition,
	&net_fake_network_quality_definition,
	&sim_status_world_definition,
	&sim_status_views_definition,
	&sim_entity_validate_definition,
	&sim_disable_aim_assist_definition,
	&sim_bandwidth_eater_definition,
	&debug_player_teleport_definition,
	&debug_players_definition,
	&debug_player_input_definition,
	&debug_survival_mode_definition,
	&display_rumble_status_lines_definition,
	&enable_pc_joystick_definition,
	&texture_cache_show_mipmap_bias_definition,
	&texture_cache_graph_definition,
	&texture_cache_list_definition,
	&texture_cache_force_low_detail_definition,
	&texture_cache_force_medium_detail_definition,
	&texture_cache_force_high_detail_definition,
	&texture_cache_status_definition,
	&texture_cache_usage_definition,
	&texture_cache_block_warning_definition,
	&texture_cache_lod_bias_definition,
	&texture_cache_dynamic_low_detail_texture_definition,
	&render_debug_low_res_textures_definition,
	&geometry_cache_graph_definition,
	&geometry_cache_list_definition,
	&geometry_cache_status_definition,
	&geometry_cache_block_warning_definition,
	&geometry_cache_never_block_definition,
	&geometry_cache_debug_display_definition,
	&director_camera_switch_fast_definition,
	&director_camera_switch_disable_definition,
	&director_camera_speed_scale_definition,
	&director_disable_first_person_definition,
	&director_use_dt_definition,
	&observer_collision_enabled_definition,
	&observer_collision_anticipation_enabled_definition,
	&observer_collision_water_flags_definition,
	&g_observer_wave_height_definition,
	&debug_recording_definition,
	&debug_recording_newlines_definition,
	&cinematic_letterbox_style_definition,
	&run_game_scripts_definition,
	&vehicle_status_display_definition,
	&vehicle_disable_suspension_animations_definition,
	&vehicle_disable_acceleration_screens_definition,
	&biped_meter_display_definition,
	&display_verbose_disk_usage_definition,
	&display_disk_usage_definition,
	&default_scenario_ai_type_definition,
	&debug_menu_enabled_definition,
	&catch_exceptions_definition,
	&debug_first_person_hide_base_definition,
	&debug_first_person_hide_movement_definition,
	&debug_first_person_hide_jitter_definition,
	&debug_first_person_hide_overlay_definition,
	&debug_first_person_hide_pitch_turn_definition,
	&debug_first_person_hide_ammo_definition,
	&debug_first_person_hide_ik_definition,
	&global_playtest_mode_definition,
	&g_override_logon_task_definition,
	&g_logon_task_override_result_code_definition,
	&ui_time_scale_definition,
	&ui_display_memory_definition,
	&ui_memory_verify_definition,
	&xov_display_memory_definition,
	&gui_debug_text_bounds_global_definition,
	&gui_debug_bitmap_bounds_global_definition,
	&gui_debug_model_bounds_global_definition,
	&gui_debug_list_item_bounds_global_definition,
	&gui_debug_list_bounds_global_definition,
	&gui_debug_group_bounds_global_definition,
	&gui_debug_screen_bounds_global_definition,
	&render_comment_flags_definition,
	&render_comment_flags_text_definition,
	&render_comment_flags_look_at_definition,
	&enable_controller_flag_drop_definition,
	&sapien_keyboard_toggle_for_camera_movement_definition,
	&override_player_representation_index_definition,
	&e3_definition,
	&debug_tag_dependencies_definition,
	&disable_network_hopper_download_definition,
	&disable_network_configuration_download_definition,
	&check_system_heap_definition,
	&data_mine_player_update_interval_definition,
	&data_mine_mp_player_update_interval_definition,
	&data_mine_debug_menu_interval_definition,
	&data_mine_spam_enabled_definition,
	&webstats_file_zip_writes_per_frame_definition,
	&debug_projectiles_definition,
	&debug_damage_effects_definition,
	&debug_damage_effect_obstacles_definition,
	&force_player_walking_definition,
	&unit_animation_report_missing_animations_definition,
	&font_cache_status_definition,
	&font_cache_debug_texture_definition,
	&font_cache_debug_graph_definition,
	&font_cache_debug_list_definition,
	&lruv_lirp_enabled_definition,
	&halt_on_stack_overflow_definition,
	&disable_progress_screen_definition,
	&render_thread_enable_definition,
	&character_force_physics_definition,
	&enable_new_ik_method_definition,
	&animation_throttle_dampening_scale_definition,
	&animation_blend_change_scale_definition,
	&biped_fitting_enable_definition,
	&biped_fitting_root_offset_enable_definition,
	&biped_pivot_enable_definition,
	&biped_pivot_allow_player_definition,
	&giant_hunt_player_definition,
	&giant_hunting_min_radius_definition,
	&giant_hunting_max_radius_definition,
	&giant_hunting_throttle_scale_definition,
	&giant_weapon_wait_time_definition,
	&giant_weapon_trigger_time_definition,
	&giant_ik_definition,
	&giant_foot_ik_definition,
	&giant_ankle_ik_definition,
	&giant_elevation_control_definition,
	&giant_force_buckle_definition,
	&giant_force_crouch_definition,
	&giant_force_death_definition,
	&giant_buckle_rotation_definition,
	&debug_objects_giant_feet_definition,
	&debug_objects_giant_buckle_definition,
	&enable_xsync_timings_definition,
	&allow_restricted_tag_groups_to_load_definition,
	&xsync_restricted_tag_groups_definition,
	&enable_cache_build_resources_definition,
	&xma_compression_level_default_definition,
	&enable_console_window_definition,
	&display_colors_in_banded_gamma_definition,
	&use_tool_command_legacy_definition,
	&maximum_tool_command_history_definition,
	&disable_unit_aim_screens_definition,
	&disable_unit_look_screens_definition,
	&disable_unit_eye_tracking_definition,
	&enable_tag_resource_xsync_definition,
	&dont_recompile_shaders_definition,
	&use_temp_directory_for_files_definition,
	&scenario_load_all_tags_definition,
	&synchronization_debug_definition,
	&profiler_pulse_rates_definition,
	&profiler_collection_interval_definition,
	&debug_objects_scenery_definition,
	&disable_switch_zone_sets_definition,
	&facial_animation_testing_enabled_definition,
	&profiler_datamine_uploads_enabled_definition,
	&debug_object_recycling_definition,
	&enable_sound_over_network_definition,
	&lsp_allow_lsp_connections_definition,
	&lsp_allow_raw_connections_definition,
	&lsp_service_id_override_definition,
	&unknown49B_definition,
	&unknown49C_definition,
	&unknown49D_definition,
	&unknown49E_definition,
	&unknown49F_definition,
	&unknown4A0_definition,
	&unknown4A1_definition,
	&unknown4A2_definition,
	&unknown4A3_definition,
	&unknown4A4_definition,
	&aiming_interpolation_stop_delta_definition,
	&aiming_interpolation_start_delta_definition,
	&aiming_interpolation_rate_definition,
	&airborne_arc_enabled_definition,
	&airborne_descent_test_duration_definition,
	&airborne_descent_test_count_definition,
	&enable_amortized_prediction_definition,
	&amortized_prediction_object_batch_size_definition,
	&enable_tag_resource_prediction_definition,
	&enable_entire_pvs_prediction_definition,
	&enable_cluster_objects_prediction_definition,
	&disable_main_loop_throttle_definition,
	&force_unit_walking_definition,
	&leap_force_start_rotation_definition,
	&leap_force_end_rotation_definition,
	&leap_force_flight_start_rotation_definition,
	&leap_force_flight_end_rotation_definition,
	&leap_flight_path_scale_definition,
	&leap_departure_power_definition,
	&leap_departure_scale_definition,
	&leap_arrival_power_definition,
	&leap_arrival_scale_definition,
	&leap_twist_power_definition,
	&leap_cannonball_power_definition,
	&leap_cannonball_scale_definition,
	&leap_idle_power_definition,
	&leap_idle_scale_definition,
	&leap_overlay_power_definition,
	&leap_overlay_scale_definition,
	&leap_interpolation_limit_definition,
	&biped_fake_soft_landing_definition,
	&biped_fake_hard_landing_definition,
	&biped_soft_landing_recovery_scale_definition,
	&biped_hard_landing_recovery_scale_definition,
	&biped_landing_absorbtion_definition,
	&debug_player_network_aiming_definition,
	&aim_assist_disable_target_lead_vector_definition,
	&enable_tag_edit_sync_definition,
	&instance_default_fade_start_pixels_definition,
	&instance_default_fade_end_pixels_definition,
	&render_debug_dont_flash_low_res_textures_definition,
	&debug_objects_pendulum_definition,
	&ui_alpha_definition,
	&ui_alpha_lockdown_definition,
	&ui_alpha_eula_accepted_definition,
	&ui_alpha_custom_games_enabled_definition,
	&net_config_client_badness_rating_threshold_override_definition,
	&net_config_disable_bad_client_anticheating_override_definition,
	&net_config_disable_bad_connectivity_anticheating_override_definition,
	&net_config_disable_bad_bandwidth_anticheating_override_definition,
	&net_config_maximum_multiplayer_split_screen_override_definition,
	&net_config_crash_handling_minidump_type_override_definition,
	&net_config_crash_handling_ui_display_override_definition,
	&online_files_slowdown_definition,
	&debug_trace_main_events_definition,
	&force_xsync_memory_buyback_definition,
	&bitmaps_trim_unused_pixels_definition,
	&bitmaps_interleave_compressed_bitmaps_definition,
	&ignore_predefined_performance_throttles_definition,
	&enable_first_person_prediction_definition,
	&enable_structure_prediction_definition,
	&enable_structure_audibility_definition,
	&debug_sound_transmission_definition,
	&minidump_force_regular_minidump_with_ui_definition,
	&giant_custom_anim_recovery_time_definition,
	&facial_animation_enable_lipsync_definition,
	&facial_animation_enable_gestures_definition,
	&facial_animation_enable_noise_definition,
	&render_alpha_to_coverage_definition,
	&rasterizer_disable_vsync_definition,
	&scale_ui_to_maintain_aspect_ratio_definition,
	&maximum_aspect_ratio_scale_percentage_definition,
	&enable_sound_transmission_definition,
	&disable_audibility_generation_definition,
	&motion_blur_expected_dt_definition,
	&motion_blur_max_x_definition,
	&motion_blur_max_y_definition,
	&motion_blur_scale_x_definition,
	&motion_blur_scale_y_definition,
	&motion_blur_center_falloff_definition,
	&unicode_warn_on_truncation_definition,
	&debug_determinism_version_definition,
	&debug_determinism_compatible_version_definition,
	&error_geometry_environment_enabled_definition,
	&error_geometry_lightmap_enabled_definition,
	&error_geometry_seam_enabled_definition,
	&error_geometry_instance_enabled_definition,
	&error_geometry_object_enabled_definition,
	&debug_objects_unit_melee_definition,
	&force_buffer_2_frames_definition,
	&disable_render_state_cache_optimization_definition,
	&utility_drive_enabled_definition,
	&debug_objects_root_node_print_definition,
	&enable_better_cpu_gpu_sync_definition,
	&require_secure_cache_files_definition,
	&debug_aim_assist_targets_enabled_definition,
	&motion_blur_max_viewport_count_definition,
	&cinematic_prediction_enable_definition,
	&render_debug_cortana_ticks_definition,
	&skies_delete_on_zone_set_switch_enable_definition,
	&reduce_widescreen_fov_during_cinematics_definition,
	&unknown50A_definition,
	&unknown50B_definition,
	&unknown50C_definition,
	&unknown50D_definition,
	&unknown50E_definition,
	&unknown50F_definition,
	&unknown510_definition,
	&unknown511_definition,
	&unknown512_definition,
	&unknown513_definition,
	&unknown514_definition,
	&unknown515_definition,
	&unknown516_definition,
	&unknown517_definition,
	&unknown518_definition,
	&unknown519_definition,
	&unknown51A_definition,
	&unknown51B_definition,
	&unknown51C_definition,
	&unknown51D_definition,
	&load_time_multiplier_definition,
	&unknown51F_definition,
	&unknown520_definition,
	&unknown521_definition,
	&unknown522_definition,
};
const int16 k_hs_external_global_count = NUMBEROF(hs_external_globals);
static_assert(k_hs_external_global_count >= k_maximum_number_of_ms23_hs_globals);

