#include "pch.h"
#include "Player.h"
#include "SpriteRenderer.h"
//#include "keyProcess.h"
#include <windows.h>
#include "Bullet.h"

#define ANTI_SPAM_LIMIT 750
#define BURST_LIMIT 10

Player::Player()
{
	Init("images\\Krishna.png", 0, 0, 59, 98, (int)Layers::player);
	SetSpeed(500);
	mAlive = true;
	mTimeSinceLastFire = ANTI_SPAM_LIMIT;
	mBulletsInBurstRemaining = BURST_LIMIT;
}


Player::~Player()
{
}

void Player::SetPositionX(int x)
{
	if (x - mWidth / 2 < 0)
	{
		x = mWidth / 2;
	}

	if (x + mWidth / 2 >= 1920)
	{
		x = 1920 - mWidth / 2;
	}

	mPosX = x;
}

void Player::SetPositionY(int y)
{
	// This is always clamped to a fixed value
	UNREFERENCED_PARAMETER(y);
	mPosY = mHeight / 2;
}

void Player::UpdateFrame(long milliseconds)
{
	mTimeSinceLastFire += milliseconds;

	std::list<Bullet*> pendingKill;

	for each (Bullet* bullet in mBulletList)
	{
		if (bullet && bullet->IsAlive())
		{
			bullet->UpdateFrame(milliseconds);
		}
		else
		{
			pendingKill.push_back(bullet);
		}
	}

	for each (Bullet* bullet in pendingKill)
	{
		mBulletList.remove(bullet);
		delete bullet;
	}

	pendingKill.clear();

	//if (IsKeyDown(VK_LEFT))
	{
		SetPositionX((int) (mPosX - mSpeed * milliseconds));
	}

	//if (IsKeyDown(VK_RIGHT))
	{
		SetPositionX((int) (mPosX + mSpeed * milliseconds));
	}

	//if (IsKeyDown(VK_SPACE))
	{
		if (mBulletsInBurstRemaining > 0)
		{
			mTimeSinceLastFire = 0;
			mBulletsInBurstRemaining--;
			Bullet* bullet = new Bullet(mPosX, mPosY, BulletType::LIMA);
			mBulletList.push_back(bullet);
		}
		else if (mBulletsInBurstRemaining == 0)
		{
			if (mTimeSinceLastFire >= ANTI_SPAM_LIMIT)
			{
				mBulletsInBurstRemaining = BURST_LIMIT;
			}
		}
	}
}