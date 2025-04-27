#pragma once

#include "cseries/cseries.hpp"

struct data_reference :
	s_datum_header
{
	uint16 pad;
	long datum_index;
	long next_reference_index;
};

struct s_data_array;

extern void __cdecl reference_list_add(s_data_array* array, long* first_reference_index, long datum_index, long payload_size, void const* payload);
extern bool __cdecl reference_list_contains(s_data_array* array, long* first_reference_index, long datum_index);
extern void __cdecl reference_list_copy(s_data_array* result, s_data_array const* source);
extern void __cdecl reference_list_delete(s_data_array* array, long first_reference_index);
extern long __cdecl reference_list_get_next_datum_and_payload(s_data_array* array, long* reference_index, long payload_size, void const** out_payload);
extern long __cdecl reference_list_get_next_datum_index(s_data_array* array, long* reference_index);
extern s_data_array* __cdecl reference_list_new(char const* name, long payload_size, long maximum_count, c_allocation_base* allocator);
extern void __cdecl reference_list_remove(s_data_array* array, long* first_reference_index, long datum_index);
extern void __cdecl reference_list_update_payload(s_data_array* array, long const* first_reference_index, long datum_index, long payload_size, void const* payload);

