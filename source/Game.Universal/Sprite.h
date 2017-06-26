#pragma once
#include "DeviceResources.h"
#include <string>

class SpriteCache;
class SpriteRenderer;

class Sprite
{
public:
	Sprite();
	~Sprite();

	void Init(std::string fileName, int x, int y, int width, int height, float scale, int layer);
	void Destroy();

	void SetPosition(int x, int y);
	virtual void SetPositionX(int x);
	virtual void SetPositionY(int y);

	int GetPositionX()				{ return mPosX; }
	int GetPositionY()				{ return mPosY; }

	int GetWidth()					{ return mWidth; }
	int GetHeight()					{ return mHeight; }

	float GetScale()				{ return mScale; }

	void SetVisible(bool visible)	{ mVisible = visible; }
	bool GetVisible() const			{ return mVisible; }

	virtual void UpdateFrame(long milliseconds) { UNREFERENCED_PARAMETER(milliseconds); }

	bool Intersects(Sprite* other);

	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>&	GetTexture()			const { return mTexture; }
	DX::Transform2D											GetTransform()			const;
	DirectX::XMFLOAT4X4										GetTextureTransform()	const;


protected:

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mTexture;
	int													mPosX;
	int													mPosY;
	int													mWidth;
	int													mHeight;
	float												mScale;
	int													mLayerID;
	bool												mVisible;
};

