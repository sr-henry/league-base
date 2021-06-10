#include "includes.h"

void Hack::Init() {
	hGameWindow = FindWindow(NULL, L"League of Legends (TM) Client");
	if (!hGameWindow)
		return;

	localEntity = EntSetting{ vec2{38 + 5, 60 + 55}, cv::Scalar(0, 41, 45), cv::Scalar(12, 62, 49), 4 };
	enemyEntity = EntSetting{ vec2{40, 57}, cv::Scalar(0, 2, 50), cv::Scalar(0, 10, 90), 4 };

	tp1 = std::chrono::system_clock::now();
	tp2 = std::chrono::system_clock::now();

	eLocalEnt = nullptr;

	aEnemyEntList.reserve(5);
	aEnemyEntListOld.reserve(5);

	fGameTime = 0.0f;

	printf("Hack Init Success!\n");
}

void Hack::Update() {

	tp2 = std::chrono::system_clock::now();
	elapsedTime = tp2 - tp1;
	tp1 = tp2;
	fElapsedTime = elapsedTime.count();

	fGameTime += fElapsedTime;

	f = std::async(std::launch::async, HttpRequestGet, "https://127.0.0.1:2999/liveclientdata/allgamedata", &httpData);

	mGameImage = wndCapture();

	GetLocalEntity();
	GetLocalEntData();

	aEnemyEntListOld.swap(aEnemyEntList);
	GetEnemyEntities();
	CalculateEnemyData();
}

void Hack::GetLocalEntity() {
	
	cv::Mat mMask;
	inRange(mGameImage, localEntity.l, localEntity.u, mMask);

	cv::Mat k0 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
	morphologyEx(mMask, mMask, cv::MORPH_OPEN, k0);

	cv::Mat k1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));
	dilate(mMask, mMask, k1);

	std::vector<std::vector<cv::Point>> contours;
	findContours(mMask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	std::vector<std::vector<cv::Point>> approx(contours.size());

	cv::Rect eRelativePos;
	
	for (size_t i = 0; i < contours.size(); i++) {

		cv::approxPolyDP(contours[i], approx[i], 0.1f * cv::arcLength(contours[i], true), true);

		if (approx[i].size() == localEntity.threshold) {
			eRelativePos = cv::boundingRect(approx[i]);

			eLocalEntBase = {
				vec2{
						eRelativePos.x + eRelativePos.width + localEntity.offset.x,
						eRelativePos.y + eRelativePos.height + localEntity.offset.y
					}
			};
			
			eLocalEnt = &eLocalEntBase;
			return;
		}
	}

	eLocalEnt = nullptr;

}

void Hack::GetEnemyEntities() { // 2.5ms

	aEnemyEntList.clear();

	cv::Mat mMask;
	inRange(mGameImage, enemyEntity.l, enemyEntity.u, mMask);

	cv::Mat k0 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	morphologyEx(mMask, mMask, cv::MORPH_OPEN, k0);

	cv::Mat k1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));
	dilate(mMask, mMask, k1);

	std::vector<std::vector<cv::Point>> contours;
	findContours(mMask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	std::vector<std::vector<cv::Point>> approx(contours.size());

	cv::Rect eRelativePos;

	int dEntCount = 0;
	for (size_t i = 0; i < contours.size(); i++) {

		cv::approxPolyDP(contours[i], approx[i], 0.1f * cv::arcLength(contours[i], true), true);
		
		if (approx[i].size() == enemyEntity.threshold) {
			eRelativePos = cv::boundingRect(approx[i]);

			aEnemyEntList.emplace_back(
				dEntCount,
				vec2{
					eRelativePos.x + eRelativePos.width + enemyEntity.offset.x,
					eRelativePos.y + eRelativePos.height + enemyEntity.offset.y
				}
			);

			dEntCount++;
		}
	}

}

void Hack::CalculateEnemyData() {

	int maxEnt;

	// check aEnemyEntList size -- current
	if (aEnemyEntList.size() < aEnemyEntListOld.size())
		maxEnt = aEnemyEntList.size();
	else
		maxEnt = aEnemyEntListOld.size();

	for (int i = 0; i < maxEnt; i++) {

		aEnemyEntList[i].vDirection = aEnemyEntList[i].vPos - aEnemyEntListOld[i].vPos;

		aEnemyEntList[i].fMagnitude = sqrt(aEnemyEntList[i].vDirection.x * aEnemyEntList[i].vDirection.x + aEnemyEntList[i].vDirection.y * aEnemyEntList[i].vDirection.y);

		aEnemyEntList[i].fSpeed = aEnemyEntList[i].fMagnitude / fElapsedTime;

	}

}

void Hack::GetLocalEntData() {

	if (httpData.empty())
		return;

	json r = json::parse(httpData);
	json chanponStats = r["activePlayer"]["championStats"];

	eLocalEntBase.fAttackSpeed = chanponStats["attackSpeed"].get<float>();
	eLocalEntBase.fWorldAARange = chanponStats["attackRange"].get<float>();

	float fWorldAARangeRadius = eLocalEntBase.fWorldAARange / 2;
	eLocalEntBase.vScreenAARange.x = fWorldAARangeRadius + fWorldAARangeRadius * 1.8f / 100;
	eLocalEntBase.vScreenAARange.y = fWorldAARangeRadius - fWorldAARangeRadius * 1.8f / 10;
	
}

Ent* Hack::GetClosestEnemy(vec2 vRef) {

	float fMaxDist = 999.0f;
	vec2 vDistance;
	float fDistance;

	if (aEnemyEntList.size() == 0)
		return nullptr;

	for (int i = 0; i < aEnemyEntList.size(); i++) {

		vDistance = aEnemyEntList[i].vPos - vRef;
		fDistance = sqrt(vDistance.x * vDistance.x + vDistance.y * vDistance.y);

		if (fDistance < fMaxDist) {
			fMaxDist = fDistance;
			eCosestEnemy = aEnemyEntList[i];
		}

	}

	return &eCosestEnemy;
}

vec2 Hack::PredictEnt(Ent e, float fPredictT) {

	if (e.fMagnitude == 0.0f || e.fMagnitude > 15.f)
		return vec2{ 0,0 };

	return (e.vDirection / e.fMagnitude) * e.fSpeed * fPredictT;

}

cv::Mat Hack::wndCapture() {
	cv::Mat mGameImage;
	HDC hdc_target, hdc;

	RECT rWindowRect;
	GetClientRect(hGameWindow, &rWindowRect);

	int nWindowX = rWindowRect.left;
	int nWindowY = rWindowRect.top;
	int nWindowH = rWindowRect.bottom;
	int nWindowW = rWindowRect.right;

	// Device context bypass
	POINT p = { nWindowX, nWindowX };
	ClientToScreen(hGameWindow, &p);

	hdc_target = GetDC(NULL); // GetWindowDC(hwnd);
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

	cvtColor(mGameImage, mGameImage, cv::COLOR_RGBA2RGB);

	return mGameImage;
}

bool Hack::IsGameRunning() {
	if (hGameWindow == GetForegroundWindow())
		return true;
	return false;
}

vec2 Hack::MousePos() {
	POINT pCursorPos;
	GetCursorPos(&pCursorPos);
	ScreenToClient(hGameWindow, &pCursorPos);
	return vec2{ static_cast<float>(pCursorPos.x), static_cast<float>(pCursorPos.y) };
}

void Hack::MouseMove(vec2 vPos) {
	vec2 mouse = MousePos();
	INPUT in = { 0 };
	in.type = INPUT_MOUSE;
	in.mi.dwFlags = MOUSEEVENTF_MOVE;
	in.mi.dx = vPos.x - mouse.x;
	in.mi.dy = vPos.y - mouse.y;
	SendInput(1, &in, sizeof(in));
}

void Hack::MouseRightClick(vec2 vPos) {
	INPUT in = { 0 };

	in.type = INPUT_MOUSE;
	in.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	in.mi.dx = vPos.x;
	in.mi.dy = vPos.y;

	SendInput(1, &in, sizeof(in));

	ZeroMemory(&in, sizeof(in));

	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	in.type = INPUT_MOUSE;
	in.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	in.mi.dx = vPos.x;
	in.mi.dy = vPos.y;

	SendInput(1, &in, sizeof(in));
}

void Hack::KeyboardPressKey(char cKey) {
	INPUT input = { 0 };
	UINT mappedkey = MapVirtualKey(LOBYTE(VkKeyScan(cKey)), 0);
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;
	SendInput(1, &input, sizeof(input));
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}
