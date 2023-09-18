#pragma once

namespace cc
{

inline
float	crudeCos( float x ) noexcept
{
	x *= 1.f/(2.f*3.14159256f);
	x -= 0.25f + float(int(x + 0.25f));
	float absx = x < 0.f ? -x : x;
	x *= 16.f * (absx - 0.5f);
	return x;
}

}