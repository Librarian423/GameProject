#pragma once
#include "SpriteObj.h"
#include "../Framework/Utils.h"

class VertexArrayObj;

class CreateMap
{
protected:
	int tileNum;
	int tileX;
	int tileY;
	Vector2i tile;

public:
	CreateMap();
	~CreateMap();

	void CreateTile();
};

