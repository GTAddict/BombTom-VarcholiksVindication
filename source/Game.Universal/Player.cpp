#include "pch.h"
#include "Player.h"
#include "SpriteRenderer.h"
//#include "keyProcess.h"
#include <windows.h>
#include "Bullet.h"

#define ANTI_SPAM_LIMIT 750
#define BURST_LIMIT 10

Player::Player(std::shared_ptr<DX::KeyboardComponent> keyboardComponent, std::shared_ptr<DX::GamePadComponent> gamepadComponent)
{
	Init(SpriteType::Krishna, 0, 0, 59, 98, 1.0f, 0, (int)Layers::player);
	SetSpeed(500);
	mAlive = true;
	mTimeSinceLastFire = ANTI_SPAM_LIMIT;
	mBulletsInBurstRemaining = BURST_LIMIT;

	mKeyboardComponent = keyboardComponent;
	mGamepadComponent = gamepadComponent;
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

	if (mKeyboardComponent->IsKeyDown(DX::Keys::Left) || mGamepadComponent->IsButtonDown(DX::GamePadButtons::DPadLeft))
	{
		SetPositionX((int) (mPosX - mSpeed * milliseconds));
	}

	if (mKeyboardComponent->IsKeyDown(DX::Keys::Right) || mGamepadComponent->IsButtonDown(DX::GamePadButtons::DPadRight))
	{
		SetPositionX((int) (mPosX + mSpeed * milliseconds));
	}

	if (mKeyboardComponent->IsKeyDown(DX::Keys::Space) || mGamepadComponent->IsButtonDown(DX::GamePadButtons::X))
	{
 		if (mBulletsInBurstRemaining > 0)
		{
			mTimeSinceLastFire = 0;
			mBulletsInBurstRemaining--;
			auto bullet = std::make_shared<Bullet>(mPosX, mPosY, BulletType::LIMA);
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