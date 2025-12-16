#include "networking/network_statistics.hpp"

#include "cseries/cseries.hpp"
#include "networking/network_time.hpp"

c_network_time_statistics::c_network_time_statistics() :
	m_period_duration_msec(0),
	m_interval_duration_msec(0),
	m_period_inverse_seconds(0.0f)
{
}

c_network_time_statistics::~c_network_time_statistics()
{
}

c_network_window_statistics::c_network_window_statistics() :
	m_window_size(0)
{
}
c_network_window_statistics::~c_network_window_statistics()
{
}

void c_network_time_statistics::add_event(int32 value)
{
	//INVOKE_CLASS_MEMBER(0x0045F2A0, c_network_time_statistics, add_event, value);

	ASSERT(value >= 0);
	
	c_network_time_statistics::check_interval_rollover();
	m_total_events++;
	m_total_values += value;
	m_current_interval.events++;
	m_current_interval.total_values += value;
}

void c_network_window_statistics::add_event(int32 value)
{
	//INVOKE_CLASS_MEMBER(0x0045F2D0, c_network_window_statistics, add_event, value);

	uns32 current_timestamp = network_time_get();
	
	ASSERT(m_window_size > 0);
	ASSERT(m_window_next_entry >= 0 && m_window_next_entry < m_window_size);
	
	m_window_aperture_msec = network_time_difference_msec(m_window_entries[m_window_next_entry].timestamp, current_timestamp);
	ASSERT(m_window_aperture_msec >= 0);
	
	ASSERT(m_window_total_values >= m_window_entries[m_window_next_entry].value);
	
	m_window_total_values -= m_window_entries[m_window_next_entry].value;
	m_window_entries[m_window_next_entry].timestamp = current_timestamp;
	m_window_entries[m_window_next_entry].value = value;
	m_window_total_values += value;
	m_window_next_entry = (m_window_next_entry + 1) % m_window_size;
}

real32 c_network_time_statistics::average_events_per_second()
{
	//return INVOKE_CLASS_MEMBER(0x0045F330, c_network_time_statistics, average_events_per_second);
	
	c_network_time_statistics::check_interval_rollover();
	return (real32)m_stored_total.events * m_period_inverse_seconds;
}

real32 c_network_window_statistics::average_values_in_window()
{
	//return INVOKE_CLASS_MEMBER(0x0045F360, c_network_window_statistics, average_values_in_window);
	
	return (real32)((real32)m_window_total_values / m_window_size);
}

real32 c_network_time_statistics::average_values_per_second()
{
	//return INVOKE_CLASS_MEMBER(0x0045F390, c_network_time_statistics, average_values_per_second);
	
	c_network_time_statistics::check_interval_rollover();
	return (real32)((real32)m_stored_total.total_values * m_period_inverse_seconds);
}

void c_network_time_statistics::check_interval_rollover()
{
	INVOKE_CLASS_MEMBER(0x0045F3C0, c_network_time_statistics, check_interval_rollover);

	// $IMPLEMENT
}

void c_network_time_statistics::initialize(int32 period_duration_msec)
{
	//INVOKE_CLASS_MEMBER(0x0045F540, c_network_time_statistics, initialize, period_duration_msec);

	ASSERT(period_duration_msec > 0);

	m_interval_duration_msec = period_duration_msec / k_network_statistics_interval_count;
	m_period_duration_msec = period_duration_msec;
	m_period_inverse_seconds = 1000.0f / (real32)period_duration_msec;
	c_network_time_statistics::reset();
}

void c_network_window_statistics::initialize(int32 window_size)
{
	INVOKE_CLASS_MEMBER(0x0045F5D0, c_network_window_statistics, initialize, window_size);
}

//.text:0045F5F0 ; int32 __cdecl network_time_difference_msec(uns32 timestamp1, uns32 timestamp2)
//.text:0045F600 ; uns32 __cdecl network_time_get()
//.text:0045F620 ; uns32 __cdecl network_time_get_exact()
//.text:0045F630 ; int32 __cdecl network_time_since(uns32 timestamp)

void c_network_time_statistics::reset()
{
	//INVOKE_CLASS_MEMBER(0x0045F650, c_network_time_statistics, reset);

	m_total_events = 0;
	m_total_values = 0;
	m_current_interval_start_timestamp = 0;
	csmemset(&m_current_interval, 0, sizeof(m_current_interval));
	m_next_interval_index = 0;
	csmemset(m_stored_intervals, 0, sizeof(m_stored_intervals));
	csmemset(&m_stored_total, 0, sizeof(m_stored_total));
}

void c_network_window_statistics::reset()
{
	INVOKE_CLASS_MEMBER(0x0045F6B0, c_network_window_statistics, reset);
}

uns64 c_network_time_statistics::total_events() const
{
	//INVOKE_CLASS_MEMBER(0x0045F700, c_network_time_statistics, total_events);

	return m_total_events;
}

uns64 c_network_time_statistics::total_values() const
{
	//INVOKE_CLASS_MEMBER(0x0045F710, c_network_time_statistics, total_values);

	return m_total_values;
}

int32 c_network_window_statistics::window_aperture_msec() const
{
	//return INVOKE_CLASS_MEMBER(0x0045F720, c_network_window_statistics, window_aperture_msec);

	return m_window_aperture_msec;
}

int32 c_network_window_statistics::window_size() const
{
	//return INVOKE_CLASS_MEMBER(0x0045F730, c_network_window_statistics, window_size);

	ASSERT(m_window_size > 0);

	return m_window_size;
}

