#pragma once
#include "GameObject.h"
#include "Pipe.h"

#define PIRANHA_PLANT_BBOX_WIDTH 16
#define PIRANHA_PLANT_BBOX_HEIGHT 25

#define PIRANHA_PLANT_DOWN_TIME_OUT 2000
#define PIRANHA_PLANT_UP_TIME_OUT 2000

#define ID_ANI_PIRANHA_PLANT 500001
#define PIRANHA_STATE_UP 100
#define PIRANHA_STATE_DOWN 200
#define PIRANHA_SPEED 0.02f;
#define DISTANCE_MARIO_SAFE_ZONE 30

class CPiranha:
	public CGameObject
{
	float ax;
	float ay;

	float minY;
	float startY;
public:
	CPiranha(float x, float y);
	virtual void SetState(int state);

	ULONGLONG down_start = -1;
	ULONGLONG up_start = -1;
	BOOLEAN isMarioSafeZone = false;
	BOOLEAN startUp = false, startDown = false;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	//virtual int IsCollidable() { return 1; };
	virtual int IsBlocking(float nx, float ny, CGameObject* target) {
		if (dynamic_cast<CPipe*>(target))	return false;
		else return true;
	}
	virtual void OnNoCollision(DWORD dt);
};
