#pragma once

#include "../Utilities/String.h"

class GamePlay;

struct GfxConfigs
{
	AnsiString Title = "Unnamed Application";
	int ScreenWidth = 960;
	int ScreenHeight = 540;
	uint8_t ColorBitsPerChannel = 8;
	uint8_t DepthBits = 24;
	uint8_t StencilBits = 8;
	uint8_t MSAASamples = 4;

	int GLMajor = 3;
	int GLMinor = 3;
	/// @brief Enable OpenGL Debug messages.
	/// @note Require GL Version 4.3+
	bool GLDebug = false;

	/// @brief Frames per second.
	float FPS = 60;
	/// @brief Global Game Controller
	GamePlay* Controller = nullptr;
};