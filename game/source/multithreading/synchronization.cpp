#include "multithreading/synchronization.hpp"

#include "cseries/cseries.hpp"

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

