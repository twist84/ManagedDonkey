#pragma once

extern void* __cdecl _physical_memory_malloc_fixed(long memory_stage, char const* name, long size, unsigned long flags);
extern unsigned long align_up(unsigned long value, long alignment_bits);

