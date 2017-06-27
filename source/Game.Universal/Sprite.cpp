#include "pch.h"
#include "Sprite.h"
#include "SpriteCache.h"
#include "SpriteRenderer.h"
#include "SpriteManagerHelper.h"
#include <cmath>

const std::map<SpriteType, std::string> Sprite::mTextureMap =
{
	{ SpriteType::TomOne,			"Content\\Textures\\TomOne.png"		},
	{ SpriteType::TomTwo,			"Content\\Textures\\TomTwo.png"		},
	{ SpriteType::TomThree,			"Content\\Textures\\TomThree.png"	},
	{ SpriteType::TomFour,			"Content\\Textures\\TomFour.png"	},
	{ SpriteType::Tom5,				"Content\\Textures\\Tom5.png"		},
	{ SpriteType::Krishna,			"Content\\Textures\\Krishna.png"	},
	{ SpriteType::RegularBullet,	"Content\\Textures\\Books.png"		},
	{ SpriteType::LimaBullet,		"Content\\Textures\\Lima.png"		},
	{ SpriteType::Board,			"Content\\Textures\\board.png"		}
};

using namespace DX;
using namespace DirectX;

Sprite::Sprite()
	: mPosX(-1)
	, mPosY(-1)
	, mWidth(-1)
	, mHeight(-1)
	, mLayerID((int)Layers::invalid)
	, mVisible(true)
{
}


Sprite::~Sprite()
 {
	Destroy();
}

void Sprite::Init(SpriteType sprite, int x, int y, int width, int height, float scale, int layer)
{

	mScale = scale;

	mWidth = width;
	mHeight = height;

	SetPositionX(x);
	SetPositionY(y);

	mLayerID = layer;

	mTexture = SpriteManagerHelper::GetInstance()->GetCache().GetTexture(ToTextureName(sprite));
	InitializeVertices();
	SpriteManagerHelper::GetInstance()->GetRenderer().Register(this, mLayerID);
}

void Sprite::InitializeVertices()
{
	VertexPositionTexture vertices[] =
	{
		VertexPositionTexture(XMFLOAT4(static_cast<float>(-mWidth/2), static_cast<float>(-mHeight/2), 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)),
		VertexPositionTexture(XMFLOAT4(static_cast<float>(-mWidth/2), static_cast<float>(mHeight/2), 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)),
		VertexPositionTexture(XMFLOAT4(static_cast<float>(mWidth/2), static_cast<float>(mHeight/2), 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)),
		VertexPositionTexture(XMFLOAT4(static_cast<float>(mWidth/2), static_cast<float>(-mHeight/2), 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)),
	};

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(VertexPositionTexture) * ARRAYSIZE(vertices);
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubResourceData = { 0 };
	vertexSubResourceData.pSysMem = vertices;
	ThrowIfFailed(SpriteManagerHelper::GetInstance()->GetDeviceResources()->GetD3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, mVertexBuffer.ReleaseAndGetAddressOf()));
}

void Sprite::Destroy()
{
	SpriteManagerHelper::GetInstance()->GetRenderer().UnRegister(this, mLayerID);
	mVertexBuffer.Reset();
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
	return DX::Transform2D(DirectX::XMFLOAT2(static_cast<float>(mPosX), static_cast<float>(mPosY)), 0, XMFLOAT2(mScale, mScale));
}

DirectX::XMFLOAT4X4 Sprite::GetTextureTransform() const
{
	XMFLOAT4X4 Transform;
	DirectX::XMMATRIX ScaleMatrix = XMMatrixScaling(1.0f, 1.0f, 0.0f);
	XMStoreFloat4x4(&Transform, ScaleMatrix);
	return Transform;
}

const std::string& Sprite::ToTextureName(SpriteType sprite)
{
	return mTextureMap.at(sprite);
}
