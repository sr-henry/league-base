#include "hack.h"

Hack::Hack()
{
	hGameWindow = FindWindow(NULL, "League of Legends (TM) Client");
	if (!hGameWindow)
		exit(1);

	utils = Utils(hGameWindow);

	tp1 = std::chrono::system_clock::now();
	tp2 = std::chrono::system_clock::now();

	enemiesList.reserve(5);
	p_enemiesList.reserve(5);

	minionsList.reserve(10);

	std::cout << "Hack init success!\n";
}

Hack::~Hack() {
	cv::destroyAllWindows();
}

void Hack::Update()
{
	tp2 = std::chrono::system_clock::now();
	elapsedTime = tp2 - tp1;
	tp1 = tp2;
	fElapsedTime = elapsedTime.count();
	gameTime += fElapsedTime;

	f = std::async(
		std::launch::async,
		HttpRequestGet,
		"https://127.0.0.1:2999/liveclientdata/allgamedata",
		&sAllGameData
	);

	WindowCapture();
	GetLocalPlayer();
	GetLocalPlayerData();
	GetEnemies();
	GetEnemiesData();
	GetMinions();
}

void Hack::GetLocalPlayer()
{
	cv::Mat mask;
	cv::inRange(mGameImage, cv::Scalar(91, 220, 43), cv::Scalar(102, 255, 69), mask);

	cv::Mat kernel0 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
	cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel0);

	cv::Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel1);
	
	cv::Mat binaryImage;
	cv::threshold(mask, binaryImage, 0, 255, cv::THRESH_BINARY);

	// Find connected components
	cv::Mat labels, stats, centroids;
	int numComponents = cv::connectedComponentsWithStats(binaryImage, labels, stats, centroids);

	// Iterate over the components (excluding background component)
	for (int label = 1; label < numComponents; ++label)
	{
		// Check the area of the component
		int area = stats.at<int>(label, cv::CC_STAT_AREA);

		// Consider only components above a certain threshold
		if (area > 100)
		{
			b_localPlayer = {
				vec2{ 
					(float)centroids.at<double>(label, 0) + 60, 
					(float)centroids.at<double>(label, 1) + 100 
				}
			};

			localPlayer = &b_localPlayer;
			return;
		}
	}

	localPlayer = nullptr;
}

void Hack::GetLocalPlayerData()
{
	if (sAllGameData.empty())
		return;

	json j;
	
	try {
		j = json::parse(sAllGameData);
	}
	catch (json::parse_error& ex) {
		//std::cout << ex.what() << std::endl;
		//std::cout << ex.id << std::endl;
		return;
	}

	b_localPlayer.stats = j["activePlayer"]["championStats"];

	// bad approx
	float a = h / w;

	float fScreenAARangeRadius = b_localPlayer.stats.attackRange * (1 - a) + 40.f;

	b_localPlayer.vScreenAARange.x = fScreenAARangeRadius;
	b_localPlayer.vScreenAARange.y = fScreenAARangeRadius * cos(35.f * 3.1415f / 180.f);

}

void Hack::GetEnemies()
{
	if (!enemiesList.empty()) {
		p_enemiesList.swap(enemiesList);
		enemiesList.clear();
	}

	cv::Mat mask;
	cv::inRange(mGameImage, cv::Scalar(100, 245, 43), cv::Scalar(128, 255, 69), mask);

	cv::Mat kernel0 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
	cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel0);

	cv::Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel1);

	cv::Mat binaryImage;
	cv::threshold(mask, binaryImage, 0, 255, cv::THRESH_BINARY);

	// Find connected components
	cv::Mat labels, stats, centroids;
	int numComponents = cv::connectedComponentsWithStats(binaryImage, labels, stats, centroids);

	int dEnemiesCount = 0;
	// Iterate over the components (excluding background component)
	for (int label = 1; label < numComponents; ++label)
	{
		// Check the area of the component
		int area = stats.at<int>(label, cv::CC_STAT_AREA);

		if (area > 150)
		{
			enemiesList.emplace_back(
				dEnemiesCount,
				vec2{
					(float)centroids.at<double>(label, 0) + 60,
					(float)centroids.at<double>(label, 1) + 100
				}
			);

			dEnemiesCount++;
		}
	}
}

void Hack::GetEnemiesData()
{
	int m_size = (p_enemiesList.size() < enemiesList.size()) ? p_enemiesList.size() : enemiesList.size();

	for (int i = 0; i < m_size; i++)
	{
		enemiesList[i].direction = enemiesList[i].pos - p_enemiesList[i].pos;
		enemiesList[i].magnitude = sqrt(enemiesList[i].direction.x * enemiesList[i].direction.x + enemiesList[i].direction.y * enemiesList[i].direction.y);
		enemiesList[i].speed = enemiesList[i].magnitude / fElapsedTime;
	}
}

void Hack::GetMinions()
{
	if (!minionsList.empty())
		minionsList.clear();

	cv::Mat mask;
	cv::inRange(mGameImage, cv::Scalar(119, 139, 113), cv::Scalar(125, 144, 243), mask);

	cv::Mat kernel2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel2);

	// Find contours in the binary image
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	// Iterate over the contours and detect rectangles
	for (const auto& contour : contours)
	{
		// Approximate the contour to a polygon
		std::vector<cv::Point> polygon;
		cv::approxPolyDP(contour, polygon, 0.04f, true);

		// Check if the polygon has 4 sides (rectangular shape)
		if (polygon.size() == 4)
		{
			// Create a rectangle bounding box around the polygon
			cv::Rect rect = cv::boundingRect(polygon);

			minionsList.emplace_back(
				vec2{ (float)rect.x + 30, (float)rect.y + 50 },
				(float)rect.width
			);
		}
	}
}

Enemy* Hack::GetClosestEnemy(vec2 ref)
{
	float fMaxDist = 9999.0f;

	if (enemiesList.size() == 0)
		return nullptr;

	for (int i = 0; i < enemiesList.size(); i++) {

		vec2 vDistance = enemiesList[i].pos - ref;
		float fDistance = sqrt(vDistance.x * vDistance.x + vDistance.y * vDistance.y);

		if (fDistance < fMaxDist) {
			fMaxDist = fDistance;
			closestEnemy = enemiesList[i];
		}

	}

	return &closestEnemy;
}

bool Hack::IsGameRunning()
{
	if (hGameWindow == GetForegroundWindow())
		return true;
	return false;
}

void Hack::WindowCapture()
{
	HDC hdc_target, hdc;

	RECT rWindowRect;
	GetClientRect(hGameWindow, &rWindowRect);

	int nWindowX = rWindowRect.left;
	int nWindowY = rWindowRect.top;
	int nWindowH = rWindowRect.bottom;
	int nWindowW = rWindowRect.right;

	h = static_cast<float>(nWindowH); w = static_cast<float>(nWindowW);

	POINT p = { nWindowX, nWindowY };
	ClientToScreen(hGameWindow, &p);

	hdc_target = GetDC(NULL);
	hdc = CreateCompatibleDC(NULL);

	mGameImage = cv::Mat(nWindowH, nWindowW, CV_8UC4);

	HBITMAP bmp = CreateCompatibleBitmap(hdc_target, nWindowW, nWindowH);

	SelectObject(hdc, bmp);

	BitBlt(hdc, 0, 0, nWindowW, nWindowH, hdc_target, p.x, p.y, SRCCOPY);

	ReleaseDC(NULL, hdc_target);

	BITMAPINFO bi = { 0 };
	BITMAPINFOHEADER* bih = &bi.bmiHeader;
	bih->biSize = sizeof(BITMAPINFOHEADER);
	bih->biBitCount = 32;
	bih->biWidth = nWindowW;
	bih->biHeight = -nWindowH;
	bih->biPlanes = 1;

	GetDIBits(hdc, bmp, 0, nWindowH, mGameImage.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	DeleteObject(bmp);
	DeleteDC(hdc);

	cv::cvtColor(mGameImage, mGameImage, cv::COLOR_RGBA2RGB);
	cv::cvtColor(mGameImage, mGameImage, cv::COLOR_RGB2HSV);
}