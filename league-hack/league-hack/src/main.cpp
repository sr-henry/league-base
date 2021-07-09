#include "league-base/hack.h"
#include "league-base/overlay/drawing.h"
#include <random>

Hack hack;
Drawing draw;

void ESP();
void AimLock();
void Orbwalker​();

int main()
{
	std::future<void> esp, aim, orb;

	while (!GetAsyncKeyState(VK_HOME))
	{
		if (hack.IsGameRunning())
		{
			hack.Update();
			
			esp = std::async(std::launch::async, ESP);
			aim = std::async(std::launch::async, AimLock);
			orb = std::async(std::launch::async, Orbwalker​);
		}
	}
	
	return 0;
}

void ESP()
{
	// settings
	static bool snaplines = true;
	static bool box2d = true;
	static bool predictlines = true;

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

	if (hack.localPlayer)
		draw.Ellipse(hack.localPlayer->pos, hack.localPlayer->vScreenAARange.x, hack.localPlayer->vScreenAARange.y, 50, 2, white);

	for (int i = 0; i < hack.enemiesList.size(); i++)
	{
		// snaplines
		if (hack.localPlayer && snaplines)
			draw.Line(hack.localPlayer->pos, hack.enemiesList[i].pos, 2, red);

		// 2D Box
		if (box2d)
			draw.Box2D(hack.enemiesList[i].pos, {0, 50}, 2, red);

		// predict lines
		if (predictlines)
			draw.Line(hack.enemiesList[i].pos, hack.enemiesList[i].Predict(0.3f), 2, green);
	}

	draw.EndRender();
}

void AimLock()
{
	// settings
	static bool usefov = true;
	static long fov = 150;
	static bool human = true;
	static long smooth = 100;
	static long delay = 20;
	static float predicttime = 0.0f;
	static bool Q = true;
	static bool W = true;
	static bool E = true;
	static bool R = true;

	static std::default_random_engine generator;
	static std::uniform_int_distribution<int> distribution(100, 250);

	if ((GetAsyncKeyState((short)'Q') && Q) || 
		(GetAsyncKeyState((short)'W') && W) || 
		(GetAsyncKeyState((short)'E') && E) ||
		(GetAsyncKeyState((short)'R') && R))
	{
		Enemy* enemy = hack.GetClosestEnemy(hack.utils.MousePos());

		if (enemy)
		{
			vec2 dist = enemy->pos - hack.utils.MousePos();

			float distance = sqrt(dist.x * dist.x + dist.y * dist.y);

			if (human)
			{
				smooth = distribution(generator);
				delay = distribution(generator) / 10;
			}

			if (!usefov || distance <= fov)
				hack.utils.MouseMoveSmooth(enemy->Predict(predicttime), smooth, delay);
		}
	}
}

void Orbwalker​()
{
	// settings
	static long lWindUpDelay = 80;
	static const char cAttackMoveKey = 'l';

	static bool bIsOrbAttackable = true;
	static bool bIsInside = false;
	static float fDelay = 0.0f;
	static Enemy* enemy = nullptr;

	if (GetAsyncKeyState(VK_SPACE))
	{
		if (!hack.localPlayer)
			return;

		enemy = hack.GetClosestEnemy(hack.localPlayer->pos);

		if (!enemy)
			return;

		bIsInside = hack.localPlayer->IsInside(*enemy, 1.5f);

		if (bIsOrbAttackable && bIsInside)
		{
			vec2 vCursorPos = hack.utils.MousePos();
			float fAttackSpeed = hack.localPlayer->stats.attackSpeed;

			hack.utils.MouseMove(enemy->pos);
			hack.utils.KeyboardPressKey(cAttackMoveKey);
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			hack.utils.MouseMove(vCursorPos);
			std::this_thread::sleep_for(std::chrono::milliseconds(lWindUpDelay));
			hack.utils.MouseRightClick(vCursorPos);

			fDelay = (hack.gameTime * 1000.0f) + (1000.0f / fAttackSpeed);

			bIsOrbAttackable = false;
		}
		else if (hack.gameTime * 1000.0f >= fDelay)
		{
			bIsOrbAttackable = true;
		}
	}
}