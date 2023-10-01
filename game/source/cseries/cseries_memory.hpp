#pragma once

extern void memmove_guarded(void* write_start, void const* source, unsigned int size, void const* bounds_lower, unsigned int bounds_size);

