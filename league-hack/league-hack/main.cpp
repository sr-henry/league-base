#include "includes.h"

Hack hack;
Drawing d3d;

// wtf
bool bClear = true;

// AA Range
float fWorldAARange = 550.0f;
float fWorldAARangeRadius = fWorldAARange / 2;
float fScreenAARangeX = fWorldAARangeRadius + fWorldAARangeRadius * 1.8f / 100;
float fScreenAARangeY = fWorldAARangeRadius - fWorldAARangeRadius * 1.8f / 10;

bool IsInside(vec2 vAARangeCenter, float fScreenAARnageX, float fScreenAARangeY, vec2 vPoint, float threshold = 1) {

	float fRes = (vPoint.x - vAARangeCenter.x) * (vPoint.x - vAARangeCenter.x) / (fScreenAARnageX * fScreenAARnageX)
		+ (vPoint.y - vAARangeCenter.y) * (vPoint.y - vAARangeCenter.y) / (fScreenAARangeY * fScreenAARangeY);

	if (fRes <= threshold)
		return true;

	return false;

}

// Features
void ESP() {
	// Dummy vars
	vec2 vBoxOffset{ 0, 50 };

	// colors
	D3DCOLOR d3dRed = D3DCOLOR_ARGB(255, 255, 0, 0);
	D3DCOLOR d3dWhite = D3DCOLOR_ARGB(255, 255, 255, 255);
	D3DCOLOR d3dGreen = D3DCOLOR_ARGB(255, 0, 255, 0);

	d3d.sRender();

	// AA Range fake
	if (hack.eLocalEnt) {
		d3d.drawEllipse(hack.eLocalEnt->vPos.x, hack.eLocalEnt->vPos.y, fScreenAARangeX, fScreenAARangeY, 50, 2, d3dWhite);
	}

	for (int i = 0; i < hack.aEnemyEntList.size(); i++) {

		std::stringstream index, distance;
		std::string s;

		index << hack.aEnemyEntList[i].Id;
		s = index.str();

		// Ent index
		d3d.drawText((char*)s.c_str(), hack.aEnemyEntList[i].vPos.x, hack.aEnemyEntList[i].vPos.y, D3DCOLOR_ARGB(255, 255, 255, 255));

		// box2d
		d3d.drawEspBox2D(hack.aEnemyEntList[i].vPos + vBoxOffset, hack.aEnemyEntList[i].vPos - vBoxOffset, 2, d3dRed);

		// snaplines
		if (hack.eLocalEnt) {
			d3d.drawLine(hack.eLocalEnt->vPos, hack.aEnemyEntList[i].vPos, 2, d3dRed);
			vec2 d = hack.aEnemyEntList[i].vPos - hack.eLocalEnt->vPos;

			// normalizar conforme a res
			float fDistance = sqrt((d.x * d.x + d.y * d.y));
			distance << fDistance;
			s = distance.str();
			d3d.drawText((char*)s.c_str(), hack.aEnemyEntList[i].vPos.x + 22, hack.aEnemyEntList[i].vPos.y - 22, d3dWhite);
		}

		// predict lines
		vec2 vPredict = hack.PredictEnt(hack.aEnemyEntList[i], 0.3f);
		d3d.drawLine(hack.aEnemyEntList[i].vPos, hack.aEnemyEntList[i].vPos + vPredict, 2, d3dGreen);
				
	}

	d3d.fRender(); bClear = false;
	
}

void AimLock() {

	if (GetAsyncKeyState((short)'Q') || GetAsyncKeyState((short)'W') || GetAsyncKeyState((short)'E')) {

		Ent* enemy = hack.GetClosestEnemy(hack.MousePos());

		if (enemy)
			hack.MouseMove(enemy->vPos);
			
	}
}

void Orbwalker​() {

	static bool bIsOrbAttackable = true;
	static bool bIsInside = false;
	static float fDelay = 0.0f;
	static float fAttackSpeed = 0.0f;

	vec2 vCursorPos = hack.MousePos();

	if (GetAsyncKeyState(VK_SPACE)) {
		
		fAttackSpeed = hack.fLocalEntAttackSpeed();

		if (!hack.eLocalEnt)
			return;

		Ent* enemy = hack.GetClosestEnemy(hack.eLocalEnt->vPos);

		if (!enemy)
			return;

		bIsInside = IsInside(hack.eLocalEnt->vPos, fScreenAARangeX, fScreenAARangeY, enemy->vPos, 1.5f);

		if (bIsOrbAttackable && bIsInside) {

			hack.MouseMove(enemy->vPos);
			hack.KeyboardPressKey('l');
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			hack.MouseMove(vCursorPos);
			std::this_thread::sleep_for(std::chrono::milliseconds(80));
			hack.MouseRightClick(vCursorPos);

			fDelay = (hack.fGameTime * 1000.0f) + (1000.0f / fAttackSpeed);

			bIsOrbAttackable = false;
		}
		else if (hack.fGameTime * 1000.0f >= fDelay) {
			bIsOrbAttackable = true;
		}
	}
}


int main() {
    
	/*
		[?] Game Settings:
		 -  colorblind mode (on)
		 -  window mode (to overlay)
		 -  nivel de cor (75)
		 -  exibir barra de vida (on)
		 -  cam mode (fixed)
		 -  movimentação com attack (set any key conjunto 2)
		 -  bind auto attack move to left-click (off)
	*/

    hack.Init();

    // Hack loop
    while (!GetAsyncKeyState(VK_HOME)) {

    	if (hack.hGameWindow == GetForegroundWindow()) {
    		hack.Update();
			
    		ESP();
    		AimLock();
    		Orbwalker​();

    	}
    	else if (!bClear) {
    		d3d.clear(); bClear = true;
    	}

    }

    d3d.clear();

    return 0;
}