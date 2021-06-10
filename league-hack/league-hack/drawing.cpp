#include "includes.h"

Drawing::Drawing() {

    HWND hOverlayWindow = FindWindow(NULL, L"O");

    RECT rect;
    GetWindowRect(hOverlayWindow, &rect);

    this->width = rect.right - rect.left;
    this->height = rect.bottom - rect.top;

    if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &d3dObject))) {
        exit(1);
    }

    ZeroMemory(&d3dParams, sizeof(d3dParams));

    d3dParams.BackBufferWidth = width;
    d3dParams.BackBufferHeight = height;
    d3dParams.Windowed = TRUE;
    d3dParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dParams.hDeviceWindow = hOverlayWindow;
    d3dParams.MultiSampleQuality = D3DMULTISAMPLE_NONE;
    d3dParams.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dParams.EnableAutoDepthStencil = TRUE;
    d3dParams.AutoDepthStencilFormat = D3DFMT_D16;

    HRESULT res = d3dObject->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hOverlayWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dParams, 0, &d3dDevice);

    if (FAILED(res)) {
        exit(1);
    }
}

Drawing::~Drawing() {
    if (d3dDevice != NULL)
        d3dDevice->Release();
    if (d3dObject != NULL)
        d3dObject->Release();
    if (linel != NULL)
        linel->Release();
    if (fontf != NULL)
        fontf->Release();
}

void Drawing::sRender() {
    d3dDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
    d3dDevice->BeginScene();
}

void Drawing::fRender() {
    d3dDevice->EndScene();
    d3dDevice->PresentEx(0, 0, 0, 0, 0);
}

void Drawing::clear() {
    d3dDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
    d3dDevice->BeginScene();
    d3dDevice->EndScene();
    d3dDevice->PresentEx(0, 0, 0, 0, 0);
}

void Drawing::drawFilledRect(int x, int y, int w, int h, D3DCOLOR color) {
    D3DRECT rect = { x,y,x + w,y + h };
    d3dDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void Drawing::drawLine(vec2 src, vec2 dst, int thickness, D3DCOLOR color) {
    if (!linel)
        D3DXCreateLine(d3dDevice, &linel);

    D3DXVECTOR2 line[2];
    line[0] = D3DXVECTOR2(src.x, src.y);
    line[1] = D3DXVECTOR2(dst.x, dst.y);

    linel->SetWidth(thickness);
    linel->Draw(line, 2, color);
}

void Drawing::drawEspBox2D(vec2 top, vec2 bot, int thickness, D3DCOLOR color) {
    int height = abs((int)(top.y - bot.y));
    vec2 tl, tr;
    tl.x = top.x - height / 4;
    tr.x = top.x + height / 4;
    tl.y = tr.y = top.y;

    vec2 bl, br;
    bl.x = bot.x - height / 4;
    br.x = bot.x + height / 4;
    bl.y = br.y = bot.y;

    this->drawLine(tl, tr, thickness, color);
    this->drawLine(bl, br, thickness, color);
    this->drawLine(tl, bl, thickness, color);
    this->drawLine(tr, br, thickness, color);
}

void Drawing::drawText(const char* text, float x, float y, D3DCOLOR color) {
    
    if (!fontf)
        D3DXCreateFont(d3dDevice, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &fontf);

    RECT rect;

    SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
    fontf->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 1, 1, 1));
    
    SetRect(&rect, x, y, x, y);
    fontf->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);

}

void Drawing::drawCircle(int x, int y, int radius, int numSides, int thickness, D3DCOLOR color) {
    if (!linel)
        D3DXCreateLine(d3dDevice, &linel);

    linel->SetWidth(thickness);

    D3DXVECTOR2 Line[128];
    float Step = PI * 2.0f / numSides;
    int Count = 0;
    for (float a = 0; a < PI * 2.0; a += Step) {
        float X1 = radius * cos(a) + x;
        float Y1 = radius * sin(a) + y;
        float X2 = radius * cos(a + Step) + x;
        float Y2 = radius * sin(a + Step) + y;
        Line[Count].x = X1;
        Line[Count].y = Y1;
        Line[Count + 1].x = X2;
        Line[Count + 1].y = Y2;
        Count += 2;
    }

    linel->Draw(Line, Count, color);

}

void Drawing::drawEllipse(int x, int y, int a, int b, int numSides, int thickness, D3DCOLOR color) {
    if (!linel)
        D3DXCreateLine(d3dDevice, &linel);

    linel->SetWidth(thickness);

    D3DXVECTOR2 Line[128];
    float Step = PI * 2.0f / numSides;
    int Count = 0;
    for (float t = 0; t < PI * 2.0; t += Step) {
        float X1 = a * cos(t) + x;
        float Y1 = b * sin(t) + y;
        float X2 = a * cos(t + Step) + x;
        float Y2 = b * sin(t + Step) + y;
        Line[Count].x = X1;
        Line[Count].y = Y1;
        Line[Count + 1].x = X2;
        Line[Count + 1].y = Y2;
        Count += 2;
    }

    linel->Draw(Line, Count, color);
    
}