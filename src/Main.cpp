#include <windows.h>

#include "Map.h"
#include "Texture.h"
#include "DisplayWindow.h"
#include "Render.h"
#include "View.h"

using namespace ye;

Map		map;
View	view;
bool	movement[4] = {};

void	makeTestMap( Map& map )
{
	map.vertices[0] = { -10.f, -10.f };
	map.vertices[1] = { 10.f, -10.f };
	map.vertices[2] = { 10.f, 10.f };
	map.vertices[3] = { -10.f, 10.f };
	
	ye::Polygon& p = map.polygons[0];
	p.edgeCount = 4;
	p.vertices[0] = 0;
	p.vertices[1] = 1;
	p.vertices[2] = 2;
	p.vertices[3] = 3;
	p.floorHeight = 0.f;
	p.ceilHeight = 2.5f;
	p.neighbors[0] = 1;

	map.vertices[4] = { 0.f, -15.f };

	ye::Polygon& p2 = map.polygons[1];
	p2.edgeCount = 3;
	p2.vertices[0] = 1;
	p2.vertices[1] = 0;
	p2.vertices[2] = 4;
	p2.floorHeight = 0.25f;
	p2.ceilHeight = 1.5f;
}

void	setMovement( bool* d )
{
	if( d )
	{ memcpy( movement, d, sizeof(movement) ); }
}

void	move( void )
{
	float dx = (movement[2] ? -1.f : 0.f) + (movement[3] ? 1.f : 0.f);
	float dz = (movement[0] ? 1.f : 0.f) + (movement[1] ? -1.f : 0.f);

	float speed = 0.15f;
	vec3 right = view.transform.getBasis(0);
	vec3 forward = cross( vec3(0,1,0), right );
	view.transform.setTranslation( view.transform.getTranslation() + speed*(dx * right + dz * forward) );
}

void	look( float px, float py )
{
	//return; //dbg, disabled for bench

	static float mx = px, my = py;
	static float angles[2] = {};

	float dx = px - mx, dy = py - my;
	angles[0] -= dx * .5f;
	angles[1] -= dy * .5f;
	const float maxViewVert = 89.f;
	angles[1] = angles[1] < -maxViewVert ? -maxViewVert :
				angles[1] > maxViewVert ? maxViewVert :
				angles[1];

	matrix m;
	m.rotate( angles[0], 1 );
	m.rotate( angles[1], 0 );
	view.transform.setRotation( m );

	mx = px; my = py;
}

int		main( int argc, char** args )
{
	makeTestMap(map);

	view = {
		.transform = {
			1,0,0,0,
			0,1,0,1.8f,
			0,0,1,5.f,
			0.f, 0.f, 0.f, 1.f
		},
		.viewAngle = 80.f,
		.polyIndex = 0
	};

	unsigned w=1280, h=1024;
	Texture disp;
	disp.resize(w,h,3);
	DisplayWindow win( 100, 100, w, h );
	win.moveFunc = setMovement;
	win.lookFunc = look;
	
	Render render;

	while( 1 )
	{
		win.processEvents();
		move();
		render.render( view, map, disp );
		win.repaint( disp );
	}
	return 0;
}