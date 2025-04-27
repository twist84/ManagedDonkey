#pragma once

using qsort_2byte_proc_t = bool(int16 a, int16 b, void const* find);
using qsort_4byte_proc_t = bool(int32 a, int32 b, void const* find);
using qsort_elements_proc_t = bool(void const* a, void const* b, void const* find);

extern void __cdecl qsort_2byte(void* elements, int32 count, qsort_2byte_proc_t* const sort_proc, void const* find);
extern void __cdecl qsort_4byte(void* elements, int32 count, qsort_4byte_proc_t* const sort_proc, void const* find);
extern void __cdecl qsort_elements(void* elements, int32 count, unsigned int size, qsort_elements_proc_t* const sort_proc, void const* find);

