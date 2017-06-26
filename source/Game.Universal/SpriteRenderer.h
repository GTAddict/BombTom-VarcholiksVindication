#pragma once
#include <list>
#include <map>
#include "DrawableGameComponent.h"

enum class Layers
{
	invalid = -1,
	first = 0,
	background = first,
	player,
	enemy,
	bullet,
	count
};

class Sprite;

typedef std::map<Layers, std::list<Sprite*>> RenderMap_t;

class SpriteRenderer : public DX::DrawableGameComponent
{
public:

	SpriteRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources, const std::shared_ptr<DX::Camera>& camera);
	~SpriteRenderer();

	void Render(const DX::StepTimer& timer) override;
	void Register(Sprite* sprite, int32_t layer);
	void UnRegister(Sprite* sprite, int32_t layer);

	virtual void CreateDeviceDependentResources() override;
	virtual void ReleaseDeviceDependentResources() override;

	void InitializeVertices();

private:

	struct VSCBufferPerObject
	{
		DirectX::XMFLOAT4X4 WorldViewProjection;
		DirectX::XMFLOAT4X4 TextureTransform;

		VSCBufferPerObject() :
			WorldViewProjection(DX::MatrixHelper::Identity), TextureTransform(DX::MatrixHelper::Identity)
		{ };

		VSCBufferPerObject(const DirectX::XMFLOAT4X4& wvp, const DirectX::XMFLOAT4X4& textureTransform) :
			WorldViewProjection(wvp), TextureTransform(textureTransform)
		{ }
	};

	Microsoft::WRL::ComPtr<ID3D11VertexShader>			mVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			mPixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			mInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				mVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				mIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				mVSCBufferPerObject;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mSpriteSheet;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			mTextureSampler;
	Microsoft::WRL::ComPtr<ID3D11BlendState>			mAlphaBlending;
	VSCBufferPerObject									mVSCBufferPerObjectData;
	std::uint32_t										mIndexCount;
	bool												mLoadingComplete;

	RenderMap_t renderMap;
};

