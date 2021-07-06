#include "utils.h"

vec2 Utils::MousePos()
{
	POINT pCursorPos;
	GetCursorPos(&pCursorPos);
	ScreenToClient(hGameWindow, &pCursorPos);
	return vec2{ static_cast<float>(pCursorPos.x), static_cast<float>(pCursorPos.y) };
}

void Utils::MouseMove(vec2 pos)
{
	vec2 mouse = MousePos();
	INPUT in = { 0 };
	in.type = INPUT_MOUSE;
	in.mi.dwFlags = MOUSEEVENTF_MOVE;
	in.mi.dx = pos.x - mouse.x;
	in.mi.dy = pos.y - mouse.y;
	SendInput(1, &in, sizeof(in));
}

void Utils::MouseMoveRelative(int x, int y)
{
	INPUT in = { 0 };
	in.type = INPUT_MOUSE;
	in.mi.dwFlags = MOUSEEVENTF_MOVE;
	in.mi.dx = x;
	in.mi.dy = y;
	SendInput(1, &in, sizeof(in));
}

void Utils::MouseMoveSmooth(vec2 pos, int smoothing, int delay)
{
	vec2 delta = pos - MousePos();

	int x = static_cast<int>(delta.x);
	int y = static_cast<int>(delta.y);

	int _x = 0, _y = 0, _t = 0;

	for (int i = 1; i <= smoothing; i++) {
		int xi = i * x / smoothing;
		int yi = i * y / smoothing;
		int ti = i * delay / smoothing;
		MouseMoveRelative(xi - _x, yi - _y);
		std::this_thread::sleep_for(std::chrono::milliseconds(ti - _t));
		_x = xi; _y = yi; _t = ti;
	}
}

void Utils::MouseRightClick(vec2 pos)
{
	INPUT in = { 0 };

	in.type = INPUT_MOUSE;
	in.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	in.mi.dx = pos.x;
	in.mi.dy = pos.y;

	SendInput(1, &in, sizeof(in));

	ZeroMemory(&in, sizeof(in));

	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	in.type = INPUT_MOUSE;
	in.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	in.mi.dx = pos.x;
	in.mi.dy = pos.y;

	SendInput(1, &in, sizeof(in));
}

void Utils::KeyboardPressKey(char key)
{
	INPUT input = { 0 };
	UINT mappedkey = MapVirtualKey(LOBYTE(VkKeyScan(key)), 0);
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;
	SendInput(1, &input, sizeof(input));
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}