#include "networking/tools/network_debug_dump.hpp"

#include "cseries/cseries_events.hpp"
#include "main/main.hpp"
#include "memory/crc.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"
#include "multithreading/threads.hpp"
#include "networking/online/online_lsp.hpp"
#include "networking/transport/transport.hpp"

REFERENCE_DECLARE(0x0199C0B0, s_netdebug_globals, g_netdebug_globals);
REFERENCE_DECLARE(0x0199FA20, c_synchronized_long, g_net_debug_thread_has_exited);

//HOOK_DECLARE(0x0042FD00, netdebug_get_next_task);
//HOOK_DECLARE(0x0042FF70, netdebug_process_file_upload);
//HOOK_DECLARE(0x004300F0, netdebug_process_next_task);
//HOOK_DECLARE(0x004301D0, netdebug_thread_function);
//HOOK_DECLARE(0x00430300, netdebug_upload_file);
//HOOK_DECLARE(0x00430380, remove_current_task);

char const* const k_sessionid_filename_pack_token = "#";
char const* const k_session_description_title_string = "title= ";
char const* const k_session_description_build_string = "build= ";
char const* const k_session_description_system_string = "system name= ";
char const* const k_netdebug_session_description_file_location = "session_description.txt";
char const* const k_debug_server_url = "/upload_server/upload.ashx";

void __cdecl add_http_xbox_upload_info_headers(c_http_post_stream* stream, struct s_netdebug_upload_task const* task)
{
	INVOKE(0x0042F360, add_http_xbox_upload_info_headers, stream, task);
}

void __cdecl build_sessionid_time_segment()
{
	INVOKE(0x0042F5A0, build_sessionid_time_segment);
}

void __cdecl compute_task_file_upload_signatures(s_netdebug_upload_task* task)
{
	INVOKE(0x0042F9B0, compute_task_file_upload_signatures, task);
}

void __cdecl create_sessionid()
{
	INVOKE(0x0042FA90, create_sessionid);
}

template<long k_filename_size>
char* file_reference_get_filename(s_file_reference const* file, char(&filename)[k_filename_size])
{
	constexpr dword_flags flags = FLAG(_name_file_bit) | FLAG(_name_extension_bit);
	return file_reference_get_name(file, flags, filename, k_filename_size);
}

void __cdecl fill_packed_sessionid_filename(char const* filename, c_static_string<256>* sessionid_filename)
{
	//INVOKE(0x0042FAF0, fill_packed_sessionid_filename, filename, sessionid_filename);

	ASSERT(filename);
	ASSERT(sessionid_filename);
	sessionid_filename->print("%s%s%s", g_netdebug_globals.sessionid.get_string(), k_sessionid_filename_pack_token, filename);
}

void __cdecl get_system_ip_addresses(c_static_string<16>* insecure_ip, c_static_string<128>* secure_ip)
{
	return INVOKE(0x0042FB90, get_system_ip_addresses, insecure_ip, secure_ip);
}

void __cdecl netdebug_dispose()
{
	INVOKE(0x0042FCB0, netdebug_dispose);
}

char const* __cdecl netdebug_get_build()
{
	return INVOKE(0x0042FCE0, netdebug_get_build);

	//if (g_netdebug_globals.initialized)
	//	return g_netdebug_globals.build.get_string();
	//return "UNINIT_BUILD";
}

void __cdecl netdebug_get_next_task()
{
	//INVOKE(0x0042FD00, netdebug_get_next_task);

	if (!g_netdebug_globals.current_task.active)
	{
		for (s_netdebug_upload_task& task : g_netdebug_globals.task_queue)
		{
			if (task.active)
			{
				csmemcpy(&g_netdebug_globals.current_task, &task, sizeof(s_netdebug_upload_task));
				g_netdebug_globals.task_fails = 0;
				memset(&task, 0, sizeof(s_netdebug_upload_task));
				task.active = false;
				break;
			}
		}
	
		if (g_netdebug_globals.current_task.active)
			compute_task_file_upload_signatures(&g_netdebug_globals.current_task);
	}
}

char const* __cdecl netdebug_get_sessionid()
{
	return INVOKE(0x0042FE40, netdebug_get_sessionid);

	//if (g_netdebug_globals.initialized)
	//	return g_netdebug_globals.sessionid.get_string();
	//return "UNINIT_SESSIONID";
}

char const* __cdecl netdebug_get_system()
{
	return INVOKE(0x0042FE60, netdebug_get_system);

	//if (g_netdebug_globals.initialized)
	//	return g_netdebug_globals.system.get_string();
	//return "UNINIT_SYSTEM";
}

char const* __cdecl netdebug_get_title()
{
	return INVOKE(0x0042FE80, netdebug_get_title);

	//if (g_netdebug_globals.initialized)
	//	return g_netdebug_globals.title.get_string();
	//return "UNINIT_TITLE";
}

bool __cdecl netdebug_initialize(char const* title_string, char const* build_identifier_string, char const* system_identifier_string)
{
	return INVOKE(0x0042FEA0, netdebug_initialize, title_string, build_identifier_string, system_identifier_string);

	//ASSERT(title_string != NULL);
	//ASSERT(build_identifier_string != NULL);
	//ASSERT(system_identifier_string != NULL);
	//g_netdebug_globals.title.set(title_string);
	//g_netdebug_globals.build.set(build_identifier_string);
	//g_netdebug_globals.system.set(system_identifier_string);
	//g_netdebug_globals.current_task.active = false;
	//netdebug_set_system_version();
	//netdebug_set_xtl_version();
	//create_sessionid();
	//main_status("session_id", "%s", g_netdebug_globals.sessionid);
	//start_thread(k_thread_net_debug);
	//g_netdebug_globals.initialized = true;
	//return true;
}

void __cdecl netdebug_prefer_internet(bool prefer_internet)
{
	INVOKE(0x0042FF60, netdebug_prefer_internet, prefer_internet);

	//g_netdebug_globals.prefer_internet = prefer_internet;
}

bool __cdecl netdebug_process_file_upload(s_netdebug_upload_task* task)
{
	//return INVOKE(0x0042FF70, netdebug_process_file_upload, task);

	ASSERT(task);

	dword error = 0;
	if (file_open(&task->file, FLAG(_file_open_flag_desired_access_read), &error))
	{
		c_http_post_source post_source{};
		char filename[256];
		file_reference_get_filename(&task->file, filename);
		c_static_string<256> sessionid_filename;
		fill_packed_sessionid_filename(filename, &sessionid_filename);
		add_http_xbox_upload_info_headers(&g_netdebug_globals.http_post_stream, task);
		post_source.set_source_as_file(&task->file);
		post_source.set_filename(sessionid_filename.get_string());
		post_source.set_content_type("application/x-halo3-upload");
		g_netdebug_globals.http_post_stream.set_source(&post_source);

		if (upload_synchronous(&g_netdebug_globals.http_client, &g_netdebug_globals.http_post_stream, 1800, task))
		{
			file_close(&task->file);
			return true;
		}
		else
		{
			GENERATE_EVENT(_event_warning, "netdebug_process_file_upload: upload failed.");
			file_close(&task->file);
		}
	}
	else
	{
		char filename[256];
		file_reference_get_filename(&task->file, filename);
		GENERATE_EVENT(_event_warning, "netdebug_process_file_upload() failed to open file '%s' for uploading", filename);
	}

	return false;
}

void __cdecl netdebug_process_next_task()
{
	//INVOKE(0x004300F0, netdebug_process_next_task);

	ASSERT(g_netdebug_globals.current_task.active);
	if (netdebug_process_file_upload(&g_netdebug_globals.current_task))
	{
		remove_current_task(true);
	}
	else if (++g_netdebug_globals.task_fails >= 3)
	{
		GENERATE_EVENT(_event_warning, "netdebug fail threshold exceeded, removing task from queue");
		remove_current_task(false);
	}
}

bool __cdecl netdebug_queue_task(s_netdebug_upload_task const* task)
{
	return INVOKE(0x00430160, netdebug_queue_task, task);
}

void __cdecl netdebug_set_sessionid(char const* sessionid)
{
	INVOKE(0x004301B0, netdebug_set_sessionid, sessionid);

	//g_netdebug_globals.sessionid.set(sessionid);
}

dword __cdecl netdebug_thread_function(void* thread_parameter)
{
	//return INVOKE(0x004301D0, netdebug_thread_function, thread_parameter);

	g_net_debug_thread_has_exited = false;
	while (!current_thread_should_exit())
	{
		current_thread_update_test_functions();
		if (g_netdebug_globals.current_task.active)
		{
			netdebug_process_next_task();
		}
		else
		{
			c_critical_section_scope section_scope(k_crit_section_network_debug);
			netdebug_get_next_task();
		}
		internal_event_wait_timeout(k_event_render_network_debug_exit, 1000);
	}
	g_net_debug_thread_has_exited = true;
	return 0;
}

void __cdecl netdebug_upload_file(char const* a1, char const* path, void(__cdecl* update_proc)(long upload_position, long upload_length), void(__cdecl* completion_proc)(bool succeeded, void* data), void* completion_data)
{
	//INVOKE(0x00430300, netdebug_upload_file, a1, path, update_proc, completion_proc, completion_data);

	ASSERT((path != NULL) && (path[0] != '\\0'));

	bool task_queued = false;

	if (g_netdebug_globals.initialized)
	{
		c_critical_section_scope section_scope(k_crit_section_network_debug);

		s_netdebug_upload_task task{};
		task.active = true;

		if (a1 && *a1)
			task.__string4.set(a1);
		else
			task.__string4.clear();

		task.update_proc = update_proc;
		task.completion_proc = completion_proc;
		task.completion_data = completion_data;

		file_reference_create_from_path(&task.file, path, false);
		if (netdebug_queue_task(&task))
		{
			task_queued = true;
		}
		else
		{
			GENERATE_EVENT(_event_warning, "netdebug_upload_file() failed to queue task");
		}
	}

	if (!task_queued && completion_proc)
		completion_proc(false, completion_data);
}

void __cdecl remove_current_task(bool succeeded)
{
	//INVOKE(0x00430380, remove_current_task, succeeded);

	if (g_netdebug_globals.current_task.completion_proc)
		g_netdebug_globals.current_task.completion_proc(succeeded, g_netdebug_globals.current_task.completion_data);
	csmemset(&g_netdebug_globals.current_task, 0, sizeof(g_netdebug_globals.current_task));
}

void __cdecl netdebug_set_system_version()
{
	INVOKE(0x00430410, netdebug_set_system_version);

	//g_netdebug_globals.system_version.set("UNKNOWN");
}

void __cdecl netdebug_set_xtl_version()
{
	INVOKE(0x00430430, netdebug_set_xtl_version);

	//g_netdebug_globals.xtl_version.set("UNKNOWN");
}

bool __cdecl upload_synchronous(c_http_client* client, c_http_stream* stream, long seconds, s_netdebug_upload_task const* task)
{
	//return INVOKE(0x00430470, upload_synchronous, client, stream, seconds, task);

	ASSERT(client);
	ASSERT(task);
	
	bool result = false;
	if (transport_available())
	{
		long connection_token = 0;
		long ip_address = 0;
		word port = 0;
	
		long acquire_server_result = 0;
		do
		{
			acquire_server_result = c_online_lsp_manager::get()->acquire_server(_online_lsp_service_type_debug, &connection_token, &ip_address, &port, "crash upload");
			sleep(0);
		} while (!acquire_server_result);
	
		if (acquire_server_result == 1)
		{
			if (client->start(stream, ip_address, port, k_debug_server_url, true))
			{
				bool upload_complete = false;
				dword timeout = system_milliseconds() + 1000 * seconds;
				while (!upload_complete && system_milliseconds() < timeout)
				{
					if (!client->do_work(&upload_complete, NULL, NULL, NULL))
					{
						client->stop();
						break;
					}
	
					if (task->update_proc)
						task->update_proc(client->get_upload_position(), client->get_upload_length());
	
					if (upload_complete)
					{
						c_online_lsp_manager::get()->disconnect_from_server(connection_token, true);
						connection_token = NONE;
						result = true;
					}
	
					if (client->is_connected())
						c_online_lsp_manager::get()->server_connected(connection_token);
	
					sleep(0);
				}
	
				if (system_milliseconds() >= timeout)
					client->stop();
			}
	
			if (!result)
			{
				GENERATE_EVENT(_event_warning, "networking:network_debug_dump: Upload failed in progress to 0x%08x.", ip_address);
				c_online_lsp_manager::get()->disconnect_from_server(connection_token, false);
				connection_token = NONE;
			}
		}
	}
	
	return result;
}

void __cdecl create_session_description()
{
	c_static_string<512> session_description;

	s_file_reference file{};
	file_reference_create_from_path(&file, k_netdebug_session_description_file_location, 0);

	if (!file_exists(&file))
		file_create(&file);

	dword error = 0;
	if (file_open(&file, FLAG(_file_open_flag_desired_access_write), &error))
	{
		session_description.print("%s%s\r\n", k_session_description_title_string, g_netdebug_globals.title.get_string());
		session_description.append_print("%s%s\r\n", k_session_description_build_string, g_netdebug_globals.build.get_string());
		session_description.append_print("%s%s\r\n", k_session_description_system_string, g_netdebug_globals.system.get_string());
		file_write(&file, session_description.length(), session_description.get_string());
		file_close(&file);
	}
}

