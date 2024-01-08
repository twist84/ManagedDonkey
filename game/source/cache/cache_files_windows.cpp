#include "cache/cache_files_windows.hpp"

REFERENCE_DECLARE(0x018A1A54, c_cache_file_copy_fake_decompressor, g_copy_decompressor);
REFERENCE_DECLARE(0x0240B1E8, s_cache_file_table_of_contents, cache_file_table_of_contents);
REFERENCE_DECLARE(0x0243C098, s_cache_file_copy_globals, cache_file_copy_globals);
REFERENCE_DECLARE(0x0243F780, c_asynchronous_io_arena, g_cache_file_io_arena);

void __cdecl cache_files_copy_do_work()
{
	INVOKE(0x005AAB20, cache_files_copy_do_work);
}

