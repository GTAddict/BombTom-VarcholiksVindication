#pragma once

#include "Sprite.h"
#include "Player.h"
#include <list>

class Bullet;

enum class EnemyType
{
	Tom,
	Paul
};

class Enemy : public Sprite
{
public:
	Enemy(Player& p);
	~Enemy();

	void SetPositionX(int x);

	void SetPositionY(int y);

	void UpdateFrame(long milliseconds);

	void SetSpeedX(float speed) { mSpeedX = speed; }
	void SetSpeedY(float speed) { mSpeedY = speed; }

	bool IsAlive() { return mIsAlive; }
	EnemyType GetEnemyType() { return mEnemyType; }

	void RandomizeTimer();

private:
	bool mIsAlive;
	float mSpeedX;
	float mSpeedY;

	float mShootTimer;
	float mAngularSpeed;

	Player& mPlayer;
	EnemyType mEnemyType;
	std::list<std::shared_ptr<Bullet>> mBulletList;
};

