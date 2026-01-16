#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "saved_games/content/content_item_metadata.hpp"
#include "tag_files/files.hpp"
#include "tag_files/files_windows.hpp"

struct s_async_task;

enum e_autosave_queue_type
{
	_autosave_queue_type_game_variant = 0,
	_autosave_queue_type_map_variant,
	_autosave_queue_type_film,

	k_autosave_queue_type_count,
};

struct s_saved_game_item_enumeration_data
{
	enum e_item_state
	{
		_item_state_none = 0,
		_item_state_ready = 1,
		_item_state_corrupt = 2,
	};

	s_file_reference file;
	e_item_state state;
	s_saved_game_item_metadata metadata;
	s_file_last_modification_date modification_date;
};
COMPILE_ASSERT(sizeof(s_saved_game_item_enumeration_data) == 0x218);

class c_autosave_queue_enumerator
{
public:
	enum e_enumeration_state
	{
		_enumeration_state_none = 0,
		_enumeration_state_starting = 1,
		_enumeration_state_running = 2,
		_enumeration_state_done = 3,
	};

public:
	bool are_items_available();

private: 
	static e_async_completion __cdecl async_do_enumerate(s_async_task* work);

public:
	bool dequeue_item(s_saved_game_item_enumeration_data* item);
	bool is_busy();
	void start(e_saved_game_file_type game_file_type);
	void stop();

private:
	e_saved_game_file_type m_game_file_type;
	c_synchronized_long m_state;
	s_saved_game_item_enumeration_data m_enumerated_files[26];
	c_synchronized_long m_enumerated_file_count;
	c_synchronized_long m_dequeued_item_count;
	s_find_file_data m_find_file_data;
	int32 m_async_task_id;
	c_synchronized_long m_async_signal;
	c_synchronized_long m_async_cancelled;
};
COMPILE_ASSERT(sizeof(c_autosave_queue_enumerator) == 0x3D28);

#pragma pack(push, 1)
struct s_autosave_queue_globals
{
	bool active;
	s_file_reference active_source_file;
	s_file_reference active_destination_file;
	byte pad[0x7];
	s_saved_game_item_metadata active_metadata;
};
COMPILE_ASSERT(sizeof(s_autosave_queue_globals) == 0x320);
#pragma pack(pop)

extern s_autosave_queue_globals& g_autosave_queue_globals;

extern void __cdecl autosave_queue_dispose();
extern void __cdecl autosave_queue_dispose_from_old_map();
extern const wchar_t* __cdecl autosave_queue_get_directory_path();
extern const wchar_t* __cdecl autosave_queue_get_filename_prefix();
extern void __cdecl autosave_queue_free_up_space();
extern void __cdecl autosave_queue_generate_new_filename(e_saved_game_file_type file_type, c_static_string<256>* filename);
extern void __cdecl autosave_queue_initialize();
extern void __cdecl autosave_queue_initialize_for_new_map();
extern int32 __cdecl autosave_queue_read_file(const s_file_reference* file, void* buffer, int32 buffer_size, c_synchronized_long* success, c_synchronized_long* done);
extern void __cdecl autosave_queue_save_current_game_variant_to_queue();
extern void __cdecl autosave_queue_save_current_map_to_queue();
extern void __cdecl delete_incomplete_files();
extern bool __cdecl free_up_autosave_space(e_autosave_queue_type autosave_queue_type);

