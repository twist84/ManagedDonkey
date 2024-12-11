#include "cache/cache_file_bulk_reader.hpp"

void c_asynchronous_io_arena::close_file(long file_index)
{
	DECLFUNC(0x006EBCE0, void, __thiscall, c_asynchronous_io_arena*, long)(this, file_index);
}

void c_asynchronous_io_arena::close_file(s_indirect_file* indirect_file_handle)
{
	DECLFUNC(0x006EBD60, void, __thiscall, c_asynchronous_io_arena*, s_indirect_file*)(this, indirect_file_handle);
}

