#pragma once

using qsort_2byte_proc_t = bool(int16 a, int16 b, const void* find);
using qsort_4byte_proc_t = bool(int32 a, int32 b, const void* find);
using qsort_elements_proc_t = bool(const void* a, const void* b, const void* find);

extern void __cdecl qsort_2byte(void* elements, int32 count, qsort_2byte_proc_t* const sort_proc, const void* find);
extern void __cdecl qsort_4byte(void* elements, int32 count, qsort_4byte_proc_t* const sort_proc, const void* find);
extern void __cdecl qsort_elements(void* elements, int32 count, unsigned int size, qsort_elements_proc_t* const sort_proc, const void* find);

