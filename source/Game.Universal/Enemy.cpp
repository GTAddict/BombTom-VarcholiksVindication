#include "pch.h"
#include "Enemy.h"
#include "SpriteRenderer.h"
#include "Player.h"
#include "Bullet.h"

#define SHOOT_INTERVAL_START	500
#define SHOOT_INTERVAL_END		4000


Enemy::Enemy(Player& player)
	: mPlayer(player)
	, mAngularSpeed((static_cast <float> (rand()) / static_cast <float> (RAND_MAX) / 500) - 0.001f)
{
	int randomSprite = rand() % 10;
	float randomRotation = static_cast<float>(rand() % 2 * M_PI);

	switch (randomSprite)
	{
	case 0:
		Init(SpriteType::TomOne, 0, 0, 40, 54, 1, randomRotation, (int)Layers::enemy);
		mEnemyType = EnemyType::Tom;
		break;
	case 1:
		Init(SpriteType::TomTwo, 0, 0, 40, 49, 1, randomRotation, (int)Layers::enemy);
		mEnemyType = EnemyType::Tom;
		break;
	case 2:
		Init(SpriteType::TomThree, 0, 0, 39, 52, 1, randomRotation, (int)Layers::enemy);
		mEnemyType = EnemyType::Tom;
		break;
	case 3:
		Init(SpriteType::TomFour, 0, 0, 40, 46, 1, randomRotation, (int)Layers::enemy);
		mEnemyType = EnemyType::Tom;
		break;
	case 4:
		Init(SpriteType::Tom5, 0, 0, 40, 50, 1, randomRotation, (int)Layers::enemy);
		mEnemyType = EnemyType::Tom;
		break;
	case 5:
		Init(SpriteType::PaulOne, 0, 0, 40, 62, 1, randomRotation, (int)Layers::enemy);
		mEnemyType = EnemyType::Paul;
		break;
	case 6:
		Init(SpriteType::PaulTwo, 0, 0, 40, 48, 1, randomRotation, (int)Layers::enemy);
		mEnemyType = EnemyType::Paul;
		break;
	case 7:
		Init(SpriteType::PaulThree, 0, 0, 40, 56, 1, randomRotation, (int)Layers::enemy);
		mEnemyType = EnemyType::Paul;
		break;
	case 8:
		Init(SpriteType::PaulFour, 0, 0, 40, 55, 1, randomRotation, (int)Layers::enemy);
		mEnemyType = EnemyType::Paul;
		break;
	case 9:
		Init(SpriteType::Paul5, 0, 0, 40, 56, 1, randomRotation, (int)Layers::enemy);
		mEnemyType = EnemyType::Paul;
		break;
	}

	if (mEnemyType == EnemyType::Paul)
	{
		mAngularSpeed = 0;
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
		std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(mPosX, mPosY);
		bullet->SetSpeed(-bullet->GetSpeed());
		mBulletList.push_back(bullet);
	}

	std::list<std::shared_ptr<Bullet>> pendingKill;

	for (auto bullet : mBulletList)
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

	for (auto bullet : pendingKill)
	{
		mBulletList.remove(bullet);
	}

	pendingKill.clear();

	if (this->Intersects(&mPlayer))
	{
		mPlayer.SetAlive(false);
	}

	for (auto bullet : mBulletList)
	{
		if (bullet->Intersects(&mPlayer))
		{
			mPlayer.SetAlive(false);
		}
	}

	for (auto bullet : mPlayer.GetBulletList())
	{
		if (this->Intersects(bullet.get()))
		{
			bullet->SetAlive(false);
			mIsAlive = false;
		}
	}

	SetPositionX((int)(mPosX + mSpeedX * milliseconds));
	SetPositionY((int)(mPosY + mSpeedY * milliseconds));

	SetRotation(GetRotation() + milliseconds * mAngularSpeed);
}
