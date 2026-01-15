#pragma once

enum e_map_memory_configuration;

enum e_ui_allocation_marker
{
	_ui_allocation_marker_dummy = 0,
};

class c_allocation_base;

class c_ui_memory_scope_lock
{
public:
	c_ui_memory_scope_lock();
	~c_ui_memory_scope_lock();
};

extern void* operator new(unsigned int size, e_ui_allocation_marker unused);
extern void operator delete(void* object, e_ui_allocation_marker unused);
extern void __cdecl user_interface_free(const void* ptr);
extern void* __cdecl user_interface_malloc(unsigned int size);
extern void* __cdecl user_interface_malloc_tracked(unsigned int size, const char* file, int32 line);
extern c_allocation_base* __cdecl user_interface_memory_allocation();
extern void __cdecl user_interface_memory_dispose();
extern void __cdecl user_interface_memory_dump(const char* prefix);
extern void __cdecl user_interface_memory_initialize(e_map_memory_configuration memory_configuration);
extern void __cdecl user_interface_memory_update_status_line();
extern void __cdecl user_interface_out_of_memory(unsigned int size, const char* file, int32 line);
extern void* __cdecl user_interface_realloc(void* ptr, unsigned int size);
extern void* __cdecl user_interface_realloc_tracked(void* ptr, unsigned int size, const char* file, int32 line);

