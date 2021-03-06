#include "pch.h"
#include "SpriteManagerHelper.h"

SpriteManagerHelper* SpriteManagerHelper::s_mInstance = nullptr;

SpriteManagerHelper::SpriteManagerHelper()
{
}


SpriteManagerHelper::~SpriteManagerHelper()
{
}

SpriteManagerHelper* SpriteManagerHelper::GetInstance()
{
	if (s_mInstance == NULL)
	{
		s_mInstance = new SpriteManagerHelper();
	}

	return s_mInstance;
}

void SpriteManagerHelper::Destroy()
{
	if (s_mInstance)
	{
		delete s_mInstance;
		s_mInstance = NULL;
	}
}

void SpriteManagerHelper::SetCache(SpriteCache& spriteCache)
{
	mpSpriteCache = &spriteCache;
}

void SpriteManagerHelper::SetRenderer(SpriteRenderer& spriteRenderer)
{
	mpSpriteRenderer = &spriteRenderer;
}

void SpriteManagerHelper::SetDeviceResources(const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	mDeviceResources = deviceResources;
}

SpriteCache& SpriteManagerHelper::GetCache()
{
	return *mpSpriteCache;
}

SpriteRenderer& SpriteManagerHelper::GetRenderer()
{
	return *mpSpriteRenderer;
}

const std::shared_ptr<DX::DeviceResources>& SpriteManagerHelper::GetDeviceResources()
{
	return mDeviceResources;
}
