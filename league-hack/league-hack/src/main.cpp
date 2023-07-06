#include "league-base/hack.h"
#include "league-base/overlay/window.h"

Hack hack;

/*
TODOS

[x] Add the overlay with the league-base

[x] Restructure the Drawing class

[ ] Try to restruct the feature creation

[ ] Create a settings structure

[ ] Create a World To Screen function

*/

void ESP();
void AimLock();
void Orbwalker​();

int main()
{
	Overlay::CreateOverlayWindow(L"League of Legends (TM) Client");
	Overlay::CreateDeviceD3D();
	Overlay::CreateImgui();

	std::future<void> esp, aim, orb;

	while (!GetAsyncKeyState(VK_HOME))
	{
		Overlay::BeginRender();

		if (hack.IsGameRunning())
		{
			hack.Update();

			//esp = std::async(std::launch::async, ESP);
			
			for (auto e : hack.enemiesList)
			{
				Overlay::Box2D(e.pos, { 5, 40 }, 2, RED);
			}

			aim = std::async(std::launch::async, AimLock);
			orb = std::async(std::launch::async, Orbwalker​);
		}

		Overlay::EndRender();
		
		// lower the number, lower the fps, be aware!
		// however, with a lower number the aimlock will be faster
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	Overlay::DestroyImgui();
	Overlay::DestroyDeviceD3D();
	Overlay::DestroyOverlayWindow();
	
	return 0;
}

void ESP()
{
	// settings
	static bool localPlayer = false;
	static bool snaplines = false;
	static bool box2d = true;
	static bool predictlines = false;

	
	


}

void AimLock()
{
	// settings
	static bool usefov = true;
	static long fov = 550;
	static long smooth = 10;
	static long delay = 2;
	static float predicttime = 0.0f;
	static bool Q = true;
	static bool W = true;
	static bool E = true;
	static bool R = true;

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

			if (!usefov || distance <= fov)
				hack.utils.MouseMoveSmooth(enemy->Predict(predicttime), smooth, delay);
		}
	}
}

float dist(vec2 a, vec2 b)
{
	vec2 dist = a - b;
	return sqrt(dist.x * dist.x + dist.y * dist.y);
}

void Orbwalker​()
{
	// settings
	static bool bLegit = false;
	static long long lWindUpDelay = 295;
	static const char cAttackMoveKey = 'l';
	static const char cMoveWithAttack = 'a';
	static const float fAttackWindupPercent = 0.16875f; // Champion Stats

	vec2 vCursorPos;
	float fAttackSpeed;
	float fAttackCooldown;
	float fAttackWindupTime;
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
			vCursorPos = hack.utils.MousePos();

			fAttackSpeed = hack.localPlayer->stats.attackSpeed;
			fAttackCooldown = 1000.0f / fAttackSpeed;
			fAttackWindupTime = fAttackCooldown * fAttackWindupPercent;

			if (bLegit)
			{
				 /*Crauzer
				 My friend's implementation
				 Way more legit but is a bit slow*/
				hack.utils.KeyboardPressKey(cMoveWithAttack);
				hack.utils.MouseLeftClick(hack.utils.MousePos());
				std::this_thread::sleep_for(std::chrono::milliseconds(lWindUpDelay));
				hack.utils.MouseRightClick(hack.utils.MousePos());
			}
			else
			{
				hack.utils.MouseMove(enemy->pos);
				hack.utils.KeyboardPressKey(cAttackMoveKey);

				hack.utils.MouseMove(vCursorPos);
				std::this_thread::sleep_for(std::chrono::milliseconds((long long)(fAttackWindupTime)));

				 /*bug fix ????
				 sometime when the Orbwalker​ is active the mouse does not
				 comeback for the original position and you get stuck in the enemy*/
				if (dist(hack.utils.MousePos(), enemy->pos) > 50)
					hack.utils.MouseRightClick(hack.utils.MousePos());
				else
					hack.utils.MouseMove(vCursorPos);
			}

			fDelay = (hack.gameTime * 1000.0f) + fAttackCooldown;

			bIsOrbAttackable = false;
		}
		else if (hack.gameTime * 1000.0f >= fDelay)
		{
			hack.utils.MouseRightClick(hack.utils.MousePos());
			bIsOrbAttackable = true;
		}
		else
		{
			hack.utils.MouseMove(hack.utils.MousePos());
		}
	}
}
