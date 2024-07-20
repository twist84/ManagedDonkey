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

	c_overlapped_task* constructor(char const* file, long line);

	bool busy() const;
	void set_file(char const* file);
	void set_line(long line);

//protected:
	c_flags<e_overlapped_task_flags, word, k_overlapped_task_flags> m_task_flags;
	c_enum<e_overlapped_task_state, short, _overlapped_task_state_none, k_overlapped_task_state_count> m_task_state;
	char const* m_file;
	long m_line;
};
static_assert(sizeof(c_overlapped_task) == 0x10);

struct s_task_slot
{
	c_overlapped_task* task;
	dword calling_result;
	bool terminated;

	// pad?
	byte __data9[0x3];
};
static_assert(sizeof(s_task_slot) == 0xC);

struct s_overlapped_globals
{
	c_static_array<s_task_slot, 64> task_slots;

	// pad?
	byte __data300[0xA];

	bool debug_render_enabled;

	// pad?
	byte __data30B[0x1];
};
static_assert(sizeof(s_overlapped_globals) == 0x30C);

extern s_overlapped_globals& g_overlapped_globals;

extern void __cdecl overlapped_render();
extern bool __cdecl overlapped_task_start_internal(c_overlapped_task* task, char const* file, long line);
extern void __cdecl overlapped_update();

