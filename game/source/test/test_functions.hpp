#pragma once

#include "tag_files/files.hpp"

struct s_test_download_storage
{
	int32 request_cookie;
	s_file_reference request_file;
};
COMPILE_ASSERT(sizeof(s_test_download_storage) == 0x114);

extern void test_download_storage_file(const char* url, const char* local_filename);
extern void test_download_storage_file_update();
extern void test_functions_update();

