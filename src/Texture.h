#pragma once

#include <stdlib.h>

namespace ye
{

struct Texture
{
	unsigned short	width = 0, height = 0;
	unsigned short	channels = 3;
	unsigned char*	pixels = nullptr;

	~Texture()
	{
		free(pixels);
	}

	bool	resize( unsigned w, unsigned h, unsigned ch = 3 )
	{
		free(pixels);
		width = w; height = h;
		channels = ch;
		pixels = (unsigned char*)malloc( width*height*channels );
		return !!pixels;
	}

	unsigned char*	sample( float u, float v ) const
	{
		// For reasons of speed, this method assumes power of two dimensions.
		unsigned x = unsigned(u * float(width)) & (width-1);
		unsigned y = unsigned(v * float(height)) & (height-1);
		return pixels + channels*( y*width + x );
	}
};

}
