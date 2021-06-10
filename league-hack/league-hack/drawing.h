#pragma once

class Drawing
{
private:
    IDirect3D9Ex* d3dObject = NULL;
    IDirect3DDevice9Ex* d3dDevice = NULL;
    D3DPRESENT_PARAMETERS d3dParams;

    int width;
    int height;

    ID3DXLine* linel;
    ID3DXFont* fontf;


public:
    Drawing();
    ~Drawing();

    void sRender();
    void fRender();
    void clear();
    
    void drawFilledRect(int x, int y, int w, int h, D3DCOLOR color);
    void drawLine(vec2 src, vec2 dst, int thickness, D3DCOLOR color);
    void drawEspBox2D(vec2 top, vec2 bot, int thickness, D3DCOLOR color);
    void drawText(const char* text, float x, float y, D3DCOLOR color);
    void drawCircle(int x, int y, int radius, int numSides, int thickness, D3DCOLOR color);
    void drawEllipse(int x, int y, int a, int b, int numSides, int thickness, D3DCOLOR color);

};

