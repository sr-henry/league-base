#include "league-base/hack.h"
#include "league-base/overlay/drawing.h"

Hack hack;
Drawing draw;

void ESP();
void AimLock();

int main()
{
	std::cout << "Hello World\n";

	std::future<void> esp, aim;

	while (!GetAsyncKeyState(VK_HOME))
	{
		if (hack.IsGameRunning())
		{
			hack.Update();
			
			esp = std::async(std::launch::async, ESP);
			aim = std::async(std::launch::async, AimLock);
		}
	}
	
	return 0;
}

void ESP()
{
	static bool bIsClear = true;

	if (!hack.IsGameRunning()) {
		if (!bIsClear)
			draw.Clear();
		return;
	}

	bIsClear = false;

	static D3DCOLOR red   = D3DCOLOR_ARGB(255, 255, 0, 0);
	static D3DCOLOR white = D3DCOLOR_ARGB(255, 255, 255, 255);
	static D3DCOLOR green = D3DCOLOR_ARGB(255, 0, 255, 0);

	draw.StartRender();

	for (int i = 0; i < hack.enemiesList.size(); i++)
	{
		// snaplines
		if (hack.localPlayer)
			draw.Line(hack.localPlayer->pos, hack.enemiesList[i].pos, 2, red);

		// 2D Box
		draw.Box2D(hack.enemiesList[i].pos, {0, 50}, 2, red);

		// predict lines
		draw.Line(hack.enemiesList[i].pos, hack.enemiesList[i].Predict(0.3f), 2, green);
	}

	draw.EndRender();
}

void AimLock()
{
	if (GetAsyncKeyState((short)'Q') || GetAsyncKeyState((short)'W') || GetAsyncKeyState((short)'R'))
	{
		if (hack.enemiesList.empty())
			return;

		hack.utils.MouseMoveSmooth(hack.enemiesList[0].pos, 200, 20);
	}
}