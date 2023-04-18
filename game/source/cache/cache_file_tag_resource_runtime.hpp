#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files_windows.hpp"

extern void patch_lz_cache_file_decompressor();

struct s_cache_file_shared_resource_usage
{
	byte __data[0x2328];
};
static_assert(sizeof(s_cache_file_shared_resource_usage) == 0x2328);

struct s_cache_file_insertion_point_resource_usage
{
	byte __data[0xB4];
};
static_assert(sizeof(s_cache_file_insertion_point_resource_usage) == 0xB4);

struct c_asynchronous_io_arena
{
	struct s_async_io_file
	{
		s_file_handle file;
		long outstanding_io_count;
		dword __unknown8;
	};

	s_file_handle m_io_completion_port;
	c_static_flags<32> m_valid_files;
	c_static_array<s_async_io_file, 32> m_files;
	long m_outstanding_io_requests;
	long __unknown18C;

	byte __data190[4];
	long __unknown194;
	long __unknown198;
	byte __data19C[24];
	long __unknown1B4;
};
static_assert(sizeof(c_asynchronous_io_arena) == 0x1B8);

struct c_cache_file_decompressor
{
public:
	virtual bool begin(c_basic_buffer<void> a1);
	virtual bool decompress_buffer(c_basic_buffer<void> a1, c_basic_buffer<void>* a2);
	virtual bool finish(c_basic_buffer<void>* a1);
};
static_assert(sizeof(c_cache_file_decompressor) == 0x4);

struct c_cache_file_uncompressed_decompressor :
	public c_cache_file_decompressor
{
	long __size;
	c_basic_buffer<void> __buffer;
};
static_assert(sizeof(c_cache_file_uncompressed_decompressor) == sizeof(c_cache_file_decompressor) + 0xC);

struct c_cache_file_streamed_sublocation_decompressor :
	public c_cache_file_decompressor
{
	struct // s_cache_file_resource_streaming_sublocation_table?
	{
		long __unknown0;
		long __unknown4;

		struct
		{
			long __unknown0;
			long __unknown4;
		}*__unknown8;
		static_assert(sizeof(*__unknown8) == 0x8);

	}*__unknown4;
	static_assert(sizeof(*__unknown4) == 0xC);

	c_basic_buffer<void> __buffer8;
	char* __unknown10;
	c_basic_buffer<void> __buffer14;
};
static_assert(sizeof(c_cache_file_streamed_sublocation_decompressor) == sizeof(c_cache_file_decompressor) + 0x18);

// #TODO: find a home
struct s_simple_read_file_ex_overlapped_result
{
	c_synchronized_long __unknown0;
	dword __unknown4;
	dword offset;
	dword size;
};
static_assert(sizeof(s_simple_read_file_ex_overlapped_result) == 0x10);

struct c_cache_file_copy_fake_decompressor :
	public c_cache_file_decompressor
{
	s_file_handle m_file_handle;
	c_synchronized_long m_file_offset;
	c_synchronized_long m_done;
	dword m_checksum;
	bool __unknown14;
	s_simple_read_file_ex_overlapped_result m_overlapped_result;
	byte m_overlapped[0x14]; // OVERLAPPED
	void* m_buffer;

	// are these bytes actually here?
	byte __data[0x8];
};
static_assert(sizeof(c_cache_file_copy_fake_decompressor) == sizeof(c_cache_file_decompressor) + 0x44);

struct c_xor_cache_file_decompressor :
	public c_cache_file_decompressor
{
	c_basic_buffer<void> __buffer0;
	c_basic_buffer<void> __buffer8;
	byte __unknownC; // xor value, *i++ ^= __unknownC
};
static_assert(sizeof(c_xor_cache_file_decompressor) == sizeof(c_cache_file_decompressor) + 0x14);

struct c_lz_cache_file_decompressor :
	public c_cache_file_decompressor
{
	c_basic_buffer<void> __buffer4;
	c_basic_buffer<void> __bufferC;
	bool __unknown14;
	c_basic_buffer<void> __buffer18;
};
static_assert(sizeof(c_lz_cache_file_decompressor) == sizeof(c_cache_file_decompressor) + 0x1C);

extern c_asynchronous_io_arena& g_cache_file_io_arena;

extern bool __fastcall lz_cache_file_decompressor_begin(c_lz_cache_file_decompressor* _this, void* unused, c_basic_buffer<void> a1);
extern bool __fastcall lz_cache_file_decompressor_decompress_buffer(c_lz_cache_file_decompressor* _this, void* unused, c_basic_buffer<void> a1, c_basic_buffer<void>* a2);
extern bool __fastcall lz_cache_file_decompressor_finish(c_lz_cache_file_decompressor* _this, void* unused, c_basic_buffer<void>* a1);

struct s_tag_resource;
extern void* __cdecl tag_resource_get(s_tag_resource const* resource);

