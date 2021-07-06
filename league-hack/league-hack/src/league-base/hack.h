#pragma once
#include "activeplayer.h"
#include "vectors.h"
#include <future>
#include <vector>
#include <Windows.h>
#include <chrono>
#include <opencv2/opencv.hpp>

void HttpRequestGet(const char* url, std::string* httpData);

struct EntitySettings
{
	vec2 offset;
	cv::Scalar l, u;
	cv::Mat k0, k1;
	int threshold;
};


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

public:
	LocalPlayer* localPlayer;
	std::vector<Enemy> enemyList;
	float gameTime;

public:
	void Update();
	bool IsGameRunning();

private:
	HWND hGameWindow;
	cv::Mat mGameImage;

	EntitySettings esLocalPlayer;
	EntitySettings esEnemy;

	std::chrono::system_clock::time_point tp1, tp2;
	std::chrono::duration<float> elapsedTime;
	float fElapsedTime;

	LocalPlayer b_localPlayer;
	std::vector<Enemy> p_enemyList;

	std::string sAllGameData;
	std::future<void> f;

private:
	void WindowCapture();
	void GetLocalPlayer();
	void GetLocalPlayerData();
	void GetEnemies();
	void GetEnemiesData();
};

