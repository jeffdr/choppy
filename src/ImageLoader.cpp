#include "ImageLoader.h"
#include <stdio.h>

namespace cc
{

namespace
{
	#pragma pack( push )
	#pragma pack( 1 )
	struct	TGAHeader
	{
		unsigned char	lengthOfID;
		unsigned char	colorMapType;
		unsigned char	imageType;
	
		unsigned short	colorMapOffset;
		unsigned short	colorMapLength;
		unsigned char	colorMapEntrySize;
	
		unsigned short	originX;
		unsigned short	originY;
		unsigned short	width;
		unsigned short	height;
		unsigned char	bitsPerPixel;
		unsigned char	descriptor;
	};
	#pragma pack(pop)
}

bool	ImageLoader::loadTGA( const char* file, Texture& t )
{
	bool ok = false;

	FILE* f = nullptr;
	fopen_s( &f, file, "rb" );
	if( f )
	{
		TGAHeader header = {};
		fread( &header, sizeof(header), 1, f );

		t.resize( header.width, header.height, 4 );
		
		if( header.bitsPerPixel == 24 )
		{
			//uncompressed: BGR -> RGBA
			unsigned pixels = ((unsigned)t.width)*t.height;
			fread( t.pixels, 1, 3*pixels, f );

			unsigned char* itr = t.pixels + 3*(pixels-1);
			unsigned char* dst = t.pixels + 4*(pixels-1);
			while( dst >= t.pixels )
			{
				unsigned char b = itr[0], g = itr[1], r = itr[2];
				dst[0] = r;	dst[1] = g; dst[2] = b;
				dst[3] = 0xFF;
				dst -= 4; itr -= 3;
			}
			ok = true;
		}
		else if( header.bitsPerPixel == 32 )
		{
			//uncompressed: BGRA -> RGBA
			unsigned pixels = ((unsigned)t.width)*t.height;
			fread( t.pixels, 1, 4*pixels, f );

			unsigned char* itr = t.pixels + 4*(pixels-1);
			while( itr >= t.pixels )
			{
				unsigned char c = itr[0];
				itr[0] = itr[2];
				itr[2] = c;
				itr -= 4;
			}
			ok = true;
		}

		fclose(f);
	}

	return ok;
}

}