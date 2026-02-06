#include "shell/shell_splash.hpp"

#include <windows.h>
#include <gdiplus.h>

#pragma comment(lib, "Gdiplus.lib")

class c_splash_screen
{
	static const int32 k_splash_timer_id = 1;

	enum e_state
	{
		_state_fading_in = 0,
		_state_hold,
		_state_fading_out,

		k_state_none = NONE
	};

public:
	c_splash_screen();
	~c_splash_screen();
	void show(const wchar_t* png_path, int fade_in_ms, int hold_ms, int fade_out_ms);

private:
	void initialize();
	void disose();
	static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
	void run(const wchar_t* path, int fade_in_ms, int hold_ms, int fade_out_ms);
	void draw(BYTE alpha);
	void cleanup();

protected:
	HINSTANCE m_instance;
	HWND m_hwnd;
	Gdiplus::Bitmap* m_bitmap;
	ULONG_PTR m_gdiplus_token;

	int m_alpha;
	int m_step;
	int m_timer_interval;
	e_state m_state;

	int m_fade_in_ms;
	int m_hold_ms;
	int m_fade_out_ms;
};

void splash_screen_show(const wchar_t* png_path, int fade_in_ms, int hold_ms, int fade_out_ms)
{
	c_splash_screen().show(png_path, fade_in_ms, hold_ms, fade_out_ms);
}

c_splash_screen::c_splash_screen() :
	m_instance(GetModuleHandle(nullptr)),
	m_hwnd(nullptr),
	m_bitmap(nullptr),
	m_gdiplus_token(nullptr),
	m_alpha(0),
	m_step(0),
	m_timer_interval(0),
	m_state(k_state_none),
	m_fade_in_ms(0),
	m_hold_ms(0),
	m_fade_out_ms(0)
{
	initialize();
}

c_splash_screen::~c_splash_screen()
{
	disose();
}

void c_splash_screen::initialize()
{
	Gdiplus::GdiplusStartupInput input;
	Gdiplus::GdiplusStartup(&m_gdiplus_token, &input, nullptr);
}

void c_splash_screen::disose()
{
	cleanup();
	Gdiplus::GdiplusShutdown(m_gdiplus_token);
}

void c_splash_screen::show(const wchar_t* png_path, int fade_in_ms, int hold_ms, int fade_out_ms)
{
	run(png_path, fade_in_ms, hold_ms, fade_out_ms);
}

LRESULT CALLBACK c_splash_screen::window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
	c_splash_screen* _this = nullptr;

	if (msg == WM_NCCREATE)
	{
		CREATESTRUCT* cs = (CREATESTRUCT*)l_param;
		_this = (c_splash_screen*)cs->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)_this);
	}
	else
	{
		_this = (c_splash_screen*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	}

	if (_this)
	{
		switch (msg)
		{
		case WM_TIMER:
		{
			if (_this->m_state == _state_fading_in)
			{
				_this->m_alpha += _this->m_step;
				if (_this->m_alpha >= 255)
				{
					_this->m_alpha = 255;
					_this->m_state = _state_hold;
					SetTimer(hwnd, k_splash_timer_id, _this->m_hold_ms, nullptr);
				}
			}
			else if (_this->m_state == _state_hold)
			{
				_this->m_state = _state_fading_out;
				_this->m_step = 255 * _this->m_timer_interval / (_this->m_fade_out_ms > 1 ? _this->m_fade_out_ms : 1);
				SetTimer(hwnd, k_splash_timer_id, _this->m_timer_interval, nullptr);
			}
			else if (_this->m_state == _state_fading_out) // fade out
			{
				_this->m_alpha -= _this->m_step;
				if (_this->m_alpha <= 0)
				{
					KillTimer(hwnd, k_splash_timer_id);
					DestroyWindow(hwnd);
					return 0;
				}
			}

			_this->draw((BYTE)_this->m_alpha);
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
	}

	return DefWindowProc(hwnd, msg, w_param, l_param);
}

void c_splash_screen::run(const wchar_t* path, int fade_in_ms, int hold_ms, int fade_out_ms)
{
	m_bitmap = Gdiplus::Bitmap::FromFile(path);
	if (!m_bitmap || m_bitmap->GetLastStatus() != Gdiplus::Ok)
	{
		return;
	}

	m_fade_in_ms = fade_in_ms;
	m_hold_ms = hold_ms;
	m_fade_out_ms = fade_out_ms;
	m_state = _state_fading_in;
	m_alpha = 0;

	int w = m_bitmap->GetWidth();
	int h = m_bitmap->GetHeight();
	m_timer_interval = 17; // ~60fps
	m_step = 255 * m_timer_interval / (fade_in_ms > 1 ? fade_in_ms : 1);

	const wchar_t* class_name = L"splash_cls";

	WNDCLASS wc = {};
	wc.lpfnWndProc = window_proc;
	wc.hInstance = m_instance;
	wc.lpszClassName = class_name;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	RegisterClass(&wc);

	int screen_w = GetSystemMetrics(SM_CXSCREEN);
	int screen_h = GetSystemMetrics(SM_CYSCREEN);

	int x = (screen_w - w) / 2;
	int y = (screen_h - h) / 2;

	m_hwnd = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
		class_name, L"", WS_POPUP,
		x, y, w, h,
		nullptr, nullptr, m_instance, this);

	ShowWindow(m_hwnd, SW_SHOWNORMAL);
	UpdateWindow(m_hwnd);

	draw((BYTE)m_alpha);
	SetTimer(m_hwnd, k_splash_timer_id, m_timer_interval, nullptr);

	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!IsDialogMessage(m_hwnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void c_splash_screen::draw(BYTE alpha)
{
	HDC screen_dc = GetDC(nullptr);
	HDC mem_dc = CreateCompatibleDC(screen_dc);

	int w = m_bitmap->GetWidth();
	int h = m_bitmap->GetHeight();

	BITMAPINFO bmi = {};
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = -h;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	void* bits = nullptr;
	HBITMAP dib = CreateDIBSection(mem_dc, &bmi, DIB_RGB_COLORS, &bits, nullptr, 0);
	(void)SelectObject(mem_dc, dib);

	Gdiplus::Graphics g(mem_dc);
	g.DrawImage(m_bitmap, 0, 0, w, h);

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = alpha;
	blend.AlphaFormat = AC_SRC_ALPHA;

	SIZE sz = { w, h };
	POINT pt_src = { 0, 0 };
	POINT pt_pos;
	RECT rect;
	GetWindowRect(m_hwnd, &rect);
	pt_pos.x = rect.left;
	pt_pos.y = rect.top;

	UpdateLayeredWindow(m_hwnd, screen_dc, &pt_pos, &sz, mem_dc, &pt_src, 0, &blend, ULW_ALPHA);

	DeleteObject(dib);
	DeleteDC(mem_dc);
	ReleaseDC(nullptr, screen_dc);
}

void c_splash_screen::cleanup()
{
	if (m_bitmap)
	{
		delete m_bitmap;
		m_bitmap = nullptr;
	}
}

