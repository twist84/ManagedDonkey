#pragma once

#include "cseries/cseries.hpp"

class c_stop_watch
{
public:
	c_stop_watch();
	c_stop_watch(bool paused);
	~c_stop_watch();

	static int64 __cdecl cycles_per_second();
	static real32 __cdecl cycles_to_seconds(int64 cycles);

private:
	static int64 __cdecl get_absolute_time();
	int64 get_current_delta() const;

public:
	int64 hold();
	int64 reset();
	int64 start();
	int64 stop();
	int64 total_elapsed_time_in_cycles() const;
	real32 total_elapsed_time_in_seconds() const;

protected:
	byte __data0[0x10];
	bool m_paused;
	byte __data11[0x7];
};
COMPILE_ASSERT(sizeof(c_stop_watch) == 0x18);

