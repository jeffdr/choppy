#pragma once

#include "Texture.h"

namespace ye
{

struct ImageLoader
{
	static bool		loadTGA( const char* file, Texture& t );
};

}