#pragma once

using qsort_2byte_proc_t = bool(short a, short b, void const* find);
using qsort_4byte_proc_t = bool(long a, long b, void const* find);
using qsort_elements_proc_t = bool(void const* a, void const* b, void const* find);

extern void __cdecl qsort_2byte(void* elements, long count, qsort_2byte_proc_t* const sort_proc, void const* find);
extern void __cdecl qsort_4byte(void* elements, long count, qsort_4byte_proc_t* const sort_proc, void const* find);
extern void __cdecl qsort_elements(void* elements, long count, unsigned int size, qsort_elements_proc_t* const sort_proc, void const* find);

