#include "xbox/xnet.hpp"

#include "cseries/cseries.hpp"
#include "networking/logic/network_broadcast_search.hpp"
#include "networking/tools/http_client.hpp"

#include <combaseapi.h>

REFERENCE_DECLARE_ARRAY(0x023901B8, s_xnet_entry, xnet_mapping, 51);

// based on https://github.com/yokimklein/AnvilDedicatedServer/blob/master/hf2p_server/source/networking/transport/transport_shim.cpp

// called from `transport_secure_key_create`
void __cdecl XNetCreateKey(s_transport_secure_identifier* out_secure_identifier)
{
	//INVOKE(0x0052D610, XNetCreateKey, out_secure_identifier);

	s_transport_secure_identifier secure_identifier;
	csmemset(&secure_identifier, 0, sizeof(s_transport_secure_identifier));

	ASSERT(SUCCEEDED(CoCreateGuid(reinterpret_cast<GUID*>(&secure_identifier))));
	*out_secure_identifier = secure_identifier;
}

// called from `XNetAddEntry`
long __cdecl XNetFindEntry(transport_address const* address, s_transport_secure_address const* secure_address, bool ignore_invalid)
{
	//return INVOKE(0x0052D6E0, XNetFindEntry, address, secure_address, ignore_invalid);

	long result = -1;
	for (long entry_index = 0; entry_index < NUMBEROF(xnet_mapping); entry_index++)
	{
		s_xnet_entry& entry = xnet_mapping[entry_index];

		if (ignore_invalid && !entry.initialized)
			continue;

		if (address && transport_address_equivalent(&entry.address, address))
			return entry_index;

		if (secure_address && transport_secure_address_compare(&entry.secure_address, secure_address))
			return entry_index;

		if (!ignore_invalid && !entry.initialized)
		{
			if (result == -1)
				result = entry_index;
		}
	}

	return result;
}

// transport security
// used in `0x00430CD0` called from `c_network_session::join_accept`
void __cdecl XNetAddEntry(transport_address const* address, s_transport_secure_address const* secure_address, s_transport_secure_identifier const* secure_identifier)
{
	//INVOKE(0x0052D7B0, XNetAddEntry, address, secure_address, secure_identifier);

	long entry_index = XNetFindEntry(address, secure_address, false);
	if (entry_index == -1)
		return;

	s_xnet_entry& entry = xnet_mapping[entry_index];
	entry.initialized = true;
	entry.address = *address;
	entry.secure_address = *secure_address;
	entry.secure_identifier = *secure_identifier;
}

// called from `transport_secure_address_decode`
bool __cdecl XNetXnAddrToInAddr(s_transport_secure_address const* secure_address, s_transport_secure_identifier const* secure_identifier, transport_address* out_address)
{
	//return INVOKE(0x0052D840, XNetXnAddrToInAddr, secure_address, secure_identifier, out_address);

	long entry_index = XNetFindEntry(nullptr, secure_address, true);
	if (entry_index == -1)
		return false;

	s_xnet_entry& entry = xnet_mapping[entry_index];
	*out_address = entry.address;

	return true;
}

// called from `transport_secure_address_retrieve`
bool __cdecl _XNetInAddrToXnAddr(transport_address const* address, s_transport_secure_address* out_secure_address)
{
	//return INVOKE(0x0052D8F0, XNetInAddrToXnAddr, address, out_secure_address);

	long entry_index = XNetFindEntry(address, nullptr, true);
	if (entry_index == -1)
		return false;

	s_xnet_entry& entry = xnet_mapping[entry_index];
	*out_secure_address = entry.secure_address;

	return true;
}

// called from `transport_secure_identifier_retrieve`
bool __cdecl XNetInAddrToXnAddr(transport_address const* address, s_transport_secure_address* out_secure_address, s_transport_secure_identifier* out_secure_identifier)
{
	//return INVOKE(0x0052D970, XNetInAddrToXnAddr, address, out_secure_address, out_secure_identifier);

	long entry_index = XNetFindEntry(address, nullptr, true);
	if (entry_index == -1)
		return false;

	s_xnet_entry& entry = xnet_mapping[entry_index];
	*out_secure_address = entry.secure_address;
	*out_secure_identifier = entry.secure_identifier;

	return true;
}

// transport security
// used in `0x00430E90` called from `c_network_channel::deallocate`
void __cdecl XNetRemoveEntry(transport_address const* address)
{
	//INVOKE(0x0052DA40, XNetRemoveEntry, address);

	long entry_index = XNetFindEntry(address, nullptr, true);
	if (entry_index == -1)
		return;

	s_xnet_entry& entry = xnet_mapping[entry_index];
	entry.initialized = false;
}

struct s_external_ip
{
	c_http_client http_client;
	c_http_get_stream get_stream;
	transport_address address;
};

s_external_ip g_external_ip = { .address = transport_address() };

dword get_external_ip()
{
	if (g_external_ip.address.ipv4_address == 0)
	{
		char const* host = "ifconfig.me";
		transport_address_from_host(host, g_external_ip.address);

		g_external_ip.http_client;
		g_external_ip.get_stream;
		g_external_ip.get_stream.add_header("Host", host);
		g_external_ip.get_stream.add_header("Connection", "close");
		g_external_ip.get_stream.add_header("User-Agent", "DonkeyClient");

		if (g_external_ip.http_client.start(&g_external_ip.get_stream, g_external_ip.address.ipv4_address, 80, "/ip", false))
		{
			bool completed_successfully = false;
			char response_content_buffer[4096]{};
			long http_response_code = 0;

			long seconds = 1800;
			dword v7 = 1000 * seconds + system_milliseconds();
			while (system_milliseconds() < v7)
			{
				long response_content_buffer_count = NUMBEROF(response_content_buffer);
				if (!g_external_ip.http_client.do_work(&completed_successfully, response_content_buffer, &response_content_buffer_count, &http_response_code))
				{
					g_external_ip.http_client.stop();
					break;
				}

				Sleep(0);

				if (completed_successfully)
					break;
			}

			if (completed_successfully)
				transport_address_from_host(response_content_buffer, g_external_ip.address);
		}
		g_external_ip.http_client.stop();
	}

	return g_external_ip.address.ipv4_address;
}

