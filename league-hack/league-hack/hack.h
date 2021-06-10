#pragma once

struct vec2 {
	float x, y;

	vec2 operator+(vec2 v) { 
		return { x + v.x, y + v.y }; 
	}

	vec2 operator-(vec2 v) {
		return { x - v.x, y - v.y }; 
	}

	vec2 operator*(float n) { 
		return { x * n , y * n }; 
	}

	vec2 operator/(float n) { 
		return { x / n, y / n }; 
	}

};

struct EntSetting {
	vec2 offset;
	cv::Scalar l, u;
	int threshold;
};

struct Ent {
	int Id;
	vec2 vPos;
	vec2 vDirection;
	float fMagnitude;
	float fSpeed;
};


class Hack 
{
public:
	HWND hGameWindow;

	Ent* eLocalEnt;
	std::vector<Ent> aEnemyEntList;

	void Init();
	void Update();

	vec2 PredictEnt(Ent e, float fPredictT);
	Ent* GetClosestEnemy(vec2 vRef);

	float fLocalEntAttackRange();
	float fLocalEntAttackSpeed();
	float fGameTime;
	
	// Utils
	vec2 MousePos();
	void MouseMove(vec2 vPos);
	void MouseRightClick(vec2 vPos);
	void KeyboardPressKey(char cKey);

private:
	// Elapsed Time
	std::chrono::system_clock::time_point tp1, tp2;
	std::chrono::duration<float> elapsedTime;
	float fElapsedTime;
	
	cv::Mat mGameImage;
	cv::Mat wndCapture();

	// api data
	std::string sGameData;

	EntSetting localEntity;
	EntSetting enemyEntity;

	// aux
	Ent eCosestEnemy;
	Ent eLocalEntBase;
	std::vector<Ent> aEnemyEntListOld;

	// Hack functions
	void GetEnemyEntities();
	void GetLocalEntity();
	void CalculateEntData();
};

