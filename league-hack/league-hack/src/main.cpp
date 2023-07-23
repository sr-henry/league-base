#include "league-base/base.h"

using namespace league_base;

/*
	This is a implementation example of a simple hack using the league base
	with some basic features such as Aimlock and Orbwalker
*/

void Aimlock();
void Orbwalker();

int main()
{
	std::future<void> aim, orb;

	while (!GetAsyncKeyState(VK_HOME))
	{	
		if (::hack.IsGameRunning())
		{
			::hack.Update();
			aim = std::async(std::launch::async, Aimlock);
			orb = std::async(std::launch::async, Orbwalker);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	}
	
	return 0;
}

// Sample Features
void Aimlock()
{
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
			if (!usefov || enemy->pos >> hack.utils.MousePos() <= fov)
				hack.utils.MouseMoveSmooth(enemy->Predict(predicttime), smooth, delay);
		}
	}
}

void Orbwalker()
{
	static bool bLegit = false;
	static long long lWindUpDelay = 295;
	static const char cAttackMoveKey = 'l';
	static const char cMoveWithAttack = 'a';
	static const float fAttackWindupPercent = 0.16875f; // Jinx - Champion Stats 

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
				/*
				Another implementation
				Way more legit but is a bit slow
				*/
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
				/*
				sometimes when the Orbwalker is active the mouse
				doesn't go back to its original position and you get stuck on the enemy
				*/
				if (hack.utils.MousePos() >> enemy->pos > 50)
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