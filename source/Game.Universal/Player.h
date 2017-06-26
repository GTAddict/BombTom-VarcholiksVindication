#pragma once
#include "Sprite.h"
#include <list>

class Sprite;
class Bullet;

class Player : public Sprite
{
public:
	Player(std::shared_ptr<DX::KeyboardComponent> keyboardComponent, std::shared_ptr<DX::GamePadComponent> gamepadComponent);
	~Player();

	virtual void SetPositionX(int x) override;
	virtual void SetPositionY(int y) override;

	virtual void UpdateFrame(long milliseconds);

	void SetSpeed(float speed) { mSpeed = speed / 1000; }
	void SetAlive(bool alive) { mAlive = alive; }
	bool GetAlive() { return mAlive; }

	const std::list<std::shared_ptr<Bullet>>& GetBulletList() const { return mBulletList; }

private:

	std::shared_ptr<DX::KeyboardComponent>	mKeyboardComponent;
	std::shared_ptr<DX::GamePadComponent>	mGamepadComponent;

	bool mAlive;
	float mSpeed;	// Pixels per millisec
	std::list<std::shared_ptr<Bullet>> mBulletList;

	float mTimeSinceLastFire;
	int mBulletsInBurstRemaining;
};

