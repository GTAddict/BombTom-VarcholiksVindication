#pragma once
#include "Sprite.h"

enum class BulletType
{
	INVALID = -1,
	REGULAR = 0,
	LIMA
};

class Bullet : public Sprite
{
public:
	Bullet(int x, int y, BulletType bulletType = BulletType::REGULAR);
	~Bullet();

	virtual void SetPositionX(int x) override;
	virtual void SetPositionY(int y) override;

	virtual void UpdateFrame(long milliseconds) override;

	void SetSpeed(float speed) { mSpeed = speed / 1000; }
	float GetSpeed() { return mSpeed * 1000; }

	bool IsAlive() { return mIsAlive; }
	void SetAlive(bool alive) { mIsAlive = alive; }

private:
	float mSpeed;
	bool mIsAlive;
};

