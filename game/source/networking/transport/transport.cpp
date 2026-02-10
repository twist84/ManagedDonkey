#include "networking/transport/transport.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/transport/transport_endpoint_set_winsock.hpp"
#include "networking/transport/transport_endpoint_winsock.hpp"
#include "networking/transport/transport_security.hpp"

#include <WinSock2.h>

REFERENCE_DECLARE(0x0199FA28, s_transport_globals, transport_globals);

HOOK_DECLARE(0x00430630, transport_available);
//HOOK_DECLARE(0x00430650, transport_dispose);

int32 x_link_status_override = NONE;

bool __cdecl transport_available()
{
	//return INVOKE(0x00430630, transport_available);

	return transport_globals.initialized && transport_globals.started;
}

void __cdecl transport_dispose()
{
	//INVOKE(0x00430650, transport_dispose);

	if (transport_globals.initialized)
	{
		transport_shutdown();
		transport_qos_dispose();
		transport_globals.transition_function_count = false;
		transport_globals.initialized = false;
	}
}

int32 __cdecl transport_get_packet_maximum_payload(int32 transport_type)
{
	//return INVOKE(0x00430680, transport_get_packet_maximum_payload, transport_type);

	return 4096;
}

int32 __cdecl transport_get_packet_overhead(int32 transport_type)
{
	//return INVOKE(0x00430690, transport_get_packet_overhead, transport_type);

	int32 overhead = 0;
	if (transport_type >= _transport_type_udp)
	{
		if (transport_type <= _transport_type_vdp)
		{
			overhead = 0x1C;
		}
		else if (transport_type == _transport_type_tcp)
		{
			overhead = 0x28;
		}
	}
	return overhead;
}

void __cdecl transport_global_update()
{
	//INVOKE(0x004306C0, transport_global_update);

	if (transport_globals.initialized)
	{
		bool network_available = transport_network_available();
		if (transport_globals.network_available != network_available)
		{
			transport_globals.network_available = network_available;
			if (network_available)
			{
				event(_event_message, "networking:transport: network interface connection restored, resetting networking");
				transport_startup();
			}
			else
			{
				event(_event_error, "networking:transport: network interface connection lost");
				transport_shutdown();
			}
		}

		if (transport_globals.network_available)
		{
			transport_secure_address_resolve();
		}
	}
}

void __cdecl transport_initialize()
{
	//INVOKE(0x004306F0, transport_initialize);

	csmemset(&transport_globals, 0, sizeof(transport_globals));
	transport_security_initialize();
	transport_qos_initialize();
	transport_globals.initialized = true;

	if (x_link_status_override == NONE || x_link_status_override > 0)
	{
		transport_startup();
	}
}

bool __cdecl transport_network_available()
{
	//return INVOKE(0x00430720, transport_network_available);

	bool connection_live = false;
	if (x_link_status_override != NONE)
	{
		connection_live = x_link_status_override > 0;
	}
	else
	{
		connection_live = transport_globals.initialized;
	}
	return connection_live;
}

void __cdecl transport_register_transition_functions(transport_startup_func_t* startup_func, transport_shutdown_func_t* shutdown_func, transport_reset_func_t* reset_func, void* context)
{
	//INVOKE(0x00430730, transport_register_transition_functions, startup_func, shutdown_func, reset_func, context);

	transport_globals.startup_functions[transport_globals.transition_function_count] = startup_func;
	transport_globals.shutdown_functions[transport_globals.transition_function_count] = shutdown_func;
	transport_globals.reset_functions[transport_globals.transition_function_count] = reset_func;
	transport_globals.transport_function_context[transport_globals.transition_function_count++] = context;
}

void __cdecl transport_reset()
{
	//INVOKE(0x00430780, transport_reset);

	ASSERT(transport_globals.initialized);

	for (int32 function_index = 0; function_index < transport_globals.transition_function_count; function_index++)
	{
		transport_reset_func_t* reset_func = transport_globals.reset_functions[function_index];
		if (reset_func)
		{
			reset_func(transport_globals.transport_function_context[function_index]);
		}
	}
}

void __cdecl transport_shutdown()
{
	//INVOKE(0x004307C0, transport_shutdown);

	if (transport_globals.started)
	{
		network_session_interface_handle_message(_network_message_network_interface_connection_lost);
		for (int32 function_index = 0; function_index < transport_globals.transition_function_count; function_index++)
		{
			transport_shutdown_func_t* shutdown_func = transport_globals.shutdown_functions[function_index];
			if (shutdown_func)
			{
				shutdown_func(transport_globals.transport_function_context[function_index]);
			}
		}

		transport_qos_shutdown();
		transport_globals.started = false;

		::WSACleanup();
	}
}

void __cdecl transport_startup()
{
	//INVOKE(0x00430820, transport_startup);

	if (!transport_globals.started)
	{
		bool success = true;
		bool started = false;

		{
			WSAData info;
			csmemset(&info, 0, sizeof(info));

			int result = ::WSAStartup(MAKEWORD(2, 2), &info);
			if (result != 0)
			{
				//event(_event_error, "networking:transport: WSAStartup() failed; error= %s",
				//	winsock_error_to_string(result));

				success = false;
			}
			else
			{
				started = true;
			}
		}

		if (success)
		{
			transport_globals.started = started;
			transport_security_startup();
			transport_qos_startup();

			for (int32 function_index = 0; function_index < transport_globals.transition_function_count; function_index++)
			{
				transport_startup_func_t* startup_func = transport_globals.startup_functions[function_index];
				if (startup_func)
				{
					startup_func(transport_globals.transport_function_context[function_index]);
				}
			}

			network_session_interface_handle_message(_network_message_network_interface_connected);
		}
		else
		{
			event(_event_error, "networking:transport: transport_startup() failed!");
		}
	}
}