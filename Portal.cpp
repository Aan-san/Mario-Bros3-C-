#include "Portal.h"
#include "PlayScence.h"

CPortal::CPortal(int scene_id, float start_x, float start_y)
{
	this->scene_id = scene_id;
	this->start_x = start_x;
	this->start_y = start_y;
	type = IGNORE;
}

void CPortal::Render()
{
	RenderBoundingBox();
}
void CPortal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;
	if (mario != NULL)
	{
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		GetBoundingBox(oLeft, oTop, oRight, oBottom);
		if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom) && mario->isWannaDown
			&& mLeft >= oLeft && mRight <= oRight)
		{
			mario->portal = this;
			if (tag == BACKTOPLAYSCENE)
				mario->StartPipeUp();
			if (tag == TOEXTRASCENE)
				mario->StartPipeDown();
			return;
		}

	}
}
void CPortal::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + PORTAL_BBOX_WIDTH;
	b = y + PORTAL_BBOX_HEIGHT;
}