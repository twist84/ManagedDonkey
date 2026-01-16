#pragma once

#include "cseries/cseries.hpp"
#include "cseries/location.hpp"

#define MAXIMUM_SOUND_SOURCES 196
#define NUMBER_OF_SOUND_SAMPLE_RATES 3

tag const k_sound_listener_header_signature = 'lshd';
tag const k_sound_listener_footer_signature = 'lsft';
int32 const k_number_of_sound_manager_listeners = 4;

struct s_sound_listener
{
	tag header_signature;
	s_location location;
	byte __data6[0x2];
	bool valid;
	bool __unknown9; // underwater?
	uns8 __unknownA;
	uns8 __unknownB;
	real_matrix4x3 matrix;
	real_vector3d velocity;
	tag footer_signature;
};
COMPILE_ASSERT(sizeof(s_sound_listener) == 0x50);

struct s_sound_azimuth_image
{
	real32 start_angle;
	real32 end_angle;
	real32 pitch_factor;
	real32 gain_fraction;
};
COMPILE_ASSERT(sizeof(s_sound_azimuth_image) == 0x10);

struct s_sound_manager_reverb
{
	real32 interpolation_speed;
	real32 reverb_gain;
	int32 sound_environment_index;
	s_sound_azimuth_image azimuth_image;
};
COMPILE_ASSERT(sizeof(s_sound_manager_reverb) == 0x1C);

struct s_platform_sound_status
{
	byte __data0[0x60];
};
COMPILE_ASSERT(sizeof(s_platform_sound_status) == 0x60);

struct s_sound_manager_globals
{
	s_platform_sound_status sound_status;

	c_static_flags<MAXIMUM_SOUND_SOURCES> used_sources;
	c_static_flags<MAXIMUM_SOUND_SOURCES> available_sources;

	bool fully_initialized;
	bool __unknown99;
	bool game_active;

	uns8 pause_state;
	bool idling;
	bool recursion_lock;

	uns8 __unknown9E;
	uns8 __unknown9F;
	uns8 __unknownA0;

	uns32 system_time;
	uns32 render_time;

	c_static_array<s_sound_listener, k_number_of_sound_manager_listeners> listeners;

	byte __data1EC[0x40];
	byte __data22C[0x4];

	c_static_array<s_sound_manager_reverb, 2> manager_reverbs;

	int32 __unknown268;

	real32 delta_time;

	int32 __unknown27C;
	real32 __unknown270;
	real32 __unknown274;
	real32 __unknown278;
	int32 __unknown28C;
	real32 __unknown280;
	real32 __unknown284;
	int32 __unknown288;
	int32 __unknown29C;
	real32 __unknown290;
	real32 __unknown294;
	real32 __unknown298;

	struct
	{
		int32 active_ducker;
		real32 active_ducker_time;
		int32 last_ducker;
		real32 inactive_time;
	} ducker;

	int16 channel_count;
	int16 __unknown2B2;
	real32 __unknown2B4;
};
COMPILE_ASSERT(sizeof(s_sound_manager_globals) == 0x2B8);

struct s_sound_channel_properties
{
	byte __data[0x4B8];
};
COMPILE_ASSERT(sizeof(s_sound_channel_properties) == 0x4B8);

extern bool debug_sound_class_totals;
extern bool debug_sound_timing;
extern bool debug_duckers;
extern bool debug_sound_listeners;
extern bool debug_sound;
extern bool debug_sound_manager_channels;

extern real32 sound_definition_map_pitch(const void* sound_definition, real32 a1, real32 a2);
extern void __cdecl sound_delete(int32 sound_index);
extern void __cdecl sound_dispose();
extern void __cdecl sound_dispose_from_old_map();
extern void __cdecl sound_dispose_from_old_structure_bsp(uns32 structure_bps_mask);
extern void __cdecl sound_game_pause_handler(bool paused);
extern void __cdecl sound_idle();
extern void __cdecl sound_initialize();
extern void __cdecl sound_initialize_for_new_map();
extern void __cdecl sound_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
extern bool __cdecl sound_is_active();
extern bool __cdecl sound_is_fading_out(int32 sound_index);
extern const s_sound_listener* __cdecl sound_manager_get_listener(int32 listener_index);
extern void __cdecl sound_render();
extern void __cdecl sound_render_dispatch();
extern void __cdecl sound_stop_all(int32 game_state_proc_flags);
extern void __cdecl sound_update();

extern void __cdecl sound_debug_render();

