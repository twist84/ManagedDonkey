#pragma once

#include "cseries/cseries.hpp"

struct object_list_header_datum :
	s_datum_header
{
	uns16 pad;
	int16 reference_count;
	int16 count;
	int32 first_reference_index;
};
static_assert(sizeof(object_list_header_datum) == 0xC);

struct object_list_datum :
	s_datum_header
{
	int32 object_index;
	int32 reference_index;
};
static_assert(sizeof(object_list_datum) == 0xC);

extern void __cdecl object_list_add(int32 object_list_index, int32 object_index);
extern void __cdecl object_list_add_reference(int32 object_list_index);
extern int16 __cdecl object_list_count(int32 object_list_index);
extern int16 __cdecl object_list_count_not_dead(int32 object_list_index);
extern void __cdecl object_list_delete(int32 list_index);
extern void __cdecl object_list_gc();
extern int32 __cdecl object_list_get_first(int32 object_list_index, int32* reference_index);
extern int32 __cdecl object_list_get_next(int32 object_list_index, int32* reference_index);
extern int32 __cdecl object_list_new();
extern bool __cdecl object_list_remove(int32 object_list_index, int32 object_index);
extern void __cdecl object_list_remove_reference(int32 object_list_index);
extern void __cdecl object_lists_dispose();
extern void __cdecl object_lists_dispose_from_old_map();
extern void __cdecl object_lists_initialize();
extern void __cdecl object_lists_initialize_for_new_map();

