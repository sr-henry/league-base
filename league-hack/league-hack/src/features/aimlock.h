#pragma once

#include "../league-base/base.h"

using namespace LeagueBase;

void Aimlock()
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