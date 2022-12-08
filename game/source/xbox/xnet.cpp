#include "xbox/xnet.hpp"

#include "cseries/cseries.hpp"

#include <assert.h>
#include <combaseapi.h>

REFERENCE_DECLARE_ARRAY(0x023901B8, s_xnet_entry, xnet_mapping, 51);

void __cdecl XNetCreateKey(s_transport_secure_identifier* out_secure_identifier)
{
	INVOKE(0x0052D610, XNetCreateKey, out_secure_identifier);

	//s_transport_secure_identifier secure_identifier;
	//csmemset(&secure_identifier, 0, sizeof(s_transport_secure_identifier));
	//
	//assert(SUCCEEDED(CoCreateGuid(reinterpret_cast<GUID*>(&secure_identifier))));
	//*out_secure_identifier = secure_identifier;
}

long __cdecl XNetFindEntry(transport_address const* address, s_transport_secure_address* secure_address, bool ignore_invalid)
{
	return INVOKE(0x0052D6E0, XNetFindEntry, address, secure_address, ignore_invalid);

	//long result = -1;
	//for (long entry_index = 0; entry_index < NUMBEROF(xnet_mapping); entry_index++)
	//{
	//	s_xnet_entry& entry = xnet_mapping[entry_index];
	//
	//	if (ignore_invalid && entry.initialized)
	//		break;
	//
	//	if (address && transport_address_equivalent(&entry.address, address))
	//		return entry_index;
	//
	//	if (secure_address && transport_secure_address_compare(&entry.secure_address ,secure_address))
	//		return entry_index;
	//
	//	result = entry_index;
	//}
	//
	//return result;
}

void __cdecl XNetAddEntry(transport_address* address, s_transport_secure_address* secure_address, s_transport_secure_identifier* secure_identifier)
{
	INVOKE(0x0052D7B0, XNetAddEntry, address, secure_address, secure_identifier);

	//long entry_index = XNetFindEntry(address, secure_address, false);
	//if (entry_index == -1)
	//	return;
	//
	//s_xnet_entry& entry = xnet_mapping[entry_index];
	//
	//entry.initialized = true;
	//entry.address = *address;
	//entry.secure_address = *secure_address;
	//entry.secure_identifier = *secure_identifier;
}

bool __cdecl XNetXnAddrToInAddr(s_transport_secure_address* secure_address, s_transport_secure_identifier* secure_identifier, transport_address* out_address)
{
	return INVOKE(0x0052D840, XNetXnAddrToInAddr, secure_address, secure_identifier, out_address);
}

bool __cdecl XNetInAddrToXnAddr(transport_address* address, s_transport_secure_address* out_secure_address)
{
	return INVOKE(0x0052D8F0, XNetInAddrToXnAddr, address, out_secure_address);
}

bool __cdecl _XNetInAddrToXnAddr(transport_address* address, s_transport_secure_address* out_secure_address, s_transport_secure_identifier* out_secure_identifier)
{
	return INVOKE(0x0052D970, _XNetInAddrToXnAddr, address, out_secure_address, out_secure_identifier);
}

void __cdecl XNetRemoveEntry(transport_address* address)
{
	INVOKE(0x0052DA40, XNetRemoveEntry, address);
}

