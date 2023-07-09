#pragma once

#include "../league-base/base.h"

using namespace LeagueBase;

static float dist(vec2 a, vec2 b)
{
	vec2 dist = a - b;
	return sqrt(dist.x * dist.x + dist.y * dist.y);
}

void Orbwalker()
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