#pragma once
#include "GameObject.h"

#define KOOPAS_GREEN 1
#define KOOPAS_RED 2
#define KOOPAS_GREEN_WING 3

// KOOPAS GREEN
#define ID_ANI_KOOPAS_WALKING_RIGHT 40001
#define ID_ANI_KOOPAS_WALKING_LEFT 40002
#define ID_ANI_KOOPAS_DEFEND 40003
#define ID_ANI_KOOPAS_IS_KICKED 40004
#define ID_ANI_KOOPAS_IS_UPSIDE 40005
#define ID_ANI_KOOPAS_COMEBACK 40006
#define ID_ANI_KOOPAS_UPSIDE_ISKICKED 40007	// b? raccoon t?n c�ng v� b? ?�
#define ID_ANI_KOOPAS_UPSIDE_COMEBACK 40008 // hi?u ?ng b? l?t �p v� b?t ??u ra kh?i mai r�a
#define ID_ANI_KOOPAS_GREEN_WING_RIGHT 40009
#define ID_ANI_KOOPAS_GREEN_WING_LEFT 40010

// KOOPAS RED
#define ID_ANI_KOOPAS_RED_WALKING_RIGHT 40011
#define ID_ANI_KOOPAS_RED_WALKING_LEFT 40012
#define ID_ANI_KOOPAS_RED_DEFEND 40013
#define ID_ANI_KOOPAS_RED_IS_KICKED 40014
#define ID_ANI_KOOPAS_RED_IS_UPSIDE 40015
#define ID_ANI_KOOPAS_RED_COMEBACK 40016
#define ID_ANI_KOOPAS_RED_UPSIDE_ISKICKED 40017 // b? raccoon t?n c�ng v� b? ?�
#define ID_ANI_KOOPAS_RED_UPSIDE_COMEBACK 40018 //b? l?t �p v� b?t ??u ra kh?i mai r�a

#define KOOPAS_GRAVITY 0.002f

#define KOOPAS_BBOX_WIDTH 14
#define KOOPAS_BBOX_HEIGHT 26

#define KOOPAS_WALKING_SPEED 0.02f
#define KOOPAS_IS_KICKED_SPEED 0.18f
#define KOOPAS_SPEED_Y_IS_FIRE_ATTACKED 0.4f
#define KOOPAS_SPEED_Y_IS_TAIL_ATTACKED 0.4f
#define KOOPAS_SPEED_X_IS_TAIL_ATTACKED 0.05f
#define KOOPAS_RED_WING_SPEED_X 0.03f

#define KOOPAS_WING_JUMP_SPEED 0.15f

#define KOOPAS_BBOX_HEIGHT_DEFEND 16
#define ADJUST_POSITION_KOOPAS_HELD 13
#define KOOPAS_COMBACK_HEIGHT_ADJUST 0.15f
#define KOOPAS_BOUNCE_SPEED 0.3f

#define KOOPAS_DEFEND_TIMEOUT 8000 // h?t th?i gian defend ( ra kh?i mai r�a v� b?t ??u ?i)
#define KOOPAS_COMBACK_START 6000 //th?i gian t�nh t? l�c defend ??n l�c c� hi?u ?ng comeback

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_DEFEND 200
#define KOOPAS_STATE_IS_KICKED 300
#define KOOPAS_STATE_UPSIDE 400
#define KOOPAS_STATE_JUMP 500

#define ADJUST_X_TO_RED_CHANGE_DIRECTION 10

#define KOOPAS_WING_GRAVITY 0.0002f
class Koopas :
	public CGameObject
{
protected:
	float ax;
	float ay;

	vector<LPGAMEOBJECT> effects;

	int mario_nx;

public:
	Koopas(float x, float y, int model);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable();
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	BOOLEAN isTailAttacked = false;
	BOOLEAN isHeld;
	BOOLEAN isDefend;
	BOOLEAN isKicked;
	BOOLEAN isComeback;
	BOOLEAN isUpside;

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithColorBox(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithGoldBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);

	virtual void SetState(int state);

	ULONGLONG defend_start;

};

