#pragma once

enum e_key_code;
enum e_mouse_button;
enum e_input_type;
struct key_stroke;
struct mouse_stroke;

extern bool __cdecl input_peek_key(key_stroke* key, e_input_type input_type);
extern bool __cdecl input_has_gamepad(short gamepad_index);
extern unsigned char __cdecl input_key_frames_down(e_key_code key_code, e_input_type input_type);
extern unsigned short __cdecl input_key_msec_down(e_key_code key_code, e_input_type input_type);
extern unsigned char __cdecl input_mouse_frames_down(e_mouse_button mouse_button, e_input_type input_type);
extern unsigned short __cdecl input_mouse_msec_down(e_mouse_button mouse_button, e_input_type input_type);

