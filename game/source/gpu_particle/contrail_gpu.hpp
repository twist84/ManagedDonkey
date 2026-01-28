#pragma once

#include "cseries/cseries.hpp"

class c_contrail_states;

struct s_gpu_constant_table;

class c_contrail_gpu :
	public s_datum_header
{
public:
	static c_contrail_gpu* __cdecl get(int32 datum_index);
	bool get_alive(void) const;
	void render(const c_contrail_states* contrail_states) const;
	void set_shader_state(s_gpu_constant_table* constant_table, const c_contrail_states* contrail_states) const;
	void set_shader_strip(s_gpu_constant_table* constant_table) const;

	static void __cdecl shell_initialize(void);
	static void __cdecl spawn_all(void);

	struct s_row :
		s_datum_header
	{
		static s_row* get(int32 datum_index);
		static c_contrail_gpu::s_row* __cdecl try_get(int32 datum_index);

		int32 m_prev;
		int32 m_next;
		uns16 m_row;
		uns8 m_used_count;
		real32 m_lifespan;
	};
	COMPILE_ASSERT(sizeof(s_row) == 0x14);

	int32 m_contrail_index;
	int32 m_row_head;
	uns16 m_profile_count;
	uns16 m_flags;

	static void*& x_stream_vertex_data;
	static uns32& x_stream_vertex_stride;
	static bool& x_debug_synchronous;
	static uns32& x_stream_offset;
	static uns32& x_stream_previous_offset;
	static uns32& x_stream_vertex_count;
};
COMPILE_ASSERT(sizeof(c_contrail_gpu) == 0x10);

