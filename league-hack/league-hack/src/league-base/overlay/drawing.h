#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include "../vectors.h"

class Drawing
{
public:
	Drawing();
    ~Drawing();

private:
    IDirect3D9Ex* d3dObject = NULL;
    IDirect3DDevice9Ex* d3dDevice = NULL;
    D3DPRESENT_PARAMETERS d3dParams;

    ID3DXLine* linel;
    ID3DXFont* fontf;

    int width, height;

public:
    void StartRender();
    void EndRender();
    void Clear();

public:
    void Line(vec2 src, vec2 dst, int thickness, D3DCOLOR color);
    void Box2D(vec2 center, vec2 offset, int thickness, D3DCOLOR color);
    void Text(const char* text, float x, float y, D3DCOLOR color);
};

