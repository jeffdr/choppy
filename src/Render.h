#pragma once

#include "Texture.h"

namespace ye
{

struct View;
struct Map;

struct	Render
{
public:
	Render();
	~Render();

	void	render( const View& view, const Map& map, Texture& disp ) const;

private:
	struct Region;
	struct SpriteRenderable;

	static void	renderRegion( const Region& region );

	Texture	testTexture;
};

}