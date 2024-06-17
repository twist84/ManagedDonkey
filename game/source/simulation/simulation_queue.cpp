#include "simulation/simulation_queue.hpp"

#include "memory/bitstream.hpp"

void c_simulation_queue::allocate(long size, c_simulation_queue_element** element_out)
{
	DECLFUNC(0x00465240, void, __thiscall, c_simulation_queue*, long, c_simulation_queue_element**)(this, size, element_out);
}

long c_simulation_queue::allocated_count() const
{
	return DECLFUNC(0x00465340, long, __thiscall, c_simulation_queue const*)(this);
}

long c_simulation_queue::allocated_encoded_size_in_bytes() const
{
	return DECLFUNC(0x00465350, long, __thiscall, c_simulation_queue const*)(this);
}

long c_simulation_queue::allocated_new_encoded_size_bytes(long size) const
{
	return DECLFUNC(0x004653A0, long, __thiscall, c_simulation_queue const*, long)(this, size);
}

long c_simulation_queue::allocated_size_in_bytes() const
{
	return DECLFUNC(0x00465400, long, __thiscall, c_simulation_queue const*)(this);
}

void c_simulation_queue::clear()
{
	DECLFUNC(0x00465410, void, __thiscall, c_simulation_queue*)(this);
}

bool c_simulation_queue::compare(c_simulation_queue* queue) const
{
	return DECLFUNC(0x00465480, bool, __thiscall, c_simulation_queue const*, c_simulation_queue*)(this, queue);
}

bool c_simulation_queue::contains_element_type(e_simulation_queue_element_type element_type) const
{
	return DECLFUNC(0x004655A0, bool, __thiscall, c_simulation_queue const*, e_simulation_queue_element_type)(this, element_type);
}

void c_simulation_queue::deallocate(c_simulation_queue_element* element)
{
	DECLFUNC(0x004655F0, void, __thiscall, c_simulation_queue*, c_simulation_queue_element*)(this, element);
}

bool c_simulation_queue::decode(c_bitstream* packet)
{
	return DECLFUNC(0x00465620, bool, __thiscall, c_simulation_queue*, c_bitstream*)(this, packet);
}

void c_simulation_queue::deque(c_simulation_queue_element** element_out)
{
	DECLFUNC(0x00465760, void, __thiscall, c_simulation_queue*, c_simulation_queue_element**)(this, element_out);
}

void c_simulation_queue::dispose()
{
	DECLFUNC(0x004657C0, void, __thiscall, c_simulation_queue*)(this);
}

void c_simulation_queue::encode(c_bitstream* packet) const
{
	DECLFUNC(0x004657E0, void, __thiscall, c_simulation_queue const*, c_bitstream*)(this, packet);
}

void c_simulation_queue::enqueue(c_simulation_queue_element* element)
{
	DECLFUNC(0x00465930, void, __thiscall, c_simulation_queue*, c_simulation_queue_element*)(this, element);
}

void c_simulation_queue::get_allocation_status(real* a1, real* a2) const
{
	DECLFUNC(0x00465970, void, __thiscall, c_simulation_queue const*, real*, real*)(this, a1, a2);
}

long c_simulation_queue::get_element_size_in_bytes(c_simulation_queue_element* element) const
{
	return DECLFUNC(0x004659B0, long, __thiscall, c_simulation_queue const*, c_simulation_queue_element*)(this, element);
}

c_simulation_queue_element* c_simulation_queue::get_first_element() const
{
	return DECLFUNC(0x004659C0, c_simulation_queue_element*, __thiscall, c_simulation_queue const*)(this);
}

c_simulation_queue_element* c_simulation_queue::get_next_element(c_simulation_queue_element* element) const
{
	return DECLFUNC(0x004659E0, c_simulation_queue_element*, __thiscall, c_simulation_queue const*)(this);
}

void c_simulation_queue::initialize()
{
	DECLFUNC(0x00465A00, void, __thiscall, c_simulation_queue*)(this);
}

long c_simulation_queue::queued_count() const
{
	return DECLFUNC(0x00465A30, long, __thiscall, c_simulation_queue const*)(this);
}

long c_simulation_queue::queued_encoded_size_in_bytes() const
{
	return DECLFUNC(0x00465A40, long, __thiscall, c_simulation_queue const*)(this);
}

long c_simulation_queue::queued_new_encoded_size_bytes(long size) const
{
	return DECLFUNC(0x00465A90, long, __thiscall, c_simulation_queue const*, long)(this, size);
}

long c_simulation_queue::queued_size_in_bytes() const
{
	return DECLFUNC(0x00465AF0, long, __thiscall, c_simulation_queue const*)(this);
}

void c_simulation_queue::transfer_elements(c_simulation_queue* simulation_queue)
{
	DECLFUNC(0x00465B00, void, __thiscall, c_simulation_queue*, c_simulation_queue*)(this, simulation_queue);
}

