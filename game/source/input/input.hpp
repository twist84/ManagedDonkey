#pragma once

enum e_key_code;
enum e_mouse_button;
enum e_input_type;
struct s_key_state;
struct s_mouse_state;

extern bool __cdecl input_peek_key(s_key_state* key, e_input_type input_type);
extern bool __cdecl input_peek_mouse(s_mouse_state* mouse, e_input_type input_type);
extern bool __cdecl input_has_gamepad(short gamepad_index);
extern unsigned char __cdecl input_key_frames_down(e_key_code key_code, e_input_type input_type);
extern unsigned short __cdecl input_key_msec_down(e_key_code key_code, e_input_type input_type);
extern unsigned char __cdecl input_mouse_frames_down(e_mouse_button mouse_button, e_input_type input_type);
extern unsigned short __cdecl input_mouse_msec_down(e_mouse_button mouse_button, e_input_type input_type);

