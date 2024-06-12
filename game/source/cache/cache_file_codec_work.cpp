#include "cache/cache_file_codec_work.hpp"

long __cdecl async_decompress_file_section(
	e_async_priority priority,
	s_file_handle file_handle,
	dword a3,
	dword processed_compressed_size,
	dword* hash,
	c_basic_buffer<void> a6,
	c_cache_file_decompressor* decompressor,
	c_basic_buffer<void> a10,
	bool* out_decompression_success,
	c_synchronized_long* in_abort_signal,
	c_synchronized_long* a13,
	c_synchronized_long* done)
{
	return INVOKE(0x006ECD80, async_decompress_file_section, priority,
		file_handle,
		a3,
		processed_compressed_size,
		hash,
		a6,
		decompressor,
		a10,
		out_decompression_success,
		in_abort_signal,
		a13,
		done);
}

