#include "pch.h"
#include "SpriteRenderer.h"
#include "Sprite.h"
#include <algorithm>

using namespace DX;
using namespace DirectX;

SpriteRenderer::SpriteRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources, const std::shared_ptr<DX::Camera>& camera)
	: DrawableGameComponent(deviceResources, camera)
	, mLoadingComplete(false)
{
}


SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::CreateDeviceDependentResources()
{
	auto loadVSTask = ReadDataAsync(L"SpriteRendererVS.cso");
	auto loadPSTask = ReadDataAsync(L"SpriteRendererPS.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
		ThrowIfFailed(
			mDeviceResources->GetD3DDevice()->CreateVertexShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				mVertexShader.ReleaseAndGetAddressOf()
			)
		);

		// Create an input layout
		ThrowIfFailed(
			mDeviceResources->GetD3DDevice()->CreateInputLayout(
				VertexPositionTexture::InputElements,
				VertexPositionTexture::InputElementCount,
				&fileData[0],
				fileData.size(),
				mInputLayout.ReleaseAndGetAddressOf()
			)
		);

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(VSCBufferPerObject), D3D11_BIND_CONSTANT_BUFFER);
		ThrowIfFailed(
			mDeviceResources->GetD3DDevice()->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				mVSCBufferPerObject.ReleaseAndGetAddressOf()
			)
		);
	});

	// After the pixel shader file is loaded, create the shader and texture sampler state.
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
		ThrowIfFailed(
			mDeviceResources->GetD3DDevice()->CreatePixelShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				mPixelShader.ReleaseAndGetAddressOf()
			)
		);

		D3D11_SAMPLER_DESC samplerStateDesc;
		ZeroMemory(&samplerStateDesc, sizeof(samplerStateDesc));
		samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.MinLOD = -FLT_MAX;
		samplerStateDesc.MaxLOD = FLT_MAX;
		samplerStateDesc.MipLODBias = 0.0f;
		samplerStateDesc.MaxAnisotropy = 1;
		samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		ThrowIfFailed(mDeviceResources->GetD3DDevice()->CreateSamplerState(&samplerStateDesc, mTextureSampler.ReleaseAndGetAddressOf()));

		D3D11_BLEND_DESC blendStateDesc = { 0 };
		blendStateDesc.RenderTarget[0].BlendEnable = true;
		blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		ThrowIfFailed(mDeviceResources->GetD3DDevice()->CreateBlendState(&blendStateDesc, mAlphaBlending.ReleaseAndGetAddressOf()));
	});

	(createPSTask && createVSTask).then([this]() { InitializeIndices(); mLoadingComplete = true; });
}

void SpriteRenderer::ReleaseDeviceDependentResources()
{
	mVertexShader.Reset();
	mPixelShader.Reset();
	mInputLayout.Reset();
	mIndexBuffer.Reset();
	mVSCBufferPerObject.Reset();
	mTextureSampler.Reset();
}

void SpriteRenderer::InitializeIndices()
{
	// Create and index buffer
	const uint32_t indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	mIndexCount = ARRAYSIZE(indices);

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(uint32_t) * mIndexCount;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubResourceData = { 0 };
	indexSubResourceData.pSysMem = indices;
	ThrowIfFailed(mDeviceResources->GetD3DDevice()->CreateBuffer(&indexBufferDesc, &indexSubResourceData, mIndexBuffer.ReleaseAndGetAddressOf()));
}

void SpriteRenderer::Render(const DX::StepTimer& timer)
{
	UNREFERENCED_PARAMETER(timer);

	if (!mLoadingComplete)
	{
		return;
	}

	ID3D11DeviceContext* direct3DDeviceContext = mDeviceResources->GetD3DDeviceContext();
	direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	direct3DDeviceContext->IASetInputLayout(mInputLayout.Get());

	static const UINT stride = sizeof(VertexPositionTexture);
	static const UINT offset = 0;

	direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	direct3DDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	direct3DDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
	direct3DDeviceContext->VSSetConstantBuffers(0, 1, mVSCBufferPerObject.GetAddressOf());
	direct3DDeviceContext->PSSetSamplers(0, 1, mTextureSampler.GetAddressOf());
	direct3DDeviceContext->OMSetBlendState(mAlphaBlending.Get(), 0, 0xFFFFFFFF);

	for (int32_t i = 0; i <= (int32_t)Layers::count; ++i)
	{
		std::list<Sprite*> sprites = renderMap[(Layers)i];
		for (auto sprite : sprites)
		{
			if (!sprite->GetVisible() || !sprite || !sprite->GetTexture())
			{
				continue;
			}

			direct3DDeviceContext->IASetVertexBuffers(0, 1, sprite->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			direct3DDeviceContext->PSSetShaderResources(0, 1, sprite->GetTexture().GetAddressOf());

			const XMMATRIX wvp = XMMatrixTranspose(sprite->GetTransform().WorldMatrix() * mCamera->ViewProjectionMatrix());
			XMStoreFloat4x4(&mVSCBufferPerObjectData.WorldViewProjection, wvp);
			XMFLOAT4X4 textureTransformF4x4 = sprite->GetTextureTransform();
			XMMATRIX textureTransform = XMLoadFloat4x4(&textureTransformF4x4);
			XMStoreFloat4x4(&mVSCBufferPerObjectData.TextureTransform, XMMatrixTranspose(textureTransform));
			direct3DDeviceContext->UpdateSubresource(mVSCBufferPerObject.Get(), 0, nullptr, &mVSCBufferPerObjectData, 0, 0);

			direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);
		}
	}
}

void SpriteRenderer::Update(const DX::StepTimer& timer)
{
	for (auto spriteList : renderMap)
	{
		for (auto sprite : spriteList.second)
		{
			sprite->UpdateFrame(static_cast<long>(timer.GetElapsedSeconds() * 1000));
		}
	}
}

void SpriteRenderer::Register(Sprite* sprite, int32_t layer)
{
	// Assuming that each sprite calls only once. Not
	// checking for multiple registrations
	renderMap[(Layers) layer].push_back(sprite);
}

void SpriteRenderer::UnRegister(Sprite* sprite, int32_t layer)
{
	// Assuming that each sprite calls only once. Not
	// checking for multiple registrations
	renderMap[(Layers)layer].remove(sprite);
}