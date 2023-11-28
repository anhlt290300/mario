#include "QBCoin.h"
#include "PlayScene.h"
#include "Score.h"

void QBCoin::Render()
{
	int aniId = -1;
	aniId = ID_ANI_QB_COIN;


	for (int i = 0; i < ListEffect.size(); i++) {
		ListEffect[i]->Render();
	}

	if (!beforeDelete) CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

QBCoin::QBCoin(float x, float y)
{
	this->x = x;
	this->y = y;

	this->ay = 0;
	this->ax = 0;

	minY = y - QB_COIN_MAX_HEIGHT;
	heightFinish = y - HEIGHT_FINISH;
}

void QBCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (beforeDelete) {
		ax = ay = 0;
	}

	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	if (y <= minY)
	{
		y = minY;
		SetState(QB_COIN_STATE_DOWN);
		isFall = true;
	}
	if (y >= heightFinish && isFall && !beforeDelete)
	{
		CScore* _obj = new CScore(GetX(), GetY(), SCORE_100);
		ListEffect.push_back(_obj);
		beforeDelete = true;
		//isDeleted = true;
	}
	if (isDeleted) {
		//TODO: Score
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

void QBCoin::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void QBCoin::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case QB_COIN_STATE_UP:
		vy = -QB_COIN_SPEED;
		break;
	case QB_COIN_STATE_DOWN:
		vy = QB_COIN_SPEED;
		break;
	}
}
