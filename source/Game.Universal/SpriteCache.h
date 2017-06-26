#pragma once
#include "DeviceResources.h"
#include <map>
#include <string>


class SpriteCache : public DX::GameComponent
{
public:
	
	SpriteCache(const std::shared_ptr<DX::DeviceResources>& deviceResources);
	~SpriteCache();

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	Load(const std::string& name);
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	GetTexture(const std::string& name);

private:
	std::map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> spriteMap;
};

