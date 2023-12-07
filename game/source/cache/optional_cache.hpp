#pragma once

#include "cseries/cseries.hpp"

enum e_map_memory_configuration;
struct s_optional_cache_user_memory_configuration;
enum e_optional_cache_user;
enum e_optional_cache_user_priority;
struct c_optional_cache_user_callback;
struct c_optional_cache_backend
{
private:
	virtual void initialize(e_map_memory_configuration memory_configuration, c_static_array<s_optional_cache_user_memory_configuration, 6> const*);
	virtual void dispose();
	virtual void* try_to_allocate(e_optional_cache_user, e_optional_cache_user_priority, long, c_optional_cache_user_callback*);
	virtual void deallocate(e_optional_cache_user, void*);

	bool m_active;
	dword __unknown8;
};
static_assert(sizeof(c_optional_cache_backend) == 0xC);
