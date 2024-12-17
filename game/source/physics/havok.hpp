#pragma once

#include "cseries/cseries.hpp"

// Havok Types

struct hkRigidBody;
struct hkMemory;

template<typename T>
struct hkPadSpu
{
	T m_storage;
};
static_assert(sizeof(hkPadSpu<char*>) == sizeof(char*));

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
	char m_bool;
};
static_assert(sizeof(hkBool) == sizeof(char));

struct hkMonitorStream
{
	hkPadSpu<char*> m_start;
	hkPadSpu<char*> m_end;
	hkPadSpu<char*> m_capacity;
	hkPadSpu<char*> m_capacityMinus16;
	hkBool m_isBufferAllocatedOnTheHeap;
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
	real havok_collision_tolerance;
	real havok_iterative_linear_cast_early_out_distance;
	long havok_environment_type;
	long havok_debug_mode;
	real havok_shape_radius;
	bool havok_disable_deactivation;
	real havok_deactivation_reference_distance;
	bool havok_jumping_beans;
	bool havok_weld_environment;
	bool havok_shape_cache;
	bool havok_shape_cache_debug;
	bool havok_enable_back_stepping;
	bool havok_enable_sweep_shapes;
	bool havok_display_stats;
	bool havok_initialize_profiling;
	bool havok_render_impacts;
	bool havok_render_proxies;
	bool havok_render_collision_damage;
	bool havok_play_impact_effects;
	bool havok_play_biped_impact_effects;
	bool havok_render_shape_properties;
	bool havok_cleanup_inactive_agents;
	bool havok_batch_add_entities_disabled;
	real havok_maximum_linear_velocity;
	real havok_minimum_maximum_penetration_depth;
	long havok_thread_count;
};
static_assert(sizeof(s_havok_constants) == 0x38);

struct s_havok_globals
{
	c_static_array<hkRigidBody*, 16> environment_bodies;
	long gamestate_disconnection_level;
	long rigid_bodies_connected_to_world_count;
	long last_time_object_rigid_bodies_added_to_world;
	long can_modify_state;
	bool rigid_bodies_active;
	bool environment_active;
	bool disable_rebuilding_environment_mopp;
	bool garbage_collection_locked;
	bool inside_simulation_step;
	bool broadphase_attachment_disabled;
	__declspec(align(8)) long thread_memory_buffer[204];
	long render_thread_memory_buffer[204];
	long render_monitor_stream_buffer[5];
	__declspec(align(8)) long render_thread_memory_stack[16384];
	long thread_monitor_stream_buffer[1][5];
	long thread_monitor_stream_buffer_count;
	__declspec(align(8)) long collision_dispatcher_buffer[1860];
	bool collision_dispatcher_buffer_initialized;
};
static_assert(sizeof(s_havok_globals) == 0x12400);

extern s_havok_constants& g_havok_constants;
extern s_havok_globals& g_havok_globals;
extern bool& g_havok_memory_always_system;

extern void __cdecl havok_can_modify_state_allow();
extern void __cdecl havok_can_modify_state_disallow();
extern void __cdecl havok_display_stats(bool display_as_event);
extern void __cdecl havok_display_stats_printf(bool display_as_event, real_argb_color const* color, char const* format, ...);
extern void __cdecl havok_dispose();
extern void __cdecl havok_dispose_from_old_map();
extern void __cdecl havok_dispose_from_old_structure_bsp(dword deactivating_structure_bsp_mask);
extern void __cdecl havok_initialize();
extern void __cdecl havok_initialize_for_new_map();
extern void __cdecl havok_initialize_for_new_structure_bsp(dword activating_structure_bsp_mask);
extern void __cdecl havok_object_set_position(long object_index, bool a2, bool a3, bool a4);
extern void __cdecl havok_prepare_fpu_for_update();
extern void __cdecl havok_restore_fpu_from_update();
extern void __cdecl havok_update();

extern s_havok_constants const* get_havok_constants();
extern void havok_debug_render();

