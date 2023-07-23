#include "window.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (Menu::open && ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

Overlay::Overlay()
{
    sTargetWindowName = L"League of Legends (TM) Client";
    sOverlayWindowName = L"league-overlay";
    CreateOverlayWindow();
    CreateDeviceD3D();
    CreateImgui();
}

Overlay::~Overlay()
{
    DestroyImgui();
    DestroyDeviceD3D();
    DestroyOverlayWindow();
}

// Window
void Overlay::CreateOverlayWindow() noexcept
{
    thWnd = FindWindowW(0, sTargetWindowName);
    if (!thWnd)
        exit(1);

    GetWindowRect(thWnd, &rect);
    WIDTH = rect.right - rect.left;
    HEIGHT = rect.bottom - rect.top;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hIcon = 0;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    wc.lpszMenuName = sOverlayWindowName;
    wc.lpszClassName = sOverlayWindowName;
    wc.hIconSm = 0;

    RegisterClassExW(&wc);

    ohWnd = CreateWindowExW(
        WS_EX_TOPMOST  | WS_EX_LAYERED,// | WS_EX_TRANSPARENT,
        wc.lpszClassName,
        sOverlayWindowName,
        WS_POPUP,
        1, 1,
        WIDTH, HEIGHT,
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr
    );

    SetLayeredWindowAttributes(ohWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);//  LWA_COLORKEY LWA_ALPHA

    ShowWindow(ohWnd, SW_SHOWDEFAULT);

    UpdateWindow(ohWnd);
}

void Overlay::DestroyOverlayWindow() noexcept
{
    DestroyWindow(ohWnd);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

// D3d9
bool Overlay::CreateDeviceD3D() noexcept
{
    if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &g_pD3D)))
        return false;

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));

    g_d3dpp.BackBufferWidth = WIDTH;
    g_d3dpp.BackBufferHeight = HEIGHT;
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.hDeviceWindow = ohWnd;
    g_d3dpp.MultiSampleQuality = D3DMULTISAMPLE_NONE;
    g_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    HRESULT res = g_pD3D->CreateDeviceEx(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        ohWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &g_d3dpp,
        0,
        &g_pd3dDevice
    );

    if (FAILED(res))
        return false;

    return true;
}

void Overlay::DestroyDeviceD3D() noexcept
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
    if (linel) { linel->Release(); linel = nullptr; }
    if (fontf) { fontf->Release(); fontf = nullptr; }
}

void Overlay::ResetDevice() noexcept
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Imgui
void Overlay::CreateImgui() noexcept
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
   
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(ohWnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);
}

void Overlay::DestroyImgui() noexcept
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void Overlay::RenderMenu() noexcept
{
    if (GetAsyncKeyState(VK_INSERT) & 1)
        Menu::open = !Menu::open;

    if (Menu::open)
    {
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        Menu::RenderUI();

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    }
}

// Render
void Overlay::BeginRender() noexcept
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        GetWindowRect(thWnd, &rect);
        WIDTH = rect.right - rect.left;
        HEIGHT = rect.bottom - rect.top;
        MoveWindow(ohWnd, rect.left, rect.top, WIDTH, HEIGHT, true);
    }

    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

    g_pd3dDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
    g_pd3dDevice->BeginScene();
}

void Overlay::EndRender() noexcept
{
    g_pd3dDevice->EndScene();
    
    HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
    if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
    {
        ResetDevice();
    }
}

// D3d9 drawing
void Overlay::Circle(vec2 center, int radius, int numSides, int thickness, D3DCOLOR color)
{
    Ellipse(center, radius, radius, numSides, thickness, color);
}

void Overlay::Ellipse(vec2 center, int a, int b, int numSides, int thickness, D3DCOLOR color)
{
    if (!linel)
        D3DXCreateLine(g_pd3dDevice, &linel);
        
    linel->SetWidth(thickness);
        
    D3DXVECTOR2 Line[128];
    float Step = PI * 2.0f / numSides;
    int Count = 0;
    for (float t = 0; t < PI * 2.0; t += Step) {
        float X1 = a * cos(t) + center.x;
        float Y1 = b * sin(t) + center.y;
        float X2 = a * cos(t + Step) + center.x;
        float Y2 = b * sin(t + Step) + center.y;
        Line[Count].x = X1;
        Line[Count].y = Y1;
        Line[Count + 1].x = X2;
        Line[Count + 1].y = Y2;
        Count += 2;
    }
        
    linel->Draw(Line, Count, color);
}

void Overlay::Line(vec2 src, vec2 dst, int thickness, D3DCOLOR color)
{
    if (!linel)
        D3DXCreateLine(g_pd3dDevice, &linel);

    D3DXVECTOR2 line[2];
    line[0] = D3DXVECTOR2(src.x, src.y);
    line[1] = D3DXVECTOR2(dst.x, dst.y);

    linel->SetWidth(thickness);
    linel->Draw(line, 2, color);
}

void Overlay::Box2D(vec2 center, vec2 offset, int thickness, D3DCOLOR color)
{
    vec2 top = center + offset;
    vec2 bot = center - offset;

    int height = abs((int)(top.y - bot.y));
   
    vec2 tl, tr;
    tl.x = top.x - height / 4;
    tr.x = top.x + height / 4;
    tl.y = tr.y = top.y;

    vec2 bl, br;
    bl.x = bot.x - height / 4;
    br.x = bot.x + height / 4;
    bl.y = br.y = bot.y;

    Line(tl, tr, thickness, color);
    Line(bl, br, thickness, color);
    Line(tl, bl, thickness, color);
    Line(tr, br, thickness, color);
}

void Overlay::Text(const char* text, vec2 position, D3DCOLOR color)
{
    if (!fontf)
        D3DXCreateFont(g_pd3dDevice, 16, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &fontf);

    RECT rect;

    SetRect(&rect, position.x + 1, position.y + 1, position.x + 1, position.y + 1);
    fontf->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 1, 1, 1));

    SetRect(&rect, position.x, position.y, position.x, position.y);
    fontf->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
}