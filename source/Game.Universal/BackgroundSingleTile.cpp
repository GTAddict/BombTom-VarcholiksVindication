#include "pch.h"
#include "BackgroundSingleTile.h"
#include "SpriteRenderer.h"


BackgroundSingleTile::BackgroundSingleTile(int x, int y, int width, int height)
{
	Init("Content\\Textures\\board.png", x, y, width, height, 1, (int) Layers::background);
}


BackgroundSingleTile::~BackgroundSingleTile()
{
}
