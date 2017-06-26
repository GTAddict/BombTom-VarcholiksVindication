#include "pch.h"
#include "SpriteCache.h"

using namespace DX;
using namespace DirectX;

SpriteCache::SpriteCache(const std::shared_ptr<DX::DeviceResources>& deviceResources)
	: GameComponent(deviceResources)
{
}


SpriteCache::~SpriteCache()
{
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SpriteCache::Load(const std::string& name)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sprite;

	// If it exists already
	if (spriteMap.find(name) != spriteMap.end())
	{
		sprite = spriteMap[name];
	}
	else // Otherwise, add it
	{
		std::wstring wName(name.begin(), name.end());
		ThrowIfFailed(CreateWICTextureFromFile(mDeviceResources->GetD3DDevice(), wName.c_str(), nullptr, sprite.ReleaseAndGetAddressOf()));
		spriteMap[name] = sprite;
	}

	return sprite;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SpriteCache::GetTexture(const std::string& name)
{
	return spriteMap[name];
}