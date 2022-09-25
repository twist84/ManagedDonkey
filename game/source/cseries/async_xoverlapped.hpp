#pragma once

#include "cseries/cseries.hpp"

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

//protected:
	word_flags m_task_flags;
	short m_task_state;
	char const* m_file;
	long m_line;
};
static_assert(sizeof(c_overlapped_task) == 0x10);