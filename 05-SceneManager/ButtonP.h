#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BUTTON_P_AVALABLE 2210
#define ID_ANI_BUTTON_P_ENALABLE 2211

#define	BUTTON_WIDTH 10
#define BUTTON_P_AVALBLE_BBOX_WIDTH 16
#define BUTTON_P_AVALBLE_BBOX_HEIGHT 16
#define BUTTON_P_ENABLE_BBOX_HEIGHT 5
#define BUTTON_P_GRAVITY_BORN	-0.00008f
#define BUTTON_P_STATE_AVALABLE 0
#define BUTTON_P_STATE_ENABLE 1

class CButtonP : public CGameObject {
public:
	float ay;
	float startY;
	CButtonP(float x, float y);
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsBlocking(float nx, float ny, CGameObject* target);
	virtual int IsCollidable() { return 1; }
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void SetState(int state);
};