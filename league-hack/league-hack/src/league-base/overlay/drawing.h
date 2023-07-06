//#pragma once
//#include <d3d9.h>
//#include <d3dx9.h>
//#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")
//#include "../vectors.h"
//
//#define PI 3.14159265358979323846
//
//class Drawing
//{
//public:
//	Drawing();
//    ~Drawing();
//
//public:
//    int width, height;
//
//private:
//    IDirect3D9Ex* d3dObject = NULL;
//    IDirect3DDevice9Ex* d3dDevice = NULL;
//    D3DPRESENT_PARAMETERS d3dParams;
//    ID3DXLine* linel;
//    ID3DXFont* fontf;
//
//public:
//    void StartRender();
//    void EndRender();
//    void Clear();
//
//public:
//    void Line(vec2 src, vec2 dst, int thickness, D3DCOLOR color);
//    void Box2D(vec2 center, vec2 offset, int thickness, D3DCOLOR color);
//    void Text(const char* text, float x, float y, D3DCOLOR color);
//    void Ellipse(vec2 center, int a, int b, int numSides, int thickness, D3DCOLOR color);
//    void Circle(vec2 center, int radius, int numSides, int thickness, D3DCOLOR color);
//};
//
