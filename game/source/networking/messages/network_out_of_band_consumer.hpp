#pragma once

struct transport_address;
class c_bitstream;
class c_network_out_of_band_consumer
{
public:
	virtual bool receive_out_of_band_packet(const transport_address* incoming_address, c_bitstream* packet) = 0;

	c_network_out_of_band_consumer();
};
COMPILE_ASSERT(sizeof(c_network_out_of_band_consumer) == 0x4);

