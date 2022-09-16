#include "rasterizer/rasterizer.hpp"

#include "cseries/cseries.hpp"

void __stdcall sub_79BA30(long width, long height)
{
	INVOKE(0x0079BA30, sub_79BA30, width, height);
}

void __cdecl c_rasterizer::begin(short_rectangle2d viewport_top, short_rectangle2d mipmap_lod_bias_top)
{
	INVOKE(0x00A1F7E0, begin, viewport_top, mipmap_lod_bias_top);
}

bool __cdecl c_rasterizer::cleanup_before_device_reset()
{
	return INVOKE(0x00A1F830, cleanup_before_device_reset);
}

void __cdecl c_rasterizer::cleanup_d3d_states()
{
	INVOKE(0x00A1F890, cleanup_d3d_states);
}

void __cdecl c_rasterizer::dispose()
{
	INVOKE(0x00A1F910, dispose);
}

void __cdecl c_rasterizer::dispose_from_old_map()
{
	INVOKE(0x00A1F950, dispose_from_old_map);
}

void __cdecl c_rasterizer::dispose_from_old_structure_bsp()
{
	INVOKE(0x00A1F9A0, dispose_from_old_structure_bsp);
}

real __cdecl c_rasterizer::get_aspect_ratio()
{
	return INVOKE(0x00A1FA30, get_aspect_ratio);
}

void __cdecl c_rasterizer::initialize()
{
	INVOKE(0x00A1FD20, initialize);
}

bool __cdecl c_rasterizer::initialize_after_device_creation_or_reset()
{
	return INVOKE(0x00A1FD80, initialize_after_device_creation_or_reset);
}

void __cdecl c_rasterizer::initialize_for_new_map()
{
	INVOKE(0x00A1FEC0, initialize_for_new_map);
}

void __cdecl c_rasterizer::restore_last_scissor_rect()
{
	INVOKE(0x00A202E0, restore_last_scissor_rect);
}

void __cdecl c_rasterizer::restore_last_viewport()
{
	INVOKE(0x00A202F0, restore_last_viewport);
}

void __cdecl c_rasterizer::shell_dispose()
{
	INVOKE(0x00A20340, shell_dispose);
}

void __cdecl c_rasterizer::shell_initialize(bool a1, bool a2)
{
	INVOKE(0x00A20370, shell_initialize, a1, a2);
}

bool __cdecl c_rasterizer::begin_frame()
{
	return INVOKE(0x00A212A0, begin_frame);
}

void __cdecl c_rasterizer::begin_high_quality_blend()
{
	INVOKE(0x00A21350, begin_high_quality_blend);
}

//void __cdecl c_rasterizer::clearf(unsigned long, union real_vector4d const*, float, unsigned long)
//00A213F0

bool __cdecl c_rasterizer::end_frame()
{
	return INVOKE(0x00A21510, end_frame);
}

void __cdecl c_rasterizer::end_high_quality_blend()
{
	INVOKE(0x00A21880, end_high_quality_blend);
}

e_platform __cdecl c_rasterizer::get_runtime_platform()
{
	return INVOKE(0x00A21A80, get_runtime_platform);
}

bool __cdecl c_rasterizer::initialize_device(bool a1, bool a2)
{
	return INVOKE(0x00A21B40, initialize_device, a1, a2);
}

void __cdecl c_rasterizer::rasterizer_device_acquire_thread()
{
	INVOKE(0x00A222D0, rasterizer_device_acquire_thread);
}

void __cdecl c_rasterizer::rasterizer_device_release_thread()
{
	INVOKE(0x00A222F0, rasterizer_device_release_thread);
}

bool __cdecl c_rasterizer::rasterizer_thread_owns_device()
{
	return INVOKE(0x00A22390, rasterizer_thread_owns_device);
}

void __cdecl c_rasterizer::initialize_window()
{
	INVOKE(0x00A223F0, initialize_window);
}

c_rasterizer::e_gpr_allocation __cdecl c_rasterizer::set_gprs_allocation(e_gpr_allocation a1)
{
	return INVOKE(0x00A228C0, set_gprs_allocation, a1);
}

void __cdecl c_rasterizer::clear_sampler_textures(dword a1)
{
	INVOKE(0x00A22AB0, clear_sampler_textures, a1);
}

enum c_rasterizer::e_cull_mode __cdecl c_rasterizer::get_cull_mode()
{
	return INVOKE(0x00A22BA0, get_cull_mode);
}

void __cdecl c_rasterizer::set_alpha_blend_mode(e_alpha_blend_mode a1)
{
	INVOKE(0x00A22D10, set_alpha_blend_mode, a1);
}

void __cdecl c_rasterizer::set_color_write_enable(long a1, long a2)
{
	INVOKE(0x00A231E0, set_color_write_enable, a1, a2);
}

void __cdecl c_rasterizer::set_cull_mode(e_cull_mode a1)
{
	INVOKE(0x00A232D0, set_cull_mode, a1);
}

bool __cdecl c_rasterizer::set_explicit_shaders(long a1, e_vertex_type a2, e_transfer_vector_vertex_types a3, e_entry_point a4)
{
	return INVOKE(0x00A23300, set_explicit_shaders, a1, a2, a3, a4);
}

bool __cdecl c_rasterizer::set_pixel_shader(c_rasterizer_pixel_shader const* a1, e_entry_point a2)
{
	return INVOKE(0x00A23460, set_pixel_shader, a1, a2);
}

void __cdecl c_rasterizer::set_aliased_surface_as_texture(long a1, e_surface a2)
{
	INVOKE(0x00A23530, set_aliased_surface_as_texture, a1, a2);
}

void __cdecl c_rasterizer::set_sampler_address_mode(long a1, e_sampler_address_mode a2)
{
	INVOKE(0x00A23570, set_sampler_address_mode, a1, a2);
}

void __cdecl c_rasterizer::set_sampler_filter_mode(long a1, e_sampler_filter_mode a2)
{
	INVOKE(0x00A23660, set_sampler_filter_mode, a1, a2);
}

void __cdecl c_rasterizer::set_sampler_texture(long a1, c_rasterizer_texture_ref a2)
{
	INVOKE(0x00A23810, set_sampler_texture, a1, a2);
};

void __cdecl c_rasterizer::set_scissor_rect(short_rectangle2d const* a1)
{
	INVOKE(0x00A239B0, set_scissor_rect, a1);
}

void __cdecl c_rasterizer::set_separate_alpha_blend_mode(e_separate_alpha_blend_mode a1)
{
	INVOKE(0x00A23A20, set_separate_alpha_blend_mode, a1);
}

void __cdecl c_rasterizer::set_stencil_mode(e_stencil_mode a1)
{
	INVOKE(0x00A23BA0, set_stencil_mode, a1);
}

bool __cdecl c_rasterizer::set_vertex_shader(c_rasterizer_vertex_shader const* a1, e_vertex_type a2, e_transfer_vector_vertex_types a3, e_entry_point a4)
{
	return INVOKE(0x00A246E0, set_vertex_shader, a1, a2, a3, a4);
}

void __cdecl c_rasterizer::set_z_buffer_mode(e_z_buffer_mode a1)
{
	INVOKE(0x00A247E0, set_z_buffer_mode, a1);
}

void __cdecl c_rasterizer::setup_occlusion_state()
{
	INVOKE(0x00A24B30, setup_occlusion_state);
}

void __cdecl c_rasterizer::setup_render_target_globals_with_exposure(real a1, real a2, bool a3)
{
	return INVOKE(0x00A24B90, setup_render_target_globals_with_exposure, a1, a2, a3);
}

void __cdecl c_rasterizer::setup_targets_distortion(short_rectangle2d* a1)
{
	INVOKE(0x00A250D0, setup_targets_distortion, a1);
}

void __cdecl c_rasterizer::setup_targets_simple()
{
	INVOKE(0x00A25280, setup_targets_simple);
}

void __cdecl c_rasterizer::setup_targets_static_lighting_alpha_blend(bool a1, bool a2)
{
	INVOKE(0x00A25400, setup_targets_static_lighting_alpha_blend, a1, a2);
}

void __cdecl c_rasterizer::draw_indexed_primitive(c_rasterizer_index_buffer const* a1, long a2, long a3, long a4, long a5)
{
	INVOKE(0x00A28270, draw_indexed_primitive, a1, a2, a3, a4, a5);
}

IDirect3DDevice9Ex*& c_rasterizer::g_device = *reinterpret_cast<IDirect3DDevice9Ex**>(0x050DADDC);
c_rasterizer::e_separate_alpha_blend_mode& c_rasterizer::g_current_separate_alpha_blend_mode = *reinterpret_cast<c_rasterizer::e_separate_alpha_blend_mode*>(0x050DADF8);
bool& c_rasterizer::initialized = *reinterpret_cast<bool*>(0x050DAE02);
bool& c_rasterizer::m_use_floating_point_z_buffer = *reinterpret_cast<bool*>(0x050DAE03);
c_rasterizer::e_stencil_mode& c_rasterizer::g_current_stencil_mode = *reinterpret_cast<c_rasterizer::e_stencil_mode*>(0x050DAE04);
dword& c_rasterizer::g_max_vs_gprs = *reinterpret_cast<dword*>(0x050DD9BC);
dword& c_rasterizer::g_max_ps_gprs = *reinterpret_cast<dword*>(0x050DD9C0);
