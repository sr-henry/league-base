#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "../game/vectors.h"
#include "menu.h"

#define PI 3.14159265358979323846

enum Colors
{
	RED = D3DCOLOR_ARGB(255, 255, 0, 0),
	GREEN = D3DCOLOR_ARGB(255, 0, 255, 0),
	BLUE = D3DCOLOR_ARGB(255, 0, 0, 255),
	WHITE = D3DCOLOR_ARGB(255, 255, 255, 255)
};

class Overlay
{
public:
	Overlay();
	~Overlay();

public:
	int WIDTH = 0;
	int HEIGHT = 0;

private: // Window data
	LPCWSTR sTargetWindowName;
	LPCWSTR sOverlayWindowName;

	RECT rect;
	HWND thWnd = nullptr;
	HWND ohWnd = nullptr;
	WNDCLASSEXW wc = { };

private: // D3d9 data
	IDirect3D9Ex* g_pD3D = NULL;
	IDirect3DDevice9Ex* g_pd3dDevice = NULL;
	D3DPRESENT_PARAMETERS g_d3dpp;
	ID3DXLine* linel;
	ID3DXFont* fontf;

private:
	void CreateOverlayWindow() noexcept;
	void DestroyOverlayWindow() noexcept;

	bool CreateDeviceD3D() noexcept;
	void DestroyDeviceD3D() noexcept;
	void ResetDevice() noexcept;

	void CreateImgui() noexcept;
	void DestroyImgui() noexcept;
	
public:
	void BeginRender() noexcept;
	void RenderMenu() noexcept;
	void EndRender() noexcept;
	
	void Line(vec2 src, vec2 dst, int thickness, D3DCOLOR color);
	void Box2D(vec2 center, vec2 offset, int thickness, D3DCOLOR color);
	void Text(const char* text, vec2 position, D3DCOLOR color);
	void Ellipse(vec2 center, int a, int b, int numSides, int thickness, D3DCOLOR color);
	void Circle(vec2 center, int radius, int numSides, int thickness, D3DCOLOR color);
};