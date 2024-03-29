#include "upload_debug.hpp"

#include "cseries/progress.hpp"
#include "main/main.hpp"
#include "networking/tools/network_debug_dump.hpp"
#include "cseries_events.hpp"

bool g_suppress_upload_debug = false;
_g_upload_debug_globals g_upload_debug_globals{};

char const* const k_crash_file_archive = "crash_report\\crash_file_archive.zip";

c_file_reference::c_file_reference(char const* path, dword flags)
{
	ASSERT(path);

	file_reference_create_from_path(this, path, false);

	dword error = 0;
	m_file_open = find_or_create(flags) && file_open(this, flags, &error);
}

c_file_reference::c_file_reference(s_file_reference const* file, dword flags)
{
	file_reference_create(this, NONE);
	file_reference_copy(this, file);

	dword error = 0;
	m_file_open = find_or_create(flags) && file_open(this, flags, &error);
}

c_file_reference::~c_file_reference()
{
	if (m_file_open)
	{
		file_close(this);
		m_file_open = false;
	}
}

bool c_file_reference::find_or_create(dword flags)
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
	g_upload_debug_globals.upload_completed = FALSE;
	g_upload_debug_globals.upload_started = FALSE;
	g_upload_debug_globals.upload_succeeded = FALSE;

	if (!g_suppress_upload_debug)
	{
		create_session_description();

		bool archive_created = false;
		if (g_fake_minidump_creation)
			archive_created = upload_debug_create_fake_archive();
		else
			archive_created = upload_debug_create_archive();

		if (archive_created)
		{
			s_file_reference file{};
			file_reference_create_from_path(&file, k_crash_file_archive, false);

			dword size = 0;
			file_get_size(&file, &size);
			ASSERT(size > 0);

			g_upload_debug_globals.upload_started = TRUE;
			g_upload_debug_globals.upload_position.set(0);
			g_upload_debug_globals.upload_length.set(100);

			progress_new("uploading crash files");
			progress_update(0, 1);

			netdebug_upload_file(NULL, k_crash_file_archive, upload_debug_update_callback, upload_debug_completion_callback, NULL);
		}
		else
		{
			upload_debug_completion_callback(false, NULL);
		}
	}

	return g_upload_debug_globals.upload_started.peek() != FALSE;
}

bool upload_debug_complete(bool* out_success)
{
	if (out_success)
	{
		bool success = false;
		if (g_upload_debug_globals.upload_completed)
			success = g_upload_debug_globals.upload_succeeded;

		*out_success = success;
	}

	return !g_upload_debug_globals.upload_started && g_upload_debug_globals.upload_completed == TRUE;// && !data_mine_uploading_files();
}

bool __cdecl upload_debug_get_output(char* output, long output_size)
{
	if (output && output_size)
	{
		*output = 0;

 		if (upload_debug_complete(NULL))
		{
			if (g_upload_debug_globals.upload_succeeded)
				csnzappendf(output, output_size, "\r\nFile upload to server complete.  (Safe to reboot)");
			else
				csnzappendf(output, output_size, "\r\nFile upload to server failed.  (Get an engineer!)");
		}
		//else if (data_mine_uploading_files())
		//{
		//	csnzappendf(output, output_size, "\r\nUploading data mine files, please wait...");
		//}
		else
		{
			long upload_position = g_upload_debug_globals.upload_position;
			long upload_length = g_upload_debug_globals.upload_length;
			real upload_progress = upload_length > 0 ? 100.0f * (real(upload_position) / real(upload_length)) : 0.0f;
			csnzappendf(output, output_size, "\r\nUploading files to server, please wait... %i %%", long(upload_progress));
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

	for (long i = 0; i < 2560 && fake_contents_written; i++)
		fake_contents_written &= file_write(&file_reference, sizeof(fake_contents), fake_contents);

	return fake_contents_written;
}

void __cdecl upload_debug_update_callback(long upload_position, long upload_length)
{
	g_upload_debug_globals.upload_position = upload_position;
	g_upload_debug_globals.upload_length = upload_length;
}

void __cdecl upload_debug_completion_callback(bool succeeded, void* data)
{
	g_upload_debug_globals.upload_started = FALSE;
	g_upload_debug_globals.upload_completed = TRUE;
	g_upload_debug_globals.upload_succeeded = succeeded;

	progress_done();

	//if (succeeded)
	//	telnet_console_print("### crash upload succeeded");
	//else
	//	telnet_console_print("### crash upload failed");
	//telnet_console_print("### crash upload completed");
}

