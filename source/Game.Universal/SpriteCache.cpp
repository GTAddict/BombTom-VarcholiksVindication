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
	// If it doesn't exist, add it
	if (spriteMap.find(name) == spriteMap.end())
	{
		std::wstring wName(name.begin(), name.end());
		ThrowIfFailed(CreateWICTextureFromFile(mDeviceResources->GetD3DDevice(), wName.c_str(), nullptr, spriteMap[name].ReleaseAndGetAddressOf()));
	}
	
	return spriteMap[name];
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SpriteCache::GetTexture(const std::string& name)
{
	return spriteMap[name];
}