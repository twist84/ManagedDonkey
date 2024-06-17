#pragma once

#include "cseries/cseries.hpp"

enum e_simulation_queue_element_type
{
	_simulation_queue_element_type_none = 0,
	_simulation_queue_element_type_event,
	_simulation_queue_element_type_entity_creation,
	_simulation_queue_element_type_entity_update,
	_simulation_queue_element_type_entity_deletion,
	_simulation_queue_element_type_entity_promotion,
	_simulation_queue_element_type_game_global_event,
	_simulation_queue_element_type_game_global_bookkeeping_event,
	_simulation_queue_element_type_player_event,
	_simulation_queue_element_type_player_update,
	_simulation_queue_element_type_gamestates_delete,
	_simulation_queue_element_type_sandbox_event,

	_simulation_queue_element_type_unknown12,

	k_simulation_queue_element_type_count
};

struct c_simulation_queue_element
{
	c_enum<e_simulation_queue_element_type, long, _simulation_queue_element_type_none, k_simulation_queue_element_type_count> m_type;
	c_simulation_queue_element* m_next;
	long m_data_size;
	byte* m_data;
};
static_assert(sizeof(c_simulation_queue_element) == 0x10);

struct c_bitstream;
struct c_simulation_queue
{
public:
	void allocate(long size, c_simulation_queue_element** element_out);
	long allocated_count() const;
	long allocated_encoded_size_in_bytes() const;
	long allocated_new_encoded_size_bytes(long size) const;
	long allocated_size_in_bytes() const;
	void clear();
	bool compare(c_simulation_queue* queue) const;
	bool contains_element_type(e_simulation_queue_element_type element_type) const;
	void deallocate(c_simulation_queue_element* element);
	bool decode(c_bitstream* packet);
	void deque(c_simulation_queue_element** element_out);
	void dispose();
	void encode(c_bitstream* packet) const;
	void enqueue(c_simulation_queue_element* element);
	void get_allocation_status(real* a1, real* a2) const;
	long get_element_size_in_bytes(c_simulation_queue_element* element) const;
	c_simulation_queue_element* get_first_element() const;
	c_simulation_queue_element* get_next_element(c_simulation_queue_element* element) const;
	void initialize();
	long queued_count() const;
	long queued_encoded_size_in_bytes() const;
	long queued_new_encoded_size_bytes(long size) const;
	long queued_size_in_bytes() const;
	void transfer_elements(c_simulation_queue* simulation_queue);

protected:
	bool m_initialized;
	long m_allocated_count;
	long m_allocated_size;
	long m_queued_count;
	long m_size;
	c_simulation_queue_element* m_head;
	c_simulation_queue_element* m_elements;
};
static_assert(sizeof(c_simulation_queue) == 0x1C);

