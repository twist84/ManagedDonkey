#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/files_windows.hpp"

struct s_file_reference
{
	dword signture;
	word_flags flags;
	short location;
	long_string path;
	s_file_handle handle;
	long position;
};
static_assert(sizeof(s_file_reference) == 0x110);