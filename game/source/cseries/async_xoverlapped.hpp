#pragma once

#include "cseries/cseries.hpp"

class c_async_xoverlapped_scope_lock
{
public:
	c_async_xoverlapped_scope_lock();
	~c_async_xoverlapped_scope_lock();
};

enum e_overlapped_task_state
{
	_overlapped_task_state_none = 0,
	_overlapped_task_state_starting,
	_overlapped_task_state_pending,
	_overlapped_task_state_completing,
	_overlapped_task_state_succeeded,
	_overlapped_task_state_failure,

	k_overlapped_task_state_count
};

struct c_overlapped_task
{
	enum e_overlapped_task_flags
	{
		_overlapped_task_bit0 = 0,
		_overlapped_task_task_recycled_during_completion_bit,

		k_overlapped_task_flags
	};

public:
	virtual void* destructor(dword);
	virtual char const* get_context_string();
	virtual dword start(void* overlapped);
	virtual void update(dword return_result);
	virtual void success(dword return_result);
	virtual void failure(dword calling_result, dword overlapped_error, dword overlapped_extended_error);
	virtual void complete();
	virtual void reset();
	virtual bool is_result_successful(dword calling_result, dword overlapped_error, dword overlapped_extended_error);

	e_overlapped_task_state get_task_state() const;
	void set_task_state_internal(e_overlapped_task_state task_state);
	void task_recycled_during_completion(bool recycled_during_completion);
	bool task_was_recycled_during_completion() const;

	c_overlapped_task* constructor(char const* file, long line);

	bool busy() const;

	char const* get_file() const;
	void set_file(char const* file);
	long get_line() const;
	void set_line(long line);

protected:
	c_flags<e_overlapped_task_flags, word, k_overlapped_task_flags> m_task_flags;
	c_enum<e_overlapped_task_state, short, _overlapped_task_state_none, k_overlapped_task_state_count> m_task_state;
	char const* m_file;
	long m_line;
};
static_assert(sizeof(c_overlapped_task) == 0x10);

struct s_task_slot;

extern s_task_slot* __cdecl find_task_slot(c_overlapped_task const* task);
extern void __cdecl overlapped_dispose();
extern void __cdecl overlapped_initialize();
extern void __cdecl overlapped_render();
extern void __cdecl overlapped_task_block_until_finished(c_overlapped_task const* task);
extern bool __cdecl overlapped_task_is_running(c_overlapped_task const* task);
extern bool __cdecl overlapped_task_start_internal(c_overlapped_task* task, char const* file, long line);
extern void __cdecl overlapped_task_terminate(c_overlapped_task* task);
extern void __cdecl overlapped_task_toggle_debug_rendering(bool toggle_debug_rendering);
extern void __cdecl overlapped_task_wait_for_all_tasks_to_finish();
extern void __cdecl overlapped_update();
extern void __cdecl task_block_until_finished(s_task_slot* task_slot);
extern bool __cdecl task_is_complete(s_task_slot* task_slot, dword* return_result, dword* calling_result, dword* overlapped_error, dword* overlapped_extended_error);
extern void __cdecl task_now_finished(s_task_slot* task_slot, dword return_result, dword calling_result, dword overlapped_error, dword overlapped_extended_error);

