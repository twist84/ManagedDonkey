#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "multithreading/threads.hpp"

enum e_critical_sections
{
	// CS:Event Logs
	k_crit_section_event_logs = 0,

	// CS:Data Mine
	k_crit_section_data_mine,

	// CS:D3D_Device
	k_crit_section_rasterizer_device,

	// CS:Terminal
	k_crit_section_terminal,

	// CS:Network Debug
	k_crit_section_network_debug,

	// CS:Render Toggle
	k_crit_section_render_thread_toggle_lock,

	// CS:Event RW Lock
	k_crit_section_event_rw_lock,

	// CS:Header Region
	k_crit_section_header_region,

	// CS:Header Section
	k_crit_section_header_subsection,

	// CS:Update Region
	k_crit_section_update_region,

	// CS:Update Section
	k_crit_section_update_subsection,

	// CS:Render Region
	k_crit_section_render_region,

	// CS:Render Section
	k_crit_section_render_subsection,

	// CS:Shared Region
	k_crit_section_shared_region,

	// CS:Shared Section
	k_crit_section_shared_subsection,

	// CS:Shared Mirror0
	k_crit_section_shared_subsection_mirror0,

	// CS:Shared Mirror1
	k_crit_section_shared_subsection_mirror1,

	// CS:Shared Mirror2
	k_crit_section_shared_subsection_mirror2,

	// CS:UI widgets
	k_crit_section_ui_widgets_lock,

	// CS:UI memory
	k_crit_section_ui_memory_lock,

	// CS:UI custom bitmaps
	k_crit_section_ui_custom_bitmaps_lock,

	// CS:Saved Game
	k_crit_section_saved_game,

	// CS:Levels
	k_crit_section_levels,

	// CS:Autosave Queue
	k_crit_section_autosave_queue,

	// CS:XOverlapped
	k_crit_section_async_xoverlapped,

	// CS:XOverlapped Memory
	k_crit_section_async_xoverlapped_memory,

	// CS:Game Engine
	k_crit_section_game_engine,

	// CS:Http Request Queue
	k_crit_section_http_request_queue,

	// CS:LSP Manager
	k_crit_section_lsp_manager,

	// CS:Online files
	k_crit_section_online_files,

	// CS:Telnet console
	k_crit_section_telnet_console,

	// CS:G-Render Region
	k_crit_section_global_render_region,

	// CS:G-Render Section
	k_crit_section_global_render_subsection,

	// CS:Bink Prediction
	k_crit_section_bink_prediction,

	// CS:Bink Texture 0
	k_crit_section_bink_texture0,

	// CS:Bink Texture 1
	k_crit_section_bink_texture1,

	// CS:Bink Memory
	k_crit_section_bink_memory,

	// CS:Font Cache
	k_crit_section_font_cache,

	// CS:Sound Cache
	k_crit_section_sound_cache,

	// CS:Tag Cache
	k_crit_section_tag_cache,

	// CS:Partition Cache
	k_crit_section_partition_cache,

	// CS:Disk Cache
	k_crit_section_disk_cache,

	// CS:Test Cache
	k_crit_section_test_cache,

	// CS:Prof Display
	k_crit_section_profiler_display,

	// CS:Prof Session
	k_crit_section_profiler_datamine_update,

	// CS:Resource RW Lock
	k_crit_section_resource_threading_access_rw_lock,

	// CS:Simple Resource Access RW Lock
	k_crit_section_simple_resource_cache_access_rw_lock,

	// CS:Resource Publish
	k_crit_section_resource_cache_publish,

	// CS:Remote Console
	k_crit_section_remote_console_commands,

	// CS:Observer Updates
	k_crit_section_observer_updates,

	// CS:Web Cache
	k_crit_section_web_cache,

	// CS:Prof-Gather
	k_crit_section_performance_data_gathering,

	// CS:Prof-Register
	k_crit_section_performance_data_registration,

	// CS:Optional Cache
	k_crit_section_optional_cache,

	// CS:Net Block Detection
	k_crit_section_network_block_detection,

	// CS:Net Statistics
	k_crit_section_network_statistics,

	// CS:DSP-Radio
	k_crit_section_dsp_radio_effect,

	// CS:Global Prefs
	k_crit_section_global_preferences,

	// CS:Texture Render
	k_crit_section_texture_render,

	k_total_critical_sections,

	k_invalid_critical_section = NONE
};

enum e_synchronization_mutexes
{
	// MTX:AsyncFree
	k_mutex_async_free_list = 0,

	// MTX:AsyncWork
	k_mutex_async_work_list,

	// MTX:DataMine
	k_mutex_data_mining,

	k_total_synchronization_mutexes
};

enum e_synchronization_events
{
	// EV:CSeriesEvent
	k_event_cseries_event_log = 0,

	// EV:UploadDebug
	k_event_upload_debug,

	// EV:NetworkDebug
	k_event_render_network_debug_exit,

	// EV:VBlankUpdate
	k_event_main_time_update_vblank,

	// EV:VBlankThrottle
	k_event_main_time_throttle_vblank,

	// EV:SoundRenderDeferredStart
	k_event_sound_render_deferred_start,

	// EV:SoundRenderDeferredComplete
	k_event_sound_render_deferred_complete,

	// EV:UpdateThreadComplete
	k_event_update_thread_complete,

	// EV:UpdateThreadBegin
	k_event_update_thread_begin,

	// EV:UpdateThreadEnd
	k_event_update_thread_end,

	// EV:BinkFree
	k_event_bink_free,

	// EV:MirrorsChanged
	k_event_mirrors_changed,

	k_total_synchronization_events
};

enum e_synchronization_semaphore
{
	// S4:AsyncWork
	k_semaphore_async_work = 0,

	// S4:EventRW
	k_semaphore_event_rw_lock,

	// S4:ResourceRW
	k_semaphore_resource_threading_access_rw_lock,

	// S4:SRPublishRW
	k_semaphore_simple_resource_cache_publish_rw_lock,

	// S4:Bink Memory
	k_semaphore_bink_memory,

	k_total_synchronization_semaphores,

	k_invalid_semaphore = NONE
};

struct s_list_entry // LIST_ENTRY
{
	s_list_entry* forward_link;
	s_list_entry* backward_link;
};
static_assert(sizeof(s_list_entry) == 0x8);

struct s_critical_section_debug // RTL_CRITICAL_SECTION_DEBUG
{
	word type;
	word creator_back_trace_index;
	struct s_critical_section* critical_section;
	s_list_entry process_locks_list;
	dword entry_count;
	dword contention_count;
	dword flags;
	word creator_back_trace_index_high;
	word spare_word;
};
static_assert(sizeof(s_critical_section_debug) == 0x20);

struct s_critical_section // RTL_CRITICAL_SECTION
{
	s_critical_section_debug* debug_info;

	//
	//  The following three fields control entering and exiting the critical
	//  section for the resource
	//

	long lock_count;
	long recursion_count;
	void* owning_thread;        // from the thread's ClientId->UniqueThread
	void* lock_semaphore;
	/* ULONG_PTR */ dword spin_count;        // force size on 64-bit systems when packed
};
static_assert(sizeof(s_critical_section) == 0x18);

struct c_synchronization_object
{
	c_interlocked_long locking_thread;
	c_interlocked_long lock_count;
};
static_assert(sizeof(c_synchronization_object) == 0x8);

struct c_critical_section_data : c_synchronization_object
{
	s_critical_section critical_section;
};
static_assert(sizeof(c_critical_section_data) == 0x20);

struct c_synchronization_handle : c_synchronization_object
{
	void* handle;
};
static_assert(sizeof(c_synchronization_handle) == 0xC);

struct c_semaphore_handle : c_synchronization_handle
{
	long thread_reference_count[k_registered_thread_count];
};
static_assert(sizeof(c_semaphore_handle) == 0x34);

struct s_synchronization_globals
{
	volatile bool initialized;
	c_critical_section_data critical_section[k_total_critical_sections];
	c_synchronization_handle mutex[k_total_synchronization_mutexes];
	c_synchronization_handle sync_event[k_total_synchronization_events];
	c_semaphore_handle semaphore[k_total_synchronization_semaphores];
};
static_assert(sizeof(s_synchronization_globals) == 0x91C);

struct c_critical_section_scope
{
public:
	c_critical_section_scope(long critical_section_id);
	c_critical_section_scope(long critical_section_id, long time_step, bool* out_lock_acquired);
	~c_critical_section_scope();

protected:
	c_synchronized_long m_critical_section_id;
	c_synchronized_long m_critical_section_entered;
};

extern s_synchronization_globals& g_synch_globals;

extern bool __cdecl event_has_automatic_reset(long event_id);
extern char const* __cdecl get_sync_primitive_name(long type, long index);
extern void __cdecl initialize_synchronization_objects();
extern void __cdecl internal_critical_section_enter(long critical_section_id);
extern void __cdecl internal_critical_section_leave(long critical_section_id);
extern bool __cdecl internal_critical_section_try_and_enter(long critical_section_id);
extern void __cdecl internal_event_reset(long event_id);
extern void __cdecl internal_event_set(long event_id);
extern void __cdecl internal_event_wait(long event_id);
extern bool __cdecl internal_event_wait_timeout(long event_id, dword timeout_in_milliseconds);
extern void __cdecl internal_mutex_release(long mutex_id);
extern void __cdecl internal_mutex_take(long mutex_id);
extern bool __cdecl internal_mutex_take_timeout(long mutex_id, dword timeout_in_milliseconds);
extern long __cdecl internal_semaphore_release(long semaphore_id);
extern void __cdecl internal_semaphore_take(long semaphore_id);
extern void __cdecl release_all_critical_sections_owned_by_thread();
extern void __cdecl release_all_locks_owned_by_thread();
extern void __cdecl release_all_mutexes_owned_by_thread();
extern void __cdecl release_all_semaphores_owned_by_thread();
extern void __cdecl release_critical_section_owned_by_thread(long thread_index, e_critical_sections critical_section_id);
extern void __cdecl release_locks_safe_for_crash_release();
extern void __cdecl render_synchronization_stats();
extern long __cdecl sempahore_get_max_signal_count(long semaphore_id);
extern bool __cdecl synchronization_objects_initialized();
extern bool __cdecl wait_for_single_object_internal(void* handle, dword timeout_in_milliseconds);

