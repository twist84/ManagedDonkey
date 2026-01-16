#pragma once

#define STRINGIFY_DETAIL(x) #x
#define STRINGIFY(x) STRINGIFY_DETAIL(x)

// $TODO global find and replace the following? `\/\/ \$TODO ([^\r\n]+)` `$TODO("$1")`
#define $TODO(msg) __pragma(message(__FILE__ "(" STRINGIFY(__LINE__) "): TODO: " msg)) \

#define XCONCAT(x, y) x ## y
#define CONCAT(x, y) XCONCAT(x, y)

#define IN_RANGE(value, begin, end) ((value) > (begin) && (value) < (end))
#define IN_RANGE_INCLUSIVE(value, begin, end) ((value) >= (begin) && (value) <= (end))
#define VALID_INDEX(index, count) ((index) >= 0 && (index) < (count))
#define VALID_COUNT(index, count) ((index) >= 0 && (index) <= (count))

// referenced
#define BIT_VECTOR_SIZE_IN_LONGS(BIT_COUNT) (((BIT_COUNT) + (LONG_BITS - 1)) >> 5)
#define BIT_VECTOR_SIZE_IN_BYTES(BIT_COUNT) (4 * BIT_VECTOR_SIZE_IN_LONGS(BIT_COUNT))
#define BIT_VECTOR_TEST_FLAG(BIT_VECTOR, BIT) ((BIT_VECTOR[BIT >> 5] & (1 << (BIT & (LONG_BITS - 1)))) != 0)

// not referenced
#define BIT_VECTOR_OR_FLAG(BIT_VECTOR, BIT) (BIT_VECTOR[BIT >> 5] |= (1 << (BIT & (LONG_BITS - 1))))
#define BIT_VECTOR_AND_FLAG(BIT_VECTOR, BIT) (BIT_VECTOR[BIT >> 5] &= ~(1 << (BIT & (LONG_BITS - 1))))
#define BIT_VECTOR_SET_FLAG(BIT_VECTOR, BIT, ENABLE) (BIT_VECTOR[BIT >> 5] = (ENABLE) ? BIT_VECTOR[BIT >> 5] | (1 << (BIT & (LONG_BITS - 1))) : BIT_VECTOR[BIT >> 5] & ~(1 << (BIT & (LONG_BITS - 1))))

#define FLOOR(a, b) ((a) <= (b) ? (b) : (a))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define PIN(a, b, c) MIN(MAX((a), (b)), (c))
#define CLAMP(a, b, c) (((a) < (b)) ? (b) : ((a) > (c)) ? (c) : (a))
#define CLAMP_LOWER(a, b, c) (((a) >= ((c) - (b))) ? ((a) - (c)) : (b))
#define CLAMP_UPPER(a, b, c) (((a) <= ((c) - (b))) ? ((a) + (b)) : (c))
#define SCALE_VALUE(a, a_min, a_max, b_min, b_max) ((b_min) + ((a) - (a_min)) * ((b_max) - (b_min)) / ((a_max) - (a_min)))

#define SIZEOF_BITS(value) 8 * sizeof(value)

const int32 CHAR_BYTES = sizeof(char);
const int32 SHORT_BYTES = sizeof(int16);
const int32 LONG_BYTES = sizeof(int32);
const int32 QWORD_BYTES = sizeof(uns64);
const int32 CHAR_BITS = SIZEOF_BITS(char);
const int32 SHORT_BITS = SIZEOF_BITS(int16);
const int32 LONG_BITS = SIZEOF_BITS(int32);
const int32 QWORD_BITS = SIZEOF_BITS(uns64);

#define FLAG(bit) ((unsigned)1 << (unsigned)(bit))
#define FLAG_64(bit) (1ULL << (unsigned)(bit))
#define RANGE(bit) (FLAG((bit) - 1))
#define MASK(bit) ((RANGE((bit))) | ((bit) <= 1 ? 0 : ((RANGE((bit)) - 1))))
#define TEST_BIT(flags, bit) (((flags) & FLAG((bit))) != 0)
#define TEST_RANGE(flags, start_bit, end_bit) (((flags) & (((1 << ((end_bit) - (start_bit) + 1)) - 1) << (start_bit))) != 0)
#define TEST_FLAG(flags, bit) (flags.test((bit)))
#define TEST_MASK(flags, mask) (((flags) & (mask)) != 0)
#define ALIGN(value, bit) (((value) & ~(FLAG((bit)) - 1)) + FLAG((bit)))
#define ALIGN_UP(value, bit) ((((value) & (FLAG((bit)) - 1)) == 0) ? (value) : ((value) | ((1 << (bit)) - 1)) + 1)
#define SET_BIT(flags, bit, enable) ((flags) = (enable) ? (flags) | FLAG((bit)) : (flags) & ~FLAG((bit)))
#define SET_MASK(flags, mask, enable) ((flags) = (enable) ? (flags) | (mask) : (flags) | ~(mask))
#define VALID_BITS(flags, max_bits) (((flags) & ~RANGE((max_bits))) != 0)

#define INVALID_ASYNC_TASK_ID (-1)

//#define CHAR_MAX char(0x7F)
#define UNSIGNED_CHAR_MAX ((uns8)-1)
static_assert(UNSIGNED_CHAR_MAX == 0xFF);

//#define SHORT_MAX int16(0x7FFF)
#define UNSIGNED_SHORT_MAX ((uns16)-1)
static_assert(UNSIGNED_SHORT_MAX == 0xFFFF);

//#define LONG_MAX int32(0x7FFFFFFF)
#define UNSIGNED_LONG_MAX ((uns32)-1)
static_assert(UNSIGNED_LONG_MAX == 0xFFFFFFFF);

#define VALID_CONTROLLER(CONTROLLER) ((CONTROLLER) >= _controller0 && (CONTROLLER) < k_number_of_controllers)

#define MAXIMUM_PLAYER_WINDOWS 4

constexpr uns64 string_hash(const char* str)
{
	const char* tmp = str;
	while (*tmp) tmp++;
	int32 len = tmp - str;

	uns64 hash = 0;
	for (int32 i = 0; i < len; i++) hash = (hash * 131) + str[i];
	return hash;
}

constexpr uns64 operator"" _hash(const char* str, size_t len)
{
	return string_hash(str);
}

constexpr size_t operator"" _len(const char* str, size_t len)
{
	return len;
}

