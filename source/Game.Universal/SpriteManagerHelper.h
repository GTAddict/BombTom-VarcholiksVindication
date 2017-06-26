#pragma once

class SpriteManagerHelper
{
public:

	static SpriteManagerHelper* GetInstance();
	void Destroy();

	void SetCache(SpriteCache& spriteCache);
	void SetRenderer(SpriteRenderer& spriteRenderer);

	SpriteCache&	GetCache();
	SpriteRenderer& GetRenderer();

private:
	SpriteManagerHelper();
	~SpriteManagerHelper();

	static SpriteManagerHelper* s_mInstance;
	SpriteCache*				mpSpriteCache;
	SpriteRenderer*				mpSpriteRenderer;
};

