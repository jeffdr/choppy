#pragma once

#include "Vertex.h"

namespace cc
{

struct	Polygon
{
	enum { MAX_EDGES = 8 };

	float	floorHeight, ceilHeight;
	short	edgeCount;
	short	vertices[MAX_EDGES];
	short	neighbors[MAX_EDGES];

	Polygon()
	{
		floorHeight = ceilHeight = 0.f;
		edgeCount = 0;
		for( int i=0; i<MAX_EDGES; ++i )
		{
			vertices[i] = -1;
			neighbors[i] = -1;
		}
	}

	bool	valid( void ) const
	{
		return edgeCount > 0;
	}
};

}