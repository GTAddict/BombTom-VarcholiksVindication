#pragma once
#include "Sprite.h"
#include <list>

class Sprite;
class Bullet;

class Player : public Sprite
{
public:
	Player();
	~Player();

	virtual void SetPositionX(int x) override;
	virtual void SetPositionY(int y) override;

	virtual void UpdateFrame(long milliseconds) override;

	void SetSpeed(float speed) { mSpeed = speed / 1000; }
	void SetAlive(bool alive) { mAlive = alive; }
	bool GetAlive() { return mAlive; }

	const std::list<Bullet*>& GetBulletList() const { return mBulletList; }

private:
	bool mAlive;
	float mSpeed;	// Pixels per millisec
	std::list<Bullet*> mBulletList;

	float mTimeSinceLastFire;
	int mBulletsInBurstRemaining;
};

