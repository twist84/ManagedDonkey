#pragma once

#include "cseries/async_helpers.hpp"
#include "game/players.hpp"
#include "networking/tools/http_client.hpp"
#include "saved_games/content/content_item_metadata.hpp"

struct s_async_task;
struct s_file_reference;
struct s_memory_pool;

enum
{
	k_data_compression_scratch_size = 0x4B000
};

enum e_queued_work_item_type
{
	_queued_work_item_type_none = 0,
	_queued_work_item_type_upload,
	_queued_work_item_type_download,
};

enum e_queued_work_item_status_state
{
	_queued_work_item_status_state_none = 0,
	_queued_work_item_status_state_upload_queued,
	_queued_work_item_status_state_upload_compressing,
	_queued_work_item_status_state_upload_compressed,
	_queued_work_item_status_state_upload_uploading,
	_queued_work_item_status_state_download_queued,
	_queued_work_item_status_state_download_downloading,
	_queued_work_item_status_state_download_downloaded,
	_queued_work_item_status_state_download_decompressing,
};

struct s_online_file_description
{
	uns64 task_owner;
	bool overwrite_if_needed;
	s_saved_game_item_metadata metadata;
};
static_assert(sizeof(s_online_file_description) == 0x108);

struct s_queued_work_item
{
	e_queued_work_item_type type;
	s_online_file_description description;
	int32 priority;
	char server_identifier[33];
	uns64 user_xuid;
	uns64 share_identifier;
	int32 slot_index;
	bool from_auto_queue;

	union
	{
		struct
		{
			char source_filename[256];
		} upload;

		struct
		{
			char destination_filename[256];
		} download;
	};
};
static_assert(sizeof(s_queued_work_item) == 0x250);

class c_online_files_enqueuer
{
public:
	enum
	{
		k_maximum_enqueue_length = 8,
	};

	s_queued_work_item m_enqueue_queue[k_maximum_enqueue_length];
	int32 m_enqueue_for_transfer_async_task_id;
	c_synchronized_long m_enqueue_for_transfer_async_task_signal;
	c_synchronized_long m_enqueue_for_transfer_async_task_success;
	__declspec(align(8)) s_queued_work_item m_enqueue_for_transfer_work_item;
};
static_assert(sizeof(c_online_files_enqueuer) == 0x14E0);

class c_signed_in_users_watcher
{
public:
	enum e_watched_state
	{
		_watched_state_signed_in_users = 0,
		_watched_state_online_users,
	};

	e_watched_state m_watched_state;
	s_player_identifier m_player_ids[4];
};
static_assert(sizeof(c_signed_in_users_watcher) == 0x24);

struct s_queued_work_item_status
{
	e_queued_work_item_type type;
	s_online_file_description description;
	int32 priority;
	e_queued_work_item_status_state status_state;
	uns64 share_identifier;
	int32 slot_index;
	real32 progress_ratio;
};
static_assert(sizeof(s_queued_work_item_status) == 0x128);

struct s_queued_work_item_status_tracker
{
	char filename[24];
	s_queued_work_item_status status;
};
static_assert(sizeof(s_queued_work_item_status_tracker) == 0x140);

struct __declspec(align(8)) c_online_files_active_transfer_tracker
{
	enum
	{
		k_online_files_maximum_active_transfers_count = 16,
	};

	enum e_tracker_state
	{
		_tracker_state_none = 0,
		_tracker_state_refresh_running,
	};

	s_queued_work_item_status_tracker m_active_transfers[k_online_files_maximum_active_transfers_count];
	int32 m_active_transfer_count;
	int32 m_file_share_changed_dirty_token;
	bool m_need_refresh;
	e_tracker_state m_state;
	int32 m_current_async_task_id;
	c_synchronized_long m_current_async_task_signal;
	c_synchronized_long m_current_async_task_success;
	bool m_transfers_list_changed;
	c_signed_in_users_watcher m_users_watcher;
};
static_assert(sizeof(c_online_files_active_transfer_tracker) == 0x1448);

class c_aligned_buffered_file_writer
{
public:
	s_file_reference* m_file;
	char* m_write_buffer;
	int32 m_write_buffer_size;
	int32 m_write_buffer_count;
};
static_assert(sizeof(c_aligned_buffered_file_writer) == 0x10);

template<int32 t_write_buffer>
class c_aligned_stored_buffered_file_writer
{
public:
	c_aligned_buffered_file_writer m_writer;
	char m_write_buffer[t_write_buffer];
};
static_assert(sizeof(c_aligned_stored_buffered_file_writer<4096>) == 0x1010);

struct s_blam_zlib_memory
{
	s_memory_pool* heap;
};
static_assert(sizeof(s_blam_zlib_memory) == 0x4);

struct __declspec(align(8)) z_stream_s
{
	struct internal_state;

	byte* next_in;
	uns32 avail_in;
	uns32 total_in;
	byte* next_out;
	uns32 avail_out;
	uns32 total_out;
	char* msg;
	internal_state* state;
	void* (__fastcall* zalloc)(void*, uns32, uns32);
	void(__fastcall* zfree)(void*, void*);
	void* opaque;
	int32 data_type;
	uns32 adler;
	uns32 reserved;
};
static_assert(sizeof(z_stream_s) == 0x38);

class c_online_file_manager
{
public:
	enum
	{
		k_online_files_maximum_enumerated_files_count = 16,
		k_online_files_retry_count = 3,
	};

	enum e_transfer_state
	{
		_transfer_state_none = 0,
		_transfer_state_enumerating_files,
		_transfer_state_compressing,
		_transfer_state_uploading,
		_transfer_state_downloading,
		_transfer_state_decompressing,
	};

private:
	static e_async_completion __cdecl async_do_cancel_transfer(s_async_task* work);

public:
	static void __cdecl dispose();
	static c_online_file_manager* __cdecl get();
	static void __cdecl initialize();
	void memory_dispose();
	void memory_initialize(e_map_memory_configuration memory_configuration);
	void start();
	void stop();
	void update();

//protected:
	bool m_running;
	e_transfer_state m_state;
	bool m_upload_directory_dirty;
	bool m_failed_last_attempt;
	bool m_failed_attempt_message_shown;
	c_synchronized_long m_highest_priority;
	c_synchronized_long m_lowest_priority;
	int32 m_next_refresh_milliseconds;
	char* m_compression_scratch_buffer;
	int32 m_compression_scratch_buffer_size;
	c_http_client m_http_client;
	c_http_post_stream m_http_post_stream;
	c_http_get_stream m_http_get_stream;

	//int32 m_quota_upstream_bytes_per_second_test_override;

	int32 m_current_async_task_id;
	c_synchronized_long m_current_async_task_signal;
	c_synchronized_long m_current_async_task_success;
	c_synchronized_long m_current_async_task_cancelled;
	bool m_current_async_task_failure_was_fatal;
	int32 m_cancel_transfer_async_task_id;
	int32 m_bump_transfer_priority_async_task_id;
	c_synchronized_long m_bump_transfer_priority_async_task_signal;
	c_synchronized_long m_cancel_transfer_async_task_signal;
	c_online_files_enqueuer m_enqueuer;
	c_signed_in_users_watcher m_users_watcher;
	c_online_files_active_transfer_tracker m_active_transfer_tracker;
	c_aligned_stored_buffered_file_writer<4096> writer;
	
	union
	{
		struct
		{
			s_queued_work_item_status_tracker enumerated_files[k_online_files_maximum_enumerated_files_count];
			int32 enumerated_files_count;
			int32 highest_priority;
			int32 lowest_priority;
		} enumerating_files;

		struct
		{
			s_file_reference queued_file;
			s_file_reference source_file;
			s_file_reference destination_file;
			s_blam_zlib_memory zlib_memory;
			z_stream_s zlib_stream;
			s_queued_work_item transfer_work_item;
		} compressing;

		struct
		{
			s_file_reference queued_file;
			s_file_reference source_file;
			s_queued_work_item transfer_work_item;
			char request_buffer[256];
		} uploading;
		struct
		{
			s_file_reference queued_file;
			s_file_reference destination_file;
			s_queued_work_item download_work_item;
			char initial_download_url[256];
			char request_buffer[256];
		} downloading;

		struct
		{
			s_file_reference queued_file;
			s_file_reference source_file;
			s_file_reference destination_file;
			s_blam_zlib_memory zlib_memory;
			z_stream_s zlib_stream;
			s_queued_work_item transfer_work_item;
		} decompressing;
	} m_state_data;
};
static_assert(sizeof(c_online_file_manager) == 0x76F8);

extern const char*& k_upload_directory;
extern int32& k_online_files_automatic_refresh_milliseconds;

extern void __cdecl online_files_dispose();
extern void __cdecl online_files_initialize();
extern void __cdecl online_files_memory_dispose();
extern void __cdecl online_files_memory_initialize(e_map_memory_configuration memory_configuration);
extern void __cdecl online_files_test_retry();
extern void __cdecl online_files_test_throttle_bandwidth(int32 bytes_per_second);
extern void __cdecl online_files_test_upload(const char* local_filename, int32 slot_index);
extern void __cdecl online_files_update();

