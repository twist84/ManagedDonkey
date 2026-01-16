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

struct s_simulation_queue_element
{
	e_simulation_queue_element_type type;
	s_simulation_queue_element* next;
	int32 data_size;
	uns8* data;
};
COMPILE_ASSERT(sizeof(s_simulation_queue_element) == 0x10);

class c_bitstream;
class c_simulation_queue
{
public:
	void allocate(int32 size, s_simulation_queue_element** element_out);
	int32 allocated_count() const;
	int32 allocated_encoded_size_in_bytes() const;
	int32 allocated_new_encoded_size_bytes(int32 size) const;
	int32 allocated_size_in_bytes() const;
	void clear();
	bool compare(c_simulation_queue* queue) const;
	bool contains_element_type(e_simulation_queue_element_type element_type) const;
	void deallocate(s_simulation_queue_element* element);
	bool decode(c_bitstream* packet);
	void deque(s_simulation_queue_element** element_out);
	void dispose();
	void encode(c_bitstream* packet) const;
	void enqueue(s_simulation_queue_element* element);
	void get_allocation_status(real32* a1, real32* a2) const;
	int32 get_element_size_in_bytes(s_simulation_queue_element* element) const;
	s_simulation_queue_element* get_first_element() const;
	s_simulation_queue_element* get_next_element(s_simulation_queue_element* element) const;
	void initialize();
	int32 queued_count() const;
	int32 queued_encoded_size_in_bytes() const;
	int32 queued_new_encoded_size_bytes(int32 size) const;
	int32 queued_size_in_bytes() const;
	void transfer_elements(c_simulation_queue* simulation_queue);

protected:
	bool m_initialized;
	int32 m_allocated_count;
	int32 m_allocated_size;
	int32 m_queued_count;
	int32 m_size;
	s_simulation_queue_element* m_head;
	s_simulation_queue_element* m_elements;
};
COMPILE_ASSERT(sizeof(c_simulation_queue) == 0x1C);

