	
#include "Utils.h"
#include "PlayScene.h"
#include "ButtonP.h"
#include "Mario.h"
CButtonP::CButtonP(float x, float y)
{
	this->x = x;
	this->y = y-16;
	this->startY = y;
	this->ay = BUTTON_P_GRAVITY_BORN;
	this->state = BUTTON_P_STATE_AVALABLE;
}

void CButtonP::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (this->state == BUTTON_P_STATE_AVALABLE)	{
		left = x - BUTTON_P_AVALBLE_BBOX_WIDTH / 2;
		top = y - BUTTON_P_AVALBLE_BBOX_HEIGHT / 2;
		right = left + BUTTON_P_AVALBLE_BBOX_WIDTH;
		bottom = top + BUTTON_P_AVALBLE_BBOX_HEIGHT;
	}
	else {
		left = x - BUTTON_P_AVALBLE_BBOX_WIDTH / 2;
		top = y - BUTTON_P_ENABLE_BBOX_HEIGHT / 2;
		right = left + BUTTON_P_AVALBLE_BBOX_WIDTH;
		bottom = top + BUTTON_P_ENABLE_BBOX_HEIGHT;
	}
	
}

void CButtonP::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//vy += ay * dt;
	
	float sl, st, sr, sb, left, top, right, bottom;
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	mario->GetBoundingBox(sl, st, sr, sb);

	GetBoundingBox(left, top, right, bottom);
	//DebugOut(L"hmmm: %d\n", CCollision::IsOverlap(left, top, right, bottom, sl, st, sr, sb));
	
	DebugOut(L"sl: %f\n", left);
	DebugOut(L"st: %f\n", top);
	DebugOut(L"sr: %f\n", right);
	DebugOut(L"sb: %f\n", bottom);
	
	if (CCollision::IsOverlap(left, top, right, bottom, sl, st, sr, sb) && state == BUTTON_P_STATE_AVALABLE) {
		this->SetState(BUTTON_P_STATE_ENABLE);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	
}

void CButtonP::Render()
{
	int aniId = ID_ANI_BUTTON_P_AVALABLE;

	if (state == BUTTON_P_STATE_ENABLE)
		aniId = ID_ANI_BUTTON_P_ENALABLE;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

int CButtonP::IsBlocking(float nx, float ny, CGameObject* target)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (target == mario) {
		return 0;
	}
	return 0;
}

void CButtonP::OnCollisionWith(LPCOLLISIONEVENT e)
{	

	
		this->SetState(BUTTON_P_STATE_ENABLE);

	
}

void CButtonP::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BUTTON_P_STATE_AVALABLE:

		break;
	case BUTTON_P_STATE_ENABLE:
		y = y + 6;
		break;
	}
}
