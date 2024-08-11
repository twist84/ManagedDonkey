#pragma once

#include "cseries/cseries.hpp"

// Havok Types

struct hkRigidBody;
struct hkMemory;

struct hkThreadMemory
{
	virtual void* alignedAllocate(int, int, enum HK_MEMORY_CLASS);
	virtual void alignedDeallocate(void*);
	virtual void setStackArea(void*, int);
	virtual void releaseCachedMemory(void);
	virtual void destructor(unsigned int);
	virtual void* onStackOverflow(int);
	virtual void onStackUnderflow(void*);

	struct Stack
	{
		int __unknown0;
		int __unknown4;
		int __unknown8;
		int __unknownC;
	};
	static_assert(sizeof(Stack) == 0x10);

	struct FreeList
	{
		void* __unknown0;
		int __unknown4;
	};
	static_assert(sizeof(FreeList) == 0x8);

	byte __data4[0xC];
	hkMemory* m_memoryAllocator;
	int m_referenceCount;
	byte __data18[0x8];
	Stack m_stack;
	byte __data30[0x4];
	int __unknown34;
	FreeList m_freeList[17];
	int __unknownC0[17];
	char __unknown104[512 + 1];
	int __data[10];
};
static_assert(sizeof(hkThreadMemory) == 0x330);

struct hkBool
{
	int m_value;
};
static_assert(sizeof(hkBool) == 0x4);

struct hkMonitorStream
{
	char* __unknown0;
	char* __unknown4;
	char* __unknown8;
	char* __unknownC;
	hkBool __unknown10;
};
static_assert(sizeof(hkMonitorStream) == 0x14);

// Blam Types

struct s_havok_gamestate
{
	long last_state_reset_time;
	long last_garbage_collection_reset_time;
};
static_assert(sizeof(s_havok_gamestate) == 0x8);

struct s_havok_constants
{
	real collision_tolerance;
	real __unknown4;
	long environment_type;
	long debug_mode;
	real shape_radius;
	bool disable_deactivation;
	real deactivation_reference_distance;
	bool jumping_beans;
	bool weld_environment;
	bool shape_cache;
	bool shape_cache_debug;
	bool enable_back_stepping;
	bool enable_sweep_shapes;
	bool display_stats;
	bool initialize_profiling;
	bool impacts_debug;
	bool proxies_debug;
	bool collision_damage_debug;
	bool play_impact_effects;
	bool play_biped_impact_effects;
	bool physics_debug;
	bool cleanup_inactive_agents;
	bool batch_add_entities_disabled;
	real __unknown2C;
	real __unknown30;
	long thread_count;
};
static_assert(sizeof(s_havok_constants) == 0x38);

struct s_havok_globals
{
	c_static_array<hkRigidBody*, 16> __unknown0;
	long last_find_inital_contact_points_player_absolute_index;
	long rigid_bodies_connected_to_world_count;
	long collision_damage_is_disable_time;
	long can_modify_state;
	bool rigid_bodies_active;
	bool environment_active;
	bool resetting_havok_state;
	bool garbage_collection_locked;
	bool inside_simulation_step;
	bool broadphase_attachment_disabled;

	hkThreadMemory thread_memory;
	hkThreadMemory render_thread_memory;
	hkMonitorStream render_thread_monitor_stream;
	char __pad6CC[4];
	byte render_thread_memory_stack_area[65536];
	c_static_array<hkMonitorStream, 1> thread_monitor_stream_buffer;
	long thread_monitor_stream_buffer_count;
};
static_assert(sizeof(s_havok_globals) == 0x106E8);

extern s_havok_constants& g_havok_constants;
extern s_havok_globals& g_havok_globals;
extern bool& g_havok_memory_always_system;

extern void __cdecl havok_can_modify_state_allow();
extern void __cdecl havok_can_modify_state_disallow();
extern void __cdecl havok_display_stats(bool display_as_event);
extern void __cdecl havok_display_stats_printf(bool display_as_event, real_argb_color const* color, char const* format, ...);
extern void __cdecl havok_object_set_position(long object_index, bool a2, bool a3, bool a4);
extern void __cdecl havok_prepare_fpu_for_update();
extern void __cdecl havok_restore_fpu_from_update();
extern void __cdecl havok_update();

extern s_havok_constants const* get_havok_constants();
extern void havok_debug_render();

