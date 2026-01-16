#pragma once

#include "cache/cache_file_runtime_codecs.hpp"

class c_xor_cache_file_decompressor :
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

	c_basic_buffer<void> m_output_buffer;
	c_basic_buffer<void> m_buffer_left;
	char const m_xor_value;
};
COMPILE_ASSERT(sizeof(c_xor_cache_file_decompressor) == sizeof(c_cache_file_decompressor) + 0x14);

class c_xor_cache_file_decompressor_service :
	c_single_instance_cache_file_decompressor_service<c_xor_cache_file_decompressor>
{
};
COMPILE_ASSERT(sizeof(c_xor_cache_file_decompressor_service) == 0x24);

