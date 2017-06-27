#include "pch.h"
#include "SpriteCache.h"

using namespace DX;
using namespace DirectX;

SpriteCache::SpriteCache(const std::shared_ptr<DX::DeviceResources>& deviceResources)
	: GameComponent(deviceResources)
{
	for (std::uint8_t i = static_cast<std::uint8_t>(SpriteType::First); i < static_cast<std::uint8_t>(SpriteType::Count); ++i)
	{
		Load(Sprite::ToTextureName(static_cast<SpriteType>(i)));
	}
}


SpriteCache::~SpriteCache()
{
	for (auto pair : mSpriteMap)
	{
		pair.second.Reset();
	}
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SpriteCache::Load(const std::string& name)
{
	// If it doesn't exist, add it
	if (mSpriteMap.find(name) == mSpriteMap.end())
	{
		std::wstring wName(name.begin(), name.end());
		ThrowIfFailed(CreateWICTextureFromFile(mDeviceResources->GetD3DDevice(), wName.c_str(), nullptr, mSpriteMap[name].ReleaseAndGetAddressOf()));
	}
	
	return mSpriteMap[name];
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SpriteCache::GetTexture(const std::string& name)
{
	return mSpriteMap[name];
}