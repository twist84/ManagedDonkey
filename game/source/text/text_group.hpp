#pragma once

#include "cseries/cseries.hpp"

const long k_hash_size = 0x14;

struct data_hash_definition
{
	byte hash_bytes[k_hash_size];
};

struct c_language_pack
{
	void* string_reference_pointer;
	void* string_data_pointer;
	long number_of_strings;
	long string_data_size;
	long string_reference_cache_offset;
	long string_data_cache_offset;
	data_hash_definition string_reference_checksum;
	data_hash_definition string_data_checksum;
	bool data_loaded;
};
static_assert(sizeof(c_language_pack) == 0x44);
