#pragma once

const int32 k_synchronized_list_alignment_bits = 4;

// SINGLE_LIST_ENTRY
struct s_synchronized_list_entry
{
	s_synchronized_list_entry* next;
};
COMPILE_ASSERT(sizeof(s_synchronized_list_entry) == 0x4);

// SLIST_HEADER
struct s_synchronized_list_header
{
	union
	{
		uns64 alignment;
		struct
		{
			s_synchronized_list_entry next;
			uns16 depth;
			uns16 sequence;
		} s;
	};
};
COMPILE_ASSERT(sizeof(s_synchronized_list_header) == 0x8);

extern void __cdecl synchronized_list_initialize(s_synchronized_list_header* header);
extern void __cdecl synchronized_list_entry_clear(s_synchronized_list_entry* entry);
extern s_synchronized_list_entry* __cdecl synchronized_list_entry_next(s_synchronized_list_entry* entry);
extern s_synchronized_list_entry* __cdecl synchronized_list_entry_set(s_synchronized_list_entry* entry, s_synchronized_list_entry* next_entry);
extern void __cdecl synchronized_list_push(s_synchronized_list_header* header, s_synchronized_list_entry* entry);
extern void __cdecl synchronized_list_push_multiple(s_synchronized_list_header* header, int32 entry_count, s_synchronized_list_entry** entries);
extern s_synchronized_list_entry* __cdecl synchronized_list_pop(s_synchronized_list_header* header);
extern s_synchronized_list_entry* __cdecl synchronized_list_flush(s_synchronized_list_header* header);
extern int32 __cdecl synchronized_list_get_count(struct s_synchronized_list_header* header);

