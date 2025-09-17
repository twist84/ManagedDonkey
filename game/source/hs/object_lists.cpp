#include "hs/object_lists.hpp"

void __cdecl object_list_add(int32 object_list_index, int32 object_index)
{
	INVOKE(0x006EA9D0, object_list_add, object_list_index, object_index);
}

void __cdecl object_list_add_reference(int32 object_list_index)
{
	INVOKE(0x006EAA20, object_list_add_reference, object_list_index);
}

int16 __cdecl object_list_count(int32 object_list_index)
{
	return INVOKE(0x006EAA60, object_list_count, object_list_index);
}

int16 __cdecl object_list_count_not_dead(int32 object_list_index)
{
	return INVOKE(0x006EAAA0, object_list_count_not_dead, object_list_index);
}

void __cdecl object_list_delete(int32 list_index)
{
	INVOKE(0x006EAB40, object_list_delete, list_index);
}

void __cdecl object_list_gc()
{
	INVOKE(0x006EAB90, object_list_gc);
}

int32 __cdecl object_list_get_first(int32 object_list_index, int32* reference_index)
{
	return INVOKE(0x006EAC20, object_list_get_first, object_list_index, reference_index);
}

int32 __cdecl object_list_get_next(int32 object_list_index, int32* reference_index)
{
	return INVOKE(0x006EAC70, object_list_get_next, object_list_index, reference_index);
}

int32 __cdecl object_list_new()
{
	return INVOKE(0x006EACA0, object_list_new);
}

bool __cdecl object_list_remove(int32 object_list_index, int32 object_index)
{
	return INVOKE(0x006EACF0, object_list_remove, object_list_index, object_index);
}

void __cdecl object_list_remove_reference(int32 object_list_index)
{
	INVOKE(0x006EAD60, object_list_remove_reference, object_list_index);
}

void __cdecl object_lists_dispose()
{
	INVOKE(0x006EADA0, object_lists_dispose);
}

void __cdecl object_lists_dispose_from_old_map()
{
	INVOKE(0x006EADB0, object_lists_dispose_from_old_map);
}

void __cdecl object_lists_initialize()
{
	INVOKE(0x006EADE0, object_lists_initialize);
}

void __cdecl object_lists_initialize_for_new_map()
{
	INVOKE(0x006EAE30, object_lists_initialize_for_new_map);
}

//.text:006EAE60 ; 
//.text:006EAEA0 ; 

