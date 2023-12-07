#include "multithreading/synchronization.hpp"

REFERENCE_DECLARE(0x0238F45C, s_synchronization_globals, g_synch_globals);

c_critical_section_scope::c_critical_section_scope(long critical_section_id) :
	m_critical_section_id(),
	m_critical_section_entered()
{
	DECLFUNC(0x0052C4A0, void, __cdecl, long)(critical_section_id);
}

c_critical_section_scope::c_critical_section_scope(long critical_section_id, long time_step, bool* out_lock_acquired) :
	m_critical_section_id(),
	m_critical_section_entered()
{
	DECLFUNC(0x0052BEE0, void, __thiscall, c_critical_section_scope*, long, long, bool*)(this, critical_section_id, time_step, out_lock_acquired);
}

c_critical_section_scope::~c_critical_section_scope()
{
	DECLFUNC(0x0052C4E0, void, __cdecl, c_synchronized_long)(m_critical_section_id);
}

void __cdecl release_locks_safe_for_crash_release()
{
	INVOKE(0x0052C9C0, release_locks_safe_for_crash_release);
}

