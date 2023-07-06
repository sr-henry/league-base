#pragma once
#include <thread>
#include <chrono>
#include <Windows.h>
#include "../vectors.h"

class Utils
{
public:
	Utils() {};
	Utils(HWND hwnd) { hGameWindow = hwnd; };

private:
	HWND hGameWindow;

public:
	vec2 MousePos();

	void MouseMove(vec2 pos);

	void MouseMoveRelative(int x, int y);

	void MouseMoveSmooth(vec2 pos, int smoothing, int delay);

	void MouseRightClick(vec2 pos);

	void MouseLeftClick(vec2 pos);

	void KeyboardPressKey(char key);
};

