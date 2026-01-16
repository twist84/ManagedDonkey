#pragma once

#include "cseries/async_buffer.hpp"

class c_saved_film_scratch_memory
{
public:
	enum
	{
		k_screenshot_width = 1920,
		k_screenshot_height = 1080,
		k_screenshot_4x3_width = 1536,
		k_screenshot_4x3_height = 1152,
		k_screenshot_file_length = 0x96000,
		k_screenshot_xmemalloc_length = 0xA00000,
		k_screenshot_argb_image_length = 0x7F9000,
	};
#define SCRATCH_MEMORY_SIZE (k_screenshot_file_length + k_screenshot_xmemalloc_length + k_screenshot_argb_image_length)

	enum e_system
	{
		_system_film_history = 0,
		_system_screenshot,

		k_system_count,
	};

	enum e_system_state
	{
		_system_state_none = 0,
		_system_state_acquired = 1,
	};

	struct s_system_data
	{
		s_system_data();

		e_system_state state;
		int32 reference_count;
		char* buffer;
		int32 buffer_length;
		c_async_stored_buffer_set<1> buffer_set;
	};
	COMPILE_ASSERT(sizeof(s_system_data) == 0x98);

public:
	c_saved_film_scratch_memory();
	//~c_saved_film_scratch_memory();
	//c_saved_film_scratch_memory& operator=(const c_saved_film_scratch_memory& __that);

	s_system_data* acquire(e_system system);
	void block_until_not_busy();
	void dispose();
	static c_saved_film_scratch_memory* get();
	void initialize();
	void memory_dispose();
	void memory_initialize();
	void release(e_system system);
	s_system_data* try_and_acquire(e_system system);
	void update();
	bool valid();

private:
	bool m_initialized;
	char* m_buffer;
	int32 m_buffer_length;
	s_system_data m_systems[k_system_count];
};
COMPILE_ASSERT(sizeof(c_saved_film_scratch_memory) == 0x13C);
