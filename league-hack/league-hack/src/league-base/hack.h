#pragma once

#include <Windows.h>
#include <vector>
#include <chrono>
#include <future>
#include <opencv2/opencv.hpp>

#include "client/activeplayer.h"
#include "vectors.h"
#include "utils/utils.h"

void HttpRequestGet(const char* url, std::string* httpData);

class Entity
{
public:
	Entity() {};
	Entity(vec2 pos) { this->pos = pos; };
	Entity(int id, vec2 pos) { this->id = id; this->pos = pos; };

public:
	int id = 0;
	vec2 pos{ 0,0 };
};


class LocalPlayer : public Entity
{
public:
	LocalPlayer() : Entity() {};
	LocalPlayer(vec2 pos) : Entity(pos) {};

public:
	championStats stats;
	vec2 vScreenAARange{ 0,0 };

public:
	bool IsInside(Entity e, float threshold = 1.0f) {
		float r = (e.pos.x - pos.x) * (e.pos.x - pos.x) / (vScreenAARange.x * vScreenAARange.x)
				+ (e.pos.y - pos.y) * (e.pos.y - pos.y) / (vScreenAARange.y * vScreenAARange.y);
		if (r <= threshold)
			return true;
		return false;
	}
};


class Enemy : public Entity
{
public:
	Enemy() : Entity() {};
	Enemy(int id, vec2 pos) : Entity(id, pos) {};

public:
	vec2 direction{ 0,0 };
	float magnitude = 0.0f;
	float speed = 0.0f;

public:
	vec2 Predict(float fPredictTime) {
		if (magnitude == 0.0f || magnitude > 15.0f)
			return pos;
		return pos + ((direction / magnitude) * speed * fPredictTime);
	}
};


class Hack
{
public:
	Hack();
	~Hack();

public:
	LocalPlayer* localPlayer;
	std::vector<Enemy> enemiesList;
	float gameTime;
	Utils utils;

public:
	void Update();
	bool IsGameRunning();
	Enemy* GetClosestEnemy(vec2 ref);

private:
	HWND hGameWindow;
	cv::Mat mGameImage;

	std::chrono::system_clock::time_point tp1, tp2;
	std::chrono::duration<float> elapsedTime;
	float fElapsedTime;

	LocalPlayer b_localPlayer;
	std::vector<Enemy> p_enemiesList;

	Enemy closestEnemy;

	std::string sAllGameData;
	std::future<void> f;

	float fAspectRatio, w, h;

private:
	void WindowCapture();
	void GetLocalPlayer();
	void GetLocalPlayerData();
	void GetEnemies();
	void GetEnemiesData();
};

