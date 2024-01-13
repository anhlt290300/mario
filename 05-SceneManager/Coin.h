#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Timer.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

class CCoin : public CGameObject {
public:
	ULONGLONG range = -1;
	int model;
	CCoin(float x, float y, int model) : CGameObject(x, y) {
		this->model = model;
		if (model == 1)
			range = GetTickCount64();
	}
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};