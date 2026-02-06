#include "objects/reference_lists.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/data.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x00BB6210, reference_list_remove);

void __cdecl reference_list_add(s_data_array* array, int32* first_reference_index, int32 datum_index, int32 payload_size, const void* payload)
{
	INVOKE(0x00BB6030, reference_list_add, array, first_reference_index, datum_index, payload_size, payload);

	//ASSERT(array);
	//ASSERT(first_reference_index);
	//
	//if (payload_size <= 0)
	//{
	//	ASSERT(array->size == sizeof(struct data_reference));
	//	ASSERT(payload == nullptr);
	//}
	//else
	//{
	//	ASSERT(payload_size == array->size - sizeof(struct data_reference));
	//	ASSERT(payload != nullptr);
	//}
	//
	//int32 reference_index = datum_new(array);
	//if (reference_index == NONE)
	//{
	//	event(_event_warning, "WARNING: maximum %ss per map (%d) exceeded.",
	//		array->name,
	//		array->maximum_count);
	//}
	//else
	//{
	//	struct data_reference* data_reference = nullptr;
	//	if (array->valid && DATUM_INDEX_TO_ABSOLUTE_INDEX(reference_index) < array->count)
	//	{
	//		data_reference = (struct data_reference*)offset_pointer(array->data, DATUM_INDEX_TO_ABSOLUTE_INDEX(reference_index) * array->size);
	//	}
	//	data_reference->datum_index = datum_index;
	//	data_reference->next_reference_index = *first_reference_index;
	//	*first_reference_index = reference_index;
	//
	//	if (payload)
	//	{
	//		csmemcpy(data_reference + 1, payload, payload_size);
	//	}
	//}
}

bool __cdecl reference_list_contains(s_data_array* array, int32* first_reference_index, int32 datum_index)
{
	return INVOKE(0x00BB6080, reference_list_contains, array, first_reference_index, datum_index);

	//int32 reference_index = *first_reference_index;
	//if (*first_reference_index == NONE)
	//{
	//	return false;
	//}
	//
	//while (true)
	//{
	//	struct data_reference* data_reference = nullptr;
	//	if (array->valid && DATUM_INDEX_TO_ABSOLUTE_INDEX(reference_index) < array->count)
	//	{
	//		data_reference = (struct data_reference*)offset_pointer(array->data, DATUM_INDEX_TO_ABSOLUTE_INDEX(reference_index) * array->size);
	//	}
	//
	//	if (data_reference->datum_index == datum_index)
	//		break;
	//
	//	reference_index = data_reference->next_reference_index;
	//	if (reference_index == NONE)
	//		return false;
	//}
	//
	//return true;
}

void __cdecl reference_list_copy(s_data_array* result, const s_data_array* source)
{
	INVOKE(0x00BB60D0, reference_list_copy, result, source);

	//data_copy(source, result);
}

void __cdecl reference_list_delete(s_data_array* array, int32 first_reference_index)
{
	INVOKE(0x00BB60F0, reference_list_delete, array, first_reference_index);

	//if (first_reference_index != NONE)
	//{
	//	int32 reference_index = first_reference_index;
	//	do
	//	{
	//		struct data_reference* data_reference = nullptr;
	//		if (array->valid && DATUM_INDEX_TO_ABSOLUTE_INDEX(reference_index) < array->count)
	//		{
	//			data_reference = (struct data_reference*)offset_pointer(array->data, DATUM_INDEX_TO_ABSOLUTE_INDEX(reference_index) * array->size);
	//		}
	//		datum_delete(array, reference_index);
	//	}
	//	while (reference_index != NONE);
	//}
}

int32 __cdecl reference_list_get_next_datum_and_payload(s_data_array* array, int32* reference_index, int32 payload_size, const void** out_payload)
{
	return INVOKE(0x00BB6130, reference_list_get_next_datum_and_payload, array, reference_index, payload_size, out_payload);

	//if (*reference_index == NONE)
	//{
	//	return NONE;
	//}
	//
	//struct data_reference* data_reference = nullptr;
	//if (array->valid && DATUM_INDEX_TO_ABSOLUTE_INDEX(*reference_index) < array->count)
	//{
	//	data_reference = (struct data_reference*)offset_pointer(array->data, DATUM_INDEX_TO_ABSOLUTE_INDEX(*reference_index) * array->size);
	//}
	//
	//ASSERT(payload_size == array->size - sizeof(struct data_reference));
	//ASSERT(payload_size > 0);
	//ASSERT(out_payload);
	//
	//*out_payload = data_reference + 1;
	//*reference_index = data_reference->next_reference_index;
	//return data_reference->datum_index;
}

int32 __cdecl reference_list_get_next_datum_index(s_data_array* array, int32* reference_index)
{
	return INVOKE(0x00BB6170, reference_list_get_next_datum_index, array, reference_index);

	//if (*reference_index == NONE)
	//{
	//	return NONE;
	//}
	//
	//struct data_reference* data_reference = nullptr;
	//if (array->valid && DATUM_INDEX_TO_ABSOLUTE_INDEX(*reference_index) < array->count)
	//{
	//	data_reference = (struct data_reference*)offset_pointer(array->data, DATUM_INDEX_TO_ABSOLUTE_INDEX(*reference_index) * array->size);
	//}
	//
	//*reference_index = data_reference->next_reference_index;
	//return data_reference->datum_index;
}

s_data_array* __cdecl reference_list_new(const char* name, int32 payload_size, int32 maximum_count, c_allocation_base* allocator)
{
	return INVOKE(0x00BB61A0, reference_list_new, name, payload_size, maximum_count, allocator);
}

void __cdecl reference_list_remove(s_data_array* array, int32* first_reference_index, int32 datum_index)
{
	//INVOKE(0x00BB6210, reference_list_remove, array, first_reference_index, datum_index);

	if (!array->valid)
	{
		return;
	}

	HOOK_INVOKE(, reference_list_remove, array, first_reference_index, datum_index);
}

void __cdecl reference_list_update_payload(s_data_array* array, const int32* first_reference_index, int32 datum_index, int32 payload_size, const void* payload)
{
	INVOKE(0x00BB6270, reference_list_update_payload, array, first_reference_index, datum_index, payload_size, payload);
}

