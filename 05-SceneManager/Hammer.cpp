#include "Hammer.h"

CHammer::CHammer(float x, float y)
{
	this->ax = 0;
	this->ay = 0;
	this->startX = x;
	limitRight = x + HAMMER_WORLD_MAP_LIMIT_WALK;
	SetState(HAMMER_WORLD_MAP_STATE_WALK);
}

void CHammer::Render()
{
	int aniId = ID_ANI_HAMMER_WORLD_MAP_WALK_RIGHT;
	if (vx < 0) {
		aniId = ID_ANI_HAMMER_WORLD_MAP_WALK_LEFT;
	}
	else {
		aniId = ID_ANI_HAMMER_WORLD_MAP_WALK_RIGHT;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CHammer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;

	if (x <= startX) {
		SetState(HAMMER_WORLD_MAP_STATE_WALK);
	}
	if (x >= limitRight)
	{
		SetState(HAMMER_WORLD_MAP_STATE_WALK_LEFT);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CHammer::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CHammer::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case HAMMER_WORLD_MAP_STATE_WALK:
		vx = HAMMER_WORLD_MAP_WALK_SPEED;
		break;
	case HAMMER_WORLD_MAP_STATE_WALK_LEFT:
		vx = -HAMMER_WORLD_MAP_WALK_SPEED;
		break;
	}
}
