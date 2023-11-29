#include "MarioStateRacoon.h"
#include "Mario.h"
#include "PlayScene.h"

MarioStateRacoon::MarioStateRacoon(CMario* mario) : BaseMarioState(mario)
{
	
}

MarioStateRacoon::~MarioStateRacoon()
{
	
}

void MarioStateRacoon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	BaseMarioState::Update(dt, coObjects);

}

void MarioStateRacoon::JumpUpdate(DWORD dt)
{
	float vx = mario->GetVX();
	float vy = mario->GetVY();

	float x = mario->GetX();
	float y = mario->GetY();

	CGame* game = CGame::GetInstance();

	CPlayScene* playScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();

	//DebugOut(L"Mario Position: %f\nCamera Position: %f\n\n", mario->GetY(), playScene->map->GetCamY());

	if (mario->isOnPlatform) {
		if (mario->GetY() >= MAX_FLY_HEIGHT) {
			playScene->isFlyCam = false;
		}
		mario->jumpState = MarioJumpState::Idle;

		if (game->IsKeyDown(DIK_S)) {
			mario->jumpState = MarioJumpState::Jump;
			mario->isOnPlatform = false;
			mario->_jumpStartHeight = y;
		}
	}

	else if (game->IsKeyDown(DIK_S)) {
		if (mario->powerMeter >= PMETER_MAX) {
			mario->jumpState = MarioJumpState::Fly;
			mario->_jumpStartHeight = y;

			//Change Camera state
			playScene->isFlyCam = true;
			//playScene->isFlyCam = true;
		};
	}

	float jumpHeight = MARIO_JUMP_HEIGHT;
	float minJumpHeight = MARIO_MIN_JUMP_HEIGHT;
	float height = 0;

	switch (mario->jumpState)
	{
	case MarioJumpState::Fly:
		height = abs(mario->_jumpStartHeight - y - vy * dt);
		minJumpHeight = MARIO_MIN_HIGH_JUMP_HEIGHT;

		if (height < minJumpHeight || (height < MARIO_SUPER_JUMP_HEIGHT && game->IsKeyDown(DIK_S))) {
			vy = -MARIO_FLYING_UP_FORCE - MARIO_GRAVITY * dt;
		}
		else {
			mario->jumpState = MarioJumpState::Fall;
			vy = -MARIO_FLYING_UP_FORCE / 2;
		}
		break;
	case MarioJumpState::HighJump:
		jumpHeight = MARIO_HIGH_JUMP_HEIGHT;
		minJumpHeight = MARIO_MIN_HIGH_JUMP_HEIGHT;
	case MarioJumpState::Jump:
		height = abs(mario->_jumpStartHeight - y - vy * dt);

		if (height < minJumpHeight || (height < jumpHeight && game->IsKeyDown(DIK_S))) {
			vy = -MARIO_PUSH_FORCE - MARIO_GRAVITY * dt;
		}
		else {
			if (game->IsKeyDown(DIK_S) && mario->jumpState == MarioJumpState::Jump) {
				mario->jumpState = MarioJumpState::HighJump;
			}
			else {
				mario->jumpState = MarioJumpState::Fall;
				vy = -MARIO_PUSH_FORCE / 2;
			}
		}
		break;
	case MarioJumpState::Float:
		break;
	case MarioJumpState::Fall:
		
		break;
	}

	mario->SetVY(vy);
}

void MarioStateRacoon::PowerMeterUpdate(DWORD dt)
{
	float vx = mario->GetVX();
	float vy = mario->GetVY();

	float x = mario->GetX();
	float y = mario->GetY();

	CGame* game = CGame::GetInstance();

	float maxRun = abs(vx) > MARIO_RUN_SPEED * 0.85f;

}

void MarioStateRacoon::AttackUpdate(DWORD dt)
{
	
}

void MarioStateRacoon::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (!mario->isOnPlatform)
	{
		//aniId = ID_ANI_MARIO_BRACE_RIGHT;
		switch (mario->jumpState)
		{
		case MarioJumpState::Fly:
			aniId = ID_ANI_RACOON_MARIO_FLYING_RIGHT;
			break;
		case MarioJumpState::Float:
			aniId = ID_ANI_RACOON_MARIO_FLOATING_RIGHT;
			break;
		case MarioJumpState::Fall:
			break;
		case MarioJumpState::Jump:
			break;
		case MarioJumpState::HighJump:
			aniId = ID_ANI_RACOON_MARIO_JUMP_WALK_RIGHT;
		default:
			break;
		}
	}


	else if (mario->isSliding && mario->GetVX() != 0) {
		aniId = ID_ANI_RACOON_MARIO_BRACE_RIGHT;
	}
	else {

		if (mario->GetVX() == 0 && mario->walkState != MarioWalkState::Sit) {
			aniId = ID_ANI_RACOON_MARIO_IDLE_RIGHT;
		}

		else {
			switch (mario->walkState)
			{
			case MarioWalkState::Run:
				aniId = ID_ANI_RACOON_MARIO_RUNNING_RIGHT;
				break;
			case MarioWalkState::Walk:
				aniId = ID_ANI_RACOON_MARIO_WALKING_RIGHT;
				break;
			case MarioWalkState::Sit:
				aniId = ID_ANI_RACOON_MARIO_SIT_RIGHT;
				break;
			default:
				aniId = ID_ANI_RACOON_MARIO_IDLE_RIGHT;
				break;
			}
		}

	}

	if (aniId == -1) aniId = ID_ANI_RACOON_MARIO_IDLE_RIGHT;
	if (mario->direct < 0) aniId += 1;

	animations->Get(aniId)->Render(mario->GetX(), mario->GetY());

}
