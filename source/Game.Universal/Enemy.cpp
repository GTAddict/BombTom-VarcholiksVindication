#include "pch.h"
#include "Enemy.h"
#include "SpriteRenderer.h"
#include "Player.h"
#include "Bullet.h"

#define SHOOT_INTERVAL_START 500
#define SHOOT_INTERVAL_END	4000


Enemy::Enemy(Player& player)
	: mPlayer(player)
{
	int randomSprite = rand() % 5;

	switch (randomSprite)
	{
	case 0:
		Init("images\\TomOne.png", 0, 0, 40, 54, (int)Layers::enemy);
		break;
	case 1:
		Init("images\\TomTwo.png", 0, 0, 40, 49, (int)Layers::enemy);
		break;
	case 2:
		Init("images\\TomThree.png", 0, 0, 39, 52, (int)Layers::enemy);
		break;
	case 3:
		Init("images\\TomFour.png", 0, 0, 40, 46, (int)Layers::enemy);
		break;
	case 4:
		Init("images\\Tom5.png", 0, 0, 40, 50, (int)Layers::enemy);
		break;
	}

	SetSpeedX(0.1f);
	SetSpeedY(-0.01f);
	int random = rand() % 2;
	if (random == 1)
	{
		SetSpeedX(mSpeedX * -1);
	}

	RandomizeTimer();
}

Enemy::~Enemy()
{
	for each (Bullet* bullet in mBulletList)
	{
		delete bullet;
	}
}

void Enemy::RandomizeTimer()
{
	mShootTimer = (float) (rand() % SHOOT_INTERVAL_END + SHOOT_INTERVAL_START);
}

void Enemy::SetPositionX(int x)
{
	mPosX = x;

	if (x > 1920 - mWidth / 2)
	{
		x = 1920 - mWidth / 2;
		mSpeedX = -mSpeedX;
	}

	if (x < mWidth / 2)
	{
		x = mWidth / 2;
		mSpeedX = -mSpeedX;
	}
}

void Enemy::SetPositionY(int y)
{
	mPosY = y;

	if (y > 1080 + mHeight / 2)
	{
		mIsAlive = false;
	}

	if (y < -mHeight / 2)
	{
		mIsAlive = false;
	}
}

void Enemy::UpdateFrame(long milliseconds)
{
	mShootTimer -= milliseconds;

	if (mShootTimer <= 0)
	{
		RandomizeTimer();
		Bullet* bullet = new Bullet(mPosX, mPosY);
		bullet->SetSpeed(-bullet->GetSpeed());
		mBulletList.push_back(bullet);
	}

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

	if (this->Intersects(&mPlayer))
	{
		mPlayer.SetAlive(false);
	}

	for each (Bullet* bullet in mBulletList)
	{
		if (bullet->Intersects(&mPlayer))
		{
			mPlayer.SetAlive(false);
		}
	}

	for each (Bullet* bullet in mPlayer.GetBulletList())
	{
		if (this->Intersects(bullet))
		{
			bullet->SetAlive(false);
			mIsAlive = false;
		}
	}

	SetPositionX((int)(mPosX + mSpeedX * milliseconds));
	SetPositionY((int)(mPosY + mSpeedY * milliseconds));
}
