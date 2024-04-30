#pragma once

#include "cseries/cseries.hpp"

struct c_stop_watch
{
public:
	c_stop_watch();
	c_stop_watch(bool paused);
	~c_stop_watch();

	static __int64 __cdecl cycles_per_second();
	static real __cdecl cycles_to_seconds(__int64 cycles);

private:
	static __int64 __cdecl get_absolute_time();
	__int64 get_current_delta() const;

public:
	__int64 hold();
	__int64 reset();
	__int64 start();
	__int64 stop();
	__int64 total_elapsed_time_in_cycles() const;
	real total_elapsed_time_in_seconds() const;

protected:
	byte __data0[0x10];
	bool m_paused;
	byte __data11[0x7];
};
static_assert(sizeof(c_stop_watch) == 0x18);

