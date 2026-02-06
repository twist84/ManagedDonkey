#include "upload_debug.hpp"

#include "cseries/progress.hpp"
#include "main/main.hpp"
#include "networking/tools/network_debug_dump.hpp"
#include "cseries_events.hpp"

#include <winsock.h>

bool g_suppress_upload_debug = false;
_g_upload_debug_globals g_upload_debug_globals{};

const char* const k_crash_file_archive = "crash_report\\crash_file_archive.zip";

c_file_reference::c_file_reference(const char* path, uns32 flags)
{
	ASSERT(path);

	file_reference_create_from_path(this, path, false);

	uns32 error = 0;
	m_is_open = find_or_create(flags) && file_open(this, flags, &error);
}

c_file_reference::c_file_reference(const s_file_reference* file, uns32 flags)
{
	file_reference_create(this, NONE);
	file_reference_copy(this, file);

	uns32 error = 0;
	m_is_open = find_or_create(flags) && file_open(this, flags, &error);
}

c_file_reference::~c_file_reference()
{
	if (m_is_open)
	{
		file_close(this);
		m_is_open = false;
	}
}

bool c_file_reference::find_or_create(uns32 flags)
{
	if (file_exists(this))
		return true;

	if (TEST_BIT(flags, 1))
	{
		file_create_parent_directories_if_not_present(this);
		return file_create(this);
	}

	return false;
}

bool __cdecl upload_debug_start()
{
	g_upload_debug_globals.archive_upload_complete = false;
	g_upload_debug_globals.archive_upload_in_progress = false;
	g_upload_debug_globals.archive_upload_success = false;

	if (!g_suppress_upload_debug)
	{
		create_session_description();
		create_and_upload_zip_archive();
	}

	return g_upload_debug_globals.archive_upload_in_progress.peek() != FALSE;
}

bool upload_debug_complete(bool* out_success)
{
	if (out_success)
	{
		bool success = false;
		if (g_upload_debug_globals.archive_upload_complete)
			success = g_upload_debug_globals.archive_upload_success;

		*out_success = success;
	}

	return !g_upload_debug_globals.archive_upload_in_progress && g_upload_debug_globals.archive_upload_complete == TRUE;// && !data_mine_uploading_files();
}

bool __cdecl upload_debug_get_output(char* buffer, int32 buffer_length)
{
	if (buffer && buffer_length)
	{
		*buffer = 0;

 		if (upload_debug_complete(nullptr))
		{
			if (g_upload_debug_globals.archive_upload_success)
				csnzappendf(buffer, buffer_length, "\r\nFile upload to server complete.  (Safe to reboot)");
			else
				csnzappendf(buffer, buffer_length, "\r\nFile upload to server failed.  (Get an engineer!)");
		}
		//else if (data_mine_uploading_files())
		//{
		//	csnzappendf(buffer, buffer_length, "\r\nUploading data mine files, please wait...");
		//}
		else
		{
			int32 upload_position = g_upload_debug_globals.current_count;
			int32 total_count = g_upload_debug_globals.total_count;
			real32 upload_progress = total_count > 0 ? 100.0f * (real32(upload_position) / real32(total_count)) : 0.0f;
			csnzappendf(buffer, buffer_length, "\r\nUploading files to server, please wait... %i %%", int32(upload_progress));
		}
	}

	return true;
}

bool __cdecl upload_debug_create_archive()
{
	return false;
}

bool __cdecl upload_debug_create_fake_archive()
{
	s_file_reference file{};
	file_reference_create_from_path(&file, k_crash_file_archive, false);
	if (file_exists(&file))
		file_delete(&file);

	c_file_reference file_reference(k_crash_file_archive, FLAG(1));

	byte fake_contents[0x1000];
	csmemset(fake_contents, 0xFE, sizeof(fake_contents));

	bool fake_contents_written = true;

	for (int32 i = 0; i < 2560 && fake_contents_written; i++)
		fake_contents_written &= file_write(&file_reference, sizeof(fake_contents), fake_contents);

	return fake_contents_written;
}

void __cdecl upload_debug_update_callback(int32 current_count, int32 total_count)
{
	g_upload_debug_globals.current_count = current_count;
	g_upload_debug_globals.total_count = total_count;
}

void __cdecl upload_debug_completion_callback(bool success, void* discard)
{
	g_upload_debug_globals.archive_upload_in_progress = false;
	g_upload_debug_globals.archive_upload_complete = true;
	g_upload_debug_globals.archive_upload_success = success;

	progress_done();

	//if (succeeded)
	//	telnet_console_print("### crash upload succeeded");
	//else
	//	telnet_console_print("### crash upload failed");
	//telnet_console_print("### crash upload completed");
}

void __cdecl create_and_upload_zip_archive()
{
	bool archive_created = false;
	if (g_fake_minidump_creation)
		archive_created = upload_debug_create_fake_archive();
	else
		archive_created = upload_debug_create_archive();

	if (archive_created)
	{
		s_file_reference file{};
		file_reference_create_from_path(&file, k_crash_file_archive, false);

		uns32 size = 0;
		file_get_size(&file, &size);
		ASSERT(size > 0);

		g_upload_debug_globals.archive_upload_in_progress = true;
		g_upload_debug_globals.current_count.set(0);
		g_upload_debug_globals.total_count.set(100);

		progress_new("uploading crash files");
		progress_update(0, 1);

		netdebug_upload_file(nullptr, k_crash_file_archive, upload_debug_update_callback, upload_debug_completion_callback, nullptr);
	}
	else
	{
		upload_debug_completion_callback(false, nullptr);
	}
}

