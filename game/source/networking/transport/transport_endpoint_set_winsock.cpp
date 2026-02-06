#include "networking/transport/transport_endpoint_set_winsock.hpp"

#include "cseries/cseries.hpp"

#include <WinSock2.h>

struct transport_endpoint_set
{
	fd_set read_fds;
	transport_endpoint** endpoint_array;
	int16 array_size;
	int16 max_index;
	int16 current_index;
	bool changed;
};
COMPILE_ASSERT(sizeof(transport_endpoint_set) == 0x110);

int16 __cdecl get_next_available_set_array_index(transport_endpoint_set* set)
{
	return INVOKE(0x004340F0, get_next_available_set_array_index, set);
}

int __cdecl poll_endpoint_array_compare_proc(const void* a, const void* b)
{
	return INVOKE(0x00434120, poll_endpoint_array_compare_proc, a, b);
}

bool __cdecl transport_endpoint_set_add_endpoint(transport_endpoint_set* set, transport_endpoint* endpoint)
{
	return INVOKE(0x00434150, transport_endpoint_set_add_endpoint, set, endpoint);
}

int16 __cdecl transport_endpoint_set_count_endpoints(transport_endpoint_set* set)
{
	return INVOKE(0x004341E0, transport_endpoint_set_count_endpoints, set);
}

transport_endpoint_set* __cdecl transport_endpoint_set_create(int16 maximum_endpoints)
{
	return INVOKE(0x004341F0, transport_endpoint_set_create, maximum_endpoints);
}

void __cdecl transport_endpoint_set_delete(transport_endpoint_set* set)
{
	INVOKE(0x00434270, transport_endpoint_set_delete, set);
}

transport_endpoint* __cdecl transport_endpoint_set_get_next_endpoint(transport_endpoint_set* set)
{
	return INVOKE(0x00434290, transport_endpoint_set_get_next_endpoint, set);
}

bool __cdecl transport_endpoint_set_poll(transport_endpoint_set* set, uns16 milliseconds_timeout)
{
	return INVOKE(0x004342D0, transport_endpoint_set_poll, set, milliseconds_timeout);
}

bool __cdecl transport_endpoint_set_remove_endpoint(transport_endpoint_set* set, transport_endpoint* endpoint)
{
	return INVOKE(0x004344A0, transport_endpoint_set_remove_endpoint, set, endpoint);
}

void __cdecl transport_endpoint_set_rewind(transport_endpoint_set* set)
{
	INVOKE(0x00434560, transport_endpoint_set_rewind, set);
}

const char* __cdecl winsock_error_to_string(int err)
{
	//return INVOKE(0x00434580, winsock_error_to_string, err);

	const char* error_string = winsock_error_to_string_no_default(err);
	if (!error_string)
	{
		static char errbuf[256]{};
		csnzprintf(errbuf, NUMBEROF(errbuf), "<unknown winsock error %d>", err);
	}

	return error_string;
}

const char* __cdecl winsock_error_to_string_no_default(int err)
{
#define ERROR_CASE(code) case (int)code: return #code
	switch (err)
	{
	ERROR_CASE(WSA_INVALID_EVENT);
	ERROR_CASE(WSA_MAXIMUM_WAIT_EVENTS);
	ERROR_CASE(WSA_NOT_ENOUGH_MEMORY);
	ERROR_CASE(WSA_INVALID_HANDLE);
	ERROR_CASE(WSA_WAIT_FAILED);
	ERROR_CASE(WSA_INVALID_PARAMETER);
	ERROR_CASE(WSA_WAIT_IO_COMPLETION);
	ERROR_CASE(WSA_WAIT_TIMEOUT);
	ERROR_CASE(WSA_OPERATION_ABORTED);
	ERROR_CASE(WSA_IO_INCOMPLETE);
	ERROR_CASE(WSA_IO_PENDING);
	ERROR_CASE(WSAEINTR);
	ERROR_CASE(WSAEBADF);
	ERROR_CASE(WSAEACCES);
	ERROR_CASE(WSAEFAULT);
	ERROR_CASE(WSAEINVAL);
	ERROR_CASE(WSAEMFILE);
	ERROR_CASE(WSAEWOULDBLOCK);
	ERROR_CASE(WSAEINPROGRESS);
	ERROR_CASE(WSAEALREADY);
	ERROR_CASE(WSAENOTSOCK);
	ERROR_CASE(WSAEDESTADDRREQ);
	ERROR_CASE(WSAEMSGSIZE);
	ERROR_CASE(WSAEPROTOTYPE);
	ERROR_CASE(WSAENOPROTOOPT);
	ERROR_CASE(WSAEPROTONOSUPPORT);
	ERROR_CASE(WSAESOCKTNOSUPPORT);
	ERROR_CASE(WSAEOPNOTSUPP);
	ERROR_CASE(WSAEPFNOSUPPORT);
	ERROR_CASE(WSAEAFNOSUPPORT);
	ERROR_CASE(WSAEADDRINUSE);
	ERROR_CASE(WSAEADDRNOTAVAIL);
	ERROR_CASE(WSAENETDOWN);
	ERROR_CASE(WSAENETUNREACH);
	ERROR_CASE(WSAENETRESET);
	ERROR_CASE(WSAECONNABORTED);
	ERROR_CASE(WSAECONNRESET);
	ERROR_CASE(WSAENOBUFS);
	ERROR_CASE(WSAEISCONN);
	ERROR_CASE(WSAENOTCONN);
	ERROR_CASE(WSAESHUTDOWN);
	ERROR_CASE(WSAETOOMANYREFS);
	ERROR_CASE(WSAETIMEDOUT);
	ERROR_CASE(WSAECONNREFUSED);
	ERROR_CASE(WSAELOOP);
	ERROR_CASE(WSAENAMETOOLONG);
	ERROR_CASE(WSAEHOSTDOWN);
	ERROR_CASE(WSAEHOSTUNREACH);
	ERROR_CASE(WSAENOTEMPTY);
	ERROR_CASE(WSAEPROCLIM);
	ERROR_CASE(WSAEUSERS);
	ERROR_CASE(WSAEDQUOT);
	ERROR_CASE(WSAESTALE);
	ERROR_CASE(WSAEREMOTE);
	ERROR_CASE(WSASYSNOTREADY);
	ERROR_CASE(WSAVERNOTSUPPORTED);
	ERROR_CASE(WSANOTINITIALISED);
	ERROR_CASE(WSAEDISCON);
	ERROR_CASE(WSAENOMORE);
	ERROR_CASE(WSAECANCELLED);
	ERROR_CASE(WSAEINVALIDPROCTABLE);
	ERROR_CASE(WSAEINVALIDPROVIDER);
	ERROR_CASE(WSAEPROVIDERFAILEDINIT);
	ERROR_CASE(WSASYSCALLFAILURE);
	ERROR_CASE(WSASERVICE_NOT_FOUND);
	ERROR_CASE(WSATYPE_NOT_FOUND);
	ERROR_CASE(WSA_E_NO_MORE);
	ERROR_CASE(WSA_E_CANCELLED);
	ERROR_CASE(WSAEREFUSED);
	ERROR_CASE(WSAHOST_NOT_FOUND);
	ERROR_CASE(WSATRY_AGAIN);
	ERROR_CASE(WSANO_RECOVERY);
	ERROR_CASE(WSANO_DATA);
	ERROR_CASE(WSA_QOS_RECEIVERS);
	ERROR_CASE(WSA_QOS_SENDERS);
	ERROR_CASE(WSA_QOS_NO_SENDERS);
	ERROR_CASE(WSA_QOS_NO_RECEIVERS);
	ERROR_CASE(WSA_QOS_REQUEST_CONFIRMED);
	ERROR_CASE(WSA_QOS_ADMISSION_FAILURE);
	ERROR_CASE(WSA_QOS_POLICY_FAILURE);
	ERROR_CASE(WSA_QOS_BAD_STYLE);
	ERROR_CASE(WSA_QOS_BAD_OBJECT);
	ERROR_CASE(WSA_QOS_TRAFFIC_CTRL_ERROR);
	ERROR_CASE(WSA_QOS_GENERIC_ERROR);
	}
	return nullptr;

#undef ERROR_CASE
}

