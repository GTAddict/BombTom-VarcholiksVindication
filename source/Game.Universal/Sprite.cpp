#include "pch.h"
#include "Sprite.h"
#include "SpriteCache.h"
#include "SpriteRenderer.h"
#include <cmath>

using namespace DX;

Sprite::Sprite(SpriteCache& spriteCache, SpriteRenderer& spriteRenderer)
	: mSpriteCache(spriteCache)
	, mSpriteRenderer(spriteRenderer)
	, mPosX(-1)
	, mPosY(-1)
	, mWidth(-1)
	, mHeight(-1)
	, mLayerID((int)Layers::invalid)
	, mVisible(true)
{
}


Sprite::~Sprite()
{
}

void Sprite::Init(std::string fileName, int x, int y, int width, int height, int layer)
{

	mWidth = width;
	mHeight = height;

	SetPositionX(x);
	SetPositionY(y);

	mLayerID = layer;

	auto LoadSpriteTask	= Concurrency::create_task([this, &fileName]() { mTexture = mSpriteCache.Load(fileName); });
	LoadSpriteTask.then([this]() { mSpriteRenderer.Register(this, mLayerID); });
}

void Sprite::Destroy()
{
	mSpriteRenderer.UnRegister(this, mLayerID);
}

void Sprite::SetPosition(int x, int y)
{
	SetPositionX(x);
	SetPositionY(y);
}

void Sprite::SetPositionX(int x)
{
	mPosX = x;
}

void Sprite::SetPositionY(int y)
{
	mPosY = y;
}

bool Sprite::Intersects(Sprite* other)
{
	if (mPosX < other->mPosX + other->mWidth
		&& mPosX + mWidth > other->mPosX
		&& mPosY < other->mPosY + other->mHeight
		&& mHeight + mPosY > other->mHeight)
	{
		return true;
	}

	return false;
}

DX::Transform2D Sprite::GetTransform() const
{
	return DX::Transform2D(DirectX::XMFLOAT2(static_cast<float>(mPosX), static_cast<float>(mPosY)));
}

const DirectX::XMFLOAT4X4& Sprite::GetTextureTransform() const
{
	static auto Scale = MatrixHelper::Identity;
	return Scale;
}
