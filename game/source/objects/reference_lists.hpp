#pragma once

#include "cseries/cseries.hpp"

struct data_reference :
	s_datum_header
{
	uns16 pad;
	int32 datum_index;
	int32 next_reference_index;
};

struct s_data_array;

extern void __cdecl reference_list_add(s_data_array* array, int32* first_reference_index, int32 datum_index, int32 payload_size, const void* payload);
extern bool __cdecl reference_list_contains(s_data_array* array, int32* first_reference_index, int32 datum_index);
extern void __cdecl reference_list_copy(s_data_array* result, const s_data_array* source);
extern void __cdecl reference_list_delete(s_data_array* array, int32 first_reference_index);
extern int32 __cdecl reference_list_get_next_datum_and_payload(s_data_array* array, int32* reference_index, int32 payload_size, const void** out_payload);
extern int32 __cdecl reference_list_get_next_datum_index(s_data_array* array, int32* reference_index);
extern s_data_array* __cdecl reference_list_new(const char* name, int32 payload_size, int32 maximum_count, c_allocation_base* allocator);
extern void __cdecl reference_list_remove(s_data_array* array, int32* first_reference_index, int32 datum_index);
extern void __cdecl reference_list_update_payload(s_data_array* array, const int32* first_reference_index, int32 datum_index, int32 payload_size, const void* payload);

