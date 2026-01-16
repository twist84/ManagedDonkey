#pragma once

#include "cache/cache_file_runtime_codecs.hpp"
#include "cseries/cseries.hpp"

class c_lz_cache_file_decompressor :
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

	c_basic_buffer<void> __buffer4;
	c_basic_buffer<void> __bufferC;
	bool __unknown14;
	c_basic_buffer<void> __buffer18;
};
COMPILE_ASSERT(sizeof(c_lz_cache_file_decompressor) == sizeof(c_cache_file_decompressor) + 0x1C);

class c_lz_cache_file_decompressor_service :
	c_single_instance_cache_file_decompressor_service<c_lz_cache_file_decompressor>
{
};
COMPILE_ASSERT(sizeof(c_lz_cache_file_decompressor_service) == 0x2C);

