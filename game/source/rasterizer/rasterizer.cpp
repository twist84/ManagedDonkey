#include "rasterizer/rasterizer.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"
#include "tag_files/files.hpp"

REFERENCE_DECLARE(0x050DADDC, IDirect3DDevice9Ex*, c_rasterizer::g_device);
REFERENCE_DECLARE(0x050DADF8, c_rasterizer::e_separate_alpha_blend_mode, c_rasterizer::g_current_separate_alpha_blend_mode);
REFERENCE_DECLARE(0x050DAE02, bool, c_rasterizer::initialized);
REFERENCE_DECLARE(0x050DAE03, bool, c_rasterizer::m_use_floating_point_z_buffer);
REFERENCE_DECLARE(0x050DAE04, c_rasterizer::e_stencil_mode, c_rasterizer::g_current_stencil_mode);
REFERENCE_DECLARE(0x050DD9BC, dword, c_rasterizer::g_max_vs_gprs);
REFERENCE_DECLARE(0x050DD9C0, dword, c_rasterizer::g_max_ps_gprs);

void(__cdecl* rasterizer_get_display_pixel_bounds)(short_rectangle2d*) = c_rasterizer::get_display_pixel_bounds;

HOOK_DECLARE_CLASS(0x00A1FAA0, c_rasterizer, get_display_pixel_bounds);
//HOOK_DECLARE_CALL(0x00A9F706, rasterizer_get_display_pixel_bounds); // ui
//HOOK_DECLARE_CALL(0x00A9FACB, rasterizer_get_display_pixel_bounds); // ui
//HOOK_DECLARE_CALL(0x00A9F80C, rasterizer_get_display_pixel_bounds); // logo
//HOOK_DECLARE_CALL(0x00A1FB18, rasterizer_get_display_pixel_bounds); // watermark

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

void __cdecl c_rasterizer::get_display_pixel_bounds(short_rectangle2d* display_pixel_bounds)
{
	//HOOK_INVOKE_CLASS(, c_rasterizer, get_display_pixel_bounds, decltype(get_display_pixel_bounds)*, display_pixel_bounds);

	*display_pixel_bounds = {};

	// TESTING
	const long resolution_index = 0;
	short resolutions[3][2]
	{
		// 1152x640
		{ 1152, 640 },

		// 1920x1080
		{ 1920, 1080 },

		// 3840x2160
		{ 3840, 2160 }
	};

	short width  = resolutions[resolution_index][0];
	short height = resolutions[resolution_index][1];

	if (width <= 8)
		width = 8;

	if (height <= 8)
		height = 8;

	display_pixel_bounds->x1 = width;
	display_pixel_bounds->y1 = height;
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

HBITMAP create_bitmap_handle(HWND window_handle)
{
	if (!window_handle)
	{
		c_console::write_line("invalid parameter: window handle cannot be NULL");

		return NULL;
	}

	HDC window_device_context = GetWindowDC(window_handle);
	if (!window_device_context)
	{
		c_console::write_line("could not get the window device context");

		return NULL;
	}

	RECT window_rect{};
	if (!GetWindowRect(window_handle, &window_rect))
	{
		c_console::write_line("could not get the window rectangle");

		ReleaseDC(window_handle, window_device_context);
		return NULL;
	}

	int width = window_rect.bottom - window_rect.top;
	int height = window_rect.right - window_rect.left;

	if (height <= 0 || width <= 0)
	{
		c_console::write_line("invalid window width or height");

		ReleaseDC(window_handle, window_device_context);
		return NULL;
	}

	HBITMAP bitmap_handle = CreateCompatibleBitmap(window_device_context, height, width);

	ReleaseDC(window_handle, window_device_context);
	return bitmap_handle;
}

bool write_window_to_bitmap(HWND window_handle, HBITMAP bitmap_handle)
{
	if (!window_handle)
	{
		c_console::write_line("invalid parameter, window handle cannot be NULL");

		return false;
	}

	if (!bitmap_handle)
	{
		c_console::write_line("invalid parameter, handle to bitmap cannot be NULL");

		return false;
	}

	HDC window_device_context = GetWindowDC(window_handle);
	if (!window_device_context)
	{
		c_console::write_line("could not get window device context for the window");

		return false;
	}

	HDC memory_device_context = CreateCompatibleDC(window_device_context);
	if (!memory_device_context)
	{
		c_console::write_line("could not create memory device context from window device context");

		ReleaseDC(window_handle, window_device_context);
		return false;
	}

	RECT window_rect{};
	if (!GetWindowRect(window_handle, &window_rect))
	{
		c_console::write_line("could not get the window rectangle");

		DeleteDC(memory_device_context);
		ReleaseDC(window_handle, window_device_context);
		return false;
	}

	int width = window_rect.bottom - window_rect.top;
	int height = window_rect.right - window_rect.left;

	if (height <= 0 || width <= 0)
	{
		c_console::write_line("invalid window width or height");

		DeleteDC(memory_device_context);
		ReleaseDC(window_handle, window_device_context);
		return false;
	}

	SelectObject(memory_device_context, bitmap_handle);

	bool result = BitBlt(memory_device_context, 0, 0, height, width, window_device_context, 0, 0, SRCCOPY);

	DeleteDC(memory_device_context);
	ReleaseDC(window_handle, window_device_context);
	return result;
}

bool create_bitmap_info_header(HWND window_handle, HBITMAP bitmap_handle, LPBITMAPINFOHEADER out_bitmap_info_header)
{
	if (!window_handle)
	{
		c_console::write_line("invalid parameter, window handle cannot be NULL");

		return false;
	}

	if (!bitmap_handle)
	{
		c_console::write_line("invalid parameter, window bitmap handle cannot be NULL");

		return false;
	}

	if (!out_bitmap_info_header)
	{
		c_console::write_line("invalid out parameter, out_bitmap_info_header cannot be NULL");

		return false;
	}

	BITMAP bitmap{};
	if (!GetObjectA(bitmap_handle, sizeof(BITMAP), &bitmap))
	{
		c_console::write_line("could not get the bitmap object from the bitmap handle");

		return false;
	}

	word bit_depth = bitmap.bmPlanes * bitmap.bmBitsPixel;

	// 24 bits per pixel
	// check if `bit_depth` is not a multiple of 8
	if (((bit_depth - 24) & ~0x8) != 0)
	{
		c_console::write_line("invalid bit depth used");

		return false;
	}

	csmemset(out_bitmap_info_header, 0, sizeof(BITMAPINFOHEADER));
	out_bitmap_info_header->biPlanes = bitmap.bmPlanes;
	out_bitmap_info_header->biBitCount = bitmap.bmBitsPixel;
	out_bitmap_info_header->biWidth = bitmap.bmWidth;
	out_bitmap_info_header->biSize = sizeof(BITMAPINFOHEADER);
	out_bitmap_info_header->biHeight = bitmap.bmHeight;
	out_bitmap_info_header->biCompression = BI_RGB;
	out_bitmap_info_header->biSizeImage = bitmap.bmWidth * bitmap.bmHeight * bit_depth / 8;

	return true;
}

bool get_device_context_for_window(HWND window_handle, const char* file_name, HBITMAP bitmap_handle, HDC window_device_context)
{
	if (!window_handle)
	{
		c_console::write_line("invalid parameter, window handle cannot be NULL");

		return false;
	}

	if (!file_name)
	{
		c_console::write_line("invalid parameter, file_name cannot be NULL");

		return false;
	}

	if (!bitmap_handle)
	{
		c_console::write_line("invalid parameter, window bitmap handle cannot be NULL");

		return false;
	}

	if (!window_device_context)
	{
		c_console::write_line("invalid parameter, window device context cannot be NULL");

		return false;
	}

	BITMAPINFO bitmap_info{};
	if (!create_bitmap_info_header(window_handle, bitmap_handle, &bitmap_info.bmiHeader))
	{
		c_console::write_line("could not create the bitmap info header");

		return false;
	}

	void* bitmap_memory = malloc(bitmap_info.bmiHeader.biSizeImage);
	if (!bitmap_memory)
	{
		c_console::write_line("could not allocate memory for the bitmap");

		return false;
	}

	csmemset(bitmap_memory, 0, bitmap_info.bmiHeader.biSizeImage);

	if (!GetDIBits(window_device_context, bitmap_handle, 0, bitmap_info.bmiHeader.biHeight, bitmap_memory, &bitmap_info, 0))
	{
		c_console::write_line("could not get the color array");

		free(bitmap_memory);
		return false;
	}

	s_file_reference info{};
	file_reference_create_from_path(&info, file_name, false);
	if (!file_create(&info))
	{
		c_console::write_line("could not create bitmap file");

		free(bitmap_memory);
		return false;
	}

	dword error = 0;
	if (!file_open(&info, FLAG(_file_open_flag_desired_access_write), &error))
	{
		c_console::write_line("could not open bitmap file");

		free(bitmap_memory);
		return false;
	}

	BITMAPFILEHEADER bitmap_file_header =
	{
		.bfType = 0x4D42,
		.bfSize = bitmap_info.bmiHeader.biSize + bitmap_info.bmiHeader.biSizeImage + sizeof(BITMAPFILEHEADER),
		.bfOffBits = bitmap_info.bmiHeader.biSize + sizeof(BITMAPFILEHEADER)
	};

	file_write(&info, sizeof(BITMAPFILEHEADER), &bitmap_file_header);
	file_write(&info, sizeof(BITMAPINFO), &bitmap_info);
	file_write(&info, bitmap_info.bmiHeader.biSizeImage, bitmap_memory);
	file_close(&info);

	return true;
}

bool rasterizer_dump_display_to_bmp(char const* file_name)
{
	if (!file_name)
	{
		c_console::write_line("invalid parameter, file_name cannot be NULL");

		return false;
	}

	s_file_reference info{};
	file_reference_create_from_path(&info, file_name, false);
	if (file_exists(&info))
		file_delete(&info);

	REFERENCE_DECLARE(0x0199C014, HWND, window_handle);
	if (!window_handle)
		return false;

	//HWND window_handle = FindWindowA(NULL, NULL);
	//if (!window_handle)
	//	return false;
	//
	//CHAR window_text[1024]{};
	//DWORD current_process_id = GetCurrentProcessId();
	//
	//if (window_handle)
	//{
	//	while (true)
	//	{
	//		if (!GetParent(window_handle))
	//		{
	//			DWORD window_thread_process_id = 0;
	//			GetWindowThreadProcessId(window_handle, &window_thread_process_id);
	//			if (current_process_id == window_thread_process_id)
	//			{
	//				GetWindowTextA(window_handle, window_text, 1024);
	//				if (!strcmp(window_text, "Halo IDE"))
	//					break;
	//
	//				if (!strncmp(window_text, "Guerilla", strlen("Guerilla")))
	//					break;
	//			}
	//		}
	//
	//		window_handle = GetWindow(window_handle, GW_HWNDNEXT);
	//		if (!window_handle)
	//			return false;
	//	}
	//}

	HBITMAP bitmap_handle = create_bitmap_handle(window_handle);
	if (!bitmap_handle)
	{
		c_console::write_line("could not create a bitmap handle for the window");

		return false;
	}

	if (!write_window_to_bitmap(window_handle, bitmap_handle))
	{
		c_console::write_line("could not write window to bitmap");

		DeleteObject(bitmap_handle);
		return false;
	}

	HDC window_device_context = GetWindowDC(window_handle);
	if (!window_device_context)
	{
		c_console::write_line("could not get the device context for the window");

		return false;
	}

	bool result = get_device_context_for_window(window_handle, file_name, bitmap_handle, window_device_context);

	ReleaseDC(window_handle, window_device_context);
	DeleteObject(bitmap_handle);
	return result;
}

