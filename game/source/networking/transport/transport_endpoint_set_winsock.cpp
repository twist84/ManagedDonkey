#include "networking/transport/transport_endpoint_set_winsock.hpp"

#include "cseries/cseries.hpp"

char const* winsock_error_to_string_no_default(int error)
{
    // $TODO: implement
    return nullptr;
}

char const* winsock_error_to_string(int error)
{
    static c_static_string<256> winsock_error_string;

    char const* result = winsock_error_to_string_no_default(error);
    if (!result)
        return result;

    winsock_error_string.print("<unknown winsock error %d>", error);
    return winsock_error_string.get_string();
}

