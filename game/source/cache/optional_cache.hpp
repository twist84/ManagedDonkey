#pragma once

#include "cseries/cseries.hpp"

enum e_optional_cache_user
{
	k_number_of_optional_cache_users = 6
};

struct s_optional_cache_user_memory_configuration
{
	long __unknown0;
	long __unknown4;
};
static_assert(sizeof(s_optional_cache_user_memory_configuration) == 0x8);

enum e_optional_cache_user_priority;
enum e_map_memory_configuration;
struct s_optional_cache_user_memory_configuration;

struct c_optional_cache_user_callback
{
public:
	virtual void get_memory_configuration(e_map_memory_configuration, s_optional_cache_user_memory_configuration*);
	virtual void idle(void const*);
	virtual void terminate(void const*);
};
static_assert(sizeof(c_optional_cache_user_callback) == sizeof(void*));

struct s_optional_cache_user
{
	c_optional_cache_user_callback* callback;
	void* allocated_memory;
};
static_assert(sizeof(s_optional_cache_user) == 0x8);

struct c_optional_cache_backend
{
private:
	virtual void initialize(e_map_memory_configuration memory_configuration, c_static_array<s_optional_cache_user_memory_configuration, k_number_of_optional_cache_users> const*);
	virtual void dispose();
	virtual void* try_to_allocate(e_optional_cache_user, e_optional_cache_user_priority, long, c_optional_cache_user_callback*);
	virtual void deallocate(e_optional_cache_user, void*);

	bool m_active;
	dword __unknown8;
};
static_assert(sizeof(c_optional_cache_backend) == 0xC);

extern void __cdecl _optional_cache_free(e_optional_cache_user user, void* buffer);
extern void* __cdecl _optional_cache_try_to_allocate(e_optional_cache_user user, e_optional_cache_user_priority user_priority, long size, c_optional_cache_user_callback* callback);
extern void __cdecl optional_cache_clear_in_game_backend(c_optional_cache_backend* backend);
extern void __cdecl optional_cache_dispose();
extern s_optional_cache_user* __cdecl optional_cache_get_user(e_optional_cache_user user);
extern void __cdecl optional_cache_initialize();
extern void __cdecl optional_cache_main_loop_idle();
extern void __cdecl optional_cache_memory_dispose();
extern void __cdecl optional_cache_memory_initialize(e_map_memory_configuration memory_configuration);
extern void __cdecl optional_cache_register_user(e_optional_cache_user user, c_optional_cache_user_callback* callback);
extern void __cdecl optional_cache_set_in_game_backend(c_optional_cache_backend* backend);
extern void __cdecl optional_cache_terminate_allocations();
extern void __cdecl optional_cache_unregister_user(e_optional_cache_user user, c_optional_cache_user_callback* callback);
extern void __cdecl optional_cache_users_get_memory_configuration(e_map_memory_configuration memory_configuration, c_static_array<s_optional_cache_user_memory_configuration, k_number_of_optional_cache_users>* out_memory_configurations);

