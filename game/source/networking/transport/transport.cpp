#include "networking/transport/transport.hpp"

#include "cseries/cseries_console.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/online/online_error.hpp"
#include "networking/transport/transport_security.hpp"

#include <string.h>
#include <WinSock2.h>

REFERENCE_DECLARE(0x0199FA28, s_transport_globals, transport_globals);

HOOK_DECLARE(0x00430630, transport_available);
HOOK_DECLARE(0x00430650, transport_dispose);

bool __cdecl transport_available()
{
	return transport_globals.initialized && transport_globals.winsock_initialized;
}

void __cdecl transport_dispose()
{
	if (transport_globals.initialized)
	{
		transport_shutdown();
		//transport_qos_dispose();
		transport_globals.transition_function_count = false;
		transport_globals.initialized = false;
	}
}

long __cdecl transport_get_packet_maximum_payload(long type)
{
	return 4096;
}

long __cdecl transport_get_packet_overhead(long type)
{
	if (type >= 0)
	{
		if (type <= 1)
			return 0x1C;

		if (type == 2)
			return 0x28;
	}

	return 0;
}

void __cdecl transport_global_update()
{
	if (transport_globals.initialized)
	{
		bool available = transport_network_available();
		if (transport_globals.is_started != available)
		{
			transport_globals.is_started = available;
			if (available)
			{
				c_console::write_line("networking:transport: network interface connection restored, resetting networking");
				transport_startup();
			}
			else
			{
				c_console::write_line("networking:transport: network interface connection lost");
				transport_shutdown();
			}
		}

		if (transport_globals.is_started)
			transport_secure_address_resolve();
	}
}

void __cdecl transport_initialize()
{
	memset(&transport_globals, 0, sizeof(transport_globals));
	transport_security_initialize();
	//transport_qos_initialize();
	transport_globals.initialized = true;

	if (transport_network_available())
		transport_startup();
}

bool __cdecl transport_network_available()
{
	return transport_globals.initialized;
}

void __cdecl transport_register_transition_functions(transport_startup_function_t startup_function, transport_shutdown_function_t shutdown_function, transport_reset_function_t reset_function, void* callback_data)
{
	transport_globals.startup_functions[transport_globals.transition_function_count] = startup_function;
	transport_globals.shutdown_functions[transport_globals.transition_function_count] = shutdown_function;
	transport_globals.reset_functions[transport_globals.transition_function_count] = reset_function;
	transport_globals.callback_datas[transport_globals.transition_function_count++] = callback_data;
}

void __cdecl transport_reset()
{
	ASSERT(transport_globals.initialized);

	for (long i = 0; i < transport_globals.transition_function_count; i++)
	{
		transport_reset_function_t reset_function = transport_globals.reset_functions[i];
		if (reset_function)
			reset_function(transport_globals.callback_datas[i]);
	}
}

void __cdecl transport_shutdown()
{
	if (transport_globals.winsock_initialized)
	{
		network_session_interface_handle_message(3);
		for (long i = 0; i < transport_globals.transition_function_count; i++)
		{
			transport_shutdown_function_t shutdown_function = transport_globals.shutdown_functions[i];
			if (shutdown_function)
				shutdown_function(transport_globals.callback_datas[i]);
		}

		//transport_qos_shutdown();
		transport_globals.winsock_initialized = false;

		WSACleanup();
	}
}

void __cdecl transport_startup()
{
	WSAData wsa_data;

	if (!transport_globals.winsock_initialized)
	{
		memset(&wsa_data, 0, sizeof(wsa_data));

		int wsa_startup_result = WSAStartup(2u, &wsa_data);
		if (wsa_startup_result)
		{
			c_console::write_line("networking:transport: WSAStartup() failed; error= %s", online_error_get_string(wsa_startup_result));
		}
		else
		{
			c_console::write_line("networking:transport: Winsock initialized");

			transport_globals.winsock_initialized = true;
			transport_security_startup();
			//transport_qos_startup();

			for (long i = 0; i < transport_globals.transition_function_count; i++)
			{
				transport_startup_function_t startup_function = transport_globals.startup_functions[i];
				if (startup_function)
					startup_function(transport_globals.callback_datas[i]);
			}

			network_session_interface_handle_message(2);
			c_console::write_line("networking:transport: Trasport global started");
		}
	}
}