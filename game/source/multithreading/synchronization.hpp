#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "multithreading/threads.hpp"

enum e_critical_sections
{
	// CS:Event Logs
	_critical_section_event_logs = 0,

	// CS:Data Mine
	_critical_section_data_mine,

	// CS:D3D_Device
	_critical_section_d3d_device,

	// CS:Terminal
	_critical_section_terminal,

	// CS:Network Debug
	_critical_section_network_debug,

	// CS:Render Toggle
	_critical_section_render_toggle,

	// CS:Event RW Lock
	_critical_section_event_read_write_lock,

	// CS:Header Region
	_critical_section_header_region,

	// CS:Header Section
	_critical_section_header_section,

	// CS:Update Region
	_critical_section_update_region,

	// CS:Update Section
	_critical_section_update_section,

	// CS:Render Region
	_critical_section_render_region,

	// CS:Render Section
	_critical_section_render_section,

	// CS:Shared Region
	_critical_section_shared_region,

	// CS:Shared Section
	_critical_section_shared_section,

	// CS:Shared Mirror0
	_critical_section_shared_mirror0,

	// CS:Shared Mirror1
	_critical_section_shared_mirror1,

	// CS:Shared Mirror2
	_critical_section_shared_mirror2,

	// CS:UI widgets
	_critical_section_ui_widgets,

	// CS:UI memory
	_critical_section_ui_memory,

	// CS:UI custom bitmaps
	_critical_section_ui_custom_bitmaps,

	// CS:Saved Game
	_critical_section_saved_game,

	// CS:Levels
	_critical_section_levels,

	// CS:Autosave Queue
	_critical_section_autosave_queue,

	// CS:XOverlapped
	_critical_section_xoverlapped,

	// CS:XOverlapped Memory
	_critical_section_xoverlapped_memory,

	// CS:Game Engine
	_critical_section_game_engine,

	// CS:Http Request Queue
	_critical_section_http_request_queue,

	// CS:LSP Manager
	_critical_section_lsp_manager,

	// CS:Online files
	_critical_section_online_files,

	// CS:Telnet console
	_critical_section_telnet_console,

	// CS:G-Render Region
	_critical_section_g_render_region,

	// CS:G-Render Section
	_critical_section_g_render_section,

	// CS:Bink Prediction
	_critical_section_bink_prediction,

	// CS:Bink Texture 0
	_critical_section_bink_texture0,

	// CS:Bink Texture 1
	_critical_section_bink_texture1,

	// CS:Bink Memory
	_critical_section_bink_memory,

	// CS:Font Cache
	_critical_section_font_cache,

	// CS:Sound Cache
	_critical_section_sound_cache,

	// CS:Tag Cache
	_critical_section_tag_cache,

	// CS:Partition Cache
	_critical_section_partition_cache,

	// CS:Disk Cache
	_critical_section_disk_cache,

	// CS:Test Cache
	_critical_section_test_cache,

	// CS:Prof Display
	_critical_section_prof_display,

	// CS:Prof Session
	_critical_section_prof_session,

	// CS:Resource RW Lock
	_critical_section_resource_read_write_lock,

	// CS:Simple Resource Access RW Lock
	_critical_section_simple_resource_access_read_write_lock,

	// CS:Resource Publish
	_critical_section_resource_publish,

	// CS:Remote Console
	_critical_section_remote_console,

	// CS:Observer Updates
	_critical_section_observer_updates,

	// CS:Web Cache
	_critical_section_web_cache,

	// CS:Prof-Gather
	_critical_section_prof_gather,

	// CS:Prof-Register
	_critical_section_prof_register,

	// CS:Optional Cache
	_critical_section_optional_cache,

	// CS:Net Block Detection
	_critical_section_network_block_detection,

	// CS:Net Statistics
	_critical_section_network_statistics,

	// CS:DSP-Radio
	_critical_section_dsp_dadio,

	// CS:Global Prefs
	_critical_section_global_preferences,

	// CS:Texture Render
	_critical_section_texture_render,

	k_total_critical_sections
};

enum e_synchronization_mutexes
{
	// MTX:AsyncFree
	_synchronization_mutex_async_free = 0,

	// MTX:AsyncWork
	_synchronization_mutex_async_work,

	// MTX:DataMine
	_synchronization_mutex_data_mine,

	k_total_synchronization_mutexes
};

enum e_synchronization_events
{
	// EV:CSeriesEvent
	_synchronization_event_cseries_event = 0,

	// EV:UploadDebug
	_synchronization_event_upload_debug,

	// EV:NetworkDebug
	_synchronization_event_network_debug,

	// EV:VBlankUpdate
	_synchronization_event_vblank_update,

	// EV:VBlankThrottle
	_synchronization_event_vblank_throttle,

	// EV:SoundRenderDeferredStart
	_synchronization_event_sound_render_deferred_start,

	// EV:SoundRenderDeferredComplete
	_synchronization_event_sound_render_deferred_complete,

	// EV:UpdateThreadComplete
	_synchronization_event_update_thread_complete,

	// EV:UpdateThreadBegin
	_synchronization_event_update_thread_begin,

	// EV:UpdateThreadEnd
	_synchronization_event_update_thread_end,

	// EV:BinkFree
	_synchronization_event_bink_free,

	// EV:MirrorsChanged
	_synchronization_event_mirrors_changed,

	k_total_synchronization_events
};

enum e_synchronization_semaphore
{
	// S4:AsyncWork
	_synchronization_semaphore_async_work = 0,

	// S4:EventRW
	_synchronization_semaphore_event_read_write,

	// S4:ResourceRW
	_synchronization_semaphore_resource_read_write,

	// S4:SRPublishRW
	_synchronization_semaphore_simple_resource_publish_read_write,

	// S4:Bink Memory
	_synchronization_semaphore_bink_memory,

	k_total_synchronization_semaphores
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

struct c_critical_section_data
{
	c_synchronization_object object;
	s_critical_section critical_section;
};
static_assert(sizeof(c_critical_section_data) == 0x20);

struct c_synchronization_handle
{
	c_synchronization_object object;
	void* handle;
};
static_assert(sizeof(c_synchronization_handle) == 0xC);

struct c_semaphore_handle
{
	c_synchronization_handle handle;
	long thread_reference_count[k_registered_thread_count];
};
static_assert(sizeof(c_semaphore_handle) == 0x34);

struct s_synchronization_globals
{
	bool initialized;
	c_critical_section_data critical_sections[k_total_critical_sections];
	c_synchronization_handle mutexes[k_total_synchronization_mutexes];
	c_synchronization_handle events[k_total_synchronization_events];
	c_semaphore_handle semaphores[k_total_synchronization_semaphores];
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

