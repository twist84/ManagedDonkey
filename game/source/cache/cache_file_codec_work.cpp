#include "cache/cache_file_codec_work.hpp"

int32 __cdecl async_decompress_file_section(
	e_async_priority priority,
	s_file_handle file_handle,
	uns32 offset,
	uns32 size,
	const uns32* verify_decompression_hash,
	c_basic_buffer<void> decompression_scratch_buffer,
	c_cache_file_decompressor* decompressor,
	c_basic_buffer<void> uncompressed_output_buffer,
	bool* out_decompression_success,
	c_synchronized_long* in_abort_signal,
	c_synchronized_long* out_bytes_read,
	c_synchronized_long* out_done)
{
	return INVOKE(0x006ECD80, async_decompress_file_section, priority,
		file_handle,
		offset,
		size,
		verify_decompression_hash,
		decompression_scratch_buffer,
		decompressor,
		uncompressed_output_buffer,
		out_decompression_success,
		in_abort_signal,
		out_bytes_read,
		out_done);
}

