#pragma once

#include "cseries/cseries.hpp"

struct c_network_time_statistics
{
private:
	static long const k_network_statistics_interval_count = 20;

public:

	struct s_interval // $TODO: find a better name?
	{
		// names based on `m_stored_total` asserts
		// the struct for `m_intervals` could be different?

		long events;
		long total_values;
	};
	static_assert(sizeof(s_interval) == 0x8);

	uint64 m_total_events;
	uint64 m_total_values;

	long __time10;
	s_interval __unknown14;

	long m_period_duration_msec;
	long m_interval_duration_msec;
	real32 m_period_duration;
	long m_next_interval_index;
	s_interval m_intervals[k_network_statistics_interval_count]; // $TODO: find a better name?
	s_interval m_stored_total;
};
static_assert(sizeof(c_network_time_statistics) == 0xD8);

struct c_network_window_statistics
{
private:
	static long const k_network_statistics_maximum_window_size = 32;

public:

	struct s_window_entry // $TODO: find a better name?
	{
		// m_window_entries[this->m_window_next_entry].add_time = network_time_get();
		long add_time;

		// m_window_entries[m_window_next_entry].value = value;
		long value;
	};
	static_assert(sizeof(s_window_entry) == 0x8);

	long m_window_size;
	long m_window_next_entry;
	s_window_entry m_window_entries[k_network_statistics_maximum_window_size];

	// m_window_total_values -= m_window_entries[m_window_next_entry].value;
	// m_window_total_values += value;
	long m_window_total_values;

	// m_window_aperture_msec = network_time_difference_msec(m_window_entries[m_window_next_entry].add_time, network_time_get());
	long m_window_aperture_msec;
};
static_assert(sizeof(c_network_window_statistics) == 0x110);

