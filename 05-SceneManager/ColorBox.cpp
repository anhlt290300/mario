#include "ColorBox.h"
#include "FireBall.h"
CColorBox::CColorBox(float x, float y, float width, float height) :CGameObject(x, y)
{
	this->width = width;
	this->height = height;
}

void CColorBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - width / 2;
	r = l + width;
	t = y - height / 2;
	b = t + height;
}

int CColorBox::IsBlocking(float nx, float ny, CGameObject* target)
{
	if (ny < 0 && !dynamic_cast<CFireBall*>(target)) {
		return 1;
	}
	return 0;
}
