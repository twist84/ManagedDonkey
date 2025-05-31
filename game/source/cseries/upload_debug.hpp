#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files.hpp"

struct c_file_reference :
	s_file_reference
{
public:
	c_file_reference(const char* path, uns32 flags);
	c_file_reference(const s_file_reference* file, uns32 flags);
	~c_file_reference();

	bool find_or_create(uns32 flags);

protected:
	bool m_is_open;
};

struct _g_upload_debug_globals
{
	c_synchronized_long archive_upload_in_progress;
	c_synchronized_long archive_upload_complete;
	c_synchronized_long archive_upload_success;
	c_synchronized_long current_count;
	c_synchronized_long total_count;
};

extern bool g_suppress_upload_debug;
extern _g_upload_debug_globals g_upload_debug_globals;
extern const char* const k_crash_file_archive;

extern bool __cdecl upload_debug_start();
extern bool upload_debug_complete(bool* out_success);
extern bool __cdecl upload_debug_get_output(char* output, int32 output_size);
extern bool __cdecl upload_debug_create_archive();
extern bool __cdecl upload_debug_create_fake_archive();

extern void __cdecl upload_debug_update_callback(int32 current_count, int32 total_count);
extern void __cdecl upload_debug_completion_callback(bool success, void* discard);
extern void __cdecl create_and_upload_zip_archive();

