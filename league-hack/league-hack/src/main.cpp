#include "league-base/hack.h"
#include "league-base/overlay/drawing.h"

Hack hack;
Drawing draw;

void ESP();

int main()
{
	std::cout << "Hello World\n";

	while (!GetAsyncKeyState(VK_HOME))
	{
		if (hack.IsGameRunning())
		{
			hack.Update();
			ESP();
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

	for (int i = 0; i < hack.enemyList.size(); i++)
	{
		// snaplines
		if (hack.localPlayer)
			draw.Line(hack.localPlayer->pos, hack.enemyList[i].pos, 2, red);

		// 2D Box
		draw.Box2D(hack.enemyList[i].pos, {0, 50}, 2, red);
	}

	draw.EndRender();
}