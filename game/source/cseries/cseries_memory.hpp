#pragma once

extern void memmove_guarded(void* write_start, const void* source, unsigned int size, const void* bounds_lower, unsigned int bounds_size);

