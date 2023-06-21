#pragma once

#include "cache/cache_file_runtime_codecs.hpp"

struct c_xor_cache_file_decompressor :
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

	c_basic_buffer<void> __buffer0;
	c_basic_buffer<void> __buffer8;
	byte __unknownC; // xor value, *i++ ^= __unknownC
};
static_assert(sizeof(c_xor_cache_file_decompressor) == sizeof(c_cache_file_decompressor) + 0x14);

struct c_xor_cache_file_decompressor_service : c_single_instance_cache_file_decompressor_service<c_xor_cache_file_decompressor>
{
};
static_assert(sizeof(c_xor_cache_file_decompressor_service) == 0x24);

