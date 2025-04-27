#pragma once

typedef void(__cdecl* transport_startup_function_t)(void*);
typedef void(__cdecl* transport_shutdown_function_t)(void*);
typedef void(__cdecl* transport_reset_function_t)(void*);

struct s_transport_globals
{
	bool initialized;
	bool winsock_initialized;
	bool is_started;
	bool is_wireless;
	int32 transition_function_count;
	transport_startup_function_t startup_functions[8];
	transport_shutdown_function_t shutdown_functions[8];
	transport_reset_function_t reset_functions[8];
	void* callback_datas[8];
};
static_assert(sizeof(s_transport_globals) == 0x88);

extern s_transport_globals& transport_globals;

extern bool __cdecl transport_available();
extern void __cdecl transport_dispose();
extern int32 __cdecl transport_get_packet_maximum_payload(int32 type);
extern int32 __cdecl transport_get_packet_overhead(int32 type);
extern void __cdecl transport_global_update();
extern void __cdecl transport_initialize();
extern bool __cdecl transport_network_available();
extern void __cdecl transport_register_transition_functions(transport_startup_function_t startup_function, transport_shutdown_function_t shutdown_function, transport_reset_function_t reset_function, void* callback_data);
extern void __cdecl transport_reset();
extern void __cdecl transport_shutdown();
extern void __cdecl transport_startup();