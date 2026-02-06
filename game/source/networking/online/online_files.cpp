#include "networking/online/online_files.hpp"

#include "cache/physical_memory_map.hpp"
#include "cseries/cseries.hpp"
#include "cseries/upload_debug.hpp"
#include "interface/user_interface_session.hpp"
#include "main/main_game.hpp"
#include "networking/tools/network_blf.hpp"
#include "saved_games/content_catalogue.hpp"

#include <time.h>
#include <winsock.h>

REFERENCE_DECLARE(0x01861658, const char*, k_upload_directory);
REFERENCE_DECLARE(0x0186165C, int32, k_online_files_automatic_refresh_milliseconds);

//const char* k_upload_directory = "upload_queue";
//int32 k_online_files_automatic_refresh_milliseconds = 30000;

REFERENCE_DECLARE(0x02179470, c_online_file_manager, g_online_file_manager);

//.text:00473CA0 ; public: c_online_file_manager::c_online_file_manager()
//.text:00473DD0 ; public: c_online_files_active_transfer_tracker::c_online_files_active_transfer_tracker()
//.text:00473E20 ; public: c_online_files_deleter::c_online_files_deleter()
//.text:00473E60 ; public: c_online_files_enqueuer::c_online_files_enqueuer()
//.text:00473EA0 ; public: c_online_files_enumerator::c_online_files_enumerator()
//.text:00473F00 ; 
//.text:00473F10 ; 
//.text:00473F20 ; 
//.text:00473F30 ; 
//.text:00473F40 ; 
//.text:00473F60 ; 
//.text:00473F80 ; 
//.text:00473F90 ; 
//.text:00473FA0 ; 
//.text:00474000 ; private: static e_async_completion __cdecl c_online_file_manager::async_do_bump_transfer_priority(s_async_task*)

e_async_completion __cdecl c_online_file_manager::async_do_cancel_transfer(s_async_task* work)
{
	return INVOKE(0x00474140, c_online_file_manager::async_do_cancel_transfer, work);

	//s_cancel_transfer_task* task = (s_cancel_transfer_task*)work;
	//
	//const char* file_suffixes[5]
	//{
	//	"queued",
	//	"compressed",
	//	"uploading",
	//	"downloading",
	//	"downloaded"
	//};
	//
	//for (int32 file_suffix_index = 0; file_suffix_index < NUMBEROF(file_suffixes); file_suffix_index++)
	//{
	//	c_static_string<256> upload_file{};
	//	upload_file.print("%s\\%s.%s", k_upload_directory, task->base_filename, file_suffixes[file_suffix_index]);
	//
	//	s_file_reference file_reference{};
	//	file_reference_create_from_path(&file_reference, upload_file.get_string(), false);
	//	if (file_exists(&file_reference))
	//	{
	//		file_delete(&file_reference);
	//	}
	//}
	//
	//g_online_file_manager.m_upload_directory_dirty = true;
	//g_online_file_manager.m_active_transfer_tracker.m_need_refresh = true;
	//return _async_completion_done;
}

//.text:004741F0 ; private: static e_async_completion __cdecl c_online_file_manager::async_do_compression(s_async_task*)
//.text:00474980 ; private: static e_async_completion __cdecl c_online_file_manager::async_do_decompression(s_async_task*)
//.text:00475230 ; private: static e_async_completion __cdecl c_online_file_manager::async_do_download(s_async_task*)
//.text:00475970 ; private: static e_async_completion __cdecl c_online_files_enqueuer::async_do_enqueue_for_transfer(s_async_task*)
//.text:00475A70 ; private: static e_async_completion __cdecl c_online_file_manager::async_do_enumerate_and_or_cleanup_files(s_async_task*)
//.text:00475F20 ; private: static e_async_completion __cdecl c_online_files_active_transfer_tracker::async_do_enumerate_files(s_async_task*)
//.text:00476100 ; private: static e_async_completion __cdecl c_online_file_manager::async_do_upload(s_async_task*)
//.text:00476A60 ; private: void c_online_file_manager::attempt_failed(bool, s_file_reference*)
//.text:00476AA0 ; public: void c_online_file_manager::bump_transfer_priority(const char*)
//.text:00476B70 ; private: void c_online_file_manager::bump_transfer_priority_update()
//.text:00476BE0 ; 
//.text:00476C30 ; public: bool c_overlapped_task::busy() const
//.text:00476C60 ; public: void c_online_file_manager::cancel_transfer(const char*)
//.text:00476D20 ; void __cdecl clean_up_task(s_work_item_transfer_task*)
//.text:00476D80 ; 
//.text:00476D90 ; public: void c_online_files_enumerator::clear()
//.text:00476DD0 ; 
//.text:00476E00 ; 
//.text:00476E30 ; 
//.text:00476E60 ; 
//.text:00476E90 ; private: static void __cdecl c_online_file_manager::create_file_reference_with_extension(s_file_reference*, const char*, const char*)

void __cdecl c_online_file_manager::dispose()
{
	INVOKE(0x00476EE0, c_online_file_manager::dispose);
}

//.text:00476EF0 ; private: void c_online_file_manager::end_enumerate_files()
//.text:00476FD0 ; public: void c_online_file_manager::enqueue_for_download(const s_online_file_description*, const char* const, uns64, uns64, int32)
//.text:00477000 ; private: void c_online_file_manager::enqueue_for_transfer(e_queued_work_item_type, const s_online_file_description*, const char*, const char* const, uns64, uns64, int32)
//.text:00477120 ; public: void c_online_files_enqueuer::enqueue_for_transfer(const s_queued_work_item*)
//.text:004771A0 ; public: void c_online_file_manager::enqueue_for_upload(const s_online_file_description*, const char*, uns64, uns64, int32)
//.text:004771D0 ; 
//.text:004771E0 ; public: bool c_content_catalogue::busy() const
//.text:00477210 ; 
//.text:00477220 ; 
//.text:00477230 ; public: bool c_aligned_stored_buffered_file_writer<4096>::flush()

c_online_file_manager* __cdecl c_online_file_manager::get()
{
	//INVOKE(0x00477240, c_online_file_manager::get);

	return &g_online_file_manager;
}

//.text:00477250 ; 
//.text:00477260 ; public: void s_work_item_transfer_task::get_base_filename(c_static_string<17>*)
//.text:004772A0 ; e_controller_index __cdecl get_controller_index_from_xuid(uns64)
//.text:004772F0 ; public: uns32 c_aligned_stored_buffered_file_writer<4096>::get_position()
//.text:00477300 ; 
//.text:00477310 ; 
//.text:00477320 ; 
//.text:00477330 ; 
//.text:00477340 ; 
//.text:00477350 ; public: int32 c_online_files_active_transfer_tracker::get_transfer_index_by_slot(uns64, int32) const
//.text:004773D0 ; public: const char* c_online_files_active_transfer_tracker::get_transfer_name(int32) const
//.text:00477400 ; public: bool c_online_files_active_transfer_tracker::get_transfer_status(int32, s_queued_work_item_status*) const
//.text:004774A0 ; public: bool c_online_files_active_transfer_tracker::get_transfer_status_by_slot(uns64, int32, s_queued_work_item_status*) const
//.text:00477550 ; private: int32 c_online_files_active_transfer_tracker::get_work_item_index(const char*)
//.text:004775C0 ; bool __cdecl get_work_item_status_from_queued_file(s_file_reference*, s_queued_work_item_status*, char*, bool*)
//.text:00477750 ; public: void c_aligned_stored_buffered_file_writer<4096>::initialize(s_file_reference*)

void __cdecl c_online_file_manager::initialize()
{
	//INVOKE(0x00477770, c_online_file_manager::initialize);
}

//.text:00477780 ; public: bool c_online_files_deleter::is_busy()
//.text:00477790 ; public: bool c_online_files_enumerator::is_busy()
//.text:004777A0 ; 
//.text:004777C0 ; 
//.text:00477800 ; 
//.text:00477820 ; 
//.text:00477860 ; public: bool c_online_file_manager::is_running() const

void c_online_file_manager::memory_dispose()
{
	INVOKE_CLASS_MEMBER(0x004778A0, c_online_file_manager, memory_dispose);
}

void c_online_file_manager::memory_initialize(e_map_memory_configuration memory_configuration)
{
	//INVOKE_CLASS_MEMBER(0x004778D0, c_online_file_manager,memory_initialize, memory_configuration);

	if (!map_memory_configuration_is_main_menu(memory_configuration))
	{
		return;
	}

	ASSERT(m_compression_scratch_buffer == nullptr);

	char* compression_scratch_buffer = (char*)_physical_memory_malloc(_memory_stage_map_configuration, "online_files", k_data_compression_scratch_size, 0);

	m_compression_scratch_buffer_size = k_data_compression_scratch_size;
	m_compression_scratch_buffer = compression_scratch_buffer;

	ASSERT(m_compression_scratch_buffer);
}

void __cdecl online_files_dispose()
{
	//INVOKE(0x00477910, online_files_dispose);

	c_online_file_manager::get()->dispose();
}

void __cdecl online_files_initialize()
{
	//INVOKE(0x00477920, online_files_initialize);

	c_online_file_manager::get()->initialize();
}

void __cdecl online_files_memory_dispose()
{
	//INVOKE(0x00477930, online_files_memory_dispose);

	c_online_file_manager::get()->stop();
	c_online_file_manager::get()->memory_dispose();
}

void __cdecl online_files_memory_initialize(e_map_memory_configuration memory_configuration)
{
	//INVOKE(0x00477970, online_files_memory_initialize, memory_configuration);

	c_online_file_manager::get()->memory_initialize(memory_configuration);
}

void __cdecl online_files_test_retry()
{
	//INVOKE(0x004779B0, online_files_test_retry);

	c_online_file_manager::get()->m_active_transfer_tracker.m_file_share_changed_dirty_token++;
	c_online_file_manager::get()->m_upload_directory_dirty = true;
	c_online_file_manager::get()->m_active_transfer_tracker.m_need_refresh = true;
}

void __cdecl online_files_test_throttle_bandwidth(int32 bytes_per_second)
{
	//INVOKE(0x004779C0, online_files_test_throttle_bandwidth, bytes_per_second);

	//c_online_file_manager::get()->m_quota_upstream_bytes_per_second_test_override = bytes_per_second;
	//c_online_file_manager::get()->m_http_client.set_upstream_quota(bytes_per_second);
}

void __cdecl online_files_test_upload(const char* local_filename, int32 slot_index)
{
	//INVOKE(0x004779D0, online_files_test_upload, local_filename, slot_index);

	//c_static_string<260> file_name_to_upload(local_filename);
	//
	//bool start_of_file_valid = false;
	//uns32 file_size = 0;
	//{
	//	char buffer[4096]{};
	//
	//	c_file_reference source_file(local_filename, 0);
	//	if (file_get_size(&source_file, &file_size) &&
	//		file_size > sizeof(s_blf_chunk_start_of_file) &&
	//		file_read(&source_file, sizeof(s_blf_chunk_start_of_file), 1, buffer))
	//	{
	//		start_of_file_valid = network_blf_verify_start_of_file(buffer, sizeof(s_blf_chunk_start_of_file), nullptr, nullptr);
	//	}
	//}
	//
	//if (start_of_file_valid)
	//{
	//	file_name_to_upload.set(local_filename);
	//}
	//else
	//{
	//	c_static_string<260> destination_file_name(local_filename);
	//	destination_file_name.append(".blf");
	//	c_file_reference reference(local_filename, 0);
	//	c_file_reference reference(destination_file_name.get_string(), FLAG(1));
	//	c_aligned_stored_buffered_file_writer<4096> writer{};
	//	file_size = 0;
	//	if (file_get_size(&reference, &file_size))
	//	{
	//		s_blf_chunk_start_of_file start_of_file{};
	//		s_blf_chunk_end_of_file end_of_file{};
	//		s_blf_header header{};
	//		header.setup('test', file_size + 0xC, 1, 1);
	//		end_of_file.total_file_size = ntohl(file_size + sizeof(s_blf_chunk_start_of_file) + sizeof(s_blf_header));
	//		if (writer.write((const char*)&start_of_file, sizeof(s_blf_chunk_start_of_file)))
	//		{
	//			if (writer.write((const char*)&header, sizeof(s_blf_header)))
	//			{
	//				// $IMPLEMENT
	//			}
	//		}
	//	}
	//}
	//
	//if (file_name_to_upload.length() > 0)
	//{
	//	c_static_wchar_string<48> name{};
	//
	//	s_player_identifier player_identifier{};
	//	uns64 user_xuid = user_interface_session_player_get_player_xuid(&player_identifier);
	//
	//	name.set_char(local_filename);
	//	s_online_file_description description{};
	//	description.metadata.size_in_bytes = file_size;
	//	description.task_owner = user_xuid;
	//	name.copy_to(description.metadata.display_name, NUMBEROF(description.metadata.display_name));
	//	description.metadata.date = _time64(nullptr);
	//
	//	g_online_file_manager.enqueue_for_transfer(
	//		_queued_work_item_type_upload,
	//		&description,
	//		file_name_to_upload.get_string(),
	//		nullptr,
	//		user_xuid,
	//		user_xuid,
	//		slot_index,
	//		0);
	//}
}

void __cdecl online_files_update()
{
	//INVOKE(0x004779E0, online_files_update);

	c_online_file_manager::get()->update();
}

//.text:004779F0 ; public: const char* c_static_string<17>::print(const char*, ...)
//.text:00477A10 ; public: const char* c_static_string<48>::print(const char*, ...)
//.text:00477A30 ; private: bool c_online_files_enumerator::process_request_result()
//.text:00478300 ; 
//.text:00478320 ; bool __cdecl read_queued_file(s_file_reference*, s_queued_work_item*)
//.text:00478380 ; public: void c_online_file_manager::refresh_transfer_state(bool, bool)
//.text:004783A0 ; 
//.text:004783C0 ; 
//.text:004783E0 ; 
//.text:00478400 ; 
//.text:00478420 ; 
//.text:00478440 ; public: bool c_aligned_stored_buffered_file_writer<4096>::set_position(uns32)
//.text:00478450 ; public: void c_online_files_active_transfer_tracker::set_transfers(s_queued_work_item_status_tracker*, int32, bool*)
//.text:00478570 ; public: void c_online_file_manager::set_upstream_quota(int32)
//.text:00478590 ; 
//.text:004785B0 ; 
//.text:004785D0 ; 
//.text:004785F0 ; 
//.text:00478610 ; private: void c_online_file_manager::setup_task(s_work_item_transfer_task*)
//.text:004786A0 ; 
//.text:004786B0 ; private: void c_online_files_active_transfer_tracker::sort_transfers()

void c_online_file_manager::start()
{
	INVOKE_CLASS_MEMBER(0x004786D0, c_online_file_manager, start);

	//if (!m_compression_scratch_buffer || m_running)
	//{
	//	return;
	//}
	//
	//ASSERT(m_compression_scratch_buffer_size == k_data_compression_scratch_size);
	//
	//m_running = true;
	//m_upload_directory_dirty = true;
	//
	//m_active_transfer_tracker.m_file_share_changed_dirty_token++;
	//m_active_transfer_tracker.m_need_refresh = true;
}

//.text:004786F0 ; public: void c_online_files_deleter::start(uns64, uns64, int32, const char* const)
//.text:00478730 ; public: void c_online_files_enumerator::start(uns64, uns64)
//.text:00478780 ; private: void c_online_file_manager::start_compressing_file(const s_queued_work_item_status_tracker*)
//.text:00478910 ; private: void c_online_file_manager::start_decompressing_file(const s_queued_work_item_status_tracker*)
//.text:00478AA0 ; private: void c_online_file_manager::start_downloading_file(const s_queued_work_item_status_tracker*, bool)
//.text:00478C40 ; private: void c_online_file_manager::start_enumerate_files()
//.text:00478D40 ; private: void c_online_files_active_transfer_tracker::start_enumerate_files()
//.text:00478E20 ; private: void c_online_file_manager::start_uploading_file(const s_queued_work_item_status_tracker*, bool)

void c_online_file_manager::stop()
{
	INVOKE_CLASS_MEMBER(0x00478FA0, c_online_file_manager, stop);

	//if (m_current_async_task_id != INVALID_ASYNC_TASK_ID)
	//{
	//	m_current_async_task_cancelled = true;
	//	async_task_change_priority(m_current_async_task_id, _async_priority_important_non_blocking);
	//	internal_async_yield_until_done(&m_current_async_task_signal, true, false, __FILE__, __LINE__);
	//	m_current_async_task_signal.set(NONE);
	//}
	//
	//if (m_bump_transfer_priority_async_task_id != INVALID_ASYNC_TASK_ID)
	//{
	//	internal_async_yield_until_done(&m_bump_transfer_priority_async_task_signal, true, false, __FILE__, __LINE__);
	//}
	//
	//if (m_cancel_transfer_async_task_id != INVALID_ASYNC_TASK_ID)
	//{
	//	internal_async_yield_until_done(&m_cancel_transfer_async_task_signal, true, false, __FILE__, __LINE__);
	//}
	//
	//m_running = false;
}

//.text:00479050 ; public: void c_online_files_deleter::stop()
//.text:004790A0 ; public: void c_online_files_enumerator::stop()
//.text:00479110 ; 

void c_online_file_manager::update()
{
	INVOKE_CLASS_MEMBER(0x00479130, c_online_file_manager, update);
}

//.text:00479580 ; public: void c_online_files_active_transfer_tracker::update()
//.text:00479610 ; public: void c_online_files_deleter::update()
//.text:00479700 ; public: void c_online_files_enqueuer::update(bool*)
//.text:00479960 ; public: void c_online_files_enumerator::update()
//.text:00479A90 ; public: void c_online_files_active_transfer_tracker::update_transfer_status(const char*, e_queued_work_item_status_state, real32)
//.text:00479B70 ; private: static bool __cdecl c_online_file_manager::users_signed_in_and_ready()

bool c_content_catalogue::valid() const
{
	return INVOKE_CLASS_MEMBER(0x00479C00, c_content_catalogue, valid);
}

//.text:00479C10 ; int32 __cdecl work_item_tracker_sort_proc(const void*, const void*)
//.text:00479C80 ; public: bool c_aligned_stored_buffered_file_writer<4096>::write(const char*, int32)
//.text:00479C90 ; bool __cdecl write_queued_file(s_file_reference*, const s_queued_work_item*)


