#include "Leaf.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Score.h"
CLeaf::CLeaf(float x, float y , int score)
{
	this->x = x;
	this->y = y;
	this->score = score;

	this->ay = 0;
	this->ax = 0;

	minY = y - LEAF_MAX_HEIGHT;
	limitLeft = x;
	limitRight = x + LEAF_MAX_RIGHT;
}

void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if(beforeDelete) {
		left = top = right = bottom = 0;
	}
	else {
		left = x - LEAF_BBOX_WIDTH / 2;
		top = y - LEAF_BBOX_HEIGHT / 2;
		right = x + LEAF_BBOX_WIDTH;
		bottom = y + LEAF_BBOX_HEIGHT;
	}
}

void CLeaf::Render()
{
	int aniId = -1;
	if (vx > 0) {
		aniId = ID_ANI_LEAF_RIGHT;
	}
	else {
		aniId = ID_ANI_LEAF_LEFT;
	}
	if (!beforeDelete) CAnimations::GetInstance()->Get(aniId)->Render(x, y);

	for (int i = 0; i < ListEffect.size(); i++) {
		ListEffect[i]->Render();
	}

	//RenderBoundingBox();
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (beforeDelete) {
		ax = ay = 0;
	}

	vy += ay * dt;
	vx += ax * dt;


	if (y <= minY)
	{
		y = minY;
		isFall = true;
	}
	if (isFall) {
		vy = LEAF_GRAVITY;
		if (x <= limitLeft)
		{
			x = limitLeft;
			vx = LEAF_SPEED_X;
		}
		if (x >= limitRight)
		{
			x = limitRight;
			vx = -LEAF_SPEED_X;
		}
	}

	for (size_t i = 0; i < ListEffect.size(); i++)
	{
		ListEffect[i]->Update(dt, coObjects);
		if (ListEffect[i]->isDeleted) {
			ListEffect.erase(ListEffect.begin() + i);
			isDeleted = true;
		}
	}

	float sl, st, sr, sb, left, top, right, bottom;
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	mario->GetBoundingBox(sl, st, sr, sb);

	GetBoundingBox(left, top, right, bottom);

	if (CCollision::IsOverlap(left, top, right, bottom, sl, st, sr, sb)) {
		LPCOLLISIONEVENT e = new CCollisionEvent(0.01f, 0, -1, 0, 0, mario, this);
		OnCollisionWith(e);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (e->obj == mario) {
		//TODO: Score
		CScore* _obj = new CScore(GetX(), GetY(), SCORE_1000);
		mario->SetLevel(MARIO_LEVEL_RACOON);
		ListEffect.push_back(_obj);
		beforeDelete = true;
		mario->SetScore(SCORE_LEAF);
	}
}

void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_UP:
		vy = -LEAF_SPEED_UP;
		break;
	}
}
