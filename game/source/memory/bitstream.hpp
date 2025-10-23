#pragma once

struct s_transport_secure_address;
union long_point3d;
union real_vector3d;

enum
{
	_bitstream_state_none = 0,

	_bitstream_state_write,
	_bitstream_state_write_finished,

	_bitstream_state_read,
	_bitstream_state_read_consistency_check,
	_bitstream_state_read_finished,

	k_bitstream_state_count,
};

class c_bitstream
{
public:
	c_bitstream() :
		m_data(0),
		m_data_max(0),
		m_data_size_bytes(0)
	{
		reset(_bitstream_state_none);
	}

	c_bitstream(byte* data, int32 data_length) :
		m_data_size_alignment(1)
	{
		set_data(data, data_length);
	}

	//template<int32 k_angle_bits, int32 k_quantization_bits>
	//void read_axes(const char* debug_string, real_vector3d* forward, real_vector3d* up);
	//
	//template<int32 k_forward_angle_bits, int32 k_up_quantization_bits>
	//void write_axes(const char* debug_string, const real_vector3d* forward, const real_vector3d* up);

	// functions as they appear in memory

	bool read_bool(const char* debug_string);
	uns32 read_integer(const char* debug_string, int32 size_in_bits);
	void read_raw_data(const char* debug_string, void* raw_data, int32 size_in_bits);
	int32 read_signed_integer(const char* debug_string, int32 size_in_bits);
	void write_bit_internal(bool value);
	void write_bool(const char* debug_string, bool value);
	void write_dword_internal(uns32 value, int32 size_in_bits);
	void write_integer(const char* debug_string, uns32 value, int32 size_in_bits);
	void write_raw_data(const char* debug_string, const void* raw_data, int32 size_in_bits);
	void write_signed_integer(const char* debug_string, int32 value, int32 size_in_bits);
	uns64 read_qword(const char* debug_string, int32 size_in_bits);
	void write_qword(const char* debug_string, uns64 value, int32 size_in_bits);
	static void angle_to_axes_internal(const real_vector3d* up, real32 forward_angle, real_vector3d* forward);
	void append(const c_bitstream* stream);
	static void axes_compute_reference_internal(const real_vector3d* up, real_vector3d* forward_reference, real_vector3d* left_reference);
	static real32 axes_to_angle_internal(const real_vector3d* forward, const real_vector3d* up);
	bool begin_consistency_check();
	void begin_reading();
	void begin_writing(int32 data_size_alignment);
	void data_is_untrusted(bool is_untrusted);

private:
	uns64 decode_qword_from_memory();

public:
	void discard_remaining_data();

private:
	void encode_qword_to_memory(uns64 value, int32 size_in_bits);

public:
	bool overflowed() const;
	bool error_occurred() const;

	bool was_reading() const;
	bool was_writing() const;

	void finish_consistency_check();
	void finish_reading();
	void finish_writing(int32* bits_wasted);

	int32 get_current_stream_bit_position();
	int32 get_space_used_in_bits();
	const byte* get_data(int32* data_length) const;
	void push_position();
	void pop_position(bool reset_to_pushed_state);

private:
	uns64 read_accumulator_from_memory(int32 size_in_bits);

public:
	bool read_bit_internal();
	void read_bits_internal(byte* data, int32 size_in_bits);
	uns32 read_dword_internal(int32 size_in_bits);

	template<typename t_enum, int32 size_in_bits>
	t_enum read_enum(const char* debug_string)
	{
		return static_cast<t_enum>(read_integer(debug_string, size_in_bits));
	}

	void read_identifier(const char* debug_string);
	real32 read_logarithmic_quantized_real(const char* debug_string, real32 min_value, real32 max_value, int32 size_in_bits);
	void read_point3d(const char* debug_string, long_point3d* point, int32 axis_encoding_size_in_bits);
	real32 read_quantized_real(const char* debug_string, real32 min_value, real32 max_value, int32 size_in_bits, bool exact_midpoint, bool exact_endpoints);
	uns64 read_qword_internal(int32 size_in_bits);
	void read_secure_address(const char* debug_string, s_transport_secure_address* address);
	void read_string(const char* debug_string, char* string, int32 max_string_size);
	void read_string_utf8(const char* debug_string, char* char_string, int32 max_string_size);
	void read_string_wchar(const char* debug_string, wchar_t* string, int32 max_string_size);
	void read_unit_vector(const char* debug_string, real_vector3d* value, int32 size_in_bits);
	void read_vector(const char* debug_string, real_vector3d* value, real32 min_magnitude, real32 max_magnitude, int32 magnitude_size_in_bits, int32 size_in_bits);
	bool reading() const;

private:
	void reset(int32 state);

public:
	void set_data(byte* data, int32 data_length);
	void skip(int32 bits_to_skip);
	bool would_overflow(int32 size_in_bits) const;
	void write_accumulator_to_memory(uns64 value, int32 size_in_bits);
	void write_bits_internal(const byte* data, int32 size_in_bits);

	template<typename t_enum, int32 size_in_bits>
	void write_enum(const char* debug_string, t_enum value)
	{
		write_integer(debug_string, value, size_in_bits);
	}

	void write_identifier(const char* identifier);
	void write_point3d(const char* debug_string, const long_point3d* point, int32 axis_encoding_size_in_bits);
	void write_point3d_efficient(const char* debug_string, const long_point3d* point1, const long_point3d* point2);
	void write_quantized_real(const char* debug_string, real32* value, real32 min_value, real32 max_value, int32 size_in_bits, bool exact_midpoint, bool exact_endpoints);
	void write_qword_internal(uns64 value, int32 size_in_bits);
	void write_secure_address(const char* debug_string, const s_transport_secure_address* address);
	void write_string(const char* debug_string, const char* string, int32 max_string_size);
	void write_string_id(const char* debug_string, int32 size_in_bits);
	void write_string_utf8(const char* debug_string, const utf8* string, int32 max_string_size);
	void write_string_wchar(const char* debug_string, const wchar_t* string, int32 max_string_size);
	void write_unit_vector(const char* debug_string, const real_vector3d* value, int32 size_in_bits);
	void write_vector(const char* debug_string, const real_vector3d* vector, real32 min_value, real32 max_value, int32 step_count_size_in_bits, int32 size_in_bits);
	bool writing() const;
private:
	static int32 const k_bitstream_maximum_position_stack_size = 4;

//protected:
public:

	struct s_bitstream_stack_entry
	{
		int32 current_memory_bit_position;
		int32 current_stream_bit_position;
		uns64 accumulator;
		int32 accumulator_bit_count;
		byte* next_data;
	};
	static_assert(sizeof(s_bitstream_stack_entry) == 0x18);

	byte* m_data;
	byte* m_data_max;
	int32 m_data_size_bytes;
	int32 m_data_size_alignment;
	int32 m_state;
	bool m_data_error_detected;
	s_bitstream_stack_entry m_bitstream_data;
	int32 m_position_stack_depth;
	s_bitstream_stack_entry m_position_stack[k_bitstream_maximum_position_stack_size];
	int32 m_number_of_bits_rewound;
	int32 m_number_of_position_resets;
};
static_assert(sizeof(c_bitstream) == 0xA0);
static_assert(0x00 == OFFSETOF(c_bitstream, m_data));
static_assert(0x04 == OFFSETOF(c_bitstream, m_data_max));
static_assert(0x08 == OFFSETOF(c_bitstream, m_data_size_bytes));
static_assert(0x0C == OFFSETOF(c_bitstream, m_data_size_alignment));
static_assert(0x10 == OFFSETOF(c_bitstream, m_state));
static_assert(0x14 == OFFSETOF(c_bitstream, m_data_error_detected));
static_assert(0x18 == OFFSETOF(c_bitstream, m_bitstream_data));
static_assert(0x30 == OFFSETOF(c_bitstream, m_position_stack_depth)); // 8 byte aligned
static_assert(0x38 == OFFSETOF(c_bitstream, m_position_stack));
static_assert(0x98 == OFFSETOF(c_bitstream, m_number_of_bits_rewound));
static_assert(0x9C == OFFSETOF(c_bitstream, m_number_of_position_resets));


