#pragma once

#include "multithreading/synchronized_value.hpp"

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

extern void __cdecl release_locks_safe_for_crash_release();

