#pragma once

struct transport_address;
struct c_bitstream;
struct c_network_out_of_band_consumer
{
public:
	virtual bool __cdecl receive_out_of_band_packet(transport_address const* address, c_bitstream* packet);
};
static_assert(sizeof(c_network_out_of_band_consumer) == 0x4);

