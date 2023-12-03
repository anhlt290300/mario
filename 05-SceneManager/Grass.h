#pragma once
#include "GameObject.h"

#define ID_ANI_GRASS 100

class CGrass :
	public CGameObject
{
public:
	CGrass(float x, float y) : CGameObject(x, y) {}
	virtual void Render();
	void Update(DWORD dt) {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) { left = top = right = bottom = 0; }
};

