#pragma once

#include "Polygon.h"

namespace ye
{

struct Map
{
	enum
	{
		MAX_VERTICES = 1024*8,
		MAX_POLYGONS = 1024,
	};

	Vertex	vertices[MAX_VERTICES];
	Polygon	polygons[MAX_POLYGONS];
	//an editor might benefit from an "edge" type here, but the renderer really doesn't need it.

	Map();
};

}