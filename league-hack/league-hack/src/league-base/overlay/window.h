#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "../vectors.h"

#define PI 3.14159265358979323846

enum Colors
{
	RED = D3DCOLOR_ARGB(255, 255, 0, 0),
	GREEN = D3DCOLOR_ARGB(255, 0, 255, 0),
	BLUE = D3DCOLOR_ARGB(255, 0, 0, 255),
	WHITE = D3DCOLOR_ARGB(255, 255, 255, 255)
};

namespace Overlay
{
	static WCHAR sOverlayWindowName[100] = L"league-overlay";

	static bool show_demo_window, show_another_window;
	
	static RECT rect;
	static int WIDTH = 0;
	static int HEIGHT = 0;
	
	// Window data
	static HWND thWnd = nullptr;
	static HWND ohWnd = nullptr;
	static WNDCLASSEXW wc = { };

	// D3d9 data
	static IDirect3D9Ex* g_pD3D = NULL;
	static IDirect3DDevice9Ex* g_pd3dDevice = NULL;
	static D3DPRESENT_PARAMETERS g_d3dpp;
	static ID3DXLine* linel;
	static ID3DXFont* fontf;
	
	void CreateOverlayWindow(
		LPCWSTR sTargetWindowName
	) noexcept;

	void DestroyOverlayWindow() noexcept;

	bool CreateDeviceD3D() noexcept;
	void DestroyDeviceD3D() noexcept;
	void ResetDevice() noexcept;
	
	void BeginRender() noexcept;
	void EndRender() noexcept;

	void CreateImgui() noexcept;
	void DestroyImgui() noexcept;
	void RenderImgui() noexcept;

	void Line(vec2 src, vec2 dst, int thickness, D3DCOLOR color);
	void Box2D(vec2 center, vec2 offset, int thickness, D3DCOLOR color);
	void Text(const char* text, float x, float y, D3DCOLOR color);
	void Ellipse(vec2 center, int a, int b, int numSides, int thickness, D3DCOLOR color);
	void Circle(vec2 center, int radius, int numSides, int thickness, D3DCOLOR color);
}