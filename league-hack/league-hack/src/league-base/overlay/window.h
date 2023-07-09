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

namespace Overlay
{
	inline WCHAR sOverlayWindowName[100] = L"league-overlay";

	inline RECT rect;
	inline int WIDTH = 0;
	inline int HEIGHT = 0;
	
	// Window data
	inline HWND thWnd = nullptr;
	inline HWND ohWnd = nullptr;
	inline WNDCLASSEXW wc = { };

	// D3d9 data
	inline IDirect3D9Ex* g_pD3D = NULL;
	inline IDirect3DDevice9Ex* g_pd3dDevice = NULL;
	inline D3DPRESENT_PARAMETERS g_d3dpp;
	inline ID3DXLine* linel;
	inline ID3DXFont* fontf;
	
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
	void RenderMenu() noexcept;

	void Line(vec2 src, vec2 dst, int thickness, D3DCOLOR color);
	void Box2D(vec2 center, vec2 offset, int thickness, D3DCOLOR color);
	void Text(const char* text, vec2 position, D3DCOLOR color);
	void Ellipse(vec2 center, int a, int b, int numSides, int thickness, D3DCOLOR color);
	void Circle(vec2 center, int radius, int numSides, int thickness, D3DCOLOR color);
}