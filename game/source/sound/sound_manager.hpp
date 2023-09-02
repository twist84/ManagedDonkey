#pragma once

#include "cseries/cseries.hpp"

struct s_sound_listener
{
	tag header_signature;
	s_location location;
	bool valid;
	bool __unknown9;
	byte __unknownA;
	byte __unknownB;
	real_matrix4x3 matrix;
	vector3d vector;
	tag footer_signature;
};
static_assert(sizeof(s_sound_listener) == 0x50);

struct s_sound_manager_reverb
{
	real __unknown0;
	real __unknown4;
	long __unknown8;
	real __unknownC;
	real __unknown10;
	real __unknown14;
	real __unknown18;
};
static_assert(sizeof(s_sound_manager_reverb) == 0x1C);

struct s_sound_manager_globals
{
	byte __data0[0x44];

	dword_flags __unknown44_sources[7]; // c_static_flags<224>
	dword_flags used_sources[7]; // c_static_flags<224>
	dword_flags available_sources[7]; // c_static_flags<224>

	bool __unknown98;
	bool __unknown99;
	bool __unknown9A;

	byte pause_state;

	bool __unknown9C;
	bool __unknown9D;
	byte __unknown9E;
	byte __unknown9F;
	bool __unknownA0;

	dword render_time;

	long __unknownA8;

	c_static_array<s_sound_listener, 4> listeners;

	byte __data1EC[64];
	byte __data22C[4];

	c_static_array<s_sound_manager_reverb, 2> manager_reverbs;

	byte __data268[56];

	byte __unknown2A0_ducker[16];

	short channel_count;
	short __unknown2B2;
	real __unknown2B4;
};
static_assert(sizeof(s_sound_manager_globals) == 0x2B8);

extern bool debug_sound_listeners;
extern bool debug_sound;
extern bool debug_sound_manager_channels;

extern s_sound_listener const* __cdecl sound_manager_get_listener(long listener_index);

extern void __cdecl sound_debug_render();

