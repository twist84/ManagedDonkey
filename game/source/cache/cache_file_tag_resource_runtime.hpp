#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files_windows.hpp"

struct s_tag_resource
{
	dword resource_handle;
	dword definition_address;
};
static_assert(sizeof(s_tag_resource) == 0x8);

template<typename t_resource_type, dword ...t_extra>
struct c_typed_tag_resource : s_tag_resource
{
	t_resource_type* get()
	{
		return reinterpret_cast<t_resource_type*>(resource_handle);
	}
};
static_assert(sizeof(s_tag_resource) == sizeof(s_tag_resource));

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

extern void patch_lz_cache_file_decompressor();

extern bool __fastcall lz_cache_file_decompressor_begin(c_lz_cache_file_decompressor* _this, void* unused, c_basic_buffer<void> a1);
extern bool __fastcall lz_cache_file_decompressor_decompress_buffer(c_lz_cache_file_decompressor* _this, void* unused, c_basic_buffer<void> a1, c_basic_buffer<void>* a2);
extern bool __fastcall lz_cache_file_decompressor_finish(c_lz_cache_file_decompressor* _this, void* unused, c_basic_buffer<void>* a1);
//extern void* __cdecl tag_resource_get(s_tag_resource const* resource);

// eventually replace this
struct s_replacement_resource_info
{
	tag group_tag;
	long tag_index;
	byte const starting_data[16];
	char const* filename;
};

// eventually replace this
s_replacement_resource_info const k_resource_replacements[]
{
	{ 'bitm', 0x000009EC, { 0xFF, 0x00, 0x49, 0x92, 0x24, 0x49, 0x92, 0x24, 0xFF, 0xFF, 0x00, 0x00, 0x55, 0x15, 0x50, 0x00 }, "data\\bitmaps\\000009EC.dds" }
};

// a replacement for the current system would be something like
// having a specific directory that is iterated through with
// find_files looking for a `*.bitmap_resource` etc,
// and populating the array with the found file data
//
// the file data would contain the following header,
// each resource type would inherit the header
//
// the following may change
// when loading the data you would load the file header first
// once you've checked the data against an existing tag/resource
// you then load the resource itself

struct s_resource_file_header
{
	// tag info
	tag group_tag;
	long tag_index;

	// resource data size and offset from file begin
	dword size;
	dword offset;

	// resource comparison data
	byte starting_data[16];
};
static_assert(sizeof(s_resource_file_header) == 0x20);

//struct s_bitmap_resource_file : s_resource_file_header
//{
//	s_dds_file dds_file;
//};

