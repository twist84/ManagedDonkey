#include "gpu_particle/contrail_gpu.hpp"

#include "effects/contrails.hpp"
#include "math/function_definitions.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"
#include "render_methods/render_method_definitions.hpp"
#include "render_methods/render_method_submit.hpp"
#include "shell/shell.hpp"
#include "sky_atm/atmosphere.hpp"

#include <d3d9.h>

class c_editable_property_base
{
public:
	byte m_input_index;
	byte m_range_input_index;
	byte m_output_modifier_type;
	byte m_output_modifier_input_index;
	c_function_definition m_function;
	real32 m_constant_value;
	uns8 m_flags;
	byte DSFDSGLKJ[3];
};
COMPILE_ASSERT(sizeof(c_editable_property_base) == 0x20);

class c_editable_property_real_point2d :
	public c_editable_property_base
{
public:
	real_point2d m_starting_interpolant;
	real_point2d m_ending_interpolant;
};
COMPILE_ASSERT(sizeof(c_editable_property_real_point2d) == 0x30);

class c_editable_property_real_vector3d :
	public c_editable_property_base
{
public:
	real_vector3d m_starting_interpolant;
	real_vector3d m_ending_interpolant;
};
COMPILE_ASSERT(sizeof(c_editable_property_real_vector3d) == 0x38);

class c_contrail_property_real :
	public c_editable_property_base
{
};

class c_contrail_property_real_point2d :
	public c_editable_property_real_point2d
{
};

class c_contrail_property_real_vector3d :
	public c_editable_property_real_vector3d
{
};

class c_contrail_property_real_rgb_color :
	public c_editable_property_base
{
};

class c_render_method_shader_contrail :
	public c_render_method
{
};

class c_contrail_definition
{
public:
	enum e_appearance_flags
	{
		_tint_from_lightmap_bit = 0,
		_double_sided_bit,
		_profile_opacity_from_scale_a_bit,
		_random_u_offset_bit,
		_random_v_offset_bit,

		k_number_of_on_disk_appearance_flags,

		_origin_faded_bit = k_number_of_on_disk_appearance_flags,
		_edge_faded_bit,
		_fogged_bit,

		k_number_of_appearance_flags,
	};

	enum e_profile
	{
		_profile_ribbon = 0,
		_profile_cross,
		_profile_ngon,
		_profile_max,
	};

	enum e_property
	{
		_contrail_property_emission_rate = 0,
		_contrail_property_profile_lifespan,
		_contrail_property_profile_self_acceleration,
		_contrail_property_profile_size,
		_contrail_property_profile_offset,
		_contrail_property_profile_rotation,
		_contrail_property_profile_rotation_rate,
		_contrail_property_profile_color,
		_contrail_property_profile_alpha,
		_contrail_property_profile_alpha2,
		_contrail_property_profile_black_point,
		_contrail_property_profile_palette,
		_contrail_property_profile_intensity,

		k_property_count,
	};

	struct s_gpu_data
	{
		s_tag_block m_properties;
		s_tag_block m_functions;
		s_tag_block m_colors;
	};

public:
	bool get_double_sided(void) const
	{
		return TEST_BIT(m_appearance_flags, _double_sided_bit);
	}

	bool get_fogged(void) const
	{
		return TEST_BIT(m_appearance_flags, _fogged_bit);
	}

public:
	string_id m_name;
	real32 m_origin_fade_range;
	real32 m_origin_fade_cutoff;
	real32 m_edge_fade_range;
	real32 m_edge_fade_cutoff;
	real32 m_lod_in_distance;
	real32 m_lod_in_feather_distance;
	real32 m_lod_out_distance;
	real32 m_lod_out_feather_distance;
	c_contrail_property_real m_emission_rate;
	c_contrail_property_real m_profile_lifespan;
	c_contrail_property_real_vector3d m_profile_self_acceleration;
	c_contrail_property_real m_profile_size;
	c_contrail_property_real_point2d m_profile_offset;
	c_contrail_property_real m_profile_rotation;
	c_contrail_property_real m_profile_rotation_rate;
	uns16 m_appearance_flags;
	uns8 m_profile_type;
	uns8 m_ngon_sides;
	c_render_method_shader_contrail m_shader;
	real_vector2d m_uv_tiling_rate;
	real_vector2d m_uv_scroll_rate;
	c_contrail_property_real_rgb_color m_profile_color;
	c_contrail_property_real m_profile_alpha;
	c_contrail_property_real m_profile_alpha2;
	c_contrail_property_real m_profile_black_point;
	c_contrail_property_real m_profile_palette;
	c_contrail_property_real m_profile_intensity;
	uns32 m_constant_per_profile_properties;
	uns32 m_used_states;
	s_gpu_data m_gpu_data;
};
COMPILE_ASSERT(sizeof(c_contrail_definition) == 0x26C);

struct s_gpu_constant_table
{
	c_rasterizer_constant_table_definition* definition;
	byte __data4[0x8];
	byte __dataC[0x1C];
};
COMPILE_ASSERT(sizeof(s_gpu_constant_table) == 0x28);

struct s_gpu_global_vertex_shader
{
	void initialize(const s_tag_reference* _shader_reference);

	s_tag_reference* shader_reference;
	s_gpu_constant_table constant_table;
	int32 vertex_type_index;
	int32 __unknown30;
};
COMPILE_ASSERT(sizeof(s_gpu_global_vertex_shader) == 0x34);

__interface IDirect3DVertexBuffer9;

class c_rasterizer_vertex_buffer
{
public:
	uns8 m_declaration_type;
	uns8 m_stride;
	uns16 m_count;
	IDirect3DVertexBuffer9* m_d3d_buffer;
	void* m_src_buffer;
};

HOOK_DECLARE_CLASS_MEMBER(0x00A57DC0, c_contrail_gpu, render);

REFERENCE_DECLARE(0x018F3A8C, bool, effects_enabled);
REFERENCE_DECLARE(0x018F3A8F, bool, effects_render);

REFERENCE_DECLARE(0x01917322, bool, contrail_render);
REFERENCE_DECLARE(0x0191736C, c_rasterizer_vertex_buffer, x_overall_storage);
REFERENCE_DECLARE(0x0191746C, s_gpu_global_vertex_shader, x_render_vertex_shader);
REFERENCE_DECLARE(0x0191C9AC, real_vector4d*, constants);

REFERENCE_DECLARE(0x0510FD20, c_rasterizer_index_buffer, x_index_buffer);
REFERENCE_DECLARE(0x0510FD30, c_rasterizer_vertex_buffer, x_vertex_buffer0);
REFERENCE_DECLARE(0x0510FD3C, c_rasterizer_vertex_buffer, x_vertex_buffer1);
REFERENCE_DECLARE(0x0510FD48, void*, c_contrail_gpu::x_stream_vertex_data);
REFERENCE_DECLARE(0x0510FD4C, uns32, c_contrail_gpu::x_stream_vertex_stride);
REFERENCE_DECLARE(0x0510FD4D, bool, c_contrail_gpu::x_debug_synchronous);
REFERENCE_DECLARE(0x0510FD50, uns32, c_contrail_gpu::x_stream_offset);
REFERENCE_DECLARE(0x0510FD54, uns32, c_contrail_gpu::x_stream_previous_offset);
REFERENCE_DECLARE(0x0510FD58, uns32, c_contrail_gpu::x_stream_vertex_count);

void __cdecl sub_5BBE10(void)
{
	INVOKE(0x005BBE10, sub_5BBE10);
}

bool __cdecl rasterizer_set_vertex_stream(const c_rasterizer_vertex_buffer* buffer)
{
	return INVOKE(0x00A5DB60, rasterizer_set_vertex_stream, buffer);
}

bool __cdecl rasterizer_set_vertex_stream_with_offset(const c_rasterizer_vertex_buffer* buffer, uns32 offset)
{
	return INVOKE(0x00A5DB90, rasterizer_set_vertex_stream_with_offset, buffer, offset);
}

void c_contrail_gpu::set_shader_state(s_gpu_constant_table* constant_table, const c_contrail_states* contrail_states) const
{
	INVOKE_CLASS_MEMBER(0x00A54940, c_contrail_gpu, set_shader_state, constant_table, contrail_states);
}

void c_contrail_gpu::set_shader_strip(s_gpu_constant_table* constant_table) const
{
	INVOKE_CLASS_MEMBER(0x00A55040, c_contrail_gpu, set_shader_strip, constant_table);
}

c_contrail_gpu* __cdecl c_contrail_gpu::get(int32 datum_index)
{
	return INVOKE(0x00A56720, c_contrail_gpu::get, datum_index);
}

c_contrail_gpu::s_row* c_contrail_gpu::s_row::get(int32 datum_index)
{
	return INVOKE(0x00A56760, c_contrail_gpu::s_row::get, datum_index);
}

//.text:00A56790 ; 
//.text:00A567B0 ; 
//.text:00A567D0 ; 
//.text:00A567F0 ; 
//.text:00A56810 ; 
//.text:00A56830 ; 

bool c_contrail_gpu::get_alive(void) const
{
	return INVOKE_CLASS_MEMBER(0x00A56850, c_contrail_gpu, get_alive);
}

//.text:00A56870 ; 
//.text:00A56880 ; 
//.text:00A56890 ; 
//.text:00A568A0 ; 
//.text:00A568B0 ; 
//.text:00A568C0 ; 
//.text:00A56900 ; 
//.text:00A56910 ; 
//.text:00A56920 ; 
//.text:00A56930 ; 
//.text:00A56940 ; 
//.text:00A56950 ; 
//.text:00A56960 ; 
//.text:00A56970 ; 
//.text:00A56980 ; 
//.text:00A56990 ; 
//.text:00A569A0 ; 
//.text:00A569B0 ; 
//.text:00A569C0 ; 
//.text:00A569D0 ; 
//.text:00A569E0 ; 
//.text:00A569F0 ; 
//.text:00A56A00 ; 
//.text:00A56A10 ; 
//.text:00A56A20 ; 
//.text:00A56A30 ; 
//.text:00A56A40 ; 
//.text:00A56A50 ; 
//.text:00A56A60 ; 
//.text:00A56A70 ; 
//.text:00A56A80 ; 
//.text:00A56A90 ; 
//.text:00A56AA0 ; 
//.text:00A56AB0 ; 
//.text:00A56AC0 ; 
//.text:00A56AD0 ; 
//.text:00A56AE0 ; 
//.text:00A56AF0 ; 
//.text:00A56B00 ; 
//.text:00A56B10 ; 
//.text:00A56B20 ; 
//.text:00A56B30 ; 
//.text:00A56B40 ; 
//.text:00A56B50 ; 
//.text:00A56B60 ; 
//.text:00A56B70 ; 
//.text:00A56B80 ; 
//.text:00A56B90 ; 
//.text:00A56BA0 ; 
//.text:00A56BC0 ; 
//.text:00A56C40 ; 
//.text:00A56CA0 ; 
//.text:00A56D20 ; 

void s_gpu_global_vertex_shader::initialize(const s_tag_reference* _shader_reference)
{
		INVOKE_CLASS_MEMBER(0x00A56D30, s_gpu_global_vertex_shader, initialize, _shader_reference);
}

//.text:00A56D50 ; 
//.text:00A57090 ; 
//.text:00A570F0 ; 
//.text:00A57100 ; 
//.text:00A57170 ; 
//.text:00A571E0 ; 
//.text:00A57290 ; 
//.text:00A57300 ; 
//.text:00A57350 ; 
//.text:00A57390 ; 
//.text:00A573D0 ; 
//.text:00A57440 ; 
//.text:00A57470 ; 
//.text:00A574A0 ; 
//.text:00A57630 ; public: static void __cdecl c_contrail_gpu::initialize_for_game(void)
//.text:00A57700 ; public: static void __cdecl c_contrail_gpu::initialize_for_map(void)
//.text:00A57770 ; 
//.text:00A577D0 ; 
//.text:00A577F0 ; 
//.text:00A57820 ; 
//.text:00A57870 ; 
//.text:00A57890 ; 
//.text:00A578B0 ; 
//.text:00A578C0 ; 
//.text:00A57900 ; 
//.text:00A57930 ; 
//.text:00A57950 ; 
//.text:00A57990 ; public: void c_contrail_gpu::queue_profile(uns8, const c_contrail_profile*, bool)

void c_contrail_gpu::render(const c_contrail_states* contrail_states) const
{
	//INVOKE_CLASS_MEMBER(0x00A57DC0, c_contrail_gpu, render, contrail_states);

	if (!sub_42E5D0() && effects_enabled)
	{
		sub_5BBE10();

		if (effects_render && contrail_render && m_row_head != NONE && m_profile_count > 1)
		{
			c_contrail* contrail = c_contrail::get(m_contrail_index);
			if (contrail == NULL)
			{
				event(_event_critical, "effects:gpu: Got NULL c_contrail in c_contrail_gpu::render");
			}

			const c_contrail_definition* contrail_definition = contrail->get_definition();
			if (contrail_definition == NULL)
			{
				event(_event_critical, "effects:gpu: Got NULL c_contrail_definition in c_contrail_gpu::render");
			}

			// $TODO PIX event

			rasterizer_set_vertex_stream(&x_vertex_buffer0); // c_rasterizer::set_vertex_stream(&c_contrail_gpu::x_vertex_buffer0);
			c_rasterizer::set_vertex_shader_constant(32, 1, constants);

			int32 number_of_sides;
			switch (contrail_definition->m_profile_type)
			{
			case c_contrail_definition::_profile_ribbon:
				number_of_sides = 1;
				break;
			case c_contrail_definition::_profile_cross:
				number_of_sides = 2;
				break;
			case c_contrail_definition::_profile_ngon:
				number_of_sides = contrail_definition->m_ngon_sides;
				break;
			default:
				UNREACHABLE();
				break;
			}

			if (contrail_definition->get_fogged())
			{
				c_atmosphere_fog_interface::restore_default_atmosphere_constants();
			}

			c_rasterizer::e_cull_mode previous_cull_mode;
			if (contrail_definition->get_double_sided())
			{
				previous_cull_mode = c_rasterizer::get_cull_mode();
				c_rasterizer::set_cull_mode(c_rasterizer::_cull_mode_off);
			}
			else
			{
				previous_cull_mode = c_rasterizer::_cull_mode_off;
			}

			const c_render_method_shader_contrail* render_method = &contrail_definition->m_shader;
			if (x_render_vertex_shader.shader_reference != NULL)
			{
				x_render_vertex_shader.initialize(&render_method->m_definition);
			}

			c_contrail_gpu::set_shader_state(&x_render_vertex_shader.constant_table, contrail_states);
			c_contrail_gpu::set_shader_strip(&x_render_vertex_shader.constant_table);

			render_method_submit_valid(true);

			const c_contrail_location* parent_location = contrail->get_parent_location();
			const c_contrail_system* parent_system = parent_location->get_parent_system();
			if (render_method_submit(parent_system->m_definition_index, render_method, _vertex_type_contrail, _transfer_vertex_none, _entry_point_default, false))
			{
				int32 row_indices[8];
				csmemset(row_indices, NONE, sizeof(row_indices));

				int32 row_count = 0;

				for (s_row* row = s_row::try_get(m_row_head); row && row_count < NUMBEROF(row_indices) - 1; row = s_row::try_get(row->m_next))
				{
					row_indices[++row_count] = row->m_row;
				}

				c_rasterizer::g_device->SetStreamSourceFreq(1u, 0x80000001u);

				if (get_alive())
				{
					uns16 remaining = m_profile_count;

					for (int32 row_num = row_count; row_num > 0 && remaining > 0; --row_num)
					{
						const int32 row = row_indices[row_num];
						if (row >= 0)
						{
							const uns32 offset = x_stream_previous_offset + x_overall_storage.m_stride * (16 * row);

							rasterizer_set_vertex_stream_with_offset(&x_overall_storage, offset);

							uns32 count = remaining >= 16 ? 16 : remaining;
							count -= x_render_vertex_shader.__unknown30;

							if (count > 0u)
							{
								c_rasterizer::g_device->SetStreamSourceFreq(0u, count | 0x40000000u);
								c_rasterizer::g_device->SetStreamSourceFreq(2u, count | 0x40000000u);

								c_rasterizer::draw_indexed_primitive(&x_index_buffer, 0, 4 * number_of_sides, 0, 6 * number_of_sides);
							}

							remaining -= 16;
						}
					}

					const uns32 tail = m_profile_count % 16;

					if (tail && row_indices[1] >= 0)
					{
						const uns32 offset = x_stream_previous_offset + x_overall_storage.m_stride * (16 * row_indices[1] + tail - 2);

						rasterizer_set_vertex_stream(&x_vertex_buffer1);
						rasterizer_set_vertex_stream_with_offset(&x_overall_storage, offset);

						c_rasterizer::g_device->SetStreamSourceFreq(0u, 0x40000001u);
						c_rasterizer::g_device->SetStreamSourceFreq(2u, 0x40000001u);

						c_rasterizer::draw_indexed_primitive(&x_index_buffer, 0, 4 * number_of_sides, 0, 6 * number_of_sides);
					}
				}

				c_rasterizer::g_device->SetStreamSourceFreq(0u, 1u);
				c_rasterizer::g_device->SetStreamSourceFreq(1u, 1u);
				c_rasterizer::g_device->SetStreamSourceFreq(2u, 1u);

				if (c_contrail_gpu::x_debug_synchronous)
				{
					c_rasterizer::wait_for_gpu_idle();
				}
			}
			else
			{
				event(_event_error, "effects:gpu: Failure of render_method_submit from c_contrail_gpu::render");
			}

			if (contrail_definition->get_double_sided())
			{
				c_rasterizer::set_cull_mode(previous_cull_mode);
			}

			render_method_submit_valid(false);
		}
	}
}

//.text:00A58130 ; 
//.text:00A58170 ; 
//.text:00A581B0 ; 
//.text:00A581F0 ; 
//.text:00A58200 ; 
//.text:00A58280 ; 
//.text:00A582A0 ; 
//.text:00A582D0 ; 
//.text:00A582F0 ; 
//.text:00A58320 ; 
//.text:00A58350 ; 
//.text:00A58390 ; 
//.text:00A583D0 ; 
//.text:00A58400 ; 

void __cdecl c_contrail_gpu::shell_initialize(void)
{
	INVOKE(0x00A58460, c_contrail_gpu::shell_initialize);
}

void __cdecl c_contrail_gpu::spawn_all(void)
{
	INVOKE(0x00A58490, c_contrail_gpu::shell_initialize);
}

//.text:.text:00A586B0 ; 

c_contrail_gpu::s_row* __cdecl c_contrail_gpu::s_row::try_get(int32 datum_index)
{
	return INVOKE(0x00A58710, c_contrail_gpu::s_row::try_get, datum_index);
}

