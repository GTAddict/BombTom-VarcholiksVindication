#pragma once

class SpriteManagerHelper
{
public:

	static SpriteManagerHelper* GetInstance();
	void Destroy();

	void SetCache(SpriteCache& spriteCache);
	void SetRenderer(SpriteRenderer& spriteRenderer);
	void SetDeviceResources(const std::shared_ptr<DX::DeviceResources>& deviceResources);

	SpriteCache&	GetCache();
	SpriteRenderer& GetRenderer();
	const std::shared_ptr<DX::DeviceResources>& GetDeviceResources();
	
private:
	SpriteManagerHelper();
	~SpriteManagerHelper();

	static SpriteManagerHelper*					s_mInstance;
	SpriteCache*								mpSpriteCache;
	SpriteRenderer*								mpSpriteRenderer;
	std::shared_ptr<DX::DeviceResources>		mDeviceResources;
};

