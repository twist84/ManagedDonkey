#include "saved_games/saved_film_scratch_memory.hpp"

#include "cache/physical_memory_map.hpp"

static REFERENCE_DECLARE(0x019428EC, c_physical_memory_allocation, g_physical_memory_allocation); // only use in this translation unit

c_saved_film_scratch_memory g_saved_film_scratch_memory{};


c_saved_film_scratch_memory::s_system_data::s_system_data() :
	state(_system_state_none),
	reference_count(0),
	buffer(nullptr),
	buffer_length(0),
	buffer_set()
{
}

c_saved_film_scratch_memory::c_saved_film_scratch_memory() :
	m_initialized(false),
	m_buffer(nullptr),
	m_buffer_length(0),
	m_systems()
{
}

//c_saved_film_scratch_memory::~c_saved_film_scratch_memory()
//c_saved_film_scratch_memory& c_saved_film_scratch_memory::operator=(const c_saved_film_scratch_memory& __that)

c_saved_film_scratch_memory::s_system_data* c_saved_film_scratch_memory::acquire(e_system system)
{
	c_saved_film_scratch_memory::s_system_data* system_data = c_saved_film_scratch_memory::try_and_acquire(system);
	ASSERT(system_data);

	return system_data;
}

void c_saved_film_scratch_memory::block_until_not_busy()
{
	for (int32 system_index = 0; system_index < k_system_count; system_index++)
	{
		m_systems[system_index].buffer_set.block_until_not_busy();
	}
}

void c_saved_film_scratch_memory::dispose()
{

}

c_saved_film_scratch_memory* c_saved_film_scratch_memory::get()
{
	return &g_saved_film_scratch_memory;
}

void c_saved_film_scratch_memory::initialize()
{
}

void c_saved_film_scratch_memory::memory_dispose()
{
	if (!m_initialized)
	{
		return;
	}

	for (int32 system_index = 0; system_index < k_system_count; system_index++)
	{
		ASSERT(m_systems[system_index].state == _system_state_none);
		m_systems[system_index].buffer_set.release_storage(nullptr);
	}

	g_physical_memory_allocation.deallocate(m_buffer);
	m_buffer = nullptr;

	m_buffer_length = 0;
	m_initialized = false;
}

void c_saved_film_scratch_memory::memory_initialize()
{
	ASSERT(!m_initialized);

	m_buffer_length = SCRATCH_MEMORY_SIZE;
	m_buffer = (char*)g_physical_memory_allocation.allocate(SCRATCH_MEMORY_SIZE, "film scratch");
	if (!m_buffer)
	{
		return;
	}

	for (int32 system_index = 0; system_index < k_system_count; system_index++)
	{
		m_systems[system_index].buffer_set.use_external_storage(&m_buffer, 1, m_buffer_length);
	}
}

void c_saved_film_scratch_memory::release(e_system system)
{
	ASSERT(m_systems[system].state == _system_state_acquired);

	if (m_systems[system].reference_count-- == 1)
	{
		m_systems[system].state = _system_state_none;
	}
}

c_saved_film_scratch_memory::s_system_data* c_saved_film_scratch_memory::try_and_acquire(e_system system)
{
	if (!m_initialized)
	{
		return nullptr;
	}

	if (m_systems[system].state == _system_state_acquired)
	{
		m_systems[system].buffer = m_buffer;
		m_systems[system].buffer_length = m_buffer_length;
		m_systems[system].reference_count++;
		return &m_systems[system];
	}

	for (int32 system_index = 0; system_index < k_system_count; system_index++)
	{
		if (m_systems[system_index].state == _system_state_acquired)
		{
			return nullptr;
		}
	}

	m_systems[system].buffer = m_buffer;
	m_systems[system].buffer_length = m_buffer_length;
	ASSERT(m_systems[system].reference_count == 0);
	m_systems[system].reference_count = 1;
	m_systems[system].state = _system_state_acquired;
	return &m_systems[system];
}

void c_saved_film_scratch_memory::update()
{

}

bool c_saved_film_scratch_memory::valid()
{
	return m_initialized;
}

