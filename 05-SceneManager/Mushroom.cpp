#include "Mushroom.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Score.h"

CMushroom::CMushroom(float x, float y, int score) : CGameObject(x, y)
{
	this->ay = 0;
	this->ax = 0;
	this->x = x;
	this->y = y;
	this->score = score;

	minY = y - MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (beforeDelete) {
		left = top = right = bottom = 0;
	}
	else {
		left = x - MUSHROOM_BBOX_WIDTH / 2;
		top = y - MUSHROOM_BBOX_HEIGHT / 2;
		right = left + MUSHROOM_BBOX_WIDTH;
		bottom = top + MUSHROOM_BBOX_HEIGHT;
	}
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	if (y <= minY)
	{
		SetState(MUSHROOM_STATE_RUN);
	}

	if (beforeDelete) {
		ax = ay = 0;
	}

	float sl, st, sr, sb, left, top, right, bottom;
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	mario->GetBoundingBox(sl, st, sr, sb);

	GetBoundingBox(left, top, right, bottom);

	if (CCollision::IsOverlap(left, top, right, bottom, sl, st, sr, sb)) {
		LPCOLLISIONEVENT e = new CCollisionEvent(0.01f, 0, -1, 0, 0, mario, this);
		OnCollisionWith(e);
	}

	for (size_t i = 0; i < ListEffect.size(); i++)
	{
		ListEffect[i]->Update(dt, coObjects);
		if (ListEffect[i]->isDeleted) {			
			ListEffect.erase(ListEffect.begin() + i);
			isDeleted = true;
		}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::Render()
{
	int aniId = -1;

	aniId = ID_ANI_MUSHROOM;

	if(!beforeDelete) CAnimations::GetInstance()->Get(aniId)->Render(x, y);

	for (int i = 0; i < ListEffect.size(); i++)	{
		ListEffect[i]->Render();
	}
	//RenderBoundingBox();
}

void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSHROOM_STATE_UP:
		vy = -MUSHROOM_SPEED_UP;
		break;
	case MUSHROOM_STATE_RUN:
		vx = MUSHROOM_RUN_SPEED;
		ay = MUSHROOM_GRAVITY;
		break;
	}
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (e->obj == mario) {
		//TODO: Score
	
		if (mario->GetLevel() == MARIO_LEVEL_RACOON) {
			CScore* _obj = new CScore(GetX(), GetY(), SCORE_2000);
			this->SetScore(2000);
			ListEffect.push_back(_obj);
			beforeDelete = true;
			mario->SetLive(1);			
		}
		else {
			mario->SetLevel(MARIO_LEVEL_BIG);
			CScore* _obj = new CScore(GetX(), GetY(), SCORE_1000);
			ListEffect.push_back(_obj);
			beforeDelete = true;
		}
		mario->SetScore(SCORE_MUSHROOM);
			
	}
}
