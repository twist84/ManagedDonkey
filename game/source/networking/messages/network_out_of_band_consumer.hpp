#pragma once

struct transport_address;
struct c_bitstream;
struct c_network_out_of_band_consumer
{
public:
	virtual bool receive_out_of_band_packet(const transport_address* incoming_address, c_bitstream* packet) = 0;

	c_network_out_of_band_consumer();
};
static_assert(sizeof(c_network_out_of_band_consumer) == 0x4);

