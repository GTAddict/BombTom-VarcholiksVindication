#pragma once

#include <list>

class BackgroundSingleTile;

class Background
{
public:
	Background(int width, int height);
	~Background();

private:
	std::list<BackgroundSingleTile*> mTileCollection;
};

