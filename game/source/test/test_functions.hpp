#pragma once

#include "tag_files/files.hpp"

struct s_test_download_storage
{
	int32 request_cookie;
	s_file_reference request_file;
};
static_assert(sizeof(s_test_download_storage) == 0x114);

extern void test_download_storage_file(char const* url, char const* local_filename);
extern void test_download_storage_file_update();
extern void test_functions_update();

