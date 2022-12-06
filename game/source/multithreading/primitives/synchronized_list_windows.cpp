#include "multithreading/primitives/synchronized_list_windows.hpp"

#include "cseries/cseries.hpp"

#include <assert.h>
#include <windows.h>

void __cdecl synchronized_list_initialize(s_synchronized_list_header* header)
{
	assert(header);

	header->alignment = 0;
}

void __cdecl synchronized_list_entry_clear(s_synchronized_list_entry* entry)
{
	entry->next = nullptr;
}

s_synchronized_list_entry* __cdecl synchronized_list_entry_next(s_synchronized_list_entry* entry)
{
	return entry->next;
}

s_synchronized_list_entry* __cdecl synchronized_list_entry_set(s_synchronized_list_entry* entry, s_synchronized_list_entry* new_entry)
{
	s_synchronized_list_entry* next = entry->next;
	entry = new_entry;
	return next;
}

void __cdecl synchronized_list_push(s_synchronized_list_header* header, s_synchronized_list_entry* entry)
{
	assert(header);
	assert(entry);
	assert(pointer_is_aligned(entry, k_synchronized_list_alignment_bits));

	InterlockedPushEntrySList((PSLIST_HEADER)header, (PSLIST_ENTRY)entry);
}

void __cdecl synchronized_list_push_multiple(s_synchronized_list_header* header, long entry_count, s_synchronized_list_entry** entries)
{
	assert(header);
	assert(entries);

	for (long i = 0; i < entry_count; i++)
	{
		synchronized_list_push(header, entries[i]);
	}
}

s_synchronized_list_entry* __cdecl synchronized_list_pop(s_synchronized_list_header* header)
{
	return (s_synchronized_list_entry*)InterlockedPopEntrySList((PSLIST_HEADER)header);
}

s_synchronized_list_entry* __cdecl synchronized_list_flush(s_synchronized_list_header* header)
{
	return (s_synchronized_list_entry*)InterlockedFlushSList((PSLIST_HEADER)header);
}

long __cdecl synchronized_list_get_count(s_synchronized_list_header* header)
{
	return header->s.depth;
}

