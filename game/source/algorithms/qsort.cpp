#include "algorithms/qsort.hpp"

#include "cseries/cseries.hpp"

void __cdecl qsort_2byte(void* elements, int32 count, qsort_2byte_proc_t* const sort_proc, void const* find)
{
	INVOKE(0x0065B070, qsort_2byte, elements, count, sort_proc, find);
}

void __cdecl qsort_4byte(void* elements, int32 count, qsort_4byte_proc_t* const sort_proc, void const* find)
{
	INVOKE(0x0065B090, qsort_4byte, elements, count, sort_proc, find);
}

void __cdecl qsort_elements(void* elements, int32 count, unsigned int size, qsort_elements_proc_t* const sort_proc, void const* find)
{
	INVOKE(0x0065B0B0, qsort_elements, elements, count, size, sort_proc, find);
}

