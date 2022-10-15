#pragma once

struct s_transport_globals
{
	bool initialized;
	bool __unknown1;
	bool __unknown2;
	bool is_wireless;
	long transition_function_count;
	void* startup_functions[8];
	void* shutdown_functions[8];
	void* reset_functions[8];
	void* unknown_functions[8];
};
static_assert(sizeof(s_transport_globals) == 0x88);

extern s_transport_globals& transport_globals;