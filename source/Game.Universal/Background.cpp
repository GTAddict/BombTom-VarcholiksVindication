#include "pch.h"
#include "Background.h"
#include "BackgroundSingleTile.h"


Background::Background(int width, int height)
{
	int tileWidth = 400;
	int tileHeight = 310;
	int currentX = tileWidth / 2;
	int currentY = tileHeight / 2;

	for (currentX = tileWidth / 2; currentX - tileWidth / 2 <= width; currentX += tileWidth)
	{
		for (currentY = tileHeight / 2; currentY - tileHeight / 2 <= height; currentY += tileHeight)
		{
			BackgroundSingleTile* tile = new BackgroundSingleTile(currentX, currentY, tileWidth, tileHeight);
			mTileCollection.push_back(tile);
		}
	} 
}


Background::~Background()
{
	for each (BackgroundSingleTile* tile in mTileCollection)
	{
		delete tile;
	}
}
