#include "Render.h"
#include "View.h"
#include "Map.h"
#include "Texture.h"
#include "ImageLoader.h"
#include "MathTools.h"
#include <math.h>
#include <chrono>
#include <xmmintrin.h>

#define THREADED_RENDER

namespace cc
{

struct	Render::SpriteRenderable
{
	vec3	position;
	vec3	planeNormal;
	vec3	planeHoriz;
	float	invSize[2];
};

struct	Render::Region
{
	const View*			view;
	const Map*			map;
	const Texture*		display;

	unsigned			startRow, endRow;
	
	SpriteRenderable*	sprites;
	unsigned			spriteCount;

	const Texture*		testTexture;

	float				distortionPhase, distortionMag;
};

Render::Render()
{
	ImageLoader::loadTGA( "data/texture/sloth.tga", testTexture );
}

Render::~Render()
{}

void	Render::render( const View& view, const Map& map, Texture& disp ) const
{
	auto startTime = std::chrono::system_clock::now();
	static float sDistortionPhase = 0.f;

	const unsigned threadCount = 8;
	std::thread threads[threadCount];
	Region regions[threadCount];

	//hardcoded test sprites
	SpriteRenderable sprites[1];
	sprites[0].position = vec3(0,1,0);
	sprites[0].planeNormal = sprites[0].position - view.transform.getTranslation();
	sprites[0].planeNormal.y = 0.f;
	sprites[0].planeNormal = normalize( sprites[0].planeNormal );
	sprites[0].planeHoriz = cross( vec3(0,1,0), sprites[0].planeNormal );
	sprites[0].invSize[0] = 0.5f / 1.f;
	sprites[0].invSize[1] = 0.5f / 1.f;

	for( unsigned i=0; i<threadCount; ++i )
	{
		regions[i].view = &view;
		regions[i].map = &map;
		regions[i].display = &disp;
		regions[i].startRow = (disp.height * i) / threadCount;
		regions[i].endRow = (disp.height * (i+1)) / threadCount;
		regions[i].sprites = sprites;
		regions[i].spriteCount = sizeof(sprites)/sizeof(sprites[0]);
		regions[i].testTexture = &testTexture;

		regions[i].distortionMag = .01f;
		regions[i].distortionPhase = (sDistortionPhase += 1.f/120.f);

		#ifdef THREADED_RENDER
			threads[i] = std::thread( Render::renderRegion, regions[i] );
		#endif
	}

	for( unsigned i=0; i<threadCount; ++i )
	{
		#ifdef THREADED_RENDER
			threads[i].join();
		#else
			renderRegion( regions[i] );
		#endif
	}

	static int sFrameInterval = 0;
	if( sFrameInterval++ % 32 == 0 )
	{
		auto endTime = std::chrono::system_clock::now();
		auto us = std::chrono::duration_cast<std::chrono::microseconds>( endTime - startTime );
		printf( "%f ms render time\n", double(us.count()) / 1000.0 );
	}
}

void	Render::renderRegion( const Region& region )
{
	const View& view = *region.view;
	const Map& map = *region.map;
	const Texture& disp = *region.display;

	//projection setup
	float verticalSpan = sinf( view.viewAngle * 3.141592f / 180.f );
	float verticalSpan2 = verticalSpan * 2.f / float(disp.height);
	float horizontalSpan = verticalSpan * float(disp.width) / float(disp.height);
	float horizontalSpan2 = horizontalSpan * 2.f / float(disp.width);
	vec3 rayOrigin = view.transform.getTranslation();

	for( unsigned py=region.startRow; py<region.endRow; ++py )
	{
		unsigned char* pixel = disp.pixels + py*disp.width*disp.channels;

		for( unsigned px=0; px<disp.width; ++px )
		{
			float rayVert = -verticalSpan + float(py)*verticalSpan2;
			float rayHoriz = -horizontalSpan + float(px)*horizontalSpan2;

			if( region.distortionMag > 0.f )
			{
				//experiment: shroom distortion function
				rayVert += region.distortionMag * crudeCos( 9.f * rayVert + region.distortionPhase );
				rayHoriz += region.distortionMag * crudeCos( 7.f * rayHoriz + region.distortionPhase );
			}

			vec3 rayDir(
				//slightly faster explicit matrix mul for z=-1
				view.transform.f[0]*rayHoriz + (view.transform.f[4]*rayVert - view.transform.f[8]),
				view.transform.f[1]*rayHoriz + (view.transform.f[5]*rayVert - view.transform.f[9]),
				view.transform.f[2]*rayHoriz + (view.transform.f[6]*rayVert - view.transform.f[10])
			);
			{
				//normalize rayDir, using fast rsqrt
				float rayLen = rayDir.x*rayDir.x + rayDir.y*rayDir.y + rayDir.z*rayDir.z;
				_mm_store_ss( &rayLen, _mm_rsqrt_ss( _mm_load_ss( &rayLen ) ) );
				rayDir *= rayLen;
			}
			
			//trace ray
			vec3 hitPos;
			float t = 0.f;
			{
				float rayInvY = 1.f / rayDir.y;

				for( int polyIndex = view.polyIndex; polyIndex >= 0; )
				{
					//fetch poly
					const Polygon& poly = map.polygons[ polyIndex ];
					
					//early out if ray does not enter the polygon within its ceiling/floor bounds
					float entryHeight = rayOrigin.y + t * rayDir.y;
					if( entryHeight > poly.ceilHeight || entryHeight < poly.floorHeight )
					{ break; }

					//determine ceiling & floor intersections
					float floort = (poly.floorHeight - rayOrigin.y) * rayInvY;
					float ceilt = (poly.ceilHeight - rayOrigin.y) * rayInvY;
					if( floort > t )
					{ t = floort; }
					else if( ceilt > t )
					{ t = ceilt; }

					//determine wall intersections
					int wall = -1;
					for( int i=0; i<poly.edgeCount; ++i )
					{
						Vertex v0 = map.vertices[ poly.vertices[i] ];
						Vertex v1 = map.vertices[ poly.vertices[i+1 >= poly.edgeCount ? 0 : i+1] ];

						float side[2] = { v1.x-v0.x, v1.z-v0.z };
						float diff[2] = { rayOrigin.x - v0.x, rayOrigin.z - v0.z };
						float dirCrossSide = side[1]*rayDir.x - side[0]*rayDir.z;
						float sideCrossDiff = side[0]*diff[1] - side[1]*diff[0];
						float tw = sideCrossDiff / dirCrossSide;
						if( tw < t && dirCrossSide > 0.f )
						{
							t = tw;
							wall = i;
						}
					}

					//ray enters next polygon, if any
					polyIndex = -1;
					if( wall >= 0 )
					{ polyIndex = poly.neighbors[wall]; }
				}

				hitPos = rayOrigin + t * rayDir;
			}

			//set uv for wall texture
			float u = 0.5f*hitPos.x, v = 0.5f*hitPos.z;

			//sprite intersection test
			//TODO: early out on pre-sorted sprite list (helps w/ overlap?)
			for( unsigned s=0; s<region.spriteCount; ++s )
			{
				const SpriteRenderable& sprite = region.sprites[s];
				float d = dot( rayDir, sprite.planeNormal );
				if( d > 0.f )
				{
					float tt = dot( sprite.position - rayOrigin, sprite.planeNormal ) / d;
					if( tt >= 0.f && tt < t )
					{
						vec3 p = rayOrigin + tt * rayDir;
						vec3 spriteCoord = p - sprite.position;
						float uu = 0.5f + dot( spriteCoord, sprite.planeHoriz ) * sprite.invSize[0];
						float vv = 0.5f + spriteCoord.y * sprite.invSize[1];
						if( uu < 1.f && uu > 0.f && vv < 1.f && vv > 0.f )
						{
							hitPos = p;
							t = tt; u = uu; v = vv;
						}
					}
				}
			}

			//sample test texture
			const unsigned char* samp = region.testTexture->sample( u,v );

			//basic attenuated lighting placeholder
			unsigned atten = unsigned( (32.f*256.f) / (t*t) );
			atten = atten > 0xff ? 0xff : atten;
			pixel[0] = (unsigned char)( (atten * samp[0]) >> 8 );
			pixel[1] = (unsigned char)( (atten * samp[1]) >> 8 );
			pixel[2] = (unsigned char)( (atten * samp[2]) >> 8 );

			pixel += disp.channels;
		}
	}
}

}