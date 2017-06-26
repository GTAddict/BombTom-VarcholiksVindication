#include "pch.h"
#include "BackgroundSingleTile.h"
#include "SpriteRenderer.h"


BackgroundSingleTile::BackgroundSingleTile(int x, int y, int width, int height)
{
	Init("images\\board.png", x, y, width, height, (int) Layers::background);
}


BackgroundSingleTile::~BackgroundSingleTile()
{
}
