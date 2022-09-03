#pragma once

#include "cseries/cseries.hpp"
#include "networking/transport/transport_security.hpp"

enum e_bitstream_state
{
	_bitstream_state_initial = 0,
	_bitstream_state_writing,
	_bitstream_state_write_finished,
	_bitstream_state_reading,
	_bitstream_state_read_only_for_consistency,
	_bitstream_state_read_finished,

	k_bitstream_state_count
};

struct c_bitstream
{
protected:
	byte* m_data;
	byte* m_data_max;

	long m_data_size_bytes;
	long m_data_size_alignment;

	c_enum<e_bitstream_state, long, k_bitstream_state_count> m_state;

	bool __unknown14;

	long m_data_size_in_bits;
	long m_current_bit_position;

	qword m_window;
	long m_window_bits_used;

	byte* m_next_data;

	long m_position_stack_depth;

	// why is this between `m_position_stack_depth` and `m_position_stack`, is `m_position_stack_depth` actually a qword?
	dword __unknown34;

	char m_position_stack[4][24];

	dword __unknown98;
	dword __unknown9C;

public:
	c_bitstream() :
		m_data(0),
		m_data_max(0),
		m_data_size_bytes(0)
	{
		reset(_bitstream_state_initial);
	}

	// functions as they appear in memory

	void begin_consistency_check();
	void begin_reading();
	void begin_writing(long data_size_alignment);
	void discard_remaining_data();
	bool overflowed() const;
	bool error_occurred() const;

	bool reading() const;
	bool writing() const;

	void finish_consistency_check();
	void finish_reading();
	void finish_writing(long* out_bits_remaining);

	long get_current_bit_position();
	byte const* get_data(long* data_length) const;
	void pop_position(bool pop);

	bool read_bool();
	void read_raw_data(void* data, long size_in_bits);
	dword read_integer(long size_in_bits);
	void read_point3d(char const* name, long_point3d* point, long axis_encoding_size_in_bits);
	real read_quantized_real(char const* name, real min_value, real max_value, long size_in_bits, bool exact_midpoint, bool exact_endpoints);
	void read_secure_address(char const* name, s_transport_secure_address* address);
	void read_string(char const* name, char* _string, long max_string_size);
	void read_string_utf8(char const* name, char* char_string, long max_string_size);
	void read_string_wchar(char const* name, wchar_t* _string, long max_string_size);
	void read_vector(char const* name, real_vector3d* vector, real min_value, real max_value, long step_count_size_in_bits, long size_in_bits);

private:
	void reset(long state);

public:
	void set_data(byte* data, long data_length);

	bool would_overflow(long size_in_bits) const;
	void write_raw_data(void const* data, long size_in_bits);
	void write_point3d(char const* name, long_point3d const* point, long axis_encoding_size_in_bits);
	void write_quantized_real(char const* name, real* value, real min_value, real max_value, long size_in_bits, bool exact_midpoint, bool exact_endpoints);
	void write_secure_address(char const* name, s_transport_secure_address const* address);
	void write_string(char const* name, char const* _string, long max_string_size);
	void write_string_utf8(char const* name, utf8 const* _string, long max_string_size);
	void write_string_wchar(char const* name, wchar_t const* _string, long max_string_size);
};
static_assert(sizeof(c_bitstream) == 0xA0);
