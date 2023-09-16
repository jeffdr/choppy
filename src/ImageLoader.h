#pragma once

#include "Texture.h"

namespace cc
{

struct ImageLoader
{
	static bool		loadTGA( const char* file, Texture& t );
};

}