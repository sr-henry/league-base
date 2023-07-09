#include "league-base/base.h"

// Feature, need to change the structure...
#include "features/aimlock.h"
#include "features/orbwalker.h"


/*
TODOS

[x] Add the overlay with the league-base

[x] Restructure the Drawing class

[ ] Try to restruct the feature creation

[ ] Create a settings structure

[ ] Create a World To Screen function

*/

int main()
{
	Overlay::CreateOverlayWindow(L"League of Legends (TM) Client");
	Overlay::CreateDeviceD3D();
	Overlay::CreateImgui();

	std::future<void> esp, aim, orb;

	while (!GetAsyncKeyState(VK_HOME))
	{
		// Hack implementation
		/*Overlay::BeginRender();

		if (GetAsyncKeyState(VK_INSERT) & 1)
			Menu::open = !Menu::open;

		Overlay::RenderMenu();

		if (LeagueBase::hack.IsGameRunning())
		{
			LeagueBase::hack.Update();

			aim = std::async(std::launch::async, Aimlock);
			orb = std::async(std::launch::async, Orbwalker);
		}

		Overlay::EndRender();*/

		// lower the number, lower the fps, be aware!
		// however, with a lower number the aimlock will be faster
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	Overlay::DestroyImgui();
	Overlay::DestroyDeviceD3D();
	Overlay::DestroyOverlayWindow();
	
	return 0;
}