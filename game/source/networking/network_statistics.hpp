#pragma once

#include "cseries/cseries.hpp"

struct c_network_time_statistics
{
private:
	static int32 const k_network_statistics_interval_count = 20;

public:

	struct s_statistics_interval
	{
		int32 events;
		int32 total_values;
	};
	static_assert(sizeof(s_statistics_interval) == 0x8);

	uns64 m_total_events;
	uns64 m_total_values;
	uns32 m_current_interval_start_timestamp;
	s_statistics_interval m_current_interval;
	int32 m_period_duration_msec;
	int32 m_interval_duration_msec;
	real32 m_period_inverse_seconds;
	int32 m_next_interval_index;
	s_statistics_interval m_stored_intervals[k_network_statistics_interval_count];
	s_statistics_interval m_stored_total;
};
static_assert(sizeof(c_network_time_statistics) == 0xD8);

struct c_network_window_statistics
{
private:
	static int32 const k_network_statistics_maximum_window_size = 32;

public:

	struct s_statistics_window_entry
	{
		uns32 timestamp;
		int32 value;
	};
	static_assert(sizeof(s_statistics_window_entry) == 0x8);

	int32 m_window_size;
	int32 m_window_next_entry;
	s_statistics_window_entry m_window_entries[k_network_statistics_maximum_window_size];
	int32 m_window_total_values;
	int32 m_window_aperture_msec;
};
static_assert(sizeof(c_network_window_statistics) == 0x110);

