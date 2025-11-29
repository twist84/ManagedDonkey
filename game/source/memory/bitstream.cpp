#include "memory/bitstream.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "math/unit_vector_quantization.hpp"
#include "memory/byte_swapping.hpp"
#include "networking/transport/transport_security.hpp"

template<typename t_type>
t_type left_shift_fast(t_type value, int32 shift_bits)
{
	ASSERT(shift_bits <= SIZEOF_BITS(t_type));
	t_type result = t_type(value << shift_bits);
	return result;
}

template<typename t_type>
t_type left_shift_safe(t_type value, int32 shift_bits)
{
	t_type result = t_type(value << shift_bits);
	if (shift_bits >= SIZEOF_BITS(t_type))
	{
		result = t_type(0);
	}
	return result;
}

template<typename t_type>
t_type right_shift_fast(t_type value, int32 shift_bits)
{
	ASSERT(shift_bits < SIZEOF_BITS(t_type));

	t_type result = t_type(value >> shift_bits);
	return result;
}

template<typename t_type>
t_type right_shift_safe(t_type value, int32 shift_bits)
{
	t_type result = t_type(value >> shift_bits);
	if (shift_bits < SIZEOF_BITS(t_type))
	{
		result = t_type(0);
	}
	return result;
}

//.text:0043B5B0 ; c_bitstream::c_bitstream(byte* data, int32 data_length)
//.text:0043B5D0 ; c_bitstream::~c_bitstream()

//template<int32 k_forward_angle_bits, int32 k_up_quantization_bits>
//void c_bitstream::read_axes(const char* debug_string, real_vector3d* forward, real_vector3d* up)
//{
//	if (read_bool("up-is-global-up3d"))
//	{
//		*up = *global_up3d;
//	}
//	else
//	{
//		dequantize_unit_vector3d(c_bitstream::read_integer("up-quantization", k_up_quantization_bits), up, k_up_quantization_bits);
//	}
//
//	angle_to_axes_internal(up, c_bitstream::read_quantized_real("forward-angle", -_pi, _pi, k_forward_angle_bits, false, false), forward);
//}

//template<int32 k_forward_angle_bits, int32 k_up_quantization_bits>
//void c_bitstream::write_axes(const char* debug_string, const real_vector3d* forward, const real_vector3d* up)
//{
//	real_vector3d dequantized_up{};
//	if (fabs(up->i - global_up3d->i) > k_real_epsilon ||
//		fabs(up->j - global_up3d->j) > k_real_epsilon ||
//		fabs(up->k - global_up3d->k) > k_real_epsilon)
//	{
//		int32 quantized_up = quantize_unit_vector3d_fast<k_up_quantization_bits>(up);
//		c_bitstream::write_bool("up-is-global-up3d", false);
//		c_bitstream::write_integer("up-quantization", quantized_up, k_up_quantization_bits);
//		dequantize_unit_vector3d(quantized_up, &dequantized_up, k_up_quantization_bits);
//	}
//	else
//	{
//		c_bitstream::write_bool("up-is-global-up3d", true);
//		dequantized_up = *global_up3d;
//	}
//}

bool c_bitstream::read_bool(const char* debug_string)
{
	ASSERT(reading());

	return DECLFUNC(0x00443960, bool, __thiscall, c_bitstream*, const char*)(this, debug_string);
	//return INVOKE_CLASS_MEMBER(0x00443960, c_bitstream, read_bool, debug_string);

	//return c_bitstream::read_bit_internal();
}

uns32 c_bitstream::read_integer(const char* debug_string, int32 size_in_bits)
{
	//return DECLFUNC(0x00443970, uns32, __thiscall, c_bitstream*, int32)(this, size_in_bits);
	//return INVOKE_CLASS_MEMBER(0x00443970, c_bitstream, read_integer, debug_string, size_in_bits);
	
	ASSERT(reading());
	ASSERT(size_in_bits > 0 && size_in_bits <= LONG_BITS);
	
	uns32 value = c_bitstream::read_dword_internal(size_in_bits);
	
	if (size_in_bits < LONG_BITS)
	{
		ASSERT(value < (uns32)(1 << size_in_bits));
	}
	
	return value;
}

void c_bitstream::read_raw_data(const char* debug_string, void* raw_data, int32 size_in_bits)
{
	DECLFUNC(0x00443980, void, __thiscall, c_bitstream*, const char*, void*, int32)(this, debug_string, raw_data, size_in_bits);

	//ASSERT(reading());
	//c_bitstream::read_bits_internal((byte*)raw_data, size_in_bits);
}

int32 c_bitstream::read_signed_integer(const char* debug_string, int32 size_in_bits)
{
	//return DECLFUNC(0x004439A0, int32, __thiscall, c_bitstream*, int32)(this, size_in_bits);

	int32 range = RANGE(size_in_bits);

	ASSERT(was_reading());
	ASSERT(size_in_bits > 0 && size_in_bits <= LONG_BITS);

	int32 value = c_bitstream::read_integer(debug_string, size_in_bits);

	if (size_in_bits < LONG_BITS && (value & range) != 0)
	{
		value |= ~MASK(size_in_bits);

		//int32 v5 = 0;
		//if (size_in_bits >= LONG_BITS)
		//{
		//	v5 = FLAG(size_in_bits);
		//}
		//
		//value |= ~((v5 ^ FLAG(size_in_bits)) - 1);
	}

	ASSERT(value >= -range && value < range);

	return value;
}

void c_bitstream::write_bit_internal(bool value)
{
	DECLFUNC(0x00444AD0, void, __thiscall, c_bitstream*, bool)(this, value);

	//ASSERT(writing());
	//
	//m_bitstream_data.current_stream_bit_position++;
	//
	//if (m_bitstream_data.accumulator_bit_count >= QWORD_BITS)
	//{
	//	uns64 bit = value ? 1 : 0;
	//	write_accumulator_to_memory(bit, 1);
	//}
	//else
	//{
	//	m_bitstream_data.accumulator = (m_bitstream_data.accumulator << 1) | (value ? 1 : 0);
	//	m_bitstream_data.accumulator_bit_count += 1;
	//}
}

void c_bitstream::write_bool(const char* debug_string, bool value)
{
	//DECLFUNC(0x00444B30, void, __thiscall, c_bitstream*, const char*, bool)(this, debug_string, value);

	ASSERT(writing());

	c_bitstream::write_bit_internal(value);
}

void c_bitstream::write_dword_internal(uns32 value, int32 size_in_bits)
{
	DECLFUNC(0x00444B90, void, __thiscall, c_bitstream*, uns32, int32)(this, value, size_in_bits);

	//ASSERT(m_state == _bitstream_state_write);
	//ASSERT(size_in_bits >= 0 && size_in_bits <= LONG_BITS);
	//ASSERT((value & ~((1 << size_in_bits) - 1)) == 0);
	//
	//int32 shift = QWORD_BITS - m_bitstream_data.accumulator_bit_count;
	//
	//if (size_in_bits > shift)
	//{
	//	write_accumulator_to_memory((uns64)value, size_in_bits);
	//}
	//else
	//{
	//	m_bitstream_data.current_stream_bit_position += size_in_bits;
	//	m_bitstream_data.accumulator <<= size_in_bits;
	//	m_bitstream_data.accumulator |= value;
	//	m_bitstream_data.accumulator_bit_count += size_in_bits;
	//}
}

void c_bitstream::write_integer(const char* debug_string, uns32 value, int32 size_in_bits)
{
	DECLFUNC(0x00444BE0, void, __thiscall, c_bitstream*, const char*, uns32, int32)(this, debug_string, value, size_in_bits);

	//ASSERT(writing());
	//ASSERT(size_in_bits > 0 && size_in_bits <= LONG_BITS);
	//
	//if (size_in_bits < LONG_BITS && value >= FLAG(size_in_bits))
	//{
	//	VASSERT(c_string_builder("%u %s max value of %u (writing %s)",
	//		value,
	//		value > FLAG(size_in_bits) ? "exceeds" : "equal to",
	//		MASK(size_in_bits),
	//		debug_string).get_string());
	//}
	//
	//c_bitstream::write_dword_internal(value, size_in_bits);
}

void c_bitstream::write_raw_data(const char* debug_string, const void* raw_data, int32 size_in_bits)
{
	DECLFUNC(0x00444C30, void, __thiscall, c_bitstream*, const char*, const void*, int32)(this, debug_string, raw_data, size_in_bits);

	//c_bitstream::write_bits_internal((const byte*)raw_data, size_in_bits);
}

void c_bitstream::write_signed_integer(const char* debug_string, int32 value, int32 size_in_bits)
{
	DECLFUNC(0x00444C50, void, __thiscall, c_bitstream*, const char*, int32, int32)(this, debug_string, value, size_in_bits);

	//int32 range = 1 << (size_in_bits - 1);
	//
	//ASSERT(writing());
	//ASSERT(size_in_bits > 0 && size_in_bits <= LONG_BITS);
	//ASSERT(value >= -range && value < range);
	//c_bitstream::write_dword_internal(value & MASK(size_in_bits), size_in_bits);
}

uns64 c_bitstream::read_qword(const char* debug_string, int32 size_in_bits)
{
	return c_bitstream::read_qword_internal(size_in_bits);

	//return DECLFUNC(0x0046E940, uns64, __thiscall, c_bitstream*, int32)(this, size_in_bits);
}

void c_bitstream::write_qword(const char* debug_string, uns64 value, int32 size_in_bits)
{
	c_bitstream::write_qword_internal(value, size_in_bits);

	//DECLFUNC(0x00470490, void, __thiscall, c_bitstream*, uns64, int32)(this, value, size_in_bits);
}

//.text:0049ED60 ; c_bitstream::c_bitstream()

//template<>
//void c_bitstream::read_axes<14, 20>(const char* debug_string, real_vector3d* forward, real_vector3d* up)
//{
//	//004A9B30
//}
//
//template<>
//void c_bitstream::write_axes<14, 20>(const char* debug_string, const real_vector3d* forward, const real_vector3d* up)
//{
//	//004A9BC0
//}
//
//template<>
//void c_bitstream::read_axes<8, 19>(const char* debug_string, real_vector3d* forward, real_vector3d* up)
//{
//	//004AB910
//}
//
//template<>
//void c_bitstream::write_axes<8, 19>(const char* debug_string, const real_vector3d* forward, const real_vector3d* up)
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

	c_bitstream::write_bits_internal(stream->m_data, stream->m_bitstream_data.current_memory_bit_position);
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
	//{
	//	cross_product3d(up, global_forward3d, forward_reference);
	//}
	//else
	//{
	//	cross_product3d(global_left3d, up, forward_reference);
	//}
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
	//DECLFUNC(0x005574B0, void, __thiscall, c_bitstream*, int32)(this, data_size_alignment);

	ASSERT(m_data_size_bytes % data_size_alignment == 0);

	m_data_size_alignment = data_size_alignment;
	reset(_bitstream_state_write);
}

//.text:005574F0 ; void bitstream_test()
//.text:00557770 ; public: static bool c_bitstream::compare_axes(int32, int32, const real_vector3d*, const real_vector3d*, const real_vector3d*, const real_vector3d*)
//.text:00557970 ; 
//.text:005579C0 ; public: static bool c_bitstream::compare_quantized_reals(real32, real32, real32, real32, int32, bool, bool, bool)
//.text:00557A10 ; public: static bool c_bitstream::compare_unit_vectors(const real_vector3d*, const real_vector3d*, int32)
//.text:00557A20 ; public: static bool c_bitstream::compare_vectors(const real_vector3d*, const real_vector3d*, real32, real32, int32, int32)

void c_bitstream::data_is_untrusted(bool is_untrusted)
{
	//DECLFUNC(0x00557D60, void, __thiscall, c_bitstream*, bool)(this, is_untrusted);

	//m_data_is_untrusted = is_untrusted;
}

uns64 c_bitstream::decode_qword_from_memory()
{
	return DECLFUNC(0x00557D70, uns64, __thiscall, c_bitstream*)(this);
	//return INVOKE_CLASS_MEMBER(0x00557D70, c_bitstream, decode_qword_from_memory);

	//uns64 value = 0;
	//int32 used_bit_count = 0;
	//
	//if ((m_bitstream_data.next_data + QWORD_BYTES) > m_data_max)
	//{
	//	while (m_bitstream_data.next_data < m_data_max)
	//	{
	//		value = left_shift_fast<uns64>(value, CHAR_BITS) | *m_bitstream_data.next_data;
	//		m_bitstream_data.next_data++;
	//		used_bit_count += CHAR_BITS;
	//	}
	//
	//	value = left_shift_fast<uns64>(value, QWORD_BITS - used_bit_count);
	//}
	//else
	//{
	//	value = bswap_uns64((uns64)m_bitstream_data.next_data);
	//	m_bitstream_data.next_data += QWORD_BYTES;
	//	used_bit_count = QWORD_BYTES;
	//}
	//
	//ASSERT(m_bitstream_data.next_data <= m_data_max);
	//m_bitstream_data.current_memory_bit_position += used_bit_count;
	//
	//return value;
}

//.text:00557EB0 ; 

void c_bitstream::discard_remaining_data()
{
	//DECLFUNC(0x00557F60, void, __thiscall, const c_bitstream*)(this);

	ASSERT(reading());

	m_bitstream_data.current_memory_bit_position = CHAR_BITS * m_data_size_bytes;
	m_bitstream_data.accumulator = 0;
	m_bitstream_data.accumulator_bit_count = 0;
}

void c_bitstream::encode_qword_to_memory(uns64 value, int32 bit_count)
{
	DECLFUNC(0x00557F80, void, __thiscall, const c_bitstream*, uns64, int32)(this, value, bit_count);
	//INVOKE_CLASS_MEMBER(0x00557F80, c_bitstream, encode_qword_to_memory, value, bit_count);

	//if ((m_bitstream_data.next_data + QWORD_BYTES) > m_data_max)
	//{
	//	while (m_bitstream_data.next_data < m_data_max)
	//	{
	//		*m_bitstream_data.next_data++ = (byte)right_shift_fast<uns64>(value, QWORD_BITS - CHAR_BITS);
	//		value = left_shift_fast<uint64_t>(value, CHAR_BITS);
	//	}
	//
	//	m_bitstream_data.current_memory_bit_position += bit_count;
	//}
	//else
	//{
	//	*(uns64*)m_bitstream_data.next_data = bswap_uns64(value);
	//	m_bitstream_data.next_data += QWORD_BYTES;
	//}
	//
	//m_bitstream_data.current_memory_bit_position += bit_count;
}

bool c_bitstream::overflowed() const
{
	ASSERT(was_reading() || was_writing());

	return m_bitstream_data.current_stream_bit_position > CHAR_BITS * m_data_size_bytes;
}

bool c_bitstream::error_occurred() const
{
	//DECLFUNC(0x00558090, bool, __thiscall, const c_bitstream*)(this);

	bool result = overflowed();
	if (m_data_error_detected)
	{
			return true;
	}
	return result;
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
	//DECLFUNC(0x005580B0, void, __thiscall, c_bitstream*)(this);

	finish_reading();
}

void c_bitstream::finish_reading()
{
	//DECLFUNC(0x005580C0, void, __thiscall, c_bitstream*)(this);

	ASSERT(reading());

	VASSERT(!overflowed(), c_string_builder("finish_reading: bitstream read off the end of the stream (%d bits > %d max-size)",
		m_bitstream_data.current_memory_bit_position, CHAR_BITS * m_data_size_bytes).get_string());

	m_state = _bitstream_state_read_finished;
}

void c_bitstream::finish_writing(int32* bits_wasted)
{
	VASSERT(!overflowed(), c_string_builder("bitstream overflowed (%d bits > %d max-size), cannot be written successfully",
		m_bitstream_data.current_stream_bit_position, CHAR_BITS * m_data_size_bytes).get_string());

	DECLFUNC(0x005580D0, void, __thiscall, c_bitstream*, int32*)(this, bits_wasted);

	//int32 accumulator_bit_count = m_bitstream_data.accumulator_bit_count;
	//if (accumulator_bit_count != QWORD_BITS)
	//{
	//	m_bitstream_data.accumulator = left_shift_fast<uns64>(m_bitstream_data.accumulator, QWORD_BITS - accumulator_bit_count);
	//}
	//
	//c_bitstream::encode_qword_to_memory(m_bitstream_data.accumulator, QWORD_BITS);
	//
	//m_bitstream_data.current_memory_bit_position += accumulator_bit_count;
	//
	//m_bitstream_data.accumulator = 0;
	//m_bitstream_data.accumulator_bit_count = 0;
	//
	//int32 total_bits = m_bitstream_data.current_memory_bit_position;
	//int32 total_bytes = (total_bits + (CHAR_BITS - 1)) / CHAR_BITS;
	//m_data_size_bytes = total_bytes;
	//m_data_max = &m_data[total_bytes];
	//
	//if (total_bytes % m_data_size_alignment)
	//{
	//	m_data_size_bytes = m_data_size_alignment + total_bytes - (total_bytes % m_data_size_alignment);
	//}
	//
	//m_state = _bitstream_state_write_finished;
	//
	//if (bits_wasted)
	//{
	//	*bits_wasted = (m_data_size_bytes * CHAR_BITS) - total_bits;
	//}
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

	//uns64 old_accumulator = m_bitstream_data.accumulator;
	//uns64 new_accumulator = c_bitstream::decode_qword_from_memory();
	//
	//m_bitstream_data.current_stream_bit_position += size_in_bits;
	//int32 bits_from_next_accumulator = m_bitstream_data.accumulator_bit_count + size_in_bits - QWORD_BITS;
	//if (bits_from_next_accumulator < QWORD_BITS)
	//{
	//	m_bitstream_data.accumulator = new_accumulator << bits_from_next_accumulator;
	//}
	//else
	//{
	//	m_bitstream_data.accumulator = 0LL;
	//}
	//m_bitstream_data.accumulator_bit_count = bits_from_next_accumulator;
	//
	//uns64 value_portion_from_old_accumulator = right_shift_fast(old_accumulator, size_in_bits);
	//uns64 value_portion_from_new_accumulator = right_shift_fast(new_accumulator, QWORD_BITS - bits_from_next_accumulator);
	//ASSERT((value_portion_from_old_accumulator & MASK(bits_from_next_accumulator)) == 0);
	//
	//return value_portion_from_old_accumulator | value_portion_from_new_accumulator;
}

bool c_bitstream::read_bit_internal()
{
	return DECLFUNC(0x00558570, bool, __thiscall, c_bitstream*)(this);

	//if (m_bitstream_data.accumulator_bit_count < QWORD_BITS)
	//{
	//	uns64 accumulator = m_bitstream_data.accumulator;
	//	bool result = (accumulator >> (QWORD_BITS - 1)) != 0;
	//
	//	m_bitstream_data.current_stream_bit_position++;
	//	m_bitstream_data.accumulator = accumulator << 1;
	//	m_bitstream_data.accumulator_bit_count += 1;
	//
	//	return result;
	//}
	//
	//uns64 value = c_bitstream::decode_qword_from_memory();
	//
	//uns64 old_accumulator = m_bitstream_data.accumulator;
	//bool result = (old_accumulator >> (QWORD_BITS - 1)) != 0;
	//
	//int32 new_accumulator_bits = m_bitstream_data.accumulator_bit_count - (QWORD_BITS - 1);
	//m_bitstream_data.accumulator_bit_count = new_accumulator_bits;
	//
	//m_bitstream_data.current_stream_bit_position++;
	//
	//if (new_accumulator_bits < QWORD_BITS)
	//{
	//	m_bitstream_data.accumulator = left_shift_fast<uns64>(value, new_accumulator_bits);
	//}
	//else
	//{
	//	m_bitstream_data.accumulator = 0;
	//}
	//
	//return result || ((value >> (QWORD_BITS - new_accumulator_bits)) != 0);
}

void c_bitstream::read_bits_internal(byte* data, int32 size_in_bits)
{
	ASSERT(reading());

	DECLFUNC(0x00558740, void, __thiscall, c_bitstream*, byte*, int32)(this, data, size_in_bits);

	//int32 size_in_qwords = size_in_bits / QWORD_BITS;
	//for (int32 qword_index = 0; qword_index < size_in_qwords; qword_index++)
	//{
	//	uns64 value = c_bitstream::read_qword_internal(QWORD_BITS);
	//	value = bswap_uns64(value);
	//
	//	*(uns64*)data = value;
	//	data += QWORD_BYTES;
	//}
	//
	//int32 remaining_bits = size_in_bits % QWORD_BITS;
	//if (remaining_bits > CHAR_BITS)
	//{
	//	uns64 value = c_bitstream::read_qword_internal(remaining_bits);
	//	value = left_shift_fast<uns64>(value, QWORD_BITS - remaining_bits);
	//
	//	int32 byte_count = remaining_bits / CHAR_BITS;
	//	for (int32 i = 0; i < byte_count; i++)
	//	{
	//		data[i] = (byte)(value >> (QWORD_BITS - CHAR_BITS));
	//		value <<= CHAR_BITS;
	//	}
	//
	//	int32 partial_bits = remaining_bits % CHAR_BITS;
	//	if (partial_bits > 0)
	//	{
	//		data[byte_count] = (byte)(value >> (QWORD_BITS - CHAR_BITS));
	//	}
	//}
}

uns32 c_bitstream::read_dword_internal(int32 size_in_bits)
{
	return DECLFUNC(0x005589A0, uns32, __thiscall, c_bitstream*, int32)(this, size_in_bits);

	//ASSERT(reading());
	//ASSERT(size_in_bits <= SIZEOF_BITS(uns32));
	//
	//return (uns32)read_qword_internal(size_in_bits);
}

void c_bitstream::read_identifier(const char* debug_string)
{
	ASSERT(reading());

	//return DECLFUNC(0x00558B90, void, __thiscall, c_bitstream*, const char*)(this, debug_string);
}

real32 c_bitstream::read_logarithmic_quantized_real(const char* debug_string, real32 min_value, real32 max_value, int32 size_in_bits)
{
	return (real32)DECLFUNC(0x00558BA0, double, __thiscall, c_bitstream*, const char*, real32, real32, int32)(this, debug_string, min_value, max_value, size_in_bits);
}

void c_bitstream::read_point3d(const char* debug_string, long_point3d* point, int32 axis_encoding_size_in_bits)
{
	ASSERT(reading());
	ASSERT(axis_encoding_size_in_bits >= 0 && axis_encoding_size_in_bits <= SIZEOF_BITS(point->n[0]));

	DECLFUNC(0x00558C50, void, __thiscall, c_bitstream*, const char*, long_point3d*, int32)(this, debug_string, point, axis_encoding_size_in_bits);

	//for (int32 axis = 0; axis < 3; axis++)
	//{
	//	point->n[axis] = c_bitstream::read_dword_internal(axis_encoding_size_in_bits);
	//	ASSERT(point->n[axis] < left_shift_fast<int32>(1, axis_encoding_size_in_bits));
	//}
}

real32 c_bitstream::read_quantized_real(const char* debug_string, real32 min_value, real32 max_value, int32 size_in_bits, bool exact_midpoint, bool exact_endpoints)
{
	ASSERT(reading());

	return DECLFUNC(0x00559080, real32, __thiscall, c_bitstream*, const char*, real32, real32, int32, bool, bool)(this, debug_string, min_value, max_value, size_in_bits, exact_midpoint, exact_endpoints);
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

void c_bitstream::read_secure_address(const char* debug_string, s_transport_secure_address* address)
{
	//DECLFUNC(0x00559360, void, __thiscall, c_bitstream*, const char*, s_transport_secure_address*)(this, debug_string, address);

	ASSERT(reading());
	ASSERT(address);

	c_bitstream::read_bits_internal(address->data, SIZEOF_BITS(s_transport_secure_address));
}

void c_bitstream::read_string(const char* debug_string, char* string, int32 max_string_size)
{
	ASSERT(reading());
	ASSERT(string);
	ASSERT(max_string_size > 0);

	DECLFUNC(0x00559380, void, __thiscall, c_bitstream*, const char*, char*, int32)(this, debug_string, string, max_string_size);
}

void c_bitstream::read_string_utf8(const char* debug_string, char* char_string, int32 max_string_size)
{
	ASSERT(reading());
	ASSERT(char_string);
	ASSERT(max_string_size > 0);

	DECLFUNC(0x00559670, void, __thiscall, c_bitstream*, const char*, utf8*, int32)(this, debug_string, char_string, max_string_size);
}

void c_bitstream::read_string_wchar(const char* debug_string, wchar_t* string, int32 max_string_size)
{
	ASSERT(reading());
	ASSERT(string);
	ASSERT(max_string_size > 0);

	DECLFUNC(0x00559880, void, __thiscall, c_bitstream*, const char*, wchar_t*, int32)(this, debug_string, string, max_string_size);
}

void c_bitstream::read_unit_vector(const char* debug_string, real_vector3d* value, int32 size_in_bits)
{
	DECLFUNC(0x00559A90, void, __thiscall, c_bitstream*, const char*, real_vector3d*, int32)(this, debug_string, value, size_in_bits);

	//int32 unit_vector_quantization = c_bitstream::read_integer(debug_string, size_in_bits);
	//dequantize_unit_vector3d(unit_vector_quantization, value, size_in_bits);
}

void c_bitstream::read_vector(const char* debug_string, real_vector3d* value, real32 min_magnitude, real32 max_magnitude, int32 magnitude_size_in_bits, int32 size_in_bits)
{
	DECLFUNC(0x00559AB0, void, __thiscall, c_bitstream*, const char*, real_vector3d*, real32, real32, int32, int32)(this, debug_string, value, min_magnitude, max_magnitude, magnitude_size_in_bits, size_in_bits);
}

bool c_bitstream::reading() const
{
	//return DECLFUNC(0x00559BC0, bool, __thiscall, const c_bitstream*)(this);

	return IN_RANGE_INCLUSIVE(m_state, _bitstream_state_read, _bitstream_state_read_consistency_check);
}

void c_bitstream::reset(int32 state)
{
	DECLFUNC(0x00559BE0, void, __thiscall, c_bitstream*, int32)(this, state);

	//ASSERT(state >= 0 && state < k_bitstream_state_count);
	//
	//m_state = state;
	//
	//m_bitstream_data.current_memory_bit_position = 0;
	//m_bitstream_data.current_stream_bit_position = 0;
	//m_bitstream_data.next_data = m_data;
	//m_bitstream_data.accumulator = 0;
	//m_bitstream_data.accumulator_bit_count = 0;
	//
	//m_position_stack_depth = 0;
	//m_data_error_detected = false;
	//
	//if (c_bitstream::writing())
	//{
	//	m_number_of_bits_rewound = 0;
	//	m_number_of_position_resets = 0;
	//}
	//else if (c_bitstream::reading())
	//{
	//	uns64 value = c_bitstream::decode_qword_from_memory();
	//	m_bitstream_data.accumulator = value;
	//
	////#if PROFILE?
	////	if ((uns32)(value >> LONG_BITS) == 'debg')
	////	{
	////		c_bitstream::read_dword_internal(LONG_BITS);
	////		event(_event_warning, "c_bitstream::reset - release version cannot read debug bitstreams");
	////
	////		m_data_error_detected = true;
	////	}
	////#endif
	//}
}

void c_bitstream::set_data(byte* data, int32 data_length)
{
	//DECLFUNC(0x00559D90, void, __thiscall, c_bitstream*, byte*, int32)(this, data, data_length);

	m_data = data;
	m_data_max = &data[data_length];
	m_data_size_bytes = data_length;
	reset(_bitstream_state_none);
}

//.text:00559DB0 ; 

void c_bitstream::skip(int32 bits_to_skip)
{
	DECLFUNC(0x00559DC0, void, __thiscall, c_bitstream*, int32)(this, bits_to_skip);
}

bool c_bitstream::would_overflow(int32 size_in_bits) const
{
	return DECLFUNC(0x00559E90, bool, __thiscall, const c_bitstream*, int32)(this, size_in_bits);

	//ASSERT(writing());
	//return size_in_bits + m_bitstream_data.current_stream_bit_position > CHAR_BITS * m_data_size_bytes;
}

void c_bitstream::write_accumulator_to_memory(uns64 value, int32 size_in_bits)
{
	DECLFUNC(0x00559EB0, void, __thiscall, c_bitstream*, uns64, int32)(this, value, size_in_bits);

	//int32 shift_bits = QWORD_BITS - m_bitstream_data.accumulator_bit_count;
	//int32 accumulator_bit_count = size_in_bits - shift_bits;
	//
	//m_bitstream_data.current_stream_bit_position += size_in_bits;
	//m_bitstream_data.accumulator = value;
	//m_bitstream_data.accumulator_bit_count = accumulator_bit_count;
	//
	//uns64 accumulator = value;
	//if (accumulator_bit_count < QWORD_BITS)
	//{
	//	uns64 shifted_accumulator = left_shift_fast<uns64>(m_bitstream_data.accumulator, shift_bits);
	//	accumulator = shifted_accumulator | (value >> accumulator_bit_count);
	//}
	//
	//c_bitstream::encode_qword_to_memory(accumulator, QWORD_BITS);
}

void c_bitstream::write_bits_internal(const byte* data, int32 size_in_bits)
{
	DECLFUNC(0x0055A000, void, __thiscall, c_bitstream*, const byte*, int32)(this, data, size_in_bits);

	//ASSERT(writing());
	//
	//int32 size_in_qwords = size_in_bits / QWORD_BITS;
	//for (int32 qword_index = 0; qword_index < size_in_qwords; qword_index++)
	//{
	//	uns64 chunk = *(const uns64*)data;
	//	chunk = bswap_uns64(chunk);
	//	c_bitstream::write_qword_internal(chunk, QWORD_BITS);
	//	data += QWORD_BYTES;
	//}
	//
	//int32 remaining_bits = size_in_bits % QWORD_BITS;
	//if (remaining_bits > 0)
	//{
	//	uns64 value = 0;
	//
	//	int32 size_in_bytes = remaining_bits / CHAR_BITS;
	//	for (int32 byte_index = 0; byte_index < size_in_bytes; byte_index++)
	//	{
	//		value = (value << CHAR_BITS) | *data++;
	//	}
	//
	//	int32 bit_remainder = remaining_bits % CHAR_BITS;
	//	if (bit_remainder > 0)
	//	{
	//		value = left_shift_fast<uns64>(value, bit_remainder);
	//		uns8 partial = (*data >> (CHAR_BITS - bit_remainder)) & MASK(bit_remainder);
	//		value |= partial;
	//	}
	//
	//	c_bitstream::write_qword_internal(value, remaining_bits);
	//}
}

void c_bitstream::write_identifier(const char* identifier)
{
	ASSERT(writing());

	//DECLFUNC(0x0055A100, void, __thiscall, const c_bitstream*, const char*)(this, identifier);
}

void c_bitstream::write_point3d(const char* debug_string, const long_point3d* point, int32 axis_encoding_size_in_bits)
{
	DECLFUNC(0x0055A1E0, void, __thiscall, c_bitstream*, const char*, const long_point3d*, int32)(this, debug_string, point, axis_encoding_size_in_bits);

	//ASSERT(axis_encoding_size_in_bits >= 0 && axis_encoding_size_in_bits <= SIZEOF_BITS(point->n[0]));
	//
	//for (int32 i = 0; i < 3; ++i)
	//{
	//	if (axis_encoding_size_in_bits < LONG_BITS)
	//	{
	//		ASSERT(point->n[i] < FLAG(axis_encoding_size_in_bits));
	//	}
	//
	//	write_dword_internal((uns32)point->n[i], axis_encoding_size_in_bits);
	//	//m_potential_bit_position += axis_encoding_size_in_bits;
	//}
}

void c_bitstream::write_point3d_efficient(const char* debug_string, const long_point3d* point1, const long_point3d* point2)
{
	DECLFUNC(0x0055A240, void, __thiscall, c_bitstream*, const char*, const long_point3d*, const long_point3d*)(this, debug_string, point1, point2);
}

void c_bitstream::write_quantized_real(const char* debug_string, real32* value, real32 min_value, real32 max_value, int32 size_in_bits, bool exact_midpoint, bool exact_endpoints)
{
	ASSERT(writing());

	DECLFUNC(0x0055A2B0, void, __thiscall, c_bitstream*, const char*, real32*, real32, real32, int32, bool, bool)(this, debug_string, value, min_value, max_value, size_in_bits, exact_midpoint, exact_endpoints);
}

void c_bitstream::write_qword_internal(uns64 value, int32 size_in_bits)
{
	DECLFUNC(0x0055A3A0, void, __thiscall, c_bitstream*, uns64, int32)(this, value, size_in_bits);

	//ASSERT(writing());
	//ASSERT(size_in_bits <= QWORD_BITS);
	//
	//int32 accumulator_bit_count = m_bitstream_data.accumulator_bit_count;
	//if (size_in_bits > QWORD_BITS - accumulator_bit_count)
	//{
	//	c_bitstream::write_accumulator_to_memory(value, size_in_bits);
	//}
	//else
	//{
	//	m_bitstream_data.current_stream_bit_position += size_in_bits;
	//	uns64 shifted = (size_in_bits >= QWORD_BITS) ? 0 : left_shift_fast<uns64>(m_bitstream_data.accumulator, size_in_bits);
	//	m_bitstream_data.accumulator = shifted | value;
	//	m_bitstream_data.accumulator_bit_count = accumulator_bit_count + size_in_bits;
	//}
}

void c_bitstream::write_secure_address(const char* debug_string, const s_transport_secure_address* address)
{
	//DECLFUNC(0x0055A410, void, __thiscall, c_bitstream*, const char*, const s_transport_secure_address*)(this, debug_string, address);

	ASSERT(writing());
	ASSERT(address);
	
	c_bitstream::write_bits_internal(address->data, SIZEOF_BITS(s_transport_secure_address));
}

void c_bitstream::write_string(const char* debug_string, const char* string, int32 max_string_size)
{
	ASSERT(writing());
	ASSERT(string);
	ASSERT(max_string_size > 0);
	//ASSERT(strnlen(string, max_string_size) < max_string_size);

	DECLFUNC(0x0055A430, void, __thiscall, c_bitstream*, const char*, const char*, int32)(this, debug_string, string, max_string_size);
}

void c_bitstream::write_string_id(const char* debug_string, int32 size_in_bits)
{
	DECLFUNC(0x0055A600, void, __thiscall, c_bitstream*, const char*, int32)(this, debug_string, size_in_bits);
}

void c_bitstream::write_string_utf8(const char* debug_string, const utf8* char_string, int32 max_string_size)
{
	ASSERT(writing());
	ASSERT(char_string);
	ASSERT(max_string_size > 0);
	//ASSERT(strnlen(char_string, max_string_size) < max_string_size);

	DECLFUNC(0x0055A650, void, __thiscall, c_bitstream*, const char*, const utf8*, int32)(this, debug_string, char_string, max_string_size);
}

void c_bitstream::write_string_wchar(const char* debug_string, const wchar_t* string, int32 max_string_size)
{
	ASSERT(writing());
	ASSERT(string);
	ASSERT(max_string_size > 0);

	DECLFUNC(0x0055A6D0, void, __thiscall, c_bitstream*, const char*, const wchar_t*, int32)(this, debug_string, string, max_string_size);
}

void c_bitstream::write_unit_vector(const char* debug_string, const real_vector3d* value, int32 size_in_bits)
{
	DECLFUNC(0x0055A750, void, __thiscall, c_bitstream*, const char*, const real_vector3d*, int32)(this, debug_string, value, size_in_bits);

	// Halo 3
	//uns32 quantize_unit_vector = quantize_unit_vector3d(value);
	//c_bitstream::write_integer("unit-vector", quantize_unit_vector, 19);

	//uns32 quantize_unit_vector = quantize_unit_vector3d(value);
	//c_bitstream::write_integer("unit-vector", quantize_unit_vector, size_in_bits);
}

void c_bitstream::write_vector(const char* debug_string, const real_vector3d* vector, real32 min_value, real32 max_value, int32 step_count_size_in_bits, int32 size_in_bits)
{
	DECLFUNC(0x0055A7B0, void, __thiscall, c_bitstream*, const char*, const real_vector3d*, real32, real32, int32, int32)(this, debug_string, vector, min_value, max_value, step_count_size_in_bits, size_in_bits);
}

bool c_bitstream::writing() const
{
	//return DECLFUNC(0x0055A9D0, bool, __thiscall, const c_bitstream*)(this);

	return m_state == _bitstream_state_write;
}

