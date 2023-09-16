#pragma once

#include "matrix.h"

namespace ye
{

struct View
{
	matrix	transform;
	float	viewAngle = 80.f;
	short	polyIndex = 0;
};

}