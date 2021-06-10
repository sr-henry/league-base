#pragma once

#include <iostream>
#include <Windows.h>
#include <opencv2/opencv.hpp>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <stdio.h>
#include <math.h>
#include <string>
#include <future>
#include <thread>
#include <chrono>

#include "hack.h"
#include "drawing.h"
#include "api.h"

#define CURL_STATICLIB
#include "curl/curl.h"

#include "json.hpp"
using json = nlohmann::json;

#define PI 3.14159265358979323846

struct Timer {
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

	Timer() {
		start = std::chrono::high_resolution_clock::now();
	}

	~Timer() {
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;
		float ms = duration.count() * 1000.0f;

		std::cout << "Timer took: " << ms << "ms" << std::endl;
	}

};

