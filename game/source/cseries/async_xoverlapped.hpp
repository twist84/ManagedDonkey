#pragma once

#include "cseries/cseries.hpp"

class c_async_xoverlapped_scope_lock
{
public:
	c_async_xoverlapped_scope_lock();
	~c_async_xoverlapped_scope_lock();
};

class c_overlapped_task
{
	enum e_overlapped_task_flags
	{
		_unused_bit = 0,
		_restarted_during_completion_bit,

		k_number_of_overlapped_task_flags
	};

public:
	virtual ~c_overlapped_task();
	virtual const char* get_context_string() const = 0;
	virtual uns32 start(void* overlapped) = 0;
	virtual void update(uns32 return_result);
	virtual void success(uns32 return_result);
	virtual void failure(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error);
	virtual void complete();
	virtual void reset();
	virtual bool is_result_successful(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error);

public:
	c_overlapped_task(const char* file, int32 line);

	bool busy() const;
	const char* get_file() const;
	int32 get_line() const;
	e_overlapped_task_state get_task_state() const;
	void set_file(const char* file);
	void set_line(int32 line);
	void set_task_state_internal(e_overlapped_task_state task_state);
	void task_recycled_during_completion(bool recycled_during_completion);
	bool task_was_recycled_during_completion() const;

protected:
	c_flags<e_overlapped_task_flags, int16, k_number_of_overlapped_task_flags> m_task_flags;
	c_enum<e_overlapped_task_state, int16, _overlapped_task_state_none, k_overlapped_task_state_count> m_task_state;
	const char* m_file;
	int32 m_line;
};
static_assert(sizeof(c_overlapped_task) == 0x10);

struct s_task_slot;

extern s_task_slot* __cdecl find_task_slot(const c_overlapped_task* task);
extern void __cdecl overlapped_dispose();
extern void __cdecl overlapped_initialize();
extern void __cdecl overlapped_render();
extern void __cdecl overlapped_task_block_until_finished(const c_overlapped_task* task);
extern bool __cdecl overlapped_task_is_running(const c_overlapped_task* task);
extern bool __cdecl overlapped_task_start_internal(c_overlapped_task* task, const char* file, int32 line);
extern void __cdecl overlapped_task_terminate(c_overlapped_task* task);
extern void __cdecl overlapped_task_toggle_debug_rendering(bool toggle_debug_rendering);
extern void __cdecl overlapped_task_wait_for_all_tasks_to_finish();
extern void __cdecl overlapped_update();
extern void __cdecl task_block_until_finished(s_task_slot* task_slot);
extern bool __cdecl task_is_complete(s_task_slot* task_slot, uns32* return_result, uns32* calling_result, uns32* overlapped_error, uns32* overlapped_extended_error);
extern void __cdecl task_now_finished(s_task_slot* task_slot, uns32 return_result, uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error);

extern void overlapped_tasks_log_to_debug_txt(e_event_level event_level);
extern void overlapped_task_display_task_descriptions();
extern void overlapped_task_inject_error(const char* context, bool inject_error);
extern void overlapped_task_pause(const char* context, bool pause);

