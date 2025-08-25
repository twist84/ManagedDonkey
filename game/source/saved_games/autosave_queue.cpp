#include "saved_games/autosave_queue.hpp"

#include "cseries/async.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "editor/editor_stubs.hpp"
#include "game/game.hpp"
#include "main/main_game.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "saved_games/saved_film_manager.hpp"

HOOK_DECLARE(0x0067BDC0, autosave_queue_get_directory_path);
HOOK_DECLARE_CLASS_MEMBER(0x0067C190, c_autosave_queue_enumerator, start);
HOOK_DECLARE_CLASS_MEMBER(0x0067C1D0, c_autosave_queue_enumerator, stop);

REFERENCE_DECLARE(0x024B06E8, s_autosave_queue_globals, g_autosave_queue_globals);

//.text:0067B610 ; public: c_autosave_queue_enumerator::c_autosave_queue_enumerator()
//.text:0067B660 ; public: c_autosave_queue_enumerator::~c_autosave_queue_enumerator()
//.text:0067B670 ; 

bool c_autosave_queue_enumerator::are_items_available()
{
	return INVOKE_CLASS_MEMBER(0x0067B680, c_autosave_queue_enumerator, are_items_available);
}

e_async_completion __cdecl c_autosave_queue_enumerator::async_do_enumerate(s_async_task* work)
{
	return INVOKE(0x0067B6B0, c_autosave_queue_enumerator::async_do_enumerate, work);
}

//.text:0067B850 ; 
//.text:0067B880 ; int32 autosave_queue_copy_to_content_item(e_controller_index controller_index, const s_file_reference* file, const s_saved_game_item_metadata* metadata, c_synchronized_long* success, c_synchronized_long* done)
//.text:0067B930 ; e_async_completion __cdecl autosave_queue_copy_to_content_item_callback(s_async_task* work)

void __cdecl autosave_queue_dispose()
{
	//INVOKE(0x0067BDA0, autosave_queue_dispose);

	ASSERT(!g_autosave_queue_globals.active);
}

void __cdecl autosave_queue_dispose_from_old_map()
{
	//INVOKE(0x0067BDB0, autosave_queue_dispose_from_old_map);
}

const wchar_t* __cdecl autosave_queue_get_directory_path()
{
	//return INVOKE(0x0067BDC0, autosave_queue_get_directory_path);

	g_autosave_queue_globals;
	blamlib_replays_path;
	const wchar_t* result = NULL;
	HOOK_INVOKE(result =, autosave_queue_get_directory_path);
	return L"autosave";
}

const wchar_t* __cdecl autosave_queue_get_filename_prefix()
{
	return L"asq";
}

void __cdecl autosave_queue_free_up_space()
{
	delete_incomplete_files();
	free_up_autosave_space(_autosave_queue_type_game_variant);
	free_up_autosave_space(_autosave_queue_type_map_variant);
	free_up_autosave_space(_autosave_queue_type_film);
}

void __cdecl autosave_queue_generate_new_filename(e_saved_game_file_type file_type, c_static_wchar_string<256>* filename)
{
	filename->set(autosave_queue_get_directory_path());
	filename->append(L"\\");
	filename->append_print(L"asq%016lX%s", system_seconds(), saved_game_filename_extension_by_game_file_type(file_type, false));
}

//.text:0067BEA0 ; 
//.text:0067BEB0 ; 

void __cdecl autosave_queue_initialize()
{
	//INVOKE(0x0067BEC0, autosave_queue_initialize);

	s_file_reference autosave_queue_directory{};
	file_reference_create_from_path_wide(&autosave_queue_directory, autosave_queue_get_directory_path(), true);
	if (!file_create_parent_directories_if_not_present(&autosave_queue_directory))
	{
		event(_event_warning, "autosave: failed to locate/create autosave queue directory '%s'", autosave_queue_get_directory_path());
	}
	g_autosave_queue_globals.active = false;
}

void __cdecl autosave_queue_initialize_for_new_map()
{
	//INVOKE(0x0067BED0, autosave_queue_initialize_for_new_map);

	if (game_options_valid() && !game_is_ui_shell() && !main_game_reset_in_progress() && !game_in_editor())
	{
		delete_incomplete_files();
	
		if (!game_is_playback() || saved_film_manager_snippets_available())
		{
			free_up_autosave_space(_autosave_queue_type_film);
		}
	
		if (game_is_multiplayer() && !game_engine_is_sandbox())
		{
			free_up_autosave_space(_autosave_queue_type_game_variant);
			autosave_queue_save_current_game_variant_to_queue();
	
			free_up_autosave_space(_autosave_queue_type_map_variant);
			autosave_queue_save_current_map_to_queue();
		}
	}
}

int32 __cdecl autosave_queue_read_file(const s_file_reference* file, void* buffer, int32 buffer_size, c_synchronized_long* success, c_synchronized_long* done)
{
	return INVOKE(0x0067BEE0, autosave_queue_read_file, file, buffer, buffer_size, success, done);
}

void __cdecl autosave_queue_save_current_game_variant_to_queue()
{
	// $IMPLEMENT
}

void __cdecl autosave_queue_save_current_map_to_queue()
{
	// $IMPLEMENT
}

void __cdecl delete_incomplete_files()
{
	s_file_reference autosave_queue_directory{};
	file_reference_create_from_path_wide(&autosave_queue_directory, autosave_queue_get_directory_path(), true);

	s_find_file_data find_file_data{};
	find_files_start_with_search_spec(&find_file_data, 0, &autosave_queue_directory, "*.temp");

	s_file_reference file{};
	while (find_files_next(&find_file_data, &file, NULL))
	{
		file_delete(&file);
	}
	find_files_end(&find_file_data);
}

bool c_autosave_queue_enumerator::dequeue_item(s_saved_game_item_enumeration_data* item)
{
	return INVOKE_CLASS_MEMBER(0x0067BF30, c_autosave_queue_enumerator, dequeue_item, item);
}

bool c_autosave_queue_enumerator::is_busy()
{
	return INVOKE_CLASS_MEMBER(0x0067BFA0, c_autosave_queue_enumerator, is_busy);
}

bool __cdecl free_up_autosave_space(e_autosave_queue_type autosave_queue_type)
{
	// $IMPLEMENT

	return false;
}

//.text:0067BFD0 ; 
//.text:0067BFF0 ; 
//.text:0067C030 ; bool __cdecl saved_game_files_filename_match(e_saved_game_file_type game_file_type, s_file_reference* file)

struct s_autosave_queue_enumerator_task
{
	union
	{
		struct
		{
			e_saved_game_file_type game_file_type;
			c_autosave_queue_enumerator* enumerator;
			bool find_files_active;
		};

		s_async_task dummy_for_size;
	};
};

void c_autosave_queue_enumerator::start(e_saved_game_file_type game_file_type)
{
	//INVOKE_CLASS_MEMBER(0x0067C190, c_autosave_queue_enumerator, start, game_file_type);

	s_autosave_queue_enumerator_task task{};

	internal_async_yield_until_done(&m_async_signal, true, false, __FILE__, __LINE__);
	ASSERT(!is_busy());

	csmemset(&task, 0, sizeof(task));
	task.game_file_type = m_game_file_type;
	task.enumerator = this;

	m_game_file_type = game_file_type;
	m_async_cancelled.set(false);
	m_state.set(_enumeration_state_starting);

	m_async_task_id = async_task_add(
		_async_priority_important_non_blocking,
		&task.dummy_for_size,
		_async_category_saved_games,
		c_autosave_queue_enumerator::async_do_enumerate,
		&m_async_signal);
}

//.text:0067C1A0 ; 

void c_autosave_queue_enumerator::stop()
{
	//INVOKE_CLASS_MEMBER(0x0067C1D0, c_autosave_queue_enumerator, stop);

	m_async_cancelled.set(true);
	internal_async_yield_until_done(&m_async_signal, true, false, __FILE__, __LINE__);
	ASSERT(!is_busy());
}

