#pragma once

#include "cache/cache_file_runtime_codecs.hpp"

class c_cache_file_uncompressed_decompressor :
	public c_cache_file_decompressor
{
	virtual bool begin(c_basic_buffer<void> a1)
	{
		throw;
	}
	virtual bool decompress_buffer(c_basic_buffer<void> a1, c_basic_buffer<void>* a2)
	{
		throw;
	}
	virtual bool finish(c_basic_buffer<void>* a1)
	{
		throw;
	}

	int32 m_bytes_processed;
	c_basic_buffer<void> __buffer;
};
COMPILE_ASSERT(sizeof(c_cache_file_uncompressed_decompressor) == sizeof(c_cache_file_decompressor) + 0xC);

