#pragma once
#include "DeviceResources.h"
#include <string>
#include <map>
#include <string>
#include <functional>

class SpriteCache;
class SpriteRenderer;

enum class SpriteType : std::uint8_t
{
	Invalid = 0,
	First = 1,
	TomOne = First,
	TomTwo,
	TomThree,
	TomFour,
	Tom5,
	PaulOne,
	PaulTwo,
	PaulThree,
	PaulFour,
	Paul5,
	Krishna,
	RegularBullet,
	LimaBullet,
	Board,
	Count
};

class Sprite
{
public:
	Sprite();
	virtual ~Sprite();

	void Init(SpriteType sprite, int x, int y, int width, int height, float scale, float rotation, int layer);
	void InitializeVertices();
	void Destroy();

	void SetPosition(int x, int y);
	virtual void SetPositionX(int x);
	virtual void SetPositionY(int y);
	void SetRotation(float rotation);

	int GetPositionX()				{ return mPosX; }
	int GetPositionY()				{ return mPosY; }

	int GetWidth()					{ return mWidth; }
	int GetHeight()					{ return mHeight; }

	float GetScale()				{ return mScale; }
	float GetRotation()				{ return mRotation; }

	void SetVisible(bool visible)	{ mVisible = visible; }
	bool GetVisible() const			{ return mVisible; }

	virtual void UpdateFrame(long milliseconds) { UNREFERENCED_PARAMETER(milliseconds); }

	bool Intersects(Sprite* other);

	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>&	GetTexture()			const { return mTexture; }
	const Microsoft::WRL::ComPtr<ID3D11Buffer>&				GetVertexBuffer()		const { return mVertexBuffer; }
	DX::Transform2D											GetTransform()			const;
	DirectX::XMFLOAT4X4										GetTextureTransform()	const;

	static const std::string&								ToTextureName(SpriteType sprite);

protected:

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		mTexture;
	Microsoft::WRL::ComPtr<ID3D11Buffer>					mVertexBuffer;

	int														mPosX;
	int														mPosY;
	int														mWidth;
	int														mHeight;
	float													mScale;
	float													mRotation;
	int														mLayerID;
	bool													mVisible;

	static const std::map<SpriteType, std::string>			mTextureMap;
};

