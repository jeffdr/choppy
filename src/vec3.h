#pragma once

#include <math.h>

namespace cc
{

struct	vec3
{
	float	x,y,z;

	vec3() : x(0), y(0), z(0)
	{}

	vec3( float s ) : x(s), y(s), z(s)
	{}

	vec3( const vec3& v ) : x(v.x), y(v.y), z(v.z)
	{}

	vec3( float xval, float yval, float zval=0.f ) : x(xval), y(yval), z(zval)
	{}

	template< unsigned tArrayLen >
	vec3( const float (&array)[tArrayLen] )
	{
		x = tArrayLen > 0 ? array[0] : 0.f;
		y = tArrayLen > 1 ? array[1] : 0.f;
		z = tArrayLen > 2 ? array[2] : 0.f;
	}

	explicit
	vec3( const float* val3f ) : x(val3f[0]), y(val3f[1]), z(val3f[2])
	{}

	float*			data( void )
	{ return &x; }

	const float*	data( void ) const
	{ return &x; }

	float&			operator[]( int index )
	{ return (&x)[index]; }

	const float&	operator[]( int index ) const
	{ return (&x)[index]; }

	vec3			operator-( void ) const
	{ return vec3( -x, -y, -z ); }

	vec3&			operator+=( const vec3& v )
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	vec3&			operator-=( const vec3& v )
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	vec3&			operator*=( const vec3& v )
	{
		x *= v.x; y *= v.y; z *= v.z;
		return *this;
	}

	vec3&			operator/=( const vec3& v )
	{
		x /= v.x; y /= v.y; z /= v.z;
		return *this;
	}

	vec3&			operator*=( float v )
	{
		x *= v; y *= v; z *= v;
		return *this;
	}

	vec3&			operator/=( float v )
	{
		x /= v; y /= v; z /= v;
		return *this;
	}
};

inline bool		operator==( const vec3& a, const vec3& b )
{ return a.x == b.x && a.y == b.y && a.z == b.z; }

inline bool		operator!=( const vec3& a, const vec3& b )
{ return a.x != b.x || a.y != b.y || a.z != b.z; }

inline vec3		operator+( const vec3& a, const vec3& b )
{ return vec3( a.x+b.x, a.y+b.y, a.z+b.z ); }

inline vec3		operator-( const vec3& a, const vec3& b )
{ return vec3( a.x-b.x, a.y-b.y, a.z-b.z ); }

inline vec3		operator*( const vec3& a, const vec3& b )
{ return vec3( a.x*b.x, a.y*b.y, a.z*b.z ); }

inline vec3		operator/( const vec3& a, const vec3& b )
{ return vec3( a.x/b.x, a.y/b.y, a.z/b.z ); }

inline float	dot( const vec3& a, const vec3& b )
{ return a.x*b.x + a.y*b.y + a.z*b.z; }

inline vec3		cross( const vec3& a, const vec3& b )
{
	return vec3(	a.y*b.z - a.z*b.y,
					a.z*b.x - a.x*b.z,
					a.x*b.y - a.y*b.x	);
}

inline vec3		normalize( const vec3& v )
{
	float len = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
	len = (len == 0.f) ? 1.f : (1.f/len);
	return vec3( v.x*len, v.y*len, v.z*len );
}

inline float	length( const vec3& v )
{ return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z); }

inline float	lengthSquared( const vec3& v )
{ return v.x*v.x + v.y*v.y + v.z*v.z; }

static_assert( sizeof(vec3) == 3*sizeof(float), "incorrect size of vec3 type" );

}
