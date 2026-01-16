#pragma once

#include "cseries/cseries.hpp"

class c_cache_file_decompressor
{
public:
	virtual bool begin(c_basic_buffer<void> output_buffer) = 0;
	virtual bool decompress_buffer(c_basic_buffer<void> input_buffer, c_basic_buffer<void>* out_decompressed_buffer) = 0;
	virtual bool finish(c_basic_buffer<void>* out_decompressed_buffer) = 0;
};
COMPILE_ASSERT(sizeof(c_cache_file_decompressor) == 0x4);

class c_cache_file_decompressor_service
{
public:
	virtual bool decompressor_available() = 0;
	virtual c_cache_file_decompressor* create_decompressor() = 0;
	virtual c_cache_file_decompressor* begin_decompression(c_basic_buffer<void> buffer) = 0;
	virtual void dispose_decompressor(c_cache_file_decompressor* decompressor) = 0;
};
COMPILE_ASSERT(sizeof(c_cache_file_decompressor_service) == 0x4);

struct s_tag_persistent_identifier;
class c_cache_file_decompressor_registry
{
public:
	c_cache_file_decompressor_registry();

	virtual c_cache_file_decompressor_service* find_decompressor_service(s_tag_persistent_identifier* identifier) = 0;
};
COMPILE_ASSERT(sizeof(c_cache_file_decompressor_registry) == 0x4);

template<typename decompressor_class_t>
class c_single_instance_cache_file_decompressor_service :
	public c_cache_file_decompressor_service
{
	virtual bool decompressor_available()
	{
		return m_decompressor == nullptr;
	}

	virtual c_cache_file_decompressor* create_decompressor()
	{
		initialize_decompressor(&m_decompressor_instance);
		return m_decompressor;
	}

	virtual c_cache_file_decompressor* begin_decompression(c_basic_buffer<void> buffer)
	{
		initialize_decompressor(&m_decompressor_instance);
		m_decompressor->begin(buffer);
		return m_decompressor;
	}

	virtual void dispose_decompressor(c_cache_file_decompressor* decompressor)
	{
		//ASSERT(m_decompressor_instance.alive());
		ASSERT(m_decompressor_instance.get() == decompressor);
		m_decompressor = nullptr;
		//ASSERT(!m_decompressor_instance.alive());
	}

	virtual void initialize_decompressor(c_typed_opaque_data<decompressor_class_t>* decompressor_instance) = 0;

	byte __data[0x4];
	c_typed_opaque_data<decompressor_class_t> m_decompressor_instance;
	c_cache_file_decompressor* m_decompressor;
};

class c_cache_file_runtime_decompressor_registry :
	public c_cache_file_decompressor_registry
{
public:
	c_cache_file_runtime_decompressor_registry();

	virtual c_cache_file_decompressor_service* find_decompressor_service(s_tag_persistent_identifier* identifier) override;
};
COMPILE_ASSERT(sizeof(c_cache_file_runtime_decompressor_registry) == sizeof(c_cache_file_decompressor_registry));

