#include "MushRoom.h"
#include "Utils.h"
#include "Mario.h"
#include "Block.h"
#include "Brick.h"
#include "PlayScence.h"
#include "IntroScene.h"
#include "PlantBullet.h"


void CMushRoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!IsInViewPort())
		isDestroyed = true;
	if (isDestroyed)
		return;
	CGameObject::Update(dt);
	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;
	CMario* mario = {};
	if (!dynamic_cast<CIntroScene*> (CGame::GetInstance()->GetCurrentScene()))
		mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	else
		mario = ((CIntroScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL)
	{
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		if (isColliding(mLeft, mTop, mRight, mBottom))
		{
			if (tag == MUSHROOM_TYPE_RED)
			{
				mario->SetLevel(MARIO_LEVEL_BIG);
				mario->AddScore(x, y, 1000);
			}
			else
			{
				mario->AddScore(x, y, 1);
				mario->AddLife();
			}
			isAppear = false;
			isDestroyed = true;
		}
	}
	if (state == MUSHROOM_STATE_WALK)
	{
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();
		CalcPotentialCollisions(coObjects, coEvents);

		if (coEvents.size() == 0)
		{
			y += dy;
			x += dx;
			vy = MUSHROOM_GRAVITY;
		}
		else
		{
			float min_tx, min_ty;
			float nx = 0, ny = 0;
			float rdx = 0;
			float rdy = 0;

			// TODO: This is a very ugly designed function!!!!
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			float x0 = x, y0 = y;
			x = x0 + min_tx * dx + nx * PUSHBACK;
			y = y0 + min_ty * dy + ny * PUSHBACK;
			//
			// Collision logic with other objects
			//
			if(mario != NULL)
				if (mario->isAtIntroScene)
					vx = -MUSHROOM_SPEED;
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				GetBoundingBox(mLeft, mTop, mRight, mBottom);
				e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);
				if (dynamic_cast<CBrick*>(e->obj))
				{
					if (e->ny < 0)
						vy = 0;
					if (e->nx != 0)
					{
						if (ceil(mBottom) != oTop)
							vx = -vx;
					}
				}
				else if (dynamic_cast<CBlock*>(e->obj))
				{
					CBlock* block = dynamic_cast<CBlock*>(e->obj);
					x = x0 + dx;
					if (ny < 0)
						vy = 0;
					else
						y = y0 + dy;
				}
				else
				{
					x = x0 + dx;
					y = y0 + dy;
				}
			}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++)
			delete coEvents[i];
	}

}
	if (state == MUSHROOM_STATE_UP)
	{
		y += dy;
		if (start_y - y >= MUSHROOM_BBOX_HEIGHT)
		{
			vy = 0;
			y = start_y - MUSHROOM_BBOX_HEIGHT - 0.1f;
			SetState(MUSHROOM_STATE_WALK);
		}
	}
}

void CMushRoom::Render()
{
	if (!isAppear || isDestroyed)
		return;
	if(tag == MUSHROOM_TYPE_GREEN)
		animation_set->at(MUSHROOM_ANI_GREEN_ID)->Render(x, y);
	else
		animation_set->at(MUSHROOM_ANI_RED_ID)->Render(x, y);
	RenderBoundingBox();
}

void CMushRoom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + MUSHROOM_BBOX_WIDTH;
	b = y + MUSHROOM_BBOX_HEIGHT;
}

void CMushRoom::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario;
	switch (state)
	{
	case MUSHROOM_STATE_IDLE:
		vx = vy = 0;
		break;
	case MUSHROOM_STATE_UP:
		vy = -0.05f;
		start_y = y;
		break;
	case MUSHROOM_STATE_WALK:
		if (!dynamic_cast<CIntroScene*> (CGame::GetInstance()->GetCurrentScene()))
		{
			mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			vx = mario->nx * MUSHROOM_SPEED;
		}
		else
			mario = ((CIntroScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		vy = MUSHROOM_GRAVITY;
		break;
	}
}