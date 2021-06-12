#pragma once

struct vec2 {
	float x, y;

	vec2 operator+(const vec2& v) const { 
		return { x + v.x, y + v.y }; 
	}

	vec2 operator-(const vec2& v) const {
		return { x - v.x, y - v.y }; 
	}

	vec2 operator*(const float& n) const { 
		return { x * n , y * n }; 
	}

	vec2 operator/(const float& n) const { 
		return { x / n, y / n }; 
	}

};

struct EntSetting {
	vec2 offset;
	cv::Scalar l, u;
	int threshold;
};

class Ent 
{
public:
	int Id;
	vec2 vPos;
	vec2 vDirection;
	float fMagnitude;
	float fSpeed;

public:
	Ent() {
		Id = 0;
		vPos = vec2{ 0,0 };
		vDirection = vec2{ 0,0 };
		fMagnitude = 0.0f;
		fSpeed = 0.0f;
	};
	Ent(int id, vec2 pos) {
		Id = id;
		vPos = pos;
		vDirection = vec2{ 0,0 };
		fMagnitude = 0.0f;
		fSpeed = 0.0f;
	}

};

class LocalEnt : public Ent
{
public:
	float fAttackSpeed;
	float fWorldAARange;
	vec2 vScreenAARange;

	bool IsInside(Ent e, float threshold = 1) {
		float fRes = (e.vPos.x - vPos.x) * (e.vPos.x - vPos.x) / (vScreenAARange.x * vScreenAARange.x)
			+ (e.vPos.y - vPos.y) * (e.vPos.y - vPos.y) / (vScreenAARange.y * vScreenAARange.y);

		if (fRes <= threshold)
			return true;

		return false;
	}

public:
	LocalEnt() : Ent() {};
	LocalEnt(vec2 pos) : Ent(0, pos) {};

};

class Hack 
{
public:
	LocalEnt* eLocalEnt;
	std::vector<Ent> aEnemyEntList;
	float fGameTime;

	void Init();
	void Update();
	
public: // Utils
	vec2 PredictEnt(Ent e, float fPredictT);
	Ent* GetClosestEnemy(vec2 vRef);
	bool IsGameRunning();
	vec2 MousePos();
	void MouseMove(vec2 vPos);
	void MouseMoveRelative(int x, int y);
	void MouseMoveSmooth(int smoothing, int delay, vec2 vPos);
	void MouseRightClick(vec2 vPos);
	void KeyboardPressKey(char cKey);
	

private:
	HWND hGameWindow;
	cv::Mat mGameImage;
	cv::Mat wndCapture();

	// Elapsed Time
	std::chrono::system_clock::time_point tp1, tp2;
	std::chrono::duration<float> elapsedTime;
	float fElapsedTime;
	
	EntSetting localEntity;
	EntSetting enemyEntity;

	Ent eCosestEnemy;
	LocalEnt eLocalEntBase;
	std::vector<Ent> aEnemyEntListOld;

	std::future<void> f;
	std::string httpData;

private:
	void GetEnemyEntities();
	void CalculateEnemyData();
	void GetLocalEntity();
	void GetLocalEntData();
};

