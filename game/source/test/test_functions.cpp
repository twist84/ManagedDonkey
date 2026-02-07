#include "test/test_functions.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "networking/logic/storage/network_http_request_queue.hpp"
#include "networking/online/online_lsp.hpp"
#include "tag_files/files_windows.hpp"

s_test_download_storage g_test_download_storage{};

void test_download_storage_file(const char* url, const char* local_filename)
{
	c_network_http_request_queue* request_queue = c_network_http_request_queue::get(_network_http_request_queue_type_optional);

	if (g_test_download_storage.request_cookie)
	{
		file_close(&g_test_download_storage.request_file);
		if (g_test_download_storage.request_cookie)
		{
			request_queue->cancel_request(g_test_download_storage.request_cookie);
			g_test_download_storage.request_cookie = 0;
		}
	}

	csmemset(&g_test_download_storage.request_file, 0, sizeof(s_file_reference));
	file_reference_create_from_path(&g_test_download_storage.request_file, local_filename, false);
	file_create_parent_directories_if_not_present(&g_test_download_storage.request_file);
	if (!file_exists(&g_test_download_storage.request_file) || file_delete(&g_test_download_storage.request_file))
	{
		enum
		{
			open_flags = FLAG(_permission_read_bit) | FLAG(_permission_write_bit) | FLAG(_permission_write_append_bit)
		};

		uns32 error = 0;
		if (file_create(&g_test_download_storage.request_file) && file_open(&g_test_download_storage.request_file, open_flags, &error))
		{
			c_network_http_request_description description{};
			description = c_network_http_request_description(url, _http_request_type_get);
			g_test_download_storage.request_cookie = request_queue->start_request(_online_lsp_service_type_title_files, &description);
		}
	}
}

void test_download_storage_file_update()
{
	char buffer[4096]{};

	if (g_test_download_storage.request_cookie)
	{
		c_network_http_request_queue* request_queue = c_network_http_request_queue::get(_network_http_request_queue_type_optional);

		int32 bytes_read = 0;
		e_network_http_request_result request_result = request_queue->read_bytes(g_test_download_storage.request_cookie, buffer, sizeof(buffer), &bytes_read, nullptr);

		bool v2 = true;
		if (bytes_read > 0 && !file_write(&g_test_download_storage.request_file, bytes_read, buffer))
			v2 = false;

		switch (request_result)
		{
		case 1:
		{
			event(_event_message, "test:download_storage_file: http request completed successfully.");
			g_test_download_storage.request_cookie = 0;
			v2 = false;
		}
		break;
		case 2:
		{
			event(_event_message, "test:download_storage_file: http request failed, cancelling.");
			g_test_download_storage.request_cookie = 0;
			v2 = false;
		}
		break;
		case 3:
		{
			event(_event_message, "test:download_storage_file: http request failed, retrying.");
			g_test_download_storage.request_cookie = 0;
			v2 = false;
		}
		break;
		}

		if (!v2)
		{
			file_close(&g_test_download_storage.request_file);
			if (g_test_download_storage.request_cookie)
			{
				request_queue->cancel_request(g_test_download_storage.request_cookie);
				g_test_download_storage.request_cookie = 0;
			}
		}
	}
}

void test_functions_update()
{
	test_download_storage_file_update();
}

//void __cdecl game_grief_update_for_test_functions_update()
//{
//	INVOKE(0x006790D0, game_grief_update_for_test_functions_update);
//
//	test_functions_update();
//}
//HOOK_DECLARE_CALL(0x0053332C, game_grief_update_for_test_functions_update); // game_tick

