#include "sound/sound_manager.hpp"

#include "cache/cache_files.hpp"
#include "game/player_mapping.hpp"
#include "memory/data.hpp"
#include "memory/module.hpp"
#include "render/render_debug.hpp"
#include "simulation/simulation.hpp"
#include "sound/sound_classes.hpp"

#include <string.h>

#define MAXIMUM_SOUND_SOURCES 196

#define NUMBER_OF_SOUND_SAMPLE_RATES 3

int32 const sound_sample_rate_samples_per_second[NUMBER_OF_SOUND_SAMPLE_RATES]
{
	22050,
	44100,
	32000
};

struct s_sound_source
{
	uns16 flags;
	int8 volume_type;
	int8 spatialization_mode;
	real32 scale;
	byte __dataC[0x4];
	s_sound_location location;
	real32 __unknown38;
	real32 minimum_distance;
	real32 maximum_distance;
	int16 inner_cone_angle_step;
	int16 outer_cone_angle_step;
	int32 __unknown48;
};
static_assert(sizeof(s_sound_source) == 0x4C);

struct s_sound_tracker
{
	bool(__cdecl* callback)(void*, int32, int32*, s_sound_source*);

	byte __data[0x2C];
};
static_assert(sizeof(s_sound_tracker) == 0x30);

struct s_cache_file_sound_permutation;
struct sound_channel_datum
{
	int32 sound_index;
	int32 __unknown4;
	int32 debug_entry_index;
	int32 __unknownC;
	int8 type_index; // c_enum<e_sound_channel_type, int8>
	uns8 flags;
	uns8 __unknownA_sound_playback; // c_flags<e_sound_playback_bit, uns8>
	uns8 __unknownB_sound_playback; // c_flags<e_sound_playback_bit, uns8>
	int16 hardware_source_index;
	int16 hardware_voice_index;

	// initial_sample_offset = sound_sample_rate_samples_per_second[sound->get_sample_rate()] * __unknown18
	real32 __unknown18;

	int32 __unknown1C;

	byte __data20[0x4];

	int32 playing_chunk_definition_index;
	int32 queued_chunk_definition_index;
	int16 playing_chunk_index;
	int16 queued_chunk_index;
	s_cache_file_sound_permutation* playing_chunk_permutation;
	s_cache_file_sound_permutation* queued_chunk_permutation;
};
static_assert(sizeof(sound_channel_datum) == 0x38);

struct s_sound_tracker_datum :
	s_datum_header
{
	uns8 flags;
	uns8 flip_flop;
	int32 definition_index;
	void* tracker_callback_data;
	s_sound_tracker tracker_data;

	byte __data3C[0x4];
};
static_assert(sizeof(s_sound_tracker_datum) == 0x40);

struct sound_datum :
	s_datum_header
{
	int8 software_reference_count;
	int8 hardware_reference_count;

	uns32 flags;

	byte __data8[0x8];

	int32 definition_index;
	int32 looping_sound_index;
	s_sound_tracker* tracker;
	s_sound_source source;
	int32 playback_controller_index;

	byte __data6C[4];

	int8 loop_track_index;
	uns8 __unknown71;
	uns8 __unknown72;
	uns8 __unknown73;

	byte __data74[0x24];

	int32 sound_tracker_index;
	int32 __unknown9C;
	real32 pitch_modifier;

	real32 __unknownA4;

	real32 maybe_pitch_scale;
	int8 pitch_range_index;

	int8 permutation_index;
	int16 permutation_chunk_index;

	c_enum<int32, uns8, 0, 4> listener_index;
	uns8 __unknownB1;
	uns8 __unknownB2;
	uns8 __unknownB3;

	int32 playback_controller_index_;

	int16 playing_channel_index; // index into `channel_get` and `g_sound_channels`
	int8 sound_fade_mode;
	int8 event_stop_reason;

	real32 __unknownBC;
	int32 __unknownC0;
	int32 __unknownC4;
};
static_assert(sizeof(sound_datum) == 0xC8);

struct s_xbox_sound_datum :
	s_datum_header
{
	byte __data0[0xA];
};
static_assert(sizeof(s_xbox_sound_datum) == 0xC);

struct looping_sound_track_datum
{
	int32 sound_index;
	byte __data4[0x8];
	int8 current_playback_state;
	int8 queued_playback_state;
	int8 playing_sound_count;
	byte __dataF[0x5];
	c_static_array<real32, 4> gains;
};
static_assert(sizeof(looping_sound_track_datum) == 0x24);

struct looping_sound_datum :
	s_datum_header
{
	byte __data2[0x6];

	int32 definition_index;
	int32 sound_identifier;

	byte __data10[0x4];

	s_sound_source source;
	uns8 flags;
	int8 component_sound_count;
	int32 playback_controller_index;

	byte __data68[0x30];

	c_static_array<looping_sound_track_datum, 4> tracks;
};
static_assert(sizeof(looping_sound_datum) == 0x128);

struct s_sound_effect_datum :
	s_datum_header
{
	byte __data2[0x1];

	int8 source_type;
	uns16 flags;
	int32 playback_controller_index;
	int32 tracker_index;
	s_sound_source source;

	byte __dataC[0x8];
};
static_assert(sizeof(s_sound_effect_datum) == 0x64);

class c_sound_playback_controller :
	public s_datum_header
{
	// reference ^= (reference ^ (reference + 1)) & 0x3F
	uns8 reference;

	int8 m_ready_count;
	int32 identifier;
	uns32 random_seed;

	byte __dataC[0x10];
};
static_assert(sizeof(c_sound_playback_controller) == 0x1C);

REFERENCE_DECLARE(0x0238E858, s_sound_manager_globals*, g_sound_manager_globals);

//  name: ""
// count: 196
//  size: 0x38
REFERENCE_DECLARE(0x0238E85C, sound_channel_datum*, g_sound_channels);

//  name: "sound sources"
// count: 384
//  size: 0xC8
REFERENCE_DECLARE(0x0238E860, c_smart_data_array<sound_datum>, g_sound_data);

REFERENCE_DECLARE(0x0238E864, bool, debug_sound_always_update_in_idle);
REFERENCE_DECLARE(0x0238E86E, bool, bool_238E86E); // unreferenced script globals
REFERENCE_DECLARE(0x0238E86F, bool, bool_238E86F); // unreferenced script globals
REFERENCE_DECLARE(0x0238E871, bool, debug_sound_force_first_person_listener);
REFERENCE_DECLARE(0x0238E874, int32, g_sound_manager_reference_count);

//  name: "xbox sound"
// count: 8192
//  size: 0xC
REFERENCE_DECLARE(0x02497CF8, c_smart_data_array<s_xbox_sound_datum>*, g_xbox_sounds);

REFERENCE_DECLARE(0x02497CFC, bool, bool_2497CFC);

//  name: "looping sounds"
// count: 128
//  size: 0x128
REFERENCE_DECLARE(0x02497D20, c_smart_data_array<looping_sound_datum>, g_looping_sound_data);

REFERENCE_DECLARE(0x02497D26, bool, enable_pc_sound); // unreferenced script globals

//  name: "sounds effects"
// count: 16
//  size: 0x64
REFERENCE_DECLARE(0x02497D30, c_smart_data_array<s_sound_effect_datum>, g_sound_effect_data);

REFERENCE_DECLARE(0x02497D38, c_sound_playback_controller*, g_default_playback_controller);

//  name: "sound playback controllers"
// count: 64
//  size: 0x1C
REFERENCE_DECLARE(0x02497D3C, c_smart_data_array<c_sound_playback_controller>, g_sound_playback_controller_data);

//  name: "  sound tracker data"
// count: 384
//  size: 0x40
REFERENCE_DECLARE(0x02497D4C, c_smart_data_array<s_sound_tracker_datum>, g_sound_tracker_data);

REFERENCE_DECLARE(0x02497D51, bool, g_sound_tracker_inside_update);
REFERENCE_DECLARE(0x02497D51, uns8, g_sound_tracker_flip_flop);

bool debug_sound_class_totals = false;
bool debug_sound_timing = false;
bool debug_duckers = false;
bool debug_sound_listeners = false;
bool debug_sound = false;
bool debug_sound_manager_channels = false;

//.text:00512F30 ; real64 __cdecl calculate_doppler_shift_cents(int32, const s_sound_location*)
//.text:00513120 ; 
//.text:005131C0 ; bool __cdecl channel_can_pause(int16)

sound_channel_datum* __cdecl channel_get(int16 index)
{
	return INVOKE(0x00513230, channel_get, index);
}

//.text:00513250 ; int16 __cdecl channel_get_state(int16)
//.text:00513280 ; void __cdecl channel_initialize(int16, const s_sound_channel_properties*)
//.text:00513350 ; void __cdecl channel_pause(int16)
//.text:005133A0 ; 
//.text:005133E0 ; void __cdecl channel_properties_initialize_from_sound(int16, const sound_datum*, const s_sound_output_parameters*, s_sound_channel_properties*)
//.text:005137E0 ; void __cdecl channel_queue_sound(int16, s_cache_file_sound_permutation*, int16)
//.text:005138A0 ; void __cdecl channel_refresh_location(int16, const s_sound_transmission_output_parameters*)
//.text:00513DB0 ; void __cdecl channel_refresh_state(int16)
//.text:00513F50 ; void __cdecl channel_restore_from_pause(int16)
//.text:00514080 ; void __cdecl channel_signal_complete(int16)
//.text:005140B0 ; 
//.text:005140E0 ; void __cdecl channel_stop(int16)
//.text:005141C0 ; bool __cdecl channel_touch(int16)
//.text:00514310 ; void __cdecl channel_update_properties(int16, const s_sound_channel_properties*, bool)
//.text:00514340 ; void __cdecl channel_update_spatialization(int16, real_decibel*, s_sound_output_parameters*)
//.text:00514660 ; 
//.text:00514670 ; 
//.text:00514680 ; 
//.text:00514690 ; 
//.text:005146A0 ; 
//.text:005146B0 ; 
//.text:005146C0 ; 
//.text:005146F0 ; 
//.text:00514720 ; bool __cdecl first_person_spatialization(int32)
//.text:00514750 ; 
//.text:00514760 ; 
//.text:00514770 ; 
//.text:00514780 ; 
//.text:00514790 ; 
//.text:005147A0 ; 
//.text:005147B0 ; 
//.text:005147D0 ; 
//.text:005147E0 ; 
//.text:005147F0 ; 
//.text:00514800 ; 
//.text:00514810 ; 
//.text:00514830 ; int32 __cdecl get_relative_priority(int32, int32)
//.text:00514890 ; 
//.text:005148A0 ; 
//.text:005148B0 ; 
//.text:005148C0 ; 
//.text:005148D0 ; 
//.text:005148E0 ; 
//.text:005148F0 ; 
//.text:00514940 ; 
//.text:00514950 ; 
//.text:00514960 ; 
//.text:00514980 ; 
//.text:00514A10 ; 
//.text:00514A20 ; 
//.text:00514A50 ; void __cdecl prioritize_sounds()
//.text:00514C90 ; 
//.text:00514CA0 ; 
//.text:00514CC0 ; 
//.text:00514CF0 ; 
//.text:00514D30 ; void __cdecl refresh_channel_sounds_for_idle()
//.text:00514DC0 ; void __cdecl refresh_channels()
//.text:00514F90 ; void __cdecl refresh_listener(bool)
//.text:00515480 ; bool __cdecl refresh_sound(int32)
//.text:00515540 ; void __cdecl refresh_sounds()
//.text:00515870 ; void __cdecl reset_platform_sound_mix()
//.text:005158E0 ; 
//.text:005158F0 ; 
//.text:00515900 ; 
//.text:00515910 ; void __cdecl scripted_sound_predict(int32)
//.text:00515970 ; 
//.text:00515980 ; 
//.text:005159B0 ; 
//.text:005159E0 ; 
//.text:00515A00 ; 
//.text:00515A30 ; 
//.text:00515A60 ; 
//.text:00515A90 ; 
//.text:00515AC0 ; 
//.text:00515AF0 ; 
//.text:00515B00 ; uns32 __cdecl sound_cache_sound_request(int32, s_sound_permutation_chunk*)
//.text:00515B20 ; real_decibel __cdecl sound_calculate_fade_db(int32)
//.text:00515CD0 ; 
//.text:00515D60 ; void __cdecl sound_calculate_listener_relative_position_and_attenuation(int32, const s_sound_source*, int32, int32, real32*, real32*, real_point3d*)
//.text:00515EC0 ; void __cdecl sound_channel_summary_build(sound_channel_summary*, int32)
//.text:00516060 ; 
//.text:00516070 ; 
//.text:005160A0 ; 
//.text:005160B0 ; 
//.text:00516120 ; void __cdecl sound_datum_lock(sound_datum*)
//.text:00516190 ; void __cdecl sound_datum_unlock(sound_datum*)
//.text:005161F0 ; 
//.text:00516200 ; bool __cdecl sound_definition_audible(int32, const s_sound_source*)
//.text:00516270 ; 
//.text:005162A0 ; bool __cdecl sound_definition_can_start_new_impulse_sound(int32, const s_sound_impulse_definition*, int32*, e_sound_impulse_new_failure_reason*)
//.text:005164F0 ; bool __cdecl sound_definition_hardware_formats_match(int32, int32)
//.text:00516500 ; 
//.text:00516520 ; bool __cdecl sound_definition_is_playable(int32, const s_sound_source*)
//.text:00516590 ; bool __cdecl sound_definition_is_valid(int32)
//.text:005165C0 ; bool __cdecl sound_definition_should_be_reimported_for_runtime(const s_cache_file_sound_definition*, e_runtime_platform_type)
//.text:005165E0 ; real_decibel __cdecl sound_definition_map_gain_db(const s_cache_file_sound_definition*, real_decibel, real32)

real32 sound_definition_map_pitch(const void* sound_definition, real32 a1, real32 a2)
{
	return INVOKE(0x00516650, sound_definition_map_pitch, sound_definition, a1, a2);
}

//.text:005166A0 ; e_sound_promotion_result __cdecl sound_definition_promote(int32, int32*)
//.text:005167E0 ; real_decibel __cdecl sound_definition_random_gain_db(const s_cache_file_sound_definition*)
//.text:00516870 ; 
//.text:005168F0 ; 
//.text:00516960 ; bool __cdecl sound_definition_is_valid_for_runtime(const s_cache_file_sound_definition*, e_runtime_platform_type)
//.text:00516980 ; real32 __cdecl sound_definition_skip_fraction(const s_cache_file_sound_definition*, real32)

void __cdecl sound_delete(int32 sound_index)
{
	INVOKE(0x005169D0, sound_delete, sound_index);
}

void __cdecl sound_dispose()
{
	INVOKE(0x00516A30, sound_dispose);
}

void __cdecl sound_dispose_from_old_map()
{
	INVOKE(0x00516A90, sound_dispose_from_old_map);
}

void __cdecl sound_dispose_from_old_structure_bsp(uns32 structure_bps_mask)
{
	INVOKE(0x00516BD0, sound_dispose_from_old_structure_bsp, structure_bps_mask);
}

//.text:00516BE0 ; int16 __cdecl sound_find_best_channel(int32, e_sound_event_stop_reason*)
//.text:00516D40 ; int16 __cdecl sound_find_channel(int32, e_sound_event_stop_reason*)
//.text:00517010 ; int16 __cdecl sound_find_like_channel(int32, int16*, int16)

void __cdecl sound_game_pause_handler(bool paused)
{
	INVOKE(0x00517100, sound_game_pause_handler, paused);
}

//.text:00517130 ; 

void __cdecl sound_idle()
{
	INVOKE(0x00517170, sound_idle);
}

//.text:005172B0 ; void __cdecl sound_impulse_mark_as_cinematic_outro(int32)
//.text:005172F0 ; int32 __cdecl sound_impulse_new(int32, int32, const s_sound_impulse_definition*, e_sound_impulse_new_failure_reason*)
//.text:00517430 ; int32 __cdecl sound_impulse_new_internal(int32, int32, int32, const s_sound_impulse_definition*, int32)
//.text:00517950 ; 

void __cdecl sound_initialize()
{
	INVOKE(0x00517960, sound_initialize);
}

void __cdecl sound_initialize_for_new_map()
{
	INVOKE(0x00517CF0, sound_initialize_for_new_map);
}

void __cdecl sound_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	INVOKE(0x00517F20, sound_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

bool __cdecl sound_is_active()
{
	return INVOKE(0x00518020, sound_is_active);
}

bool __cdecl sound_is_fading_out(int32 sound_index)
{
	return INVOKE(0x00518050, sound_is_fading_out, sound_index);
}

//.text:00518090 ; 
//.text:005180C0 ; 
//.text:005180D0 ; bool __cdecl sound_make_permutation_ready(sound_datum*, bool*)
//.text:00518160 ; 
//.text:005181D0 ; bool __cdecl sound_manager_allocate_hardware_voice_explicit(int16)
//.text:00518230 ; bool __cdecl sound_manager_can_update_trackers()
//.text:00518250 ; void __cdecl sound_manager_free_hardware_voice(int16)
//.text:00518290 ; uns8 __cdecl sound_manager_get_flip_flop()

const s_sound_listener* __cdecl sound_manager_get_listener(int32 index)
{
	return INVOKE(0x005182A0, sound_manager_get_listener, index);
}

//.text:005182C0 ; void __cdecl sound_manager_handle_tag_reload()
//.text:005182D0 ; real_decibel __cdecl sound_manager_master_gain_db(int16)
//.text:00518510 ; void __cdecl sound_manager_prepare_for_tag_reload()
//.text:00518520 ; void __cdecl sound_manager_refresh_listeners()
//.text:00518530 ; void __cdecl sound_manager_reset_promotion_timers()
//.text:005185A0 ; void __cdecl sound_manager_set_ducker(e_sound_class_ducker)
//.text:00518620 ; void __cdecl sound_manager_set_global_environment(const s_sound_global_environment_parameters*)
//.text:00518630 ; void __cdecl sound_manager_set_sound_environment(const s_sound_manager_environment*)
//.text:00518650 ; const s_sound_listener* __cdecl sound_manager_try_and_get_default_listener()
//.text:00518680 ; void __cdecl sound_manager_update_sound_environment()
//.text:005189C0 ; void __cdecl sound_notify_on_stop_internal(int32, e_sound_event_stop_reason)
//.text:00518A20 ; void __cdecl sound_notify_playback_position_internal(int32, int32)
//.text:00518AE0 ; void __cdecl sound_notify_playing_internal(int32)
//.text:00518B40 ; void __cdecl sound_pause(e_sound_pause_state)
//.text:00518CE0 ; void __cdecl sound_permutation_add_reference(s_sound_permutation_chunk*)
//.text:00518CF0 ; 
//.text:00518D00 ; void __cdecl sound_permutation_release(s_sound_permutation_chunk*)
//.text:00518D10 ; bool __cdecl sound_preempts_sound(int32, int32, real32)

void __cdecl sound_render()
{
	INVOKE(0x00518E20, sound_render);
}

void __cdecl sound_render_dispatch()
{
	INVOKE(0x005190E0, sound_render_dispatch);

	//internal_event_wait(k_event_sound_render_deferred_start);
	//platform_sound_end_frame_deferred();
	//internal_event_set(k_event_sound_render_deferred_complete);
}

//.text:00519100 ; uns32 __cdecl sound_render_time()
//.text:00519110 ; real32 __cdecl sound_scale_value(real32, real32, const s_sound_parameter_range*)
//.text:00519140 ; real32 __cdecl sound_scale_value(real32, real32, const s_sound_parameter_range_shorts*)
//.text:00519180 ; void __cdecl sound_set_cinematic_skip_fade(real32)
//.text:005191B0 ; void __cdecl sound_set_playing_permutation(int32, int32, int16, int16, int16)
//.text:00519200 ; bool __cdecl sound_should_update_listener(int32)
//.text:00519220 ; real32 __cdecl sound_source_calculate_distance_rolloff_factor(const s_sound_source*, int32, int32, const real_point3d*)
//.text:00519290 ; real32 __cdecl sound_source_calculate_distance_rolloff_factor_simple(const s_sound_source*, int32, real32)
//.text:005192D0 ; real32 __cdecl sound_source_calculate_location_rolloff_factor(const s_sound_source*, int32, int32, const real_point3d*, const real_vector3d*, real32*)
//.text:005195F0 ; 
//.text:00519640 ; 
//.text:005196A0 ; real_vector3d* __cdecl sound_source_get_forward(const s_sound_source*, real_vector3d*)

real32 __cdecl sound_source_get_inner_cone_angle(const s_sound_source* source, int32 sound_definition_index)
{
	ASSERT(source);

	return INVOKE(0x005196D0, sound_source_get_inner_cone_angle, source, sound_definition_index);
}

//.text:00519750 ; 
//.text:00519790 ; real32 __cdecl sound_source_get_maximum_distance(const s_sound_source*, int32)
//.text:005197D0 ; 

real32 __cdecl sound_source_get_outer_cone_angle(const s_sound_source* source, int32 sound_definition_index)
{
	ASSERT(source);

	return INVOKE(0x00519810, sound_source_get_outer_cone_angle, source, sound_definition_index);
}

//.text:00519890 ; 
//.text:005198C0 ; void __cdecl sound_source_get_transmission_envelope(const s_sound_source*, int32, int32, s_sound_transmission_envelope*)
//.text:00519980 ; real_vector3d* __cdecl sound_source_get_up(const s_sound_source*, real_vector3d*)

void __cdecl sound_source_get_world_position(const s_sound_source* source, int32 listener_index, real_point3d* world_position)
{
	INVOKE(0x005199B0, sound_source_get_world_position, source, listener_index, world_position);
}

//.text:00519AD0 ; bool __cdecl sound_source_has_directional_attenuation(const s_sound_source*, int32)
//.text:00519B20 ; void __cdecl sound_start_fade_internal(int16, sound_event_fade_reason, real32, int32, int32)
//.text:00519BF0 ; void __cdecl sound_stop(int32, e_sound_event_stop_reason)

void __cdecl sound_stop_all(int32 game_state_proc_flags)
{
	INVOKE(0x00519CD0, sound_stop_all, game_state_proc_flags);
}

//.text:00519ED0 ; void __cdecl sound_stop_all_impulse()
//.text:0051A050 ; void __cdecl sound_stop_and_notify(int32, e_sound_event_stop_reason)
//.text:0051A0B0 ; 
//.text:0051A120 ; 
//.text:0051A130 ; bool __cdecl sound_track_and_update(int32, const s_sound_tracker*, c_flags<e_sound_datum_flag_bits, uns32, 18>*)
//.text:0051A230 ; 
//.text:0051A270 ; const s_sound_propagation_definition* __cdecl sound_try_to_get_global_propagation(bool)

void __cdecl sound_update()
{
	INVOKE(0x0051A2C0, sound_update);

	//refresh_listener(false);
}

//.text:0051A2D0 ; void __cdecl sound_update_fade(int32)
//.text:0051A320 ; void __cdecl sound_update_frame()
//.text:0051A330 ; 
//.text:0051A3A0 ; void __cdecl sound_update_pause_fade()
//.text:0051A4F0 ; bool __cdecl sound_update_readiness(int32, bool*)
//.text:0051A530 ; void __cdecl sound_update_time()
//.text:0051A880 ; 
//.text:0051A8B0 ; int32 __cdecl source_audible(const s_sound_source*, real32, real32*)
//.text:0051A8E0 ; int32 __cdecl source_audible_internal(const s_sound_source*, real32, real32, real32*)
//.text:0051AA30 ; real32 __cdecl source_distance(int32, const s_sound_source*)
//.text:0051AA70 ; real32 __cdecl source_distance_squared(int32, const s_sound_source*)
//.text:0051ACC0 ; int32 __cdecl source_really_audible(const s_sound_source*, int32, real32, real32*)
//.text:0051AD00 ; void __cdecl stop_impulse_sounds_by_sound_class(int32, e_sound_event_stop_reason)
//.text:0051AF10 ; void __cdecl stop_sounds_by_sound_class(int32, e_sound_event_stop_reason)
//.text:0051AF30 ; public: bool sound_class_definition::stop_when_object_dies() const
//.text:0051AF40 ; public: bool sound_class_definition::stops_when_attached_to_object() const
//.text:0051AF70 ; public: bool sound_class_definition::stops_with_object() const
//.text:0051AF80 ; 
//.text:0051AFA0 ; 
//.text:0051AFC0 ; 
//.text:0051AFE0 ; 
//.text:0051B000 ; 
//.text:0051B030 ; 
//.text:0051B060 ; 
//.text:0051B070 ; void __cdecl touch_channels(s_game_sound_obstruction_parameters*, c_static_array<const s_sound_environment_parameters*, 196>*, c_static_array<const s_sound_environment_parameters*, 196>*)
//.text:0051B350 ; void __cdecl update_channel_for_impulse_sound(int16, real_decibel, const s_sound_output_parameters*)
//.text:0051B7D0 ; void __cdecl update_channel_hardware(const c_static_array<const s_sound_transmission_output_parameters*, 196>&, const c_static_array<const s_sound_transmission_output_parameters*, 196>&)
//.text:0051B870 ; void __cdecl update_channels()

real32 __cdecl sound_definition_get_maximum_distance(int32 sound_definition_index)
{
	return INVOKE(0x00661EF0, sound_definition_get_maximum_distance, sound_definition_index);
}

real32 __cdecl sound_definition_get_minimum_distance(int32 sound_definition_index)
{
	return INVOKE(0x00661FC0, sound_definition_get_minimum_distance, sound_definition_index);
}

const char* path_add_parent_directory(const char* original_path, const char* stripped_path)
{
	const char* path_end = &original_path[strlen_debug(original_path)];
	ASSERT(IN_RANGE_INCLUSIVE(stripped_path, original_path, path_end));

	const char* v5 = stripped_path - 1;
	if (stripped_path - 1 <= original_path)
		v5 = original_path;

	const char* i = 0;
	for (i = v5; i > original_path && *(i - 1) != '\\'; i--)
		;

	return i;
}

void __cdecl render_debug_sound(int32 sound_index)
{
	if (!debug_sound)
	{
		return;
	}

	sound_datum* sound = DATUM_TRY_AND_GET(g_sound_data, sound_datum, sound_index);
	if (!sound)
	{
		return;
	}

	if (!TEST_BIT(sound->flags, 1))
	{
		return;
	}

	sound_channel_datum* channel = nullptr;
	for (int16 channel_index = 0; channel_index < g_sound_manager_globals->channel_count; channel_index++)
	{
		sound_channel_datum* channel_ = channel_get(channel_index);
		if (channel_->sound_index == sound_index)
		{
			channel = channel_;
			break;
		}
	}

	int16 hardware_source_index = NONE;
	int16 hardware_voice_index = NONE;
	if (channel)
	{
		hardware_source_index = channel->hardware_source_index;
		hardware_voice_index = channel->hardware_voice_index;
	}

	void* sound_definition = TAG_GET(SOUND_TAG, void, sound->definition_index);

	const char* tag_name = tag_get_name(sound->definition_index);

	c_static_string<512> debug_string;
	debug_string.print(
		"%s|n"
		"source %d|n"
		"voice %d|n"
		"pitch %.2f|n"
		"scale %.2f|n"
		"%.2f,%.2f,%.2f",
		path_add_parent_directory(tag_name, tag_name_strip_path(tag_name)),
		hardware_source_index,
		hardware_voice_index,
		sound_definition_map_pitch(sound_definition, sound->__unknownA4, sound->source.scale),
		sound->source.scale,
		sound->source.location.position.x,
		sound->source.location.position.y,
		sound->source.location.position.z);

	render_debug_string_at_point(&sound->source.location.position, debug_string.get_string(), global_real_argb_white);

	real32 minimum_distance = sound_definition_get_minimum_distance(sound->definition_index);
	if (TEST_BIT(sound->source.flags, 2))
	{
		minimum_distance = sound->source.minimum_distance;
	}

	real32 maximum_distance = sound_definition_get_maximum_distance(sound->definition_index);
	if (TEST_BIT(sound->source.flags, 3))
	{
		maximum_distance = sound->source.maximum_distance;
	}

	real32 outer_cone_angle = sound_source_get_outer_cone_angle(&sound->source, sound->definition_index);
	real32 inner_cone_angle = sound_source_get_inner_cone_angle(&sound->source, sound->definition_index);

	real_point3d cone_point{};
	quaternion_transform_point(&sound->source.location.orientation, (real_point3d*)global_forward3d, &cone_point);

	real_point3d volume_type_point{};
	switch (sound->source.volume_type)
	{
	case 0:
	{
		render_debug_sphere(false, &sound->source.location.position, minimum_distance, global_real_argb_green);
		render_debug_sphere(false, &sound->source.location.position, maximum_distance, global_real_argb_darkgreen);
	}
	break;
	case 1:
	{
		quaternion_transform_point(&sound->source.location.orientation, (real_point3d*)global_up3d, &volume_type_point);
		volume_type_point.x *= sound->source.__unknown38;
		volume_type_point.y *= sound->source.__unknown38;
		volume_type_point.z *= sound->source.__unknown38;
		render_debug_cylinder(false, &sound->source.location.position, (real_vector3d*)&volume_type_point, minimum_distance, global_real_argb_green);
		render_debug_cylinder(false, &sound->source.location.position, (real_vector3d*)&volume_type_point, maximum_distance, global_real_argb_darkgreen);
	}
	break;
	case 2:
	{
		quaternion_transform_point(&sound->source.location.orientation, (real_point3d*)global_up3d, &volume_type_point);
		volume_type_point.x *= sound->source.__unknown38;
		volume_type_point.y *= sound->source.__unknown38;
		volume_type_point.z *= sound->source.__unknown38;
		render_debug_pill(false, &sound->source.location.position, (real_vector3d*)&volume_type_point, minimum_distance, global_real_argb_green);
		render_debug_pill(false, &sound->source.location.position, (real_vector3d*)&volume_type_point, maximum_distance, global_real_argb_darkgreen);
	}
	break;
	}

	if (inner_cone_angle > 0.0f && inner_cone_angle < PI || outer_cone_angle > 0.0f && outer_cone_angle < PI)
	{
		render_debug_cone_outline(false, &sound->source.location.position, (real_vector3d*)&cone_point, maximum_distance, inner_cone_angle, global_real_argb_orange);
		render_debug_cone_outline(false, &sound->source.location.position, (real_vector3d*)&cone_point, maximum_distance, outer_cone_angle, global_real_argb_red);
	}

	if (sound->listener_index == NONE || TEST_BIT(sound->source.flags, 7) || !TEST_BIT(sound->source.flags, 0))
	{
		return;
	}

	int32 unit_index = player_mapping_get_unit_by_output_user(sound->listener_index);
	if (unit_index != NONE)
	{
		const s_sound_listener* listener = sound_manager_get_listener(sound->listener_index); // listener_get_internal
		ASSERT(listener->valid);

		switch ((16 * sound->source.spatialization_mode) >> 4)
		{
		case 0:
		{
			volume_type_point = listener->matrix.position;
		}
		break;
		case 1:
		{
			sound_source_get_world_position(&sound->source, sound->listener_index, &volume_type_point);
		}
		break;
		case 2:
		{
			matrix4x3_transform_point(&listener->matrix, &sound->source.location.position, &volume_type_point);
		}
		break;
		}

		unit_get_head_position(unit_index, &cone_point);
		render_debug_line(true, &volume_type_point, &cone_point, global_real_argb_red);
	}
}

const char* __cdecl get_channel_source_type_name(int32 type, bool verbose)
{
	ASSERT(VALID_INDEX(type, 5));

	switch (type)
	{
	case 0: return verbose ? "Sound_Mono   " : "S_1";
	case 1: return verbose ? "Sound_Stereo " : "S_2";
	case 2: return verbose ? "Sound_quad   " : "S_4";
	case 3: return verbose ? "Sound_5_dot_1" : "S_6";
	case 4: return verbose ? "Sound_Codec  " : "S_C";
	case 5: return verbose ? "Music_Mono   " : "M_1";
	case 6: return verbose ? "Music_Stereo " : "M_2";
	case 7: return verbose ? "Music_Quad   " : "M_4";
	case 8: return verbose ? "Music_5_dot_1" : "M_6";
	case 9: return verbose ? "Music_Codec  " : "M_C";
	}

	return "";
}

void __cdecl sound_debug_render()
{
	c_static_string<8192> debug_string;

	//platform_sound_debug_render();

	if (debug_sound_class_totals)
	{
		//int32 v5 = NONE;
		//for (int32 i = 0; i < k_sound_class_count; i++)
		//{
		//	if (!strchr(sound_class_names[i], '!'))
		//	{
		//		if (TEST_BIT(++v5, 0))
		//		{
		//			status_line_appendf(sound_class_totals_status_lines[v5 / 2], "% 40s: % 2d", sound_class_names[i], sound_class_totals[i]);
		//		}
		//		else
		//		{
		//			status_line_appendf(sound_class_totals_status_lines[v5 / 2], "% 40s: % 2d", sound_class_names[i], sound_class_totals[i]);
		//		}
		//	}
		//}
	}

	if (debug_sound_timing)
	{
		debug_string.append_print("Sound system_time: %6.4f render_time: %6.4f delta_time: %1.4f|n",
			g_sound_manager_globals->system_time / 1000.0f,
			g_sound_manager_globals->render_time / 1000.0f,
			g_sound_manager_globals->delta_time);

		debug_string.append("|n");
	}

	if (debug_duckers)
	{
		if (g_sound_manager_globals->ducker.active_ducker != NONE && g_sound_manager_globals->ducker.active_ducker_time >= 5.0f)
		{
			debug_string.append_print("ducker inactive for %1.2f seconds|n",
				g_sound_manager_globals->ducker.inactive_time);
		}
		else
		{
			debug_string.append_print("active ducker: %d time: %1.2f last ducker: %d inactive time: %1.2f|n",
				g_sound_manager_globals->ducker.active_ducker,
				g_sound_manager_globals->ducker.active_ducker_time,
				g_sound_manager_globals->ducker.last_ducker,
				g_sound_manager_globals->ducker.inactive_time);
		}
		debug_string.append("|n");
	}

	if (debug_sound_listeners)
	{
		for (int32 i = 0; i < k_number_of_sound_manager_listeners; i++)
		{
			const s_sound_listener* listener = sound_manager_get_listener(i);
			if (listener->valid)
			{
				render_debug_matrix(true, &listener->matrix, 0.3f);
			}
		}
	}

	if (debug_sound)
	{
		c_data_iterator<sound_datum> sound_iterator;
		sound_iterator.begin(g_sound_data);
		do
		{
			render_debug_sound(sound_iterator.get_index());
		} while (sound_iterator.next());

		int32 sound_datums = NONE;
		if (g_sound_data)
		{
			sound_datums = g_sound_data->actual_count;
		}

		int32 looping_sound_datums = NONE;
		if (g_looping_sound_data)
		{
			looping_sound_datums = g_looping_sound_data->actual_count;
		}

		int32 playback_controllers = NONE;
		if (g_sound_playback_controller_data)
		{
			playback_controllers = g_sound_playback_controller_data->actual_count;
		}

		int32 sound_effects = NONE;
		if (g_sound_effect_data)
		{
			sound_effects = g_sound_effect_data->actual_count;
		}

		int32 voices = NONE;

		debug_string.append_print(
			"|tsound datums: %d|n"
			"|tlooping sound datums: %d|n"
			"|tplayback controllers: %d|n"
			"|tsound effects: %d|n"
			"|tvoices: %d|n",
			sound_datums,
			looping_sound_datums,
			playback_controllers,
			sound_effects,
			voices);

		debug_string.append("|n");
	}

	if (debug_sound_manager_channels)
	{
		int32 inuse[5]{};
		int32 count[5]{};

		for (int16 channel_index = 0; channel_index < g_sound_manager_globals->channel_count; channel_index++)
		{
			sound_channel_datum* channel = channel_get(channel_index);
			if (channel->sound_index != NONE)
			{
				inuse[channel->__unknownA_sound_playback]++;
			}

			count[channel->__unknownA_sound_playback]++;
		}

		for (int32 i = 0; i < NUMBEROF(inuse); i++)
		{
			debug_string.append_print("Channels %s: %4d / %4d |n",
				get_channel_source_type_name(i, true),
				inuse[i],
				count[i]);
		}

		debug_string.append("|n");
	}

	if (debug_sound_manager_channels)
	{
		for (int16 channel_index = 0; channel_index < g_sound_manager_globals->channel_count; channel_index++)
		{
			sound_channel_datum* channel = channel_get(channel_index);
			if (channel->sound_index == NONE)
			{
				continue;
			}
	
			sound_datum* sound = DATUM_TRY_AND_GET(g_sound_data, sound_datum, channel->sound_index);
			if (!sound)
			{
				continue;
			}

			const char* sound_playback_name = get_channel_source_type_name(channel->__unknownA_sound_playback, true);
			const char* sound_name = tag_name_strip_path(tag_get_name(sound->definition_index));

			debug_string.append_print("c%3d: %s: %7.2f %s|n",
				channel_index,
				sound_playback_name,
				channel->__unknown18,
				sound_name);
		}

		debug_string.append("|n");
	}

	if (!debug_string.is_empty())
		render_debug_string(debug_string.get_string());

	//looping_sound_render_debug();
}

