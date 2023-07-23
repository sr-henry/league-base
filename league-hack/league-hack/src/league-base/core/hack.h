#pragma once
#include <Windows.h>
#include <vector>
#include <chrono>
#include <future>
#include <opencv2/opencv.hpp>
#include "../game/objects.h"
#include "../utils/utils.h"
#include "../client/requests.h"

class Hack
{
public:
	Hack();
	~Hack();

public:
	LocalPlayer* localPlayer;
	std::vector<Enemy> enemiesList;
	std::vector<Minion> minionsList;
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
	void GetMinions();
};

