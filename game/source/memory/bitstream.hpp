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
public:
	c_bitstream() :
		m_data(0),
		m_data_max(0),
		m_data_size_bytes(0)
	{
		reset(_bitstream_state_initial);
	}

	c_bitstream(byte* data, long data_length) :
		m_data_size_alignment(1)
	{
		set_data(data, data_length);
	}

	//template<long k_angle_bits, long k_quantization_bits>
	//void read_axes(char const* name, real_vector3d* forward, real_vector3d* up);
	//
	//template<long k_forward_angle_bits, long k_up_quantization_bits>
	//void write_axes(char const* name, real_vector3d const* forward, real_vector3d const* up);

	// functions as they appear in memory

	void read_raw_data(char const* name, void* value, long size_in_bits);
	long read_signed_integer(char const* name, long size_in_bits);
	void write_bool(char const* name, bool value);
	void write_integer(char const* name, dword value, long size_in_bits);
	void write_raw_data(char const* name, void const* value, long size_in_bits);
	void write_signed_integer(char const* name, long value, long size_in_bits);
	qword read_qword(char const* name, long size_in_bits);
	void write_qword(char const* name, qword value, long size_in_bits);
	static void angle_to_axes_internal(real_vector3d const* up, real forward_angle, real_vector3d* forward);
	void append(c_bitstream const* stream);
	static void axes_compute_reference_internal(real_vector3d const* up, real_vector3d* forward_reference, real_vector3d* left_reference);
	static real axes_to_angle_internal(real_vector3d const* forward, real_vector3d const* up);
	bool begin_consistency_check();
	void begin_reading();
	void begin_writing(long data_size_alignment);
	void data_is_untrusted(bool is_untrusted);

private:
	qword decode_qword_from_memory();

public:
	void discard_remaining_data();

private:
	void encode_qword_to_memory(qword value, long size_in_bits);

public:
	bool overflowed() const;
	bool error_occurred() const;

	bool was_reading() const;
	bool was_writing() const;

	void finish_consistency_check();
	void finish_reading();
	void finish_writing(long* bits_wasted);

	long get_current_stream_bit_position();
	long get_space_used_in_bits();
	byte const* get_data(long* data_length) const;
	void push_position();
	void pop_position(bool pop);

private:
	qword read_accumulator_from_memory(long a1);

public:
	bool read_bool(char const* name);
	void read_bits_internal(void* data, long size_in_bits);
	dword read_integer(char const* name, long size_in_bits);
	void read_identifier(char const* identifier);
	void read_point3d(char const* name, int32_point3d* point, long axis_encoding_size_in_bits);
	real read_quantized_real(char const* name, real min_value, real max_value, long size_in_bits, bool exact_midpoint, bool exact_endpoints);
	qword read_qword_internal(long size_in_bits);
	void read_secure_address(char const* name, s_transport_secure_address* address);
	void read_string(char const* name, char* _string, long max_string_size);
	void read_string_utf8(char const* name, char* char_string, long max_string_size);
	void read_string_wchar(char const* name, wchar_t* _string, long max_string_size);
	void read_unit_vector(char const* name, real_vector3d* unit_vector, long size_in_bits);
	void read_vector(char const* name, real_vector3d* vector, real min_value, real max_value, long step_count_size_in_bits, long size_in_bits);

	template<typename t_enum, long size_in_bits>
	t_enum read_enum(char const* name)
	{
		return static_cast<t_enum>(read_integer(name, size_in_bits));
	}

private:
	void reset(long state);

public:
	void set_data(byte* data, long data_length);
	void skip(long bits_to_skip);
	bool would_overflow(long size_in_bits) const;
	void write_accumulator_to_memory(qword value, long size_in_bits);
	void write_bits_internal(void const* data, long size_in_bits);
	void write_identifier(char const* identifier);
	void write_point3d(char const* name, int32_point3d const* point, long axis_encoding_size_in_bits);
	void write_quantized_real(char const* name, real* value, real min_value, real max_value, long size_in_bits, bool exact_midpoint, bool exact_endpoints);
	void write_qword_internal(qword value, long size_in_bits);
	void write_secure_address(char const* name, s_transport_secure_address const* address);
	void write_string(char const* name, char const* _string, long max_string_size);
	void write_string_utf8(char const* name, utf8 const* _string, long max_string_size);
	void write_string_wchar(char const* name, wchar_t const* _string, long max_string_size);
	void write_unit_vector(char const* name, real_vector3d const* unit_vector, long size_in_bits);
	void write_vector(char const* name, real_vector3d const* vector, real min_value, real max_value, long step_count_size_in_bits, long size_in_bits);
	bool writing() const;

	template<typename t_enum, long size_in_bits>
	void write_enum(char const* name, t_enum value)
	{
		write_integer(name, value, size_in_bits);
	}

private:
	static long const k_bitstream_maximum_position_stack_size = 4;

protected:

	struct s_bitstream_stack_entry
	{
		long current_memory_bit_position;
		long current_stream_bit_position;
		qword accumulator;
		long accumulator_bit_count;
		byte* next_data;
	};

	byte* m_data;
	byte* m_data_max;
	long m_data_size_bytes;
	long m_data_size_alignment;
	c_enum<e_bitstream_state, long, _bitstream_state_initial, k_bitstream_state_count> m_state;
	bool m_data_error_detected;
	s_bitstream_stack_entry m_bitstream_data;

	long m_position_stack_depth;

	// why is this between `m_position_stack_depth` and `m_position_stack`, is `m_position_stack_depth` actually a qword?
	dword __unknown34; // long m_potential_bit_position?

	s_bitstream_stack_entry m_position_stack[k_bitstream_maximum_position_stack_size];
	long m_number_of_bits_rewound;
	long m_number_of_position_resets;
};
static_assert(sizeof(c_bitstream) == 0xA0);

