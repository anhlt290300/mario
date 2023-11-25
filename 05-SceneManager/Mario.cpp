#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"

#include "BaseMarioState.h"
#include "MarioStateSmall.h"
#include "MarioStateBig.h"

#include "Collision.h"
#include "PlayScene.h"

CMario::CMario(float x, float y) : CGameObject(x, y) {
	this->stateHandler = new MarioStateSmall(this);
	isSitting = false;
	maxVx = 0.0f;
	ax = 0.0f;
	ay = MARIO_GRAVITY;
	die_start = -1;
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	untouchable_start = -1;
	isOnPlatform = false;
	coin = 0;
	isShootingFire = false;
	this->x = x;
	this->y = y;
	score = 0;
	live = 0;
	hand = NULL;
	isDisable = false;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float l, t, r, b;
	this->GetBoundingBox(l, t, r, b);
	ay = MARIO_GRAVITY;


	LPGAME game = CGame::GetInstance();
	CPlayScene* playScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();

	if (game->IsKeyDown(DIK_1)) {
		SetLevel(MARIO_LEVEL_SMALL);
	}
	if (game->IsKeyDown(DIK_2)) {
		SetLevel(MARIO_LEVEL_BIG);
	}

	if (game->IsKeyDown(DIK_5)) {
		SetPosition(2012, 250);
	}
	if (game->IsKeyDown(DIK_6)) {
		SetPosition(1440, 50);

		playScene->isFlyCam = true;
	}
	if (game->IsKeyDown(DIK_7)) {
		SetPosition(2260, 50);
		playScene->isFlyCam = true;
	}

	if (!isDisable) {
		if (this->x < 0) {
			this->x = 0;
		}
	}

	if (isReturnY) {
		//DebugOut(L"isReturnY %d\n", isReturnY);
		y = y - 10;
		isReturnY = false;
	}	

	this->vy += ay * dt;

	stateHandler->Update(dt, coObjects);

	//inPipeTimer->Update(dt);

	for (size_t i = 0; i < ListEffect.size(); i++)
	{
		ListEffect[i]->Update(dt, coObjects);
		if (ListEffect[i]->isDeleted) {
			ListEffect.erase(ListEffect.begin() + i);
		}
	}

	//isOnPlatform = true;
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	if (!isInPipe) y += vy * dt;
	else y += vy;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking(e->nx, e->ny, this))
	{

		vy = 0;
		if (e->ny < 0) {			
			isOnPlatform = true;
		}
		else {
			vy = 0.000001f;
			this->jumpState = MarioJumpState::Fall;
		}
	}
	if (e->nx != 0 && e->obj->IsBlocking(e->nx, e->ny, this))
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			/*if (goomba->GetType() == NORMAL_GOOMBA) {
				goomba->SetState(GOOMBA_STATE_DIE);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else
			{
				if (goomba->GetState() == GOOMBA_STATE_WALKING) {
					goomba->SetState(GOOMBA_STATE_DIE);
					vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
				else
				{
					goomba->SetState(GOOMBA_STATE_WALKING);
					vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
			}*/
		}
	}
	else // hit by Goomba
	{
		//if (goomba->GetState() != GOOMBA_STATE_DIE)
		//{
		SetHurt();
		//}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}



void CMario::Render()
{
	stateHandler->Render();

	for (int i = 0; i < ListEffect.size(); i++)
	{
		ListEffect[i]->Render();
	}

	DebugOutTitle(L"Coins: %f", GetScore());//coin
	RenderBoundingBox();
}

void CMario::SetState(int state)
{
	//DebugOut(L"state %d \n", state);
	this->state = state;
	// DIE is the end state, cannot be changed! 
	//if (this->state == MARIO_STATE_DIE) return; 

}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	stateHandler->GetBoundingBox(left, top, right, bottom);
}

BaseMarioState* CMario::GetStateHandler()
{
	return this->stateHandler;
}

void CMario::SitStateUpdate()
{
	LPGAME game = CGame::GetInstance();
	if (game->IsKeyDown(DIK_DOWN)) {
		walkState = MarioWalkState::Sit;
	}
	if (game->IsKeyUp(DIK_DOWN) && walkState == MarioWalkState::Sit) {
		walkState = MarioWalkState::Idle;
	}
}

void CMario::WalkStateUpdate(DWORD dt)
{
	LPGAME game = CGame::GetInstance();
	float vx_check = GetVX();
	if (game->IsKeyDown(DIK_RIGHT) || game->IsKeyDown(DIK_LEFT))
	{
		//direct = game->IsKeyDown(DIK_RIGHT) ? 1 : -1;
		int keySign = game->IsKeyDown(DIK_LEFT) ? -1 : 1;
		//DebugOut(L"Directtt %d \n", keySign);

		if (isOnPlatform) walkState = MarioWalkState::Walk;
		ax = MARIO_WALKING_SPEED * keySign;
		float maxSpeed = MARIO_RUNNING_MAX_SPEED;

		if (game->IsKeyDown(DIK_A)) {
			if (isOnPlatform) {
				walkState = MarioWalkState::Run;
			}
			ax = MARIO_RUNNING_SPEED * keySign;
			maxSpeed = MARIO_RUNNING_MAX_SPEED;
		}
		DebugOut(L"Directtt %f \n", (float)GetVX() * keySign);
		if (GetVX() * keySign < 0) {
			isSliding = true;
			ax = (FLOAT)((game->IsKeyDown(DIK_A) ? MARIO_SKID_ACCELERATION : MARIO_SKID_ACCELERATION * 0.5) * keySign);

			if (!isOnPlatform) {
				ax = MARIO_SKID_ACCELERATION * keySign * 2;
			}
		}
		//DebugOut(L"[INFO] Current accelerate: %d \n", ax);
		vx_check += ax * dt;
		//DebugOut(L"Directtt %f \n", vx_check);
		float fly_sp = MAX_FLY_SPEED;
		if (jumpState != MarioJumpState::Idle)
			maxSpeed = (maxSpeed > fly_sp) ? fly_sp : maxSpeed;//min(maxSpeed, MAX_FLY_SPEED);

		if (abs(GetVX()) > maxSpeed) {
			int sign = GetVX() < 0 ? -1 : 1;
			if (abs(GetVX()) - maxSpeed > MARIO_ACCEL_WALK_X * dt) {
				vx_check -= MARIO_ACCEL_WALK_X * dt * sign;
			}
			else {
				vx_check = maxSpeed * sign;
			}
		}
		if (GetVX() * direct >= 0) {
			isSliding = 0;
		}
		direct = vx_check < 0 ? -1 : 1;
	}
	else {
		//else			isSliding = 0;
			//walkState = MarioWalkState::Idle;	
		if (abs(vx_check) > drag * dt) {
			int sign = vx_check < 0 ? -1 : 1;
			vx_check -= drag * dt * sign;
		}
		else {
			vx_check = 0.0f;
			if (walkState != MarioWalkState::Sit) {
				walkState = MarioWalkState::Idle;
			}
		}
	}
	//DebugOut(L"Directtt %d \n", isSliding);
	if (walkState != MarioWalkState::Sit) {
		drag = walkState == MarioWalkState::Run ? MARIO_RUN_DRAG_FORCE : MARIO_WALK_DRAG_FORCE;
	}
	drag *= isOnPlatform;

	//SetSpeed(vx_check, vy );
	vx = vx_check;
}

void CMario::JumpStateUpdate()
{
	LPGAME game = CGame::GetInstance();
}

void CMario::SetHurt()
{
	if (untouchable == 0) {
		if (level > MARIO_LEVEL_SMALL) {
			level--;
			SetLevel(level);
			//StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			die_start = GetTickCount64();
		}
	}
	else return;
}


void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	if (this->level == MARIO_LEVEL_RACOON)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	if (this->level == MARIO_LEVEL_FIRE)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;

	switch (l)
	{
	case MARIO_LEVEL_SMALL:
		this->stateHandler = new MarioStateSmall(this);
		break;
	case MARIO_LEVEL_BIG:
		this->stateHandler = new MarioStateBig(this);
		break;
	}
}


//void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
//{
//	if (level==MARIO_LEVEL_BIG)
//	{
//		if (isSitting)
//		{
//			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
//			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
//			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
//			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
//		}
//		else 
//		{
//			left = x - MARIO_BIG_BBOX_WIDTH/2;
//			top = y - MARIO_BIG_BBOX_HEIGHT/2;
//			right = left + MARIO_BIG_BBOX_WIDTH;
//			bottom = top + MARIO_BIG_BBOX_HEIGHT;
//		}
//	}
//	else
//	{
//		left = x - MARIO_SMALL_BBOX_WIDTH/2;
//		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
//		right = left + MARIO_SMALL_BBOX_WIDTH;
//		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
//	}
//}
//
//void CMario::SetLevel(int l)
//{
//	// Adjust position to avoid falling off platform
//	if (this->level == MARIO_LEVEL_SMALL)
//	{
//		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
//	}
//	level = l;
//}
//
