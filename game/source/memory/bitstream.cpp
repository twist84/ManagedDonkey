#include "memory/bitstream.hpp"

#include "math/unit_vector_quantization.hpp"
#include "memory/byte_swapping.hpp"

#define HIBYTE(x) ((byte)((x) >> 8 * (sizeof(x) - 1)))

// ===================== halo 4 begin =====================

template<typename t_type>
t_type left_shift_fast(t_type value, int32 shift_bits)
{
	ASSERT(shift_bits <= SIZEOF_BITS(t_type));

	return t_type(value << shift_bits);
}

template<typename t_type>
t_type left_shift_safe(t_type value, int32 shift_bits)
{
	if (shift_bits >= SIZEOF_BITS(t_type))
		return t_type(0);

	return t_type(value << shift_bits);
}

template<typename t_type>
t_type right_shift_fast(t_type value, int32 shift_bits)
{
	ASSERT(shift_bits < SIZEOF_BITS(t_type));

	return t_type(value >> shift_bits);
}

template<typename t_type>
t_type right_shift_safe(t_type value, int32 shift_bits)
{
	if (shift_bits < SIZEOF_BITS(t_type))
		return t_type(0);

	return t_type(value >> shift_bits);
}

// ====================== halo 4 end ======================

//.text:0043B5B0 ; c_bitstream::c_bitstream(byte* data, int32 data_length)
//.text:0043B5D0 ; c_bitstream::~c_bitstream()

//template<int32 k_forward_angle_bits, int32 k_up_quantization_bits>
//void c_bitstream::read_axes(const char* name, real_vector3d* forward, real_vector3d* up)
//{
//	if (read_bool("up-is-global-up3d"))
//	{
//		*up = *global_up3d;
//	}
//	else
//	{
//		dequantize_unit_vector3d(read_integer("up-quantization", k_up_quantization_bits), up, k_up_quantization_bits);
//	}
//
//	angle_to_axes_internal(up, read_quantized_real("forward-angle", -_pi, _pi, k_forward_angle_bits, false, false), forward);
//}

//template<int32 k_forward_angle_bits, int32 k_up_quantization_bits>
//void c_bitstream::write_axes(const char* name, const real_vector3d* forward, const real_vector3d* up)
//{
//	real_vector3d dequantized_up{};
//	if (fabs(up->i - global_up3d->i) > k_real_epsilon ||
//		fabs(up->j - global_up3d->j) > k_real_epsilon ||
//		fabs(up->k - global_up3d->k) > k_real_epsilon)
//	{
//		int32 quantized_up = quantize_unit_vector3d_fast<k_up_quantization_bits>(up);
//		write_bool("up-is-global-up3d", false);
//		write_integer("up-quantization", quantized_up, k_up_quantization_bits);
//		dequantize_unit_vector3d(quantized_up, &dequantized_up, k_up_quantization_bits);
//	}
//	else
//	{
//		write_bool("up-is-global-up3d", true);
//		dequantized_up = *global_up3d;
//	}
//}

void c_bitstream::read_raw_data(const char* name, void* value, int32 size_in_bits)
{
	//read_bits_internal(value, size_in_bits);

	DECLFUNC(0x00443980, void, __thiscall, c_bitstream*, const char*, void*, int32)(this, name, value, size_in_bits);
}

int32 c_bitstream::read_signed_integer(const char* name, int32 size_in_bits)
{
	int32 range = 1 << (size_in_bits - 1);

	ASSERT(was_reading());
	ASSERT(size_in_bits > 0 && size_in_bits <= LONG_BITS);

	int32 value = read_integer(name, size_in_bits);

	if (size_in_bits < 32 && (value & (1 << (size_in_bits - 1))) != 0)
		value |= ~((1 << size_in_bits) - 1);

	ASSERT(value >= -range && value < range);

	return value;

	//return DECLFUNC(0x004439A0, int32, __thiscall, c_bitstream*, int32)(this, size_in_bits);
}

void c_bitstream::write_bool(const char* name, bool value)
{
	DECLFUNC(0x00444B30, void, __thiscall, c_bitstream*, const char*, bool)(this, name, value);
}

void c_bitstream::write_integer(const char* name, uns32 value, int32 size_in_bits)
{
	DECLFUNC(0x00444BE0, void, __thiscall, c_bitstream*, const char*, uns32, int32)(this, name, value, size_in_bits);
}

void c_bitstream::write_raw_data(const char* name, const void* value, int32 size_in_bits)
{
	//write_bits_internal(value, size_in_bits);
	DECLFUNC(0x00444C30, void, __thiscall, c_bitstream*, const char*, const void*, int32)(this, name, value, size_in_bits);
}

void c_bitstream::write_signed_integer(const char* name, int32 value, int32 size_in_bits)
{
	DECLFUNC(0x00444C50, void, __thiscall, c_bitstream*, const char*, int32, int32)(this, name, value, size_in_bits);
}

uns64 c_bitstream::read_qword(const char* name, int32 size_in_bits)
{
	return read_qword_internal(size_in_bits);

	//return DECLFUNC(0x0046E940, uns64, __thiscall, c_bitstream*, int32)(this, size_in_bits);
}

void c_bitstream::write_qword(const char* name, uns64 value, int32 size_in_bits)
{
	write_qword_internal(value, size_in_bits);

	//DECLFUNC(0x00470490, void, __thiscall, c_bitstream*, uns64, int32)(this, value, size_in_bits);
}

//.text:0049ED60 ; c_bitstream::c_bitstream()

//template<>
//void c_bitstream::read_axes<14, 20>(const char* name, real_vector3d* forward, real_vector3d* up)
//{
//	//004A9B30
//}
//
//template<>
//void c_bitstream::write_axes<14, 20>(const char* name, const real_vector3d* forward, const real_vector3d* up)
//{
//	//004A9BC0
//}
//
//template<>
//void c_bitstream::read_axes<8, 19>(const char* name, real_vector3d* forward, real_vector3d* up)
//{
//	//004AB910
//}
//
//template<>
//void c_bitstream::write_axes<8, 19>(const char* name, const real_vector3d* forward, const real_vector3d* up)
//{
//	//004AB9A0
//}

void c_bitstream::angle_to_axes_internal(const real_vector3d* up, real32 forward_angle, real_vector3d* forward)
{
	INVOKE(0x00556FC0, c_bitstream::angle_to_axes_internal, up, forward_angle, forward);
}

void c_bitstream::append(const c_bitstream* stream)
{
	ASSERT(stream->m_state == _bitstream_state_write_finished);
	ASSERT(writing());

	write_bits_internal(stream->m_data, stream->m_bitstream_data.current_memory_bit_position);
	m_number_of_bits_rewound += stream->m_number_of_bits_rewound;
	m_number_of_position_resets += stream->m_number_of_position_resets;

	//DECLFUNC(0x00557100, void, __thiscall, c_bitstream*, const c_bitstream*)(this, stream);
}

void c_bitstream::axes_compute_reference_internal(const real_vector3d* up, real_vector3d* forward_reference, real_vector3d* left_reference)
{
	INVOKE(0x00557150, c_bitstream::axes_compute_reference_internal, up, forward_reference, left_reference);

	//ASSERT(up);
	//ASSERT(forward_reference);
	//ASSERT(left_reference);
	//
	//assert_valid_real_normal3d(up);
	//
	//if (fabsf(dot_product3d(up, global_forward3d)) < fabsf(dot_product3d(up, global_left3d)))
	//	cross_product3d(up, global_forward3d, forward_reference);
	//else
	//	cross_product3d(global_left3d, up, forward_reference);
	//real32 forward_magnitude = normalize3d(forward_reference);
	//ASSERT(forward_magnitude > k_real_epsilon);
	//
	//cross_product3d(up, forward_reference, left_reference);
	//real32 left_magnitude = normalize3d(left_reference);
	//ASSERT(left_magnitude > k_real_epsilon);
	//
	//assert_valid_real_vector3d_axes3(forward_reference, left_reference, up);
}

real32 c_bitstream::axes_to_angle_internal(const real_vector3d* forward, const real_vector3d* up)
{
	return INVOKE(0x005573F0, c_bitstream::axes_to_angle_internal, forward, up);

	//real_vector3d forward_reference{};
	//real_vector3d left_reference{};
	//axes_compute_reference_internal(up, &forward_reference, &left_reference);
	//return arctangent(dot_product3d(&left_reference, forward), dot_product3d(&left_reference, forward));
}


bool c_bitstream::begin_consistency_check()
{
	reset(_bitstream_state_read_consistency_check);
	return true;

	//DECLFUNC(0x00557490, void, __thiscall, c_bitstream*)(this);
}

void c_bitstream::begin_reading()
{
	reset(_bitstream_state_read);

	//DECLFUNC(0x005574A0, void, __thiscall, c_bitstream*)(this);
}

void c_bitstream::begin_writing(int32 data_size_alignment)
{
	ASSERT(m_data_size_bytes % data_size_alignment == 0);

	m_data_size_alignment = data_size_alignment;
	reset(_bitstream_state_write);

	//DECLFUNC(0x005574B0, void, __thiscall, c_bitstream*, int32)(this, data_size_alignment);
}

//.text:005574F0 ; void bitstream_test()
//.text:00557770 ; public: static bool c_bitstream::compare_axes(int32, int32, const real_vector3d*, const real_vector3d*, const real_vector3d*, const real_vector3d*)
//.text:00557970 ; 
//.text:005579C0 ; public: static bool c_bitstream::compare_quantized_reals(real32, real32, real32, real32, int32, bool, bool, bool)
//.text:00557A10 ; public: static bool c_bitstream::compare_unit_vectors(const real_vector3d*, const real_vector3d*, int32)
//.text:00557A20 ; public: static bool c_bitstream::compare_vectors(const real_vector3d*, const real_vector3d*, real32, real32, int32, int32)

void c_bitstream::data_is_untrusted(bool is_untrusted)
{
	// `m_data_is_untrusted` doesn't exist in release builds
	//m_data_is_untrusted = is_untrusted;

	// this function is empty in release builds
	DECLFUNC(0x00557D60, void, __thiscall, c_bitstream*, bool)(this, is_untrusted);
}

uns64 c_bitstream::decode_qword_from_memory()
{
	return DECLFUNC(0x00557D70, uns64, __thiscall, c_bitstream*)(this);
	//return INVOKE_CLASS_MEMBER(0x00557D70, c_bitstream, decode_qword_from_memory);

	//byte* next_data = m_bitstream_data.next_data;
	//byte* data_max = m_data_max;
	//uns64 value = 0;
	//byte* v4 = next_data + 8;
	//int32 size_in_bits = 0;
	//if (next_data + CHAR_BITS > data_max)
	//{
	//	// $TODO: implement this
	//}
	//else
	//{
	//	m_bitstream_data.next_data = v4;
	//	size_in_bits = SIZEOF_BITS(uns64);
	//	value = *reinterpret_cast<uns64*>(m_bitstream_data.next_data);
	//}
	//
	//ASSERT(m_bitstream_data.next_data <= m_data_max);
	//m_bitstream_data.current_memory_bit_position += size_in_bits;
	//return value;
}

//.text:00557EB0 ; 

void c_bitstream::discard_remaining_data()
{
	ASSERT(reading());

	m_bitstream_data.current_memory_bit_position = 8 * m_data_size_bytes;
	m_bitstream_data.accumulator = 0;
	m_bitstream_data.accumulator_bit_count = 0;

	//DECLFUNC(0x00557F60, void, __thiscall, const c_bitstream*)(this);
}

void c_bitstream::encode_qword_to_memory(uns64 value, int32 size_in_bits)
{
	DECLFUNC(0x00557F80, void, __thiscall, const c_bitstream*, uns64, int32)(this, value, size_in_bits);
	//INVOKE_CLASS_MEMBER(0x00557F80, c_bitstream, encode_qword_to_memory, value, size_in_bits);

	//uns64 temp_value = bswap_uns64(value);
	//if (m_bitstream_data.next_data + CHAR_BITS > m_data_max)
	//{
	//	do
	//	{
	//		byte byte_to_write = static_cast<byte>(temp_value & MASK(CHAR_BITS));
	//		*m_bitstream_data.next_data++ = byte_to_write;
	//		temp_value >>= CHAR_BITS;
	//	} while (m_bitstream_data.next_data < m_data_max);
	//
	//	//while (m_bitstream_data.next_data < m_data_max)
	//	//{
	//	//	*m_bitstream_data.next_data++ = static_cast<byte>(right_shift_fast<uns64>(temp_value, SIZEOF_BITS(uns64) - CHAR_BITS) & MASK(CHAR_BITS));
	//	//	temp_value <<= CHAR_BITS;
	//	//}
	//}
	//else
	//{
	//	*reinterpret_cast<uns64*>(m_bitstream_data.next_data) = temp_value;
	//	m_bitstream_data.next_data += sizeof(uns64);
	//}
	//m_bitstream_data.current_memory_bit_position += size_in_bits;
}

bool c_bitstream::overflowed() const
{
	ASSERT(was_reading() || was_writing());

	return m_bitstream_data.current_stream_bit_position > 8 * m_data_size_bytes;
}

bool c_bitstream::error_occurred() const
{
	bool result = overflowed();
	if (m_data_error_detected)
		return true;
	return result;

	//DECLFUNC(0x00558090, bool, __thiscall, const c_bitstream*)(this);
}

bool c_bitstream::was_reading() const
{
	return IN_RANGE_INCLUSIVE(m_state, _bitstream_state_read, _bitstream_state_read_finished);
}

bool c_bitstream::was_writing() const
{
	return IN_RANGE_INCLUSIVE(m_state, _bitstream_state_write, _bitstream_state_write_finished);
}

void c_bitstream::finish_consistency_check()
{
	finish_reading();

	//DECLFUNC(0x005580B0, void, __thiscall, c_bitstream*)(this);
}

void c_bitstream::finish_reading()
{
	ASSERT(reading());

	if (overflowed())
	{
		VASSERT(c_string_builder("finish_reading: bitstream read off the end of the stream (%d bits > %d max-size)",
			m_bitstream_data.current_memory_bit_position, 8 * m_data_size_bytes).get_string());
	}

	m_state = _bitstream_state_read_finished;

	//DECLFUNC(0x005580C0, void, __thiscall, c_bitstream*)(this);
}

void c_bitstream::finish_writing(int32* bits_wasted)
{
	if (overflowed())
	{
		VASSERT(c_string_builder("bitstream overflowed (%d bits > %d max-size), cannot be written successfully",
			m_bitstream_data.current_stream_bit_position, 8 * m_data_size_bytes).get_string());
	}

	DECLFUNC(0x005580D0, void, __thiscall, c_bitstream*, int32*)(this, bits_wasted);
}

int32 c_bitstream::get_current_stream_bit_position()
{
	return m_bitstream_data.current_stream_bit_position;

	//DECLFUNC(0x00558240, void, __thiscall, c_bitstream*)(this);
}

int32 c_bitstream::get_space_used_in_bits()
{
	ASSERT(was_writing());
	return m_bitstream_data.accumulator_bit_count + m_bitstream_data.current_stream_bit_position;
}

const byte* c_bitstream::get_data(int32* data_length) const
{
	//return DECLFUNC(0x00558250, const byte*, __thiscall, const c_bitstream*, int32*)(this, data_length);

	ASSERT(!writing());
	ASSERT(data_length);

	*data_length = m_data_size_bytes;
	return m_data;
}

void c_bitstream::push_position()
{
	ASSERT(writing());
	ASSERT(m_position_stack_depth < k_bitstream_maximum_position_stack_size);

	m_position_stack[m_position_stack_depth++] = m_bitstream_data;
}

void c_bitstream::pop_position(bool reset_to_pushed_state)
{
	ASSERT(reading() || writing());
	ASSERT(m_position_stack_depth > 0);

	DECLFUNC(0x00558310, void, __thiscall, c_bitstream*, bool)(this, reset_to_pushed_state);
}

uns64 c_bitstream::read_accumulator_from_memory(int32 size_in_bits)
{
	return DECLFUNC(0x005583D0, bool, __thiscall, c_bitstream*, int32)(this, size_in_bits);

	//uns64 accumulator = m_bitstream_data.accumulator;
	//uns64 qword_from_memory = c_bitstream::decode_qword_from_memory();
	//
	//m_bitstream_data.current_stream_bit_position += size_in_bits;
	//int32 bits_from_next_accumulator = m_bitstream_data.accumulator_bit_count + size_in_bits - QWORD_BITS;
	//if (bits_from_next_accumulator < QWORD_BITS)
	//{
	//	m_bitstream_data.accumulator = qword_from_memory << bits_from_next_accumulator;
	//}
	//else
	//{
	//	m_bitstream_data.accumulator = 0LL;
	//}
	//m_bitstream_data.accumulator_bit_count = bits_from_next_accumulator;
	//
	//uns64 value_portion_from_old_accumulator = right_shift_fast(accumulator, size_in_bits);
	//uns64 value_portion_from_new_accumulator = right_shift_fast(qword_from_memory, QWORD_BITS - bits_from_next_accumulator);
	//ASSERT((value_portion_from_old_accumulator & MASK(bits_from_next_accumulator)) == 0);
	//
	//return value_portion_from_old_accumulator | value_portion_from_new_accumulator;
}

bool c_bitstream::read_bool(const char* name)
{
	ASSERT(reading());

	return DECLFUNC(0x00558570, bool, __thiscall, c_bitstream*)(this);
}

void c_bitstream::read_bits_internal(void* data, int32 size_in_bits)
{
	ASSERT(reading());

	DECLFUNC(0x00558740, void, __thiscall, c_bitstream*, void*, int32)(this, data, size_in_bits);
}

uns32 c_bitstream::read_integer(const char* name, int32 size_in_bits)
{
	ASSERT(reading());
	ASSERT(size_in_bits > 0 && size_in_bits <= LONG_BITS);

	//uns32 value = read_value_internal(size_in_bits)
	//ASSERT(value < (1 << size_in_bits));

	return DECLFUNC(0x005589A0, uns32, __thiscall, c_bitstream*, int32)(this, size_in_bits);
}

void c_bitstream::read_identifier(const char* identifier)
{
	ASSERT(reading());

	//return DECLFUNC(0x00558B90, void, __thiscall, c_bitstream*, const char*)(this, identifier);
}

void c_bitstream::read_point3d(const char* name, long_point3d* point, int32 axis_encoding_size_in_bits)
{
	ASSERT(reading());
	ASSERT(axis_encoding_size_in_bits >= 0 && axis_encoding_size_in_bits <= SIZEOF_BITS(point->n[0]));

	//for (int32 axis = 0; axis < 3; ++axis)
	//{
	//	point->n[axis] = read_value_internal(axis_encoding_size_in_bits);
	//	ASSERT(point->n[axis] < (1 << axis_encoding_size_in_bits));
	//}

	DECLFUNC(0x00558C50, void, __thiscall, c_bitstream*, const char*, long_point3d*, int32)(this, name, point, axis_encoding_size_in_bits);
}

real32 c_bitstream::read_quantized_real(const char* name, real32 min_value, real32 max_value, int32 size_in_bits, bool exact_midpoint, bool exact_endpoints)
{
	ASSERT(reading());

	return DECLFUNC(0x00559080, real32, __thiscall, c_bitstream*, const char*, real32, real32, int32, bool, bool)(this, name, min_value, max_value, size_in_bits, exact_midpoint, exact_endpoints);
}

uns64 c_bitstream::read_qword_internal(int32 size_in_bits)
{
	ASSERT(reading());
	ASSERT(size_in_bits > 0 && size_in_bits <= QWORD_BITS);

	return DECLFUNC(0x00559160, uns64, __thiscall, c_bitstream*, int32)(this, size_in_bits);

	//ASSERT(reading());
	//ASSERT(size_in_bits > 0 && size_in_bits <= QWORD_BITS);
	//
	//if (size_in_bits > QWORD_BITS - m_bitstream_data.accumulator_bit_count)
	//{
	//	return c_bitstream::read_accumulator_from_memory(size_in_bits);
	//}
	//
	//uns64 value = right_shift_fast(m_bitstream_data.accumulator, QWORD_BITS - size_in_bits);
	//m_bitstream_data.current_stream_bit_position += size_in_bits;
	//uns64 accumulator = m_bitstream_data.accumulator << size_in_bits;
	//if (size_in_bits >= QWORD_BITS)
	//{
	//	accumulator = 0LL;
	//}
	//m_bitstream_data.accumulator_bit_count += size_in_bits;
	//m_bitstream_data.accumulator = accumulator;
}

void c_bitstream::read_secure_address(const char* name, s_transport_secure_address* address)
{
	ASSERT(reading());
	ASSERT(address);

	//read_bits_internal(address, 128);

	DECLFUNC(0x00559360, void, __thiscall, c_bitstream*, const char*, s_transport_secure_address*)(this, name, address);
}

void c_bitstream::read_string(const char* name, char* string, int32 max_string_size)
{
	ASSERT(reading());
	ASSERT(string);
	ASSERT(max_string_size > 0);

	DECLFUNC(0x00559380, void, __thiscall, c_bitstream*, const char*, char*, int32)(this, name, string, max_string_size);
}

void c_bitstream::read_string_utf8(const char* name, char* char_string, int32 max_string_size)
{
	ASSERT(reading());
	ASSERT(char_string);
	ASSERT(max_string_size > 0);

	DECLFUNC(0x00559670, void, __thiscall, c_bitstream*, const char*, utf8*, int32)(this, name, char_string, max_string_size);
}

void c_bitstream::read_string_wchar(const char* name, wchar_t* string, int32 max_string_size)
{
	ASSERT(reading());
	ASSERT(string);
	ASSERT(max_string_size > 0);

	DECLFUNC(0x00559880, void, __thiscall, c_bitstream*, const char*, wchar_t*, int32)(this, name, string, max_string_size);
}

void c_bitstream::read_unit_vector(const char* name, real_vector3d* unit_vector, int32 size_in_bits)
{
	DECLFUNC(0x00559A90, void, __thiscall, c_bitstream*, const char*, real_vector3d*, int32)(this, name, unit_vector, size_in_bits);
}

void c_bitstream::read_vector(const char* name, real_vector3d* vector, real32 min_value, real32 max_value, int32 step_count_size_in_bits, int32 size_in_bits)
{
	DECLFUNC(0x00559AB0, void, __thiscall, c_bitstream*, const char*, real_vector3d*, real32, real32, int32, int32)(this, name, vector, min_value, max_value, step_count_size_in_bits, size_in_bits);
}

void c_bitstream::reset(int32 state)
{
	ASSERT(state >= 0 && state < k_bitstream_state_count);
	DECLFUNC(0x00559BE0, void, __thiscall, c_bitstream*, int32)(this, state);
}

bool c_bitstream::reading() const
{
	return IN_RANGE_INCLUSIVE(m_state, _bitstream_state_read, _bitstream_state_read_consistency_check);
}

void c_bitstream::set_data(byte* data, int32 data_length)
{
	m_data = data;
	m_data_max = &data[data_length];
	m_data_size_bytes = data_length;
	reset(_bitstream_state_none);

	//DECLFUNC(0x00559D90, void, __thiscall, c_bitstream*, byte*, int32)(this, data, data_length);
}

void c_bitstream::skip(int32 bits_to_skip)
{
	DECLFUNC(0x00559DC0, void, __thiscall, c_bitstream*, int32)(this, bits_to_skip);
}

bool c_bitstream::would_overflow(int32 size_in_bits) const
{
	return DECLFUNC(0x00559E90, bool, __thiscall, const c_bitstream*, int32)(this, size_in_bits);
}

void c_bitstream::write_accumulator_to_memory(uns64 value, int32 size_in_bits)
{
	DECLFUNC(0x00559EB0, void, __thiscall, c_bitstream*, uns64, int32)(this, value, size_in_bits);

	//uns64 accumulator = m_bitstream_data.accumulator;
	//int32 shift_bits = SIZEOF_BITS(uns64) - m_bitstream_data.accumulator_bit_count;
	//m_bitstream_data.current_stream_bit_position += size_in_bits;
	//int32 accumulator_bit_count = size_in_bits - shift_bits;
	//
	//if (accumulator_bit_count < SIZEOF_BITS(uns64))
	//	accumulator = left_shift_fast(accumulator, shift_bits) | (value >> accumulator_bit_count);
	//
	//byte* next_data = m_bitstream_data.next_data;
	//byte* data_max = m_data_max;
	//
	//if (next_data + SIZEOF_BITS(byte) > data_max)
	//{
	//	if (next_data < data_max)
	//	{
	//		do
	//		{
	//			byte next_byte = HIBYTE(accumulator);
	//			accumulator <<= SIZEOF_BITS(byte);
	//			*m_bitstream_data.next_data++ = next_byte;
	//		} while (m_bitstream_data.next_data < m_data_max);
	//	}
	//
	//	m_bitstream_data.current_memory_bit_position += SIZEOF_BITS(uns64);
	//}
	//else
	//{
	//	*(uns64*)next_data = bswap_uns64(accumulator);
	//	m_bitstream_data.next_data += SIZEOF_BITS(byte);
	//	m_bitstream_data.current_memory_bit_position += SIZEOF_BITS(uns64);
	//}
}

void c_bitstream::write_bits_internal(const void* data, int32 size_in_bits)
{
	DECLFUNC(0x0055A000, void, __thiscall, c_bitstream*, const void*, int32)(this, data, size_in_bits);
}

void c_bitstream::write_identifier(const char* identifier)
{
	ASSERT(writing());

	//DECLFUNC(0x0055A100, void, __thiscall, const c_bitstream*, const char*)(this, identifier);
}

void c_bitstream::write_point3d(const char* name, const long_point3d* point, int32 axis_encoding_size_in_bits)
{
	DECLFUNC(0x0055A1E0, void, __thiscall, c_bitstream*, const char*, const long_point3d*, int32)(this, name, point, axis_encoding_size_in_bits);
}

void c_bitstream::write_quantized_real(const char* name, real32* value, real32 min_value, real32 max_value, int32 size_in_bits, bool exact_midpoint, bool exact_endpoints)
{
	ASSERT(writing());

	DECLFUNC(0x0055A2B0, void, __thiscall, c_bitstream*, const char*, real32*, real32, real32, int32, bool, bool)(this, name, value, min_value, max_value, size_in_bits, exact_midpoint, exact_endpoints);
}

void c_bitstream::write_qword_internal(uns64 value, int32 size_in_bits)
{
	ASSERT(writing());
	ASSERT(size_in_bits <= SIZEOF_BITS(uns64));

	DECLFUNC(0x0055A3A0, void, __thiscall, c_bitstream*, uns64, int32)(this, value, size_in_bits);
}

void c_bitstream::write_secure_address(const char* name, const s_transport_secure_address* address)
{
	ASSERT(writing());
	ASSERT(address);

	//write_bits_internal(address, 128);

	DECLFUNC(0x0055A410, void, __thiscall, c_bitstream*, const char*, const s_transport_secure_address*)(this, name, address);
}

void c_bitstream::write_string(const char* name, const char* string, int32 max_string_size)
{
	ASSERT(writing());
	ASSERT(string);
	ASSERT(max_string_size > 0);
	//ASSERT(strnlen(string, max_string_size) < max_string_size);

	DECLFUNC(0x0055A430, void, __thiscall, c_bitstream*, const char*, const char*, int32)(this, name, string, max_string_size);
}

void c_bitstream::write_string_utf8(const char* name, const utf8* char_string, int32 max_string_size)
{
	ASSERT(writing());
	ASSERT(char_string);
	ASSERT(max_string_size > 0);
	//ASSERT(strnlen(char_string, max_string_size) < max_string_size);

	DECLFUNC(0x0055A650, void, __thiscall, c_bitstream*, const char*, const utf8*, int32)(this, name, char_string, max_string_size);
}

void c_bitstream::write_string_wchar(const char* name, const wchar_t* string, int32 max_string_size)
{
	ASSERT(writing());
	ASSERT(string);
	ASSERT(max_string_size > 0);

	DECLFUNC(0x0055A6D0, void, __thiscall, c_bitstream*, const char*, const wchar_t*, int32)(this, name, string, max_string_size);
}

void c_bitstream::write_unit_vector(const char* name, const real_vector3d* unit_vector, int32 size_in_bits)
{
	DECLFUNC(0x0055A750, void, __thiscall, c_bitstream*, const char*, const real_vector3d*, int32)(this, name, unit_vector, size_in_bits);
}

void c_bitstream::write_vector(const char* name, const real_vector3d* vector, real32 min_value, real32 max_value, int32 step_count_size_in_bits, int32 size_in_bits)
{
	DECLFUNC(0x0055A7B0, void, __thiscall, c_bitstream*, const char*, const real_vector3d*, real32, real32, int32, int32)(this, name, vector, min_value, max_value, step_count_size_in_bits, size_in_bits);
}

bool c_bitstream::writing() const
{
	//return DECLFUNC(0x0055A9D0, bool, __thiscall, const c_bitstream*)(this);

	return m_state == _bitstream_state_write;
}

