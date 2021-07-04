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

struct championStats {
	float abilityHaste;
	float abilityPower;
	float armor;
	float armorPenetrationFlat;
	float armorPenetrationPercent;
	float attackDamage;
	float attackRange;
	float attackSpeed;
	float bonusArmorPenetrationPercent;
	float bonusMagicPenetrationPercent;
	float critChance;
	float critDamage;
	float currentHealth;
	float healShieldPower;
	float healthRegenRate;
	float lifeSteal;
	float magicLethality;
	float magicPenetrationFlat;
	float magicPenetrationPercent;
	float magicResist;
	float maxHealth;
	float moveSpeed;
	float omnivamp;
	float physicalLethality;
	float physicalVamp;
	float resourceMax;
	float resourceRegenRate;
	std::string resourceType;
	float resourceValue;
	float spellVamp;
	float tenacity;
};

struct EntSetting {
	vec2 offset;
	cv::Scalar l, u;
	int threshold;
};

class Ent 
{
public:
	Ent() {};

	Ent(vec2 pos) { vPos = pos; };

	Ent(int id, vec2 pos) { Id = id; vPos = pos; };

public:
	int Id = 0;
	vec2 vPos{ 0,0 };
};

class LocalEnt : public Ent
{
public:
	LocalEnt() : Ent() {};
	LocalEnt(vec2 pos) : Ent(pos) {};

public:
	championStats stats;
	vec2 vScreenAARange;

public:
	bool IsInside(Ent e, float threshold = 1) {
		float fRes = (e.vPos.x - vPos.x) * (e.vPos.x - vPos.x) / (vScreenAARange.x * vScreenAARange.x)
			+ (e.vPos.y - vPos.y) * (e.vPos.y - vPos.y) / (vScreenAARange.y * vScreenAARange.y);

		if (fRes <= threshold)
			return true;

		return false;
	}

};


class Enemy : public Ent
{
public:
	Enemy() : Ent() {};
	Enemy(int id, vec2 pos) : Ent(id, pos) {};

public:
	vec2 vDirection;
	float fMagnitude;
	float fSpeed;

public:
	vec2 Predict(float fPredictT) {

		if (fMagnitude == 0.0f || fMagnitude > 15.f)
			return vPos;

		return vPos + ((vDirection / fMagnitude) * fSpeed * fPredictT);
	}

};


class Hack
{
public:
	Hack();

public:
	LocalEnt* eLocalEnt;
	std::vector<Enemy> aEnemyEntList;
	float fGameTime;

public:
	void Update();
	bool IsGameRunning();
	Enemy* GetClosestEnemy(vec2 vRef);

public: // Utils
	vec2 MousePos();
	void MouseMove(vec2 vPos);
	void MouseMoveRelative(int x, int y);
	void MouseMoveSmooth(int smoothing, int delay, vec2 vPos);
	void MouseRightClick(vec2 vPos);
	void KeyboardPressKey(char cKey);
	
private:
	HWND hGameWindow;
	cv::Mat mGameImage;
	
	EntSetting localEntity;
	EntSetting enemyEntity;

	std::chrono::system_clock::time_point tp1, tp2;
	std::chrono::duration<float> elapsedTime;
	float fElapsedTime;
	
	Enemy eCosestEnemy;
	LocalEnt eLocalEntBase;
	std::vector<Enemy> aEnemyEntListOld;

	std::future<void> f;
	std::string httpData;

private:
	void WindowCapture();
	void GetEnemyEntities();
	void CalculateEnemyData();
	void GetLocalEntity();
	void GetLocalEntData();
};

