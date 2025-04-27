#include "profiler/profiler_stopwatch.hpp"

c_stop_watch::c_stop_watch() :
	__data0(),
	m_paused(),
	__data11()
{
	DECLFUNC(0x00618DD0, c_stop_watch*, __thiscall, c_stop_watch*)(this);
}

c_stop_watch::c_stop_watch(bool paused) :
	__data0(),
	m_paused(),
	__data11()
{
	DECLFUNC(0x00618E30, c_stop_watch*, __thiscall, c_stop_watch*, bool)(this, paused);
}

c_stop_watch::~c_stop_watch()
{
}

int64 __cdecl c_stop_watch::cycles_per_second()
{
	return INVOKE(0x00618E80, cycles_per_second);
}

real32 __cdecl c_stop_watch::cycles_to_seconds(int64 cycles)
{
	return INVOKE(0x00618EB0, cycles_to_seconds, cycles);
}

int64 __cdecl c_stop_watch::get_absolute_time()
{
	return INVOKE(0x00618F50, get_absolute_time);
}

int64 c_stop_watch::get_current_delta() const
{
	return INVOKE_CLASS_MEMBER(0x00618F80, c_stop_watch, get_current_delta);
}

int64 c_stop_watch::hold()
{
	return INVOKE_CLASS_MEMBER(0x00618FD0, c_stop_watch, hold);
}

int64 c_stop_watch::reset()
{
	return INVOKE_CLASS_MEMBER(0x00619030, c_stop_watch, reset);
}

int64 c_stop_watch::start()
{
	return INVOKE_CLASS_MEMBER(0x006190A0, c_stop_watch, start);
}

int64 c_stop_watch::stop()
{
	return INVOKE_CLASS_MEMBER(0x00619100, c_stop_watch, stop);
}

int64 c_stop_watch::total_elapsed_time_in_cycles() const
{
	return INVOKE_CLASS_MEMBER(0x00619170, c_stop_watch, total_elapsed_time_in_cycles);
}

real32 c_stop_watch::total_elapsed_time_in_seconds() const
{
	return INVOKE_CLASS_MEMBER(0x006191E0, c_stop_watch, total_elapsed_time_in_seconds);
}

