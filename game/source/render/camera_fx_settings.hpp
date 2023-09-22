#pragma once

#include "cseries/cseries.hpp"

struct s_scripted_exposure
{
	byte __data[0x34];
};
static_assert(sizeof(s_scripted_exposure) == 0x34);

struct c_camera_fx_settings
{
	byte __data[0x170];
};
static_assert(sizeof(c_camera_fx_settings) == 0x170);

struct c_camera_fx_values
{
	c_camera_fx_settings m_settings;
	byte __data[0x80];
};
static_assert(sizeof(c_camera_fx_values) == 0x1F0);

