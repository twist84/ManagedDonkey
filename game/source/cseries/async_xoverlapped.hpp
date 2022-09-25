#pragma once

#include "cseries/cseries.hpp"

enum e_overlapped_task_state
{
	_overlapped_task_state_none = 0,
	_overlapped_task_state_starting,
	_overlapped_task_state_pending,
	_overlapped_task_state_completing,
	_overlapped_task_state_succeeded,
	_overlapped_task_state_unknown5,

	k_overlapped_task_state_count
};

struct c_overlapped_task
{
public:
	virtual void* destructor(dword);
	virtual const char* get_context_string();
	virtual dword start(void*);
	virtual void update(dword);
	virtual void success(dword);
	virtual void failure(dword, dword, dword);
	virtual void complete();
	virtual void reset();
	virtual bool is_result_successful(dword, dword, dword);

	c_overlapped_task(char const* file, long line);

	bool busy() const;
	void set_file(char const* file);
	void set_line(long line);

//protected:
	word_flags m_task_flags;

	//c_enum<e_overlapped_task_state, short, k_overlapped_task_state_count> m_task_state;
	short m_task_state;

	char const* m_file;
	long m_line;
};
static_assert(sizeof(c_overlapped_task) == 0x10);
