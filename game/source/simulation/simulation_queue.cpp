#include "simulation/simulation_queue.hpp"

#include "memory/bitstream.hpp"

void c_simulation_queue::allocate(int32 size, c_simulation_queue_element** element_out)
{
	INVOKE_CLASS_MEMBER(0x00465240, c_simulation_queue, allocate, size, element_out);
}

int32 c_simulation_queue::allocated_count() const
{
	return INVOKE_CLASS_MEMBER(0x00465340, c_simulation_queue, allocated_count);
}

int32 c_simulation_queue::allocated_encoded_size_in_bytes() const
{
	return INVOKE_CLASS_MEMBER(0x00465350, c_simulation_queue, allocated_encoded_size_in_bytes);
}

int32 c_simulation_queue::allocated_new_encoded_size_bytes(int32 size) const
{
	return INVOKE_CLASS_MEMBER(0x004653A0, c_simulation_queue, allocated_new_encoded_size_bytes, size);
}

int32 c_simulation_queue::allocated_size_in_bytes() const
{
	return INVOKE_CLASS_MEMBER(0x00465400, c_simulation_queue, allocated_size_in_bytes);
}

void c_simulation_queue::clear()
{
	INVOKE_CLASS_MEMBER(0x00465410, c_simulation_queue, clear);
}

bool c_simulation_queue::compare(c_simulation_queue* queue) const
{
	return INVOKE_CLASS_MEMBER(0x00465480, c_simulation_queue, compare, queue);
}

bool c_simulation_queue::contains_element_type(e_simulation_queue_element_type element_type) const
{
	return INVOKE_CLASS_MEMBER(0x004655A0, c_simulation_queue, contains_element_type, element_type);
}

void c_simulation_queue::deallocate(c_simulation_queue_element* element)
{
	INVOKE_CLASS_MEMBER(0x004655F0, c_simulation_queue, deallocate, element);
}

bool c_simulation_queue::decode(c_bitstream* packet)
{
	return INVOKE_CLASS_MEMBER(0x00465620, c_simulation_queue, decode, packet);
}

void c_simulation_queue::deque(c_simulation_queue_element** element_out)
{
	INVOKE_CLASS_MEMBER(0x00465760, c_simulation_queue, deque, element_out);
}

void c_simulation_queue::dispose()
{
	INVOKE_CLASS_MEMBER(0x004657C0, c_simulation_queue, dispose);
}

void c_simulation_queue::encode(c_bitstream* packet) const
{
	INVOKE_CLASS_MEMBER(0x004657E0, c_simulation_queue, encode, packet);
}

void c_simulation_queue::enqueue(c_simulation_queue_element* element)
{
	INVOKE_CLASS_MEMBER(0x00465930, c_simulation_queue, enqueue, element);
}

void c_simulation_queue::get_allocation_status(real32* a1, real32* a2) const
{
	INVOKE_CLASS_MEMBER(0x00465970, c_simulation_queue, get_allocation_status, a1, a2);
}

int32 c_simulation_queue::get_element_size_in_bytes(c_simulation_queue_element* element) const
{
	return INVOKE_CLASS_MEMBER(0x004659B0, c_simulation_queue, get_element_size_in_bytes, element);
}

c_simulation_queue_element* c_simulation_queue::get_first_element() const
{
	return INVOKE_CLASS_MEMBER(0x004659C0, c_simulation_queue, get_first_element);
}

c_simulation_queue_element* c_simulation_queue::get_next_element(c_simulation_queue_element* element) const
{
	return INVOKE_CLASS_MEMBER(0x004659E0, c_simulation_queue, get_next_element, element);
}

void c_simulation_queue::initialize()
{
	INVOKE_CLASS_MEMBER(0x00465A00, c_simulation_queue, initialize);
}

int32 c_simulation_queue::queued_count() const
{
	return INVOKE_CLASS_MEMBER(0x00465A30, c_simulation_queue, queued_count);
}

int32 c_simulation_queue::queued_encoded_size_in_bytes() const
{
	return INVOKE_CLASS_MEMBER(0x00465A40, c_simulation_queue, queued_encoded_size_in_bytes);
}

int32 c_simulation_queue::queued_new_encoded_size_bytes(int32 size) const
{
	return INVOKE_CLASS_MEMBER(0x00465A90, c_simulation_queue, queued_new_encoded_size_bytes, size);
}

int32 c_simulation_queue::queued_size_in_bytes() const
{
	return INVOKE_CLASS_MEMBER(0x00465AF0, c_simulation_queue, queued_size_in_bytes);
}

void c_simulation_queue::transfer_elements(c_simulation_queue* simulation_queue)
{
	INVOKE_CLASS_MEMBER(0x00465B00, c_simulation_queue, transfer_elements, simulation_queue);
}

