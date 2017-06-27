#include "pch.h"
#include "BackgroundSingleTile.h"
#include "SpriteRenderer.h"


BackgroundSingleTile::BackgroundSingleTile(int x, int y, int width, int height)
{
	Init(SpriteType::Board, x, y, width, height, 1, (int) Layers::background);
}


BackgroundSingleTile::~BackgroundSingleTile()
{
}
