#include "includes.h"
#include "menu.h"

Hack hack;
Drawing d3d;

const char* logo = R"(
     __                           __            __  
    / /__ ___ ____ ___ _____ ____/ /  ___ _____/ /__
   / / -_) _ `/ _ `/ // / -_)___/ _ \/ _ `/ __/  '_/
  /_/\__/\_,_/\_, /\_,_/\__/   /_//_/\_,_/\__/_/\_\ 
             /___/                                  

	[?] Game Settings:
	 -  colorblind mode (on)
	 -  window mode (to ESP)
	 -  show life bar (on)
	 -  cam mode (fixed)
	 -  movimentacao com attack (set any key conjunto 2)
	 -  bind auto attack move to left-click (off)

)";

struct Settings {
	bool esp = true;
	bool autoAttackRange = true;
	bool snaplines = true;
	bool box2D = true;
	bool statusText = true;
	bool predEsp = true;
	bool aim = true;
	bool bUseFov = false;
	int fov = 100;
	bool uniformDistribution = false;
	int smooth = 150;
	int delay = 15;
	bool orb = true;
}settings;


void ESP();
void AimLock();
void Orbwalker​();


int main() {

	std::cout << logo;

	std::future<void> aim, orb;
	
    while (!GetAsyncKeyState(VK_HOME))
	{
	
    	if (hack.IsGameRunning()) 
		{
    		hack.Update();
			
			ESP();
			aim = std::async(std::launch::async, AimLock);
			orb = std::async(std::launch::async, Orbwalker​);

    	}

    }

    return 0;
}


void ESP() {
	static bool bClear = true;

	if (!settings.esp || !hack.IsGameRunning()) {
		if (!bClear)
			d3d.clear();
		return;
	}	
		
	bClear = false;

	// Dummy vars
	static vec2 vBoxOffset{ 0, 50 };
	static D3DCOLOR d3dRed   = D3DCOLOR_ARGB(255, 255, 0, 0);
	static D3DCOLOR d3dWhite = D3DCOLOR_ARGB(255, 255, 255, 255);
	static D3DCOLOR d3dGreen = D3DCOLOR_ARGB(255, 0, 255, 0);

	d3d.sRender();

	// AA Range
	if (hack.eLocalEnt && settings.autoAttackRange) {
		d3d.drawEllipse(hack.eLocalEnt->vPos.x, hack.eLocalEnt->vPos.y, hack.eLocalEnt->vScreenAARange.x, hack.eLocalEnt->vScreenAARange.y, 50, 2, d3dWhite);
	}

	for (int i = 0; i < hack.aEnemyEntList.size(); i++) {

		if (hack.eLocalEnt && settings.statusText) {
			std::stringstream index, distance;
			std::string s;

			// Enemy Id
			index << hack.aEnemyEntList[i].Id;
			s = index.str();
			d3d.drawText((char*)s.c_str(), hack.aEnemyEntList[i].vPos.x, hack.aEnemyEntList[i].vPos.y, d3dWhite);

			// Enemy distance
			vec2 d = hack.aEnemyEntList[i].vPos - hack.eLocalEnt->vPos;
			distance << sqrt((d.x * d.x + d.y * d.y));
			s = distance.str();
			d3d.drawText((char*)s.c_str(), hack.aEnemyEntList[i].vPos.x + 22, hack.aEnemyEntList[i].vPos.y - 22, d3dWhite);
		}

		// box2d
		if (settings.box2D) {
			d3d.drawEspBox2D(hack.aEnemyEntList[i].vPos + vBoxOffset, hack.aEnemyEntList[i].vPos - vBoxOffset, 2, d3dRed);
		}
			

		// snaplines
		if (hack.eLocalEnt && settings.snaplines) {
			d3d.drawLine(hack.eLocalEnt->vPos, hack.aEnemyEntList[i].vPos + vec2{ 5, 40 }, 2, d3dRed);
		}
			
		// predict lines
		if (settings.predEsp) {
			d3d.drawLine(hack.aEnemyEntList[i].vPos, hack.aEnemyEntList[i].Predict(0.3f), 2, d3dGreen);
		}

	}

	d3d.fRender();

}

void AimLock() {

	if (!settings.aim)
		return;

	static std::default_random_engine generator;
	static std::uniform_int_distribution<int> distribution(100, 200);

	static int smooth = settings.smooth;
	static int delay = settings.delay;

	if (settings.uniformDistribution) {
		smooth = distribution(generator);
		delay = distribution(generator) / 10;
	}

	if (GetAsyncKeyState((short)'Q') || GetAsyncKeyState((short)'W') || GetAsyncKeyState((short)'R')) {
		Ent* enemy = hack.GetClosestEnemy(hack.MousePos());

		if (enemy) {

			vec2 dist = enemy->vPos - hack.MousePos();

			float distance = sqrt(dist.x * dist.x + dist.y * dist.y);

			if (!settings.bUseFov || distance <= settings.fov)
				hack.MouseMoveSmooth(smooth, delay, enemy->vPos);

		}
			
	}
}

void Orbwalker​() {

	if (!settings.orb)
		return;

	static bool bIsOrbAttackable = true;
	static bool bIsInside = false;
	static float fDelay = 0.0f;
	static long lWindUpDelay = 80;
	static const char cAttackMoveKey = 'l';

	vec2 vCursorPos = hack.MousePos();

	if (GetAsyncKeyState(VK_SPACE)) {

		if (!hack.eLocalEnt)
			return;

		Enemy* enemy = hack.GetClosestEnemy(hack.eLocalEnt->vPos);

		if (!enemy)
			return;

		float fAttackSpeed = hack.eLocalEnt->stats.attackSpeed;

		bIsInside = hack.eLocalEnt->IsInside(*enemy, 1.5f);

		if (bIsOrbAttackable && bIsInside) {

			hack.MouseMove(enemy->vPos);
			hack.KeyboardPressKey(cAttackMoveKey);
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			hack.MouseMove(vCursorPos);
			std::this_thread::sleep_for(std::chrono::milliseconds(lWindUpDelay));
			hack.MouseRightClick(vCursorPos);

			fDelay = (hack.fGameTime * 1000.0f) + (1000.0f / fAttackSpeed);

			bIsOrbAttackable = false;
		}
		else if (hack.fGameTime * 1000.0f >= fDelay) {
			bIsOrbAttackable = true;
		}
	}
}