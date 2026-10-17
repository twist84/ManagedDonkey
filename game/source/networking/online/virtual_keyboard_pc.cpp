#include "networking/online/online_guide_pc.hpp"

#include "cseries/cseries.hpp"
#include "input/input_windows.hpp"

#include <windows.h>
#include <windowsx.h>
#include <xinput.h>
#include <wchar.h>
#include <algorithm>
#include <string>
#include <vector>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "xinput.lib")

namespace
{
    class c_virtual_keyboard_window;
    c_virtual_keyboard_window* g_virtual_keyboard = nullptr;
    bool g_game_input_paused_for_keyboard = false;

    void pause_game_input_for_keyboard()
    {
        if (!g_game_input_paused_for_keyboard)
        {
            input_deactivate();
            g_game_input_paused_for_keyboard = true;
        }
    }

    void resume_game_input_for_keyboard()
    {
        if (g_game_input_paused_for_keyboard)
        {
            input_activate();
            g_game_input_paused_for_keyboard = false;
        }
    }

    constexpr wchar_t k_window_class_name[] = L"pc_xbox360_virtual_keyboard";

    constexpr int k_reference_width = 1280;
    constexpr int k_reference_height = 720;

    constexpr int k_panel_left = 206;
    constexpr int k_panel_top = 100;
    constexpr int k_panel_right = 1089;
    constexpr int k_panel_bottom = 568;

    constexpr int k_input_left = 236;
    constexpr int k_input_top = 248;
    constexpr int k_input_right = 1044;
    constexpr int k_input_bottom = 292;

    constexpr int k_keyboard_left = 384;
    constexpr int k_keyboard_top = 328;
    constexpr int k_key_width = 48;
    constexpr int k_key_height = 38;
    constexpr int k_key_gap = 5;

    constexpr int k_left_button_left = 236;
    constexpr int k_left_button_right = 374;
    constexpr int k_right_button_left = 919;
    constexpr int k_right_button_right = 1059;

    constexpr int k_cursor_top = 328;
    constexpr int k_cursor_bottom = 409;
    constexpr int k_symbols_top = 414;
    constexpr int k_symbols_bottom = 452;
    constexpr int k_caps_top = 457;
    constexpr int k_caps_bottom = 538;

    constexpr int k_backspace_left = 384;
    constexpr int k_backspace_top = 500;
    constexpr int k_backspace_right = 644;
    constexpr int k_backspace_bottom = 538;

    constexpr int k_space_left = 649;
    constexpr int k_space_top = 500;
    constexpr int k_space_right = 909;
    constexpr int k_space_bottom = 538;

    constexpr int k_done_left = 919;
    constexpr int k_done_top = 457;
    constexpr int k_done_right = 1059;
    constexpr int k_done_bottom = 538;

    constexpr UINT k_controller_timer_id = 1;
    constexpr UINT k_controller_poll_ms = 16;
    constexpr SHORT k_stick_deadzone = 7849;

    constexpr int k_index_backspace = 40;
    constexpr int k_index_space = 41;
    constexpr int k_index_done = 42;
    constexpr int k_index_cursor_left = 43;
    constexpr int k_index_caps = 44;
    constexpr int k_index_cursor_right = 45;

    enum class e_key_type
    {
        character,
        backspace,
        space,
        done,
        caps,
        left,
        right,
        symbols,
        accents,
    };

    struct s_key
    {
        RECT rect{};
        wchar_t character = L'\0';
        wchar_t const* label = L"";
        e_key_type type = e_key_type::character;
        bool enabled = true;
    };

    int scale_x(int value, int width)
    {
        return MulDiv(value, width, k_reference_width);
    }

    int scale_y(int value, int height)
    {
        return MulDiv(value, height, k_reference_height);
    }

    RECT scale_rect(RECT rect, int width, int height)
    {
        return RECT{
            scale_x(rect.left, width),
            scale_y(rect.top, height),
            scale_x(rect.right, width),
            scale_y(rect.bottom, height) };
    }

    int round_radius(int width, int height)
    {
        return __max(2, scale_y(4, height));
    }

    class c_virtual_keyboard_window
    {
    public:
        c_virtual_keyboard_window(
            wchar_t const* default_text,
            wchar_t const* title_text,
            wchar_t const* description_text,
            wchar_t* result_text,
            unsigned long maximum_character_count,
            OVERLAPPED* overlapped,
            DWORD controller_index)
            : m_title_text(title_text != nullptr ? title_text : L"Virtual Keyboard"),
            m_description_text(description_text != nullptr ? description_text : L""),
            m_result_text(result_text),
            m_maximum_character_count(maximum_character_count),
            m_overlapped(overlapped),
            m_controller_index(controller_index)
        {
            m_text = default_text != nullptr ? default_text : L"";
            m_caret = static_cast<int>(m_text.size());
        }

        bool create()
        {
            bool result = false;

            WNDCLASSEXW window_class{};
            window_class.cbSize = sizeof(window_class);
            window_class.hInstance = GetModuleHandleW(nullptr);
            window_class.lpfnWndProc = &c_virtual_keyboard_window::static_window_proc;
            window_class.lpszClassName = k_window_class_name;
            window_class.hCursor = LoadCursorW(nullptr, IDC_ARROW);
            window_class.hbrBackground = nullptr;

            if (RegisterClassExW(&window_class) != 0 || GetLastError() == ERROR_CLASS_ALREADY_EXISTS)
            {
                int width = GetSystemMetrics(SM_CXSCREEN);
                int height = GetSystemMetrics(SM_CYSCREEN);

                m_handle = CreateWindowExW(
                    WS_EX_APPWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED,
                    k_window_class_name,
                    m_title_text.c_str(),
                    WS_POPUP,
                    0,
                    0,
                    width,
                    height,
                    nullptr,
                    nullptr,
                    GetModuleHandleW(nullptr),
                    this);

                if (m_handle != nullptr)
                {
                    SetLayeredWindowAttributes(m_handle, RGB(0, 0, 0), 0, LWA_COLORKEY);
                    create_fonts();
                    build_keyboard();
                    ShowWindow(m_handle, SW_SHOW);
                    UpdateWindow(m_handle);
                    SetFocus(m_handle);
                    SetTimer(m_handle, k_controller_timer_id, k_controller_poll_ms, nullptr);
                    pause_game_input_for_keyboard();
                    result = true;
                }
            }

            return result;
        }

        HWND handle() const
        {
            return m_handle;
        }

        void set_completed()
        {
            if (m_completed)
            {
                return;
            }

            m_completed = true;

            ULONG_PTR bytes_written = 0;
            if (m_result_text != nullptr && m_maximum_character_count != 0)
            {
                size_t copy_count = __min(
                    m_text.size(),
                    static_cast<size_t>(m_maximum_character_count - 1));

                if (copy_count != 0)
                {
                    wmemcpy(m_result_text, m_text.data(), copy_count);
                }

                m_result_text[copy_count] = L'\0';
                bytes_written = static_cast<ULONG_PTR>(copy_count * sizeof(wchar_t));
            }

            complete_overlapped(ERROR_SUCCESS, bytes_written);
            resume_game_input_for_keyboard();
            DestroyWindow(m_handle);
        }

        void set_cancelled()
        {
            if (m_completed)
            {
                return;
            }

            m_completed = true;
            complete_overlapped(ERROR_CANCELLED, 0);
            resume_game_input_for_keyboard();
            DestroyWindow(m_handle);
        }

        static LRESULT CALLBACK static_window_proc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param)
        {
            c_virtual_keyboard_window* instance = reinterpret_cast<c_virtual_keyboard_window*>(GetWindowLongPtrW(window_handle, GWLP_USERDATA));

            if (message == WM_NCCREATE)
            {
                CREATESTRUCTW* create = reinterpret_cast<CREATESTRUCTW*>(l_param);
                instance = static_cast<c_virtual_keyboard_window*>(create->lpCreateParams);
                SetWindowLongPtrW(window_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(instance));
                instance->m_handle = window_handle;
            }

            LRESULT result = 0;
            if (instance != nullptr)
            {
                result = instance->window_proc(message, w_param, l_param);
            }
            else
            {
                result = DefWindowProcW(window_handle, message, w_param, l_param);
            }
            return result;
        }

    private:
        HFONT make_font(int pixel_height, int weight = FW_NORMAL)
        {
            return CreateFontW(
                pixel_height,
                0,
                0,
                0,
                weight,
                FALSE,
                FALSE,
                FALSE,
                DEFAULT_CHARSET,
                OUT_TT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY,
                DEFAULT_PITCH | FF_SWISS,
                L"Segoe UI");
        }

        void create_fonts()
        {
            int height = GetClientRectHeight();
            m_title_font = make_font(scale_y(28, height), FW_NORMAL);
            m_header_font = make_font(scale_y(18, height));
            m_input_font = make_font(scale_y(20, height));
            m_key_font = make_font(scale_y(18, height));
            m_side_font = make_font(scale_y(16, height));
            m_footer_font = make_font(scale_y(16, height));
            m_badge_font = make_font(scale_y(12, height), FW_SEMIBOLD);
        }

        void build_keyboard()
        {
            m_keys.clear();

            wchar_t const* rows[] =
            {
                L"1234567890",
                L"qwertyuiop",
                L"asdfghjkl-",
                L"zxcvbnm_@."
            };

            for (int row = 0; row < 4; ++row)
            {
                for (int column = 0; column < 10; ++column)
                {
                    int left = k_keyboard_left + column * (k_key_width + k_key_gap);
                    int top = k_keyboard_top + row * (k_key_height + k_key_gap);

                    s_key key{};
                    key.rect = RECT{ left, top, left + k_key_width, top + k_key_height };
                    key.character = rows[row][column];
                    key.label = nullptr;
                    key.type = e_key_type::character;
                    m_keys.push_back(key);
                }
            }

            add_key(
                RECT{ k_backspace_left, k_backspace_top, k_backspace_right, k_backspace_bottom },
                L"Backspace",
                L'\0',
                e_key_type::backspace);

            add_key(
                RECT{ k_space_left, k_space_top, k_space_right, k_space_bottom },
                L"Space",
                L'\0',
                e_key_type::space);

            add_key(
                RECT{ k_done_left, k_done_top, k_done_right, k_done_bottom },
                L"Done",
                L'\0',
                e_key_type::done);

            add_key(
                RECT{ k_left_button_left, k_cursor_top, k_left_button_right, k_cursor_bottom },
                L"Cursor",
                L'\0',
                e_key_type::left);

            add_key(
                RECT{ k_left_button_left, k_caps_top, k_left_button_right, k_caps_bottom },
                L"Caps",
                L'\0',
                e_key_type::caps);

            add_key(
                RECT{ k_right_button_left, k_cursor_top, k_right_button_right, k_cursor_bottom },
                L"Cursor",
                L'\0',
                e_key_type::right);

            add_key(
                RECT{ k_left_button_left, k_symbols_top, k_left_button_right, k_symbols_bottom },
                L"Symbols",
                L'\0',
                e_key_type::symbols,
                false);

            add_key(
                RECT{ k_right_button_left, k_symbols_top, k_right_button_right, k_symbols_bottom },
                L"Accents",
                L'\0',
                e_key_type::accents,
                false);
        }

        void add_key(RECT rect, wchar_t const* label, wchar_t character, e_key_type type, bool enabled = true)
        {
            s_key key{};
            key.rect = rect;
            key.character = character;
            key.label = label;
            key.type = type;
            key.enabled = enabled;
            m_keys.push_back(key);
        }

        RECT to_client_rect(RECT rect) const
        {
            RECT client_rect{};
            GetClientRect(m_handle, &client_rect);
            return scale_rect(rect, client_rect.right, client_rect.bottom);
        }

        void fill_rect(HDC device_context, RECT rect, COLORREF color) const
        {
            HBRUSH brush = CreateSolidBrush(color);
            FillRect(device_context, &rect, brush);
            DeleteObject(brush);
        }

        void fill_round_rect(HDC device_context, RECT rect, COLORREF fill, COLORREF border, int radius) const
        {
            HBRUSH brush = CreateSolidBrush(fill);
            HPEN pen = CreatePen(PS_SOLID, 1, border);
            HBRUSH old_brush = static_cast<HBRUSH>(SelectObject(device_context, brush));
            HPEN old_pen = static_cast<HPEN>(SelectObject(device_context, pen));
            RoundRect(device_context, rect.left, rect.top, rect.right, rect.bottom, radius, radius);
            SelectObject(device_context, old_brush);
            SelectObject(device_context, old_pen);
            DeleteObject(pen);
            DeleteObject(brush);
        }

        void frame_rect(HDC device_context, RECT rect, COLORREF color) const
        {
            HPEN pen = CreatePen(PS_SOLID, 1, color);
            HPEN old_pen = static_cast<HPEN>(SelectObject(device_context, pen));
            HBRUSH old_brush = static_cast<HBRUSH>(SelectObject(device_context, GetStockObject(HOLLOW_BRUSH)));
            Rectangle(device_context, rect.left, rect.top, rect.right, rect.bottom);
            SelectObject(device_context, old_brush);
            SelectObject(device_context, old_pen);
            DeleteObject(pen);
        }

        void draw_text(HDC device_context, HFONT font, RECT rect, wchar_t const* text, UINT format, COLORREF color) const
        {
            HFONT old_font = static_cast<HFONT>(SelectObject(device_context, font));
            COLORREF old_color = SetTextColor(device_context, color);
            int old_mode = SetBkMode(device_context, TRANSPARENT);
            DrawTextW(device_context, text != nullptr ? text : L"", -1, &rect, format);
            SetBkMode(device_context, old_mode);
            SetTextColor(device_context, old_color);
            SelectObject(device_context, old_font);
        }

        void draw_circle(HDC device_context, RECT rect, COLORREF fill, COLORREF border) const
        {
            HBRUSH brush = CreateSolidBrush(fill);
            HPEN pen = CreatePen(PS_SOLID, 1, border);
            HBRUSH old_brush = static_cast<HBRUSH>(SelectObject(device_context, brush));
            HPEN old_pen = static_cast<HPEN>(SelectObject(device_context, pen));
            Ellipse(device_context, rect.left, rect.top, rect.right, rect.bottom);
            SelectObject(device_context, old_pen);
            SelectObject(device_context, old_brush);
            DeleteObject(pen);
            DeleteObject(brush);
        }

        int GetClientRectWidth() const
        {
            RECT rect{};
            GetClientRect(m_handle, &rect);
            return rect.right;
        }

        int GetClientRectHeight() const
        {
            RECT rect{};
            GetClientRect(m_handle, &rect);
            return rect.bottom;
        }

        void draw_bumper_badge(HDC device_context, RECT rect, wchar_t const* text)
        {
            fill_round_rect(device_context, rect, RGB(58, 62, 66), RGB(40, 43, 46), scale_y(3, GetClientRectHeight()));
            draw_text(device_context, m_badge_font, rect, text, DT_CENTER | DT_VCENTER | DT_SINGLELINE, RGB(236, 238, 240));
        }

        void draw_face_badge(HDC device_context, RECT rect, wchar_t letter, COLORREF fill)
        {
            draw_circle(device_context, rect, fill, fill);
            wchar_t buffer[2]{ letter, L'\0' };
            draw_text(device_context, m_badge_font, rect, buffer, DT_CENTER | DT_VCENTER | DT_SINGLELINE, RGB(255, 255, 255));
        }

        void draw_stick_icon(HDC device_context, RECT rect, COLORREF color)
        {
            int cx = (rect.left + rect.right) / 2;
            int cy = rect.top + (rect.bottom - rect.top) * 2 / 5;
            int radius = __max(6, (rect.bottom - rect.top) / 5);
            RECT head{ cx - radius, cy - radius, cx + radius, cy + radius };
            HPEN pen = CreatePen(PS_SOLID, 2, color);
            HBRUSH old_brush = static_cast<HBRUSH>(SelectObject(device_context, GetStockObject(HOLLOW_BRUSH)));
            HPEN old_pen = static_cast<HPEN>(SelectObject(device_context, pen));
            Ellipse(device_context, head.left, head.top, head.right, head.bottom);
            SelectObject(device_context, old_pen);
            SelectObject(device_context, old_brush);
            DeleteObject(pen);

            RECT nub{ cx - radius / 3, cy - radius / 3, cx + radius / 3, cy + radius / 3 };
            draw_circle(device_context, nub, color, color);

            RECT shaft{ cx - radius / 5, cy + radius / 2, cx + radius / 5, rect.bottom - scale_y(8, GetClientRectHeight()) };
            fill_rect(device_context, shaft, color);
        }

        void draw_trigger_icon(HDC device_context, RECT rect, COLORREF color)
        {
            int width = GetClientRectWidth();
            int height = GetClientRectHeight();
            int cx = (rect.left + rect.right) / 2;
            POINT body[4]{
                { cx - scale_x(10, width), rect.bottom - scale_y(8, height) },
                { cx - scale_x(7, width), rect.top + scale_y(8, height) },
                { cx + scale_x(7, width), rect.top + scale_y(8, height) },
                { cx + scale_x(10, width), rect.bottom - scale_y(8, height) },
            };
            HBRUSH brush = CreateSolidBrush(color);
            HPEN pen = CreatePen(PS_SOLID, 1, color);
            HBRUSH old_brush = static_cast<HBRUSH>(SelectObject(device_context, brush));
            HPEN old_pen = static_cast<HPEN>(SelectObject(device_context, pen));
            Polygon(device_context, body, 4);
            SelectObject(device_context, old_pen);
            SelectObject(device_context, old_brush);
            DeleteObject(pen);
            DeleteObject(brush);
        }

        void draw_side_key(HDC device_context, s_key const& key, bool selected)
        {
            RECT rect = to_client_rect(key.rect);
            int width = GetClientRectWidth();
            int height = GetClientRectHeight();
            int radius = round_radius(width, height);

            bool enabled = key.enabled;
            COLORREF fill = RGB(226, 230, 232);
            COLORREF text = RGB(44, 47, 49);
            COLORREF border = RGB(145, 149, 152);

            if (!enabled)
            {
                fill = RGB(198, 202, 205);
                text = RGB(142, 146, 149);
            }
            else if (selected)
            {
                fill = RGB(122, 164, 58);
                text = RGB(255, 255, 255);
                border = RGB(96, 132, 42);
            }
            else if (key.type == e_key_type::caps && m_caps)
            {
                fill = RGB(210, 220, 196);
            }

            fill_round_rect(device_context, rect, fill, border, radius);

            RECT badge_row = rect;
            badge_row.bottom = badge_row.top + __max(22, (rect.bottom - rect.top) / 3);

            RECT label_rect = rect;
            label_rect.top = badge_row.bottom;

            if (key.type == e_key_type::left)
            {
                RECT arrow{ rect.left + scale_x(16, width), badge_row.top, rect.left + scale_x(42, width), badge_row.bottom };
                draw_text(device_context, m_side_font, arrow, L"\x2190", DT_CENTER | DT_VCENTER | DT_SINGLELINE, text);
                RECT badge{ arrow.right + scale_x(4, width), badge_row.top + scale_y(6, height), arrow.right + scale_x(40, width), badge_row.bottom - scale_y(4, height) };
                draw_bumper_badge(device_context, badge, L"LB");
                draw_text(device_context, m_side_font, label_rect, L"Cursor", DT_CENTER | DT_VCENTER | DT_SINGLELINE, text);
            }
            else if (key.type == e_key_type::right)
            {
                RECT badge{ rect.right - scale_x(78, width), badge_row.top + scale_y(6, height), rect.right - scale_x(42, width), badge_row.bottom - scale_y(4, height) };
                draw_bumper_badge(device_context, badge, L"RB");
                RECT arrow{ badge.right, badge_row.top, rect.right - scale_x(12, width), badge_row.bottom };
                draw_text(device_context, m_side_font, arrow, L"\x2192", DT_CENTER | DT_VCENTER | DT_SINGLELINE, text);
                draw_text(device_context, m_side_font, label_rect, L"Cursor", DT_CENTER | DT_VCENTER | DT_SINGLELINE, text);
            }
            else if (key.type == e_key_type::symbols)
            {
                RECT icon = badge_row;
                icon.top += scale_y(4, height);
                draw_trigger_icon(device_context, icon, text);
                RECT lt{ rect.left + (rect.right - rect.left) / 2 - scale_x(16, width), badge_row.top + scale_y(4, height), rect.left + (rect.right - rect.left) / 2 + scale_x(16, width), badge_row.bottom - scale_y(2, height) };
                draw_bumper_badge(device_context, lt, L"LT");
                draw_text(device_context, m_side_font, label_rect, L"Symbols", DT_CENTER | DT_VCENTER | DT_SINGLELINE, text);
            }
            else if (key.type == e_key_type::accents)
            {
                RECT icon = badge_row;
                icon.top += scale_y(4, height);
                draw_trigger_icon(device_context, icon, text);
                RECT rt{ rect.left + (rect.right - rect.left) / 2 - scale_x(16, width), badge_row.top + scale_y(4, height), rect.left + (rect.right - rect.left) / 2 + scale_x(16, width), badge_row.bottom - scale_y(2, height) };
                draw_bumper_badge(device_context, rt, L"RT");
                draw_text(device_context, m_side_font, label_rect, L"Accents", DT_CENTER | DT_VCENTER | DT_SINGLELINE, text);
            }
            else if (key.type == e_key_type::caps)
            {
                RECT icon{ rect.left, badge_row.top + scale_y(4, height), rect.right, label_rect.top - scale_y(2, height) };
                draw_stick_icon(device_context, icon, text);
                RECT l_badge{ rect.left + (rect.right - rect.left) / 2 - scale_x(12, width), badge_row.top + scale_y(6, height), rect.left + (rect.right - rect.left) / 2 + scale_x(12, width), badge_row.top + scale_y(24, height) };
                draw_bumper_badge(device_context, l_badge, L"L");
                draw_text(device_context, m_side_font, label_rect, L"Caps", DT_CENTER | DT_VCENTER | DT_SINGLELINE, text);
            }
        }

        void draw_key(HDC device_context, s_key const& key, bool selected)
        {
            if (key.type == e_key_type::left || key.type == e_key_type::right ||
                key.type == e_key_type::caps || key.type == e_key_type::symbols ||
                key.type == e_key_type::accents)
            {
                draw_side_key(device_context, key, selected);
                return;
            }

            RECT rect = to_client_rect(key.rect);
            int width = GetClientRectWidth();
            int height = GetClientRectHeight();
            int radius = round_radius(width, height);
            bool done = key.type == e_key_type::done;

            COLORREF fill = RGB(236, 238, 239);
            COLORREF text = RGB(44, 47, 50);
            COLORREF border = RGB(160, 164, 168);

            if (done)
            {
                fill = selected ? RGB(168, 196, 96) : RGB(181, 204, 122);
                text = RGB(40, 48, 32);
                border = RGB(150, 172, 96);
            }
            else if (selected)
            {
                fill = RGB(122, 164, 58);
                text = RGB(255, 255, 255);
                border = RGB(96, 132, 42);
            }

            fill_round_rect(device_context, rect, fill, border, radius);

            if (key.type == e_key_type::character)
            {
                wchar_t character = key.character;
                if (m_caps && character >= L'a' && character <= L'z')
                {
                    character = static_cast<wchar_t>(character - (L'a' - L'A'));
                }
                wchar_t text_buffer[2]{ character, L'\0' };
                draw_text(
                    device_context,
                    m_key_font,
                    rect,
                    text_buffer,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE,
                    text);
            }
            else if (key.type == e_key_type::backspace || key.type == e_key_type::space)
            {
                int badge_size = scale_y(22, height);
                RECT badge{
                    rect.left + (rect.right - rect.left) / 2 - scale_x(70, width),
                    rect.top + (rect.bottom - rect.top - badge_size) / 2,
                    rect.left + (rect.right - rect.left) / 2 - scale_x(70, width) + badge_size,
                    rect.top + (rect.bottom - rect.top + badge_size) / 2 };

                COLORREF badge_color = key.type == e_key_type::backspace ? RGB(46, 113, 176) : RGB(214, 161, 28);
                draw_face_badge(
                    device_context,
                    badge,
                    key.type == e_key_type::backspace ? L'X' : L'Y',
                    badge_color);

                RECT label = rect;
                label.left = badge.right + scale_x(10, width);
                label.right = rect.right - scale_x(18, width);
                draw_text(
                    device_context,
                    m_key_font,
                    label,
                    key.label,
                    DT_LEFT | DT_VCENTER | DT_SINGLELINE,
                    selected ? RGB(255, 255, 255) : RGB(44, 47, 50));
            }
            else if (done)
            {
                int play_size = scale_y(28, height);
                RECT play{
                    rect.left + (rect.right - rect.left) / 2 - scale_x(52, width),
                    rect.top + (rect.bottom - rect.top - play_size) / 2,
                    rect.left + (rect.right - rect.left) / 2 - scale_x(52, width) + play_size,
                    rect.top + (rect.bottom - rect.top + play_size) / 2 };

                draw_circle(device_context, play, RGB(244, 246, 236), RGB(244, 246, 236));

                POINT triangle[3]{
                    { play.left + scale_x(10, width), play.top + scale_y(7, height) },
                    { play.left + scale_x(10, width), play.bottom - scale_y(7, height) },
                    { play.right - scale_x(7, width), play.top + (play.bottom - play.top) / 2 },
                };

                HBRUSH triangle_brush = CreateSolidBrush(RGB(90, 132, 40));
                HPEN triangle_pen = CreatePen(PS_SOLID, 1, RGB(90, 132, 40));
                HBRUSH old_brush = static_cast<HBRUSH>(SelectObject(device_context, triangle_brush));
                HPEN old_pen = static_cast<HPEN>(SelectObject(device_context, triangle_pen));
                Polygon(device_context, triangle, 3);
                SelectObject(device_context, old_pen);
                SelectObject(device_context, old_brush);
                DeleteObject(triangle_pen);
                DeleteObject(triangle_brush);

                RECT label = rect;
                label.left = play.right + scale_x(8, width);
                draw_text(device_context, m_key_font, label, key.label, DT_LEFT | DT_VCENTER | DT_SINGLELINE, text);
            }
        }

        void draw_vertical_gradient(HDC device_context, RECT rect, COLORREF top, COLORREF bottom)
        {
            TRIVERTEX vertices[2]{};
            vertices[0].x = rect.left;
            vertices[0].y = rect.top;
            vertices[0].Red = static_cast<COLOR16>(GetRValue(top) << 8);
            vertices[0].Green = static_cast<COLOR16>(GetGValue(top) << 8);
            vertices[0].Blue = static_cast<COLOR16>(GetBValue(top) << 8);
            vertices[0].Alpha = 0;

            vertices[1].x = rect.right;
            vertices[1].y = rect.bottom;
            vertices[1].Red = static_cast<COLOR16>(GetRValue(bottom) << 8);
            vertices[1].Green = static_cast<COLOR16>(GetGValue(bottom) << 8);
            vertices[1].Blue = static_cast<COLOR16>(GetBValue(bottom) << 8);
            vertices[1].Alpha = 0;

            GRADIENT_RECT gradient_rect{ 0, 1 };
            GradientFill(device_context, vertices, 2, &gradient_rect, 1, GRADIENT_FILL_RECT_V);
        }

        void paint(HDC device_context)
        {
            RECT client_rect{};
            GetClientRect(m_handle, &client_rect);
            int width = client_rect.right;
            int height = client_rect.bottom;

            fill_rect(device_context, client_rect, RGB(0, 0, 0));

            draw_text(
                device_context,
                m_title_font,
                RECT{ scale_x(k_panel_left, width), scale_y(48, height), scale_x(700, width), scale_y(92, height) },
                m_title_text.c_str(),
                DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX,
                RGB(240, 242, 243));

            RECT panel = scale_rect(RECT{ k_panel_left, k_panel_top, k_panel_right, k_panel_bottom }, width, height);
            fill_rect(device_context, panel, RGB(212, 216, 219));

            RECT upper_panel{
                panel.left,
                panel.top,
                panel.right,
                scale_y(318, height) };
            draw_vertical_gradient(device_context, upper_panel, RGB(142, 150, 156), RGB(90, 98, 104));

            if (!m_description_text.empty())
            {
                draw_text(
                    device_context,
                    m_header_font,
                    RECT{
                        scale_x(k_input_left, width),
                        scale_y(118, height),
                        scale_x(k_input_right, width),
                        scale_y(170, height) },
                        m_description_text.c_str(),
                        DT_LEFT | DT_WORDBREAK | DT_NOPREFIX,
                        RGB(244, 246, 247));
            }

            RECT input = to_client_rect(RECT{ k_input_left, k_input_top, k_input_right, k_input_bottom });
            fill_round_rect(device_context, input, RGB(243, 243, 243), RGB(142, 146, 150), round_radius(width, height));

            RECT input_text{
                input.left + scale_x(12, width),
                input.top,
                input.right - scale_x(12, width),
                input.bottom };
            draw_text(
                device_context,
                m_input_font,
                input_text,
                m_text.c_str(),
                DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX,
                RGB(37, 40, 42));

            if (GetTickCount() / 530 % 2 == 0)
            {
                SIZE text_size{};
                HFONT old_font = static_cast<HFONT>(SelectObject(device_context, m_input_font));
                std::wstring caret_prefix = m_text.substr(0, static_cast<size_t>(m_caret));
                GetTextExtentPoint32W(
                    device_context,
                    caret_prefix.c_str(),
                    static_cast<int>(caret_prefix.size()),
                    &text_size);
                SelectObject(device_context, old_font);

                int caret_x = input_text.left + text_size.cx;
                int caret_top = input.top + scale_y(8, height);
                int caret_bottom = input.bottom - scale_y(8, height);
                fill_rect(
                    device_context,
                    RECT{ caret_x, caret_top, caret_x + __max(1, scale_x(2, width)), caret_bottom },
                    RGB(32, 34, 36));
            }

            for (size_t i = 0; i < m_keys.size(); ++i)
            {
                draw_key(device_context, m_keys[i], static_cast<int>(i) == m_selected_key);
            }

            int badge_size = __max(18, scale_y(22, height));
            RECT footer_a{
                scale_x(220, width),
                scale_y(590, height),
                scale_x(400, width),
                scale_y(628, height) };
            RECT a_badge{
                footer_a.left,
                footer_a.top + (footer_a.bottom - footer_a.top - badge_size) / 2,
                footer_a.left + badge_size,
                footer_a.top + (footer_a.bottom - footer_a.top - badge_size) / 2 + badge_size };
            draw_face_badge(device_context, a_badge, L'A', RGB(90, 176, 54));
            RECT a_label = footer_a;
            a_label.left = a_badge.right + scale_x(8, width);
            draw_text(device_context, m_footer_font, a_label, L"Select", DT_LEFT | DT_VCENTER | DT_SINGLELINE, RGB(230, 232, 234));

            RECT footer_b{
                scale_x(360, width),
                scale_y(590, height),
                scale_x(540, width),
                scale_y(628, height) };
            RECT b_badge{
                footer_b.left,
                footer_b.top + (footer_b.bottom - footer_b.top - badge_size) / 2,
                footer_b.left + badge_size,
                footer_b.top + (footer_b.bottom - footer_b.top - badge_size) / 2 + badge_size };
            draw_face_badge(device_context, b_badge, L'B', RGB(196, 58, 48));
            RECT b_label = footer_b;
            b_label.left = b_badge.right + scale_x(8, width);
            draw_text(device_context, m_footer_font, b_label, L"Back", DT_LEFT | DT_VCENTER | DT_SINGLELINE, RGB(230, 232, 234));
        }

        int key_at_point(POINT point) const
        {
            int result = -1;

            for (size_t i = 0; i < m_keys.size(); ++i)
            {
                if (!m_keys[i].enabled)
                {
                    continue;
                }

                RECT rect = to_client_rect(m_keys[i].rect);
                if (PtInRect(&rect, point) != FALSE)
                {
                    result = static_cast<int>(i);
                    break;
                }
            }

            return result;
        }

        void invalidate_key(int key_index)
        {
            if (key_index >= 0 && key_index < static_cast<int>(m_keys.size()))
            {
                RECT rect = to_client_rect(m_keys[key_index].rect);
                InflateRect(&rect, 2, 2);
                InvalidateRect(m_handle, &rect, FALSE);
            }
        }

        void select_key(int key_index)
        {
            if (key_index >= 0 && key_index < static_cast<int>(m_keys.size()) &&
                m_keys[key_index].enabled && key_index != m_selected_key)
            {
                int previous_key = m_selected_key;
                m_selected_key = key_index;
                invalidate_key(previous_key);
                invalidate_key(m_selected_key);
            }
        }

        void move_selection(int dx, int dy)
        {
            int next = m_selected_key;

            if (m_selected_key >= 0 && m_selected_key < 40)
            {
                int row = m_selected_key / 10;
                int column = m_selected_key % 10;

                if (dx < 0)
                {
                    if (column > 0)
                    {
                        next = row * 10 + (column - 1);
                    }
                    else if (row <= 1)
                    {
                        next = k_index_cursor_left;
                    }
                    else if (row == 3)
                    {
                        next = k_index_caps;
                    }
                }
                else if (dx > 0)
                {
                    if (column < 9)
                    {
                        next = row * 10 + (column + 1);
                    }
                    else if (row <= 1)
                    {
                        next = k_index_cursor_right;
                    }
                    else if (row == 3)
                    {
                        next = k_index_done;
                    }
                }
                else if (dy < 0)
                {
                    if (row > 0)
                    {
                        next = (row - 1) * 10 + column;
                    }
                }
                else if (dy > 0)
                {
                    if (row < 3)
                    {
                        next = (row + 1) * 10 + column;
                    }
                    else
                    {
                        next = column < 5 ? k_index_backspace : k_index_space;
                    }
                }
            }
            else if (m_selected_key == k_index_cursor_left)
            {
                if (dx > 0) next = 0;
                else if (dy > 0) next = 20;
            }
            else if (m_selected_key == k_index_cursor_right)
            {
                if (dx < 0) next = 9;
                else if (dy > 0) next = 29;
            }
            else if (m_selected_key == k_index_caps)
            {
                if (dx > 0) next = 30;
                else if (dy < 0) next = 20;
                else if (dy > 0) next = k_index_backspace;
            }
            else if (m_selected_key == k_index_done)
            {
                if (dx < 0) next = k_index_space;
                else if (dy < 0) next = 39;
            }
            else if (m_selected_key == k_index_backspace)
            {
                if (dx > 0) next = k_index_space;
                else if (dx < 0) next = k_index_caps;
                else if (dy < 0) next = 32;
            }
            else if (m_selected_key == k_index_space)
            {
                if (dx < 0) next = k_index_backspace;
                else if (dx > 0) next = k_index_done;
                else if (dy < 0) next = 37;
            }
            else
            {
                next = 24;
            }

            select_key(next);
        }

        void activate_key(int key_index)
        {
            if (key_index < 0 || key_index >= static_cast<int>(m_keys.size()))
            {
                return;
            }

            s_key const& key = m_keys[key_index];
            if (!key.enabled)
            {
                return;
            }

            switch (key.type)
            {
            case e_key_type::character:
            {
                wchar_t character = key.character;
                if (m_caps && character >= L'a' && character <= L'z')
                {
                    character = static_cast<wchar_t>(character - (L'a' - L'A'));
                }
                append_character(character);
            }
            break;

            case e_key_type::backspace:
                delete_character();
                break;

            case e_key_type::space:
                append_character(L' ');
                break;

            case e_key_type::done:
                set_completed();
                break;

            case e_key_type::caps:
                m_caps = !m_caps;
                {
                    RECT keyboard_rect = to_client_rect(RECT{
                        k_keyboard_left,
                        k_keyboard_top,
                        k_keyboard_left + 10 * (k_key_width + k_key_gap),
                        k_keyboard_top + 4 * (k_key_height + k_key_gap) });
                    InvalidateRect(m_handle, &keyboard_rect, FALSE);
                    invalidate_key(k_index_caps);
                }
                break;

            case e_key_type::left:
                move_caret(-1);
                break;

            case e_key_type::right:
                move_caret(1);
                break;

            case e_key_type::symbols:
            case e_key_type::accents:
                break;
            }
        }

        void invalidate_input()
        {
            RECT rect = to_client_rect(RECT{ k_input_left, k_input_top, k_input_right, k_input_bottom });
            InflateRect(&rect, 2, 2);
            InvalidateRect(m_handle, &rect, FALSE);
        }

        void append_character(wchar_t character)
        {
            if (m_maximum_character_count > 0 && m_text.size() + 1 < m_maximum_character_count)
            {
                m_text.insert(m_text.begin() + m_caret, character);
                ++m_caret;
                invalidate_input();
            }
        }

        void delete_character()
        {
            if (m_caret > 0 && !m_text.empty())
            {
                m_text.erase(m_text.begin() + (m_caret - 1));
                --m_caret;
                invalidate_input();
            }
        }

        void move_caret(int delta)
        {
            int next = m_caret + delta;
            if (next < 0) next = 0;
            if (next > static_cast<int>(m_text.size())) next = static_cast<int>(m_text.size());
            if (next != m_caret)
            {
                m_caret = next;
                invalidate_input();
            }
        }

        void poll_controller()
        {
            XINPUT_STATE state{};
            DWORD status = XInputGetState(m_controller_index, &state);

            if (status != ERROR_SUCCESS)
            {
                m_previous_buttons = 0;
                return;
            }

            WORD buttons = state.Gamepad.wButtons;
            WORD pressed = static_cast<WORD>(buttons & ~m_previous_buttons);
            m_previous_buttons = buttons;

            if ((pressed & XINPUT_GAMEPAD_B) != 0)
            {
                set_cancelled();
            }
            else if ((pressed & XINPUT_GAMEPAD_A) != 0)
            {
                activate_key(m_selected_key);
            }
            else if ((pressed & XINPUT_GAMEPAD_X) != 0)
            {
                activate_key(k_index_backspace);
            }
            else if ((pressed & XINPUT_GAMEPAD_Y) != 0)
            {
                activate_key(k_index_space);
            }
            else if ((pressed & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0)
            {
                move_caret(-1);
            }
            else if ((pressed & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0)
            {
                move_caret(1);
            }
            else if ((pressed & XINPUT_GAMEPAD_LEFT_THUMB) != 0)
            {
                activate_key(k_index_caps);
            }
            else if ((pressed & XINPUT_GAMEPAD_START) != 0)
            {
                activate_key(k_index_done);
            }

            if (m_completed)
            {
                return;
            }

            int dx = 0;
            int dy = 0;

            if ((pressed & XINPUT_GAMEPAD_DPAD_LEFT) != 0)
            {
                dx = -1;
            }
            else if ((pressed & XINPUT_GAMEPAD_DPAD_RIGHT) != 0)
            {
                dx = 1;
            }
            else if ((pressed & XINPUT_GAMEPAD_DPAD_UP) != 0)
            {
                dy = -1;
            }
            else if ((pressed & XINPUT_GAMEPAD_DPAD_DOWN) != 0)
            {
                dy = 1;
            }

            SHORT stick_x = state.Gamepad.sThumbLX;
            SHORT stick_y = state.Gamepad.sThumbLY;

            if (dx == 0 && dy == 0)
            {
                if (stick_x > k_stick_deadzone)
                {
                    dx = 1;
                }
                else if (stick_x < -k_stick_deadzone)
                {
                    dx = -1;
                }
                else if (stick_y > k_stick_deadzone)
                {
                    dy = -1;
                }
                else if (stick_y < -k_stick_deadzone)
                {
                    dy = 1;
                }
            }

            if (dx != 0 || dy != 0)
            {
                DWORD now = GetTickCount();
                if (pressed != 0 || now - m_last_navigation_time >= 120)
                {
                    move_selection(dx, dy);
                    m_last_navigation_time = now;
                }
            }
        }

        void complete_overlapped(ULONG status, ULONG_PTR bytes)
        {
            if (m_overlapped != nullptr)
            {
                m_overlapped->InternalLow = status;
                m_overlapped->InternalHigh = bytes;
                m_overlapped->dwExtendedError = status;
                if (m_overlapped->hEvent != nullptr)
                {
                    SetEvent(m_overlapped->hEvent);
                }
            }
        }

        LRESULT window_proc(UINT message, WPARAM w_param, LPARAM l_param)
        {
            LRESULT result = 0;

            switch (message)
            {
            case WM_PAINT:
            {
                PAINTSTRUCT paint_struct{};
                HDC device_context = BeginPaint(m_handle, &paint_struct);

                RECT client_rect{};
                GetClientRect(m_handle, &client_rect);
                int width = client_rect.right - client_rect.left;
                int height = client_rect.bottom - client_rect.top;

                HDC buffer_dc = CreateCompatibleDC(device_context);
                HBITMAP buffer_bitmap = CreateCompatibleBitmap(device_context, width, height);
                HBITMAP previous_bitmap = static_cast<HBITMAP>(SelectObject(buffer_dc, buffer_bitmap));

                paint(buffer_dc);
                BitBlt(
                    device_context,
                    paint_struct.rcPaint.left,
                    paint_struct.rcPaint.top,
                    paint_struct.rcPaint.right - paint_struct.rcPaint.left,
                    paint_struct.rcPaint.bottom - paint_struct.rcPaint.top,
                    buffer_dc,
                    paint_struct.rcPaint.left,
                    paint_struct.rcPaint.top,
                    SRCCOPY);

                SelectObject(buffer_dc, previous_bitmap);
                DeleteObject(buffer_bitmap);
                DeleteDC(buffer_dc);
                EndPaint(m_handle, &paint_struct);
            }
            break;

            case WM_ERASEBKGND:
                result = TRUE;
                break;

            case WM_LBUTTONDOWN:
            {
                POINT point{ GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param) };
                int key_index = key_at_point(point);
                if (key_index >= 0)
                {
                    select_key(key_index);
                    activate_key(key_index);
                }
            }
            break;

            case WM_KEYDOWN:
                switch (w_param)
                {
                case VK_RETURN:
                    activate_key(m_selected_key);
                    break;

                case VK_ESCAPE:
                    set_cancelled();
                    break;

                case VK_BACK:
                case VK_DELETE:
                    activate_key(k_index_backspace);
                    break;

                case VK_LEFT:
                    if (GetKeyState(VK_CONTROL) < 0)
                    {
                        move_caret(-1);
                    }
                    else
                    {
                        move_selection(-1, 0);
                    }
                    break;

                case VK_RIGHT:
                    if (GetKeyState(VK_CONTROL) < 0)
                    {
                        move_caret(1);
                    }
                    else
                    {
                        move_selection(1, 0);
                    }
                    break;

                case VK_UP:
                    move_selection(0, -1);
                    break;

                case VK_DOWN:
                    move_selection(0, 1);
                    break;

                case VK_CAPITAL:
                    activate_key(k_index_caps);
                    break;

                case VK_HOME:
                    m_caret = 0;
                    invalidate_input();
                    break;

                case VK_END:
                    m_caret = static_cast<int>(m_text.size());
                    invalidate_input();
                    break;

                default:
                    break;
                }
                break;

            case WM_CHAR:
                if (w_param == L' ')
                {
                    append_character(L' ');
                }
                else if (w_param >= 32 && w_param != 127)
                {
                    append_character(static_cast<wchar_t>(w_param));
                }
                break;

            case WM_TIMER:
                if (w_param == k_controller_timer_id)
                {
                    poll_controller();
                    if (GetTickCount() / 530 != m_last_caret_blink)
                    {
                        m_last_caret_blink = GetTickCount() / 530;
                        invalidate_input();
                    }
                }
                break;

            case WM_CLOSE:
                set_cancelled();
                break;

            case WM_NCDESTROY:
                KillTimer(m_handle, k_controller_timer_id);
                release_fonts();
                resume_game_input_for_keyboard();
                if (g_virtual_keyboard == this)
                {
                    g_virtual_keyboard = nullptr;
                }
                break;

            default:
                result = DefWindowProcW(m_handle, message, w_param, l_param);
                break;
            }

            return result;
        }

        void release_fonts()
        {
            HFONT fonts[] =
            {
                m_title_font,
                m_header_font,
                m_input_font,
                m_key_font,
                m_side_font,
                m_footer_font,
                m_badge_font,
            };

            for (HFONT font : fonts)
            {
                if (font != nullptr)
                {
                    DeleteObject(font);
                }
            }

            m_title_font = nullptr;
            m_header_font = nullptr;
            m_input_font = nullptr;
            m_key_font = nullptr;
            m_side_font = nullptr;
            m_footer_font = nullptr;
            m_badge_font = nullptr;
        }

        HWND m_handle = nullptr;
        HFONT m_title_font = nullptr;
        HFONT m_header_font = nullptr;
        HFONT m_input_font = nullptr;
        HFONT m_key_font = nullptr;
        HFONT m_side_font = nullptr;
        HFONT m_footer_font = nullptr;
        HFONT m_badge_font = nullptr;
        std::vector<s_key> m_keys;
        std::wstring m_title_text;
        std::wstring m_description_text;
        std::wstring m_text;
        wchar_t* m_result_text = nullptr;
        unsigned long m_maximum_character_count = 0;
        OVERLAPPED* m_overlapped = nullptr;
        DWORD m_controller_index = 0;
        WORD m_previous_buttons = 0;
        DWORD m_last_navigation_time = 0;
        DWORD m_last_caret_blink = 0;
        int m_selected_key = 24;
        int m_caret = 0;
        bool m_caps = false;
        bool m_completed = false;
    };

    void initialize_overlapped(OVERLAPPED* overlapped)
    {
        if (overlapped != nullptr)
        {
            overlapped->Internal = ERROR_IO_PENDING;
            overlapped->InternalHigh = 0;
            if (overlapped->hEvent != nullptr)
            {
                ResetEvent(overlapped->hEvent);
            }
        }
    }
}

uns32 __cdecl online_guide_show_virtual_keyboard_ui(
    e_controller_index controller_index,
    uns32 character_flags,
    const wchar_t* default_text,
    const wchar_t* title_text,
    const wchar_t* description_text,
    wchar_t* result_text,
    uns32 maximum_character_count,
    void* overlapped)
{
    (void)character_flags;

    OVERLAPPED* platform_handle = static_cast<OVERLAPPED*>(overlapped);
    uns32 result = ERROR_INVALID_PARAMETER;

    if (platform_handle != nullptr && result_text != nullptr && maximum_character_count != 0 && g_virtual_keyboard == nullptr)
    {
        initialize_overlapped(platform_handle);

        c_virtual_keyboard_window* keyboard = new c_virtual_keyboard_window(
            default_text,
            title_text,
            description_text,
            result_text,
            maximum_character_count,
            platform_handle,
            static_cast<DWORD>(controller_index));

        if (keyboard->create())
        {
            g_virtual_keyboard = keyboard;
            result = ERROR_IO_PENDING;
        }
        else
        {
            platform_handle->Internal = ERROR_GEN_FAILURE;
            delete keyboard;
            result = ERROR_GEN_FAILURE;
        }
    }

    return result;
}

