#include "pch.h"
#include "Bullet.h"
#include "SpriteRenderer.h"


Bullet::Bullet(int x, int y, BulletType bulletType)
{
	if (bulletType == BulletType::REGULAR)
	{
		Init("Content\\Textures\\Books.png", x, y, 27, 20, 1, (int)Layers::bullet);
	}
	else if (bulletType == BulletType::LIMA)
	{
		Init("Content\\Textures\\Lima.png", x, y, 20, 20, 1, (int)Layers::bullet);
	}

	SetSpeed(1000);
	mIsAlive = true;
}

Bullet::~Bullet()
{
}

void Bullet::SetPositionX(int x)
{
	mPosX = x;

	if (x > 1920 + mWidth / 2)
	{
		mIsAlive = false;
	}

	if (x < -mWidth / 2)
	{
		mIsAlive = false;
	}
}

void Bullet::SetPositionY(int y)
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

void Bullet::UpdateFrame(long milliseconds)
{
	SetPositionY((int) (mPosY + mSpeed * milliseconds));
}

