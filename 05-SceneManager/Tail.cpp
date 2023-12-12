#include "Tail.h"
#include "QuestionBrick.h"
#include "Mario.h"
#include "PlayScene.h"
#include "GoldBrick.h"

void CTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - 8;
	right = x + 20;
	top = y;
	bottom = y + 5;
}

void CTail::Render()
{
	//RenderBoundingBox();
}

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->x = this->mario->GetX();
	this->y = this->mario->GetY();
	DebugOut(L"tracking %d\n", this->tracking);
	//// Check tail true of flase


	for (int i = 0; i < coObjects->size(); i++)
	{
		if (CCollision::GetInstance()->CheckAABB(this, coObjects->at(i))) {
			if (coObjects->at(i)->GetModel() == ENEMY) {
				OnCollisionWithEnemy(coObjects->at(i));
			}
			else if (dynamic_cast<CQuestionBrick*>(coObjects->at(i)))
				OnCollisionWithQuestionBrick(coObjects->at(i));
			else if (coObjects->at(i)->GetModel() == GOLDBRICK)//dynamic_cast<GoldBrick*>(coObjects->at(i)))
				OnCollisionWithGoldBrick(coObjects->at(i));
		}

	}
}

void CTail::OnCollisionWithQuestionBrick(LPGAMEOBJECT& e)
{
	CQuestionBrick* questionBrick = dynamic_cast<CQuestionBrick*>(e);
	if (!questionBrick->isEmpty && this->tracking) {
		questionBrick->Bounce();
		this->IsTracking();
	}
}

void CTail::OnCollisionWithGoldBrick(LPGAMEOBJECT& e)
{
	CGoldBrick* goldbrick = dynamic_cast<CGoldBrick*>(e);
	if (!goldbrick->isEmpty && this->tracking) {
		goldbrick->SetBreak(true);
		this->IsTracking();
	}
}

void CTail::OnCollisionWithEnemy(LPGAMEOBJECT& e)
{
	if (this->tracking) {
		e->SetState(ENEMY_STATE_IS_TAIL_ATTACKED);
		this->IsTracking();
	}
}

CTail::CTail(CMario* mario)
{
	this->mario = mario;
}
