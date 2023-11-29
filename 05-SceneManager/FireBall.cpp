#include "FireBall.h"
#include "ColorBox.h"
void CFireBall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != FIRE_BALL_DISAPPEAR) {
		left = x - FIRE_BALL_BBOX_WIDTH / 2;
		top = y - FIRE_BALL_BBOX_HEIGHT / 2;
		right = left + FIRE_BALL_BBOX_WIDTH;
		bottom = top + FIRE_BALL_BBOX_HEIGHT;
	}
	else left = top = right = bottom = 0;
}

void CFireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (!checkObjectInCamera(this)) {
		isDeleted = true;
	}

	if (isDisappear && GetTickCount64() - start_disappear > FIRE_BALL_DISAPPEAR_EFFECT_TIME_OUT) { // effect disappear
		start_disappear = -1;
		isDisappear = false;
		isDeleted = true;		
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CFireBall::Render()
{
	int aniId = FIREBALL_ANI_MOVE;
	if (state == FIRE_BALL_DISAPPEAR) aniId = FIREBALL_ANI_BOOM;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CFireBall::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CFireBall::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && !dynamic_cast<CColorBox*>(e->obj))
	{
		
		SetState(FIRE_BALL_DISAPPEAR);

	}
	else if (e->nx != 0 )
	{
		SetState(FIRE_BALL_DISAPPEAR);
	}

	if (e->obj->GetType() == ENEMY) {
		e->obj->SetState(ENEMY_STATE_IS_FIRE_ATTACKED);
		SetState(FIRE_BALL_DISAPPEAR);
	}
}

int CFireBall::IsCollidable()
{
	/*if (state == FIRE_FROM_MARIO) {
		return 1;
	}
	else
		return 0;*/
	return 1;
}

CFireBall::CFireBall(float x, float y) :CGameObject(x, y)
{
	this->x = x;
	this->y = y;
	this->ax = 0;
	this->ay = FIRE_GRAVITY;
}

void CFireBall::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	mario_dir = mario->GetCurrentDirection();//Get Mario's current direction

	switch (state)
	{
	case FIRE_FROM_MARIO:
		vx = mario_dir * FIRE_BALL_SPEED;
		break;
	case FIRE_BALL_STATE_FPP_SHOOT_NEAR:
		vx = nx * FIRE_BALL_FPP_SHOOT_SPEED_X_NEAR;
		vy = ny * FIRE_BALL_FPP_SHOOT_SPEED_Y;
		isEnemyShoot = true;
		ay = 0;
		break;
	case FIRE_BALL_STATE_FPP_SHOOT_FAR:
		vx = nx * FIRE_BALL_FPP_SHOOT_SPEED_X_FAR;
		vy = ny * FIRE_BALL_FPP_SHOOT_SPEED_Y;
		isEnemyShoot = true;
		ay = 0;
		break;
	case FIRE_BALL_DISAPPEAR:
		vx = 0;		
		vy = 0;
		ay = 0;
		start_disappear = GetTickCount64();
		isDisappear = true;
		break;
	}
}

