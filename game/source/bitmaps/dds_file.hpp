#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/files.hpp"

struct s_dds_pixel_format
{
	dword size;
	dword flags;
	dword signature;
	dword rgb_bit_count;
	dword r_bit_mask;
	dword g_bit_mask;
	dword b_bit_mask;
	dword a_bit_mask;
};
static_assert(sizeof(s_dds_pixel_format) == 0x20);

struct s_dds_header
{
	dword size;
	dword flags;
	dword height;
	dword width;
	union
	{
		dword pitch;
		dword linear_size;
	};
	dword depth;
	dword mip_map_count;
	dword reserved1[11];
	s_dds_pixel_format dds_pixel_format;
	dword caps[4];
	dword reserved2;
};
static_assert(sizeof(s_dds_header) == 0x7C);

struct s_dds_file
{
	dword header_signature;
	s_dds_header header;

#pragma warning(push)
#pragma warning(disable : 4200)
	byte data[];
#pragma warning(pop)
};
static_assert(sizeof(s_dds_file) == 0x80);

class c_dds_file
{
public:
	c_dds_file(char const* filename) :
		m_file(),
		m_buffer()
	{
		file_reference_create_from_path(&m_file, filename, false);

		if (!file_exists(&m_file))
			return;

		dword file_size = 0;
		if (!file_get_size(&m_file, &file_size))
			return;

		m_buffer = new byte[file_size]{};
		if (!file_read_into_buffer(&m_file, m_buffer, file_size))
			return;

		ASSERT(m_buffer != nullptr);
	}

	~c_dds_file()
	{
		delete[] m_buffer;
		m_buffer = nullptr;
	}

	s_dds_file* get()
	{
		return static_cast<s_dds_file*>(m_buffer);
	}


protected:
	s_file_reference m_file;
	void* m_buffer;
};

