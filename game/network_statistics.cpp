#include "networking/network_statistics.hpp"

#include "cseries/cseries.hpp"
#include "networking/network_time.hpp"

//void c_network_time_statistics::add_event(int32 value)
//{
//	//INVOKE_CLASS_MEMBER(0x0045F2A0, c_network_time_statistics, add_event, value);
//
//	ASSERT(value >= 0);
//
//	c_network_time_statistics::check_interval_rollover();
//	m_total_events++;
//	m_total_values += value;
//	m_current_interval.events++;
//	m_current_interval.total_values += value;
//}

//void c_network_window_statistics::add_event(int32 value)
//{
//	//INVOKE_CLASS_MEMBER(0x0045F2D0, c_network_window_statistics, add_event, value);
//
//	uns32 current_timestamp = network_time_get();
//
//	ASSERT(m_window_size > 0);
//	ASSERT(m_window_next_entry >= 0 && m_window_next_entry < m_window_size);
//
//	m_window_aperture_msec = network_time_difference_msec(m_window_entries[m_window_next_entry].timestamp, current_timestamp);
//	ASSERT(m_window_aperture_msec >= 0);
//
//	ASSERT(m_window_total_values >= m_window_entries[m_window_next_entry].value);
//
//	m_window_total_values -= m_window_entries[m_window_next_entry].value;
//	m_window_entries[m_window_next_entry].timestamp = current_timestamp;
//	m_window_entries[m_window_next_entry].value = value;
//	m_window_total_values += value;
//	m_window_next_entry = (m_window_next_entry + 1) % m_window_size;
//}

//.text:0045F330 ; public: real32 c_network_time_statistics::average_events_per_second()
//.text:0045F360 ; public: real32 c_network_window_statistics::average_values_in_window()
//.text:0045F390 ; public: real32 c_network_time_statistics::average_values_per_second()
//.text:0045F3C0 ; private: void c_network_time_statistics::check_interval_rollover()
//.text:0045F540 ; public: void c_network_time_statistics::initialize(int32 period_duration_msec)
//.text:0045F5D0 ; public: void c_network_window_statistics::initialize(int32 window_size)
//.text:0045F5F0 ; int32 __cdecl network_time_difference_msec(uns32 timestamp1, uns32 timestamp2)
//.text:0045F600 ; uns32 __cdecl network_time_get()
//.text:0045F620 ; uns32 __cdecl network_time_get_exact()
//.text:0045F630 ; int32 __cdecl network_time_since(uns32 timestamp)
//.text:0045F650 ; public: void c_network_time_statistics::reset()
//.text:0045F6B0 ; public: void c_network_window_statistics::reset()
//.text:0045F700 ; public: uns64 c_network_time_statistics::total_events() const
//.text:0045F710 ; public: uns64 c_network_time_statistics::total_values() const
//.text:0045F720 ; public: int32 c_network_window_statistics::window_aperture_msec() const
//.text:0045F730 ; public: int32 c_network_window_statistics::window_size() const

