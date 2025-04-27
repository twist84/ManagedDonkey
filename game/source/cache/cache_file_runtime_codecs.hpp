#pragma once

#include "cseries/cseries.hpp"

struct c_cache_file_decompressor
{
public:
	virtual bool begin(c_basic_buffer<void> output_buffer) = 0;
	virtual bool decompress_buffer(c_basic_buffer<void> input_buffer, c_basic_buffer<void>* out_decompressed_buffer) = 0;
	virtual bool finish(c_basic_buffer<void>* out_decompressed_buffer) = 0;
};
static_assert(sizeof(c_cache_file_decompressor) == 0x4);

struct c_cache_file_decompressor_service
{
public:
	virtual bool decompressor_available() = 0;
	virtual c_cache_file_decompressor* create_decompressor() = 0;
	virtual c_cache_file_decompressor* begin_decompression(c_basic_buffer<void> buffer) = 0;
	virtual void dispose_decompressor(c_cache_file_decompressor* decompressor) = 0;
};
static_assert(sizeof(c_cache_file_decompressor_service) == 0x4);

struct s_tag_persistent_identifier;
struct c_cache_file_decompressor_registry
{
public:
	c_cache_file_decompressor_registry();

	virtual c_cache_file_decompressor_service* find_decompressor_service(s_tag_persistent_identifier* identifier) = 0;
};
static_assert(sizeof(c_cache_file_decompressor_registry) == 0x4);

template<typename decompressor_class_t>
struct c_single_instance_cache_file_decompressor_service :
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

	uint8 __data[4];
	c_typed_opaque_data<decompressor_class_t> m_decompressor_instance;
	c_cache_file_decompressor* m_decompressor;
};

struct c_cache_file_runtime_decompressor_registry :
	public c_cache_file_decompressor_registry
{
public:
	c_cache_file_runtime_decompressor_registry();

	virtual c_cache_file_decompressor_service* find_decompressor_service(s_tag_persistent_identifier* identifier) override;
};
static_assert(sizeof(c_cache_file_runtime_decompressor_registry) == sizeof(c_cache_file_decompressor_registry));

