#pragma once

#include "vec3.h"

namespace ye
{

struct	matrix
{
	float	f[16];
	
						matrix( void )
	{
		f[0] = f[5] = f[10] = f[15] = 1.0f;
		f[4] = f[8] = f[12] = 0.0f;
		f[1] = f[9] = f[13] = 0.0f;
		f[2] = f[6] = f[14] = 0.0f;
		f[3] = f[7] = f[11] = 0.0f;
	}
	
						matrix( const matrix& other )
	{
		for( int i=0; i<16; ++i )
		{ f[i] = other.f[i]; }
	}
	
						matrix( const float* m )
	{
		for( int i=0; i<16; ++i )
		{ f[i] = m[i]; }
	}
	
						matrix(	float c0r0, float c1r0, float c2r0, float c3r0,
								float c0r1, float c1r1, float c2r1, float c3r1,
								float c0r2, float c1r2, float c2r2, float c3r2,
								float c0r3, float c1r3, float c2r3, float c3r3 )
	{
		f[0] = c0r0; f[4] = c1r0; f[8]  = c2r0; f[12] = c3r0;
		f[1] = c0r1; f[5] = c1r1; f[9]  = c2r1; f[13] = c3r1;
		f[2] = c0r2; f[6] = c1r2; f[10] = c2r2; f[14] = c3r2;
		f[3] = c0r3; f[7] = c1r3; f[11] = c2r3; f[15] = c3r3;
	}

	float*				data( void )
	{
		return f;
	}
	
	const float*		data( void ) const
	{
		return f;
	}
	
	float*				operator[]( int col )
	{
		return f + 4*col;
	}
	
	const float*		operator[]( int col ) const
	{
		return f + 4*col;
	}

	vec3				getBasis( unsigned axis ) const
	{
		return vec3( f[4*axis+0], f[4*axis+1], f[4*axis+2] );
	}

	void				setBasis( unsigned axis, float x, float y, float z, float w=0.f )
	{
		f[4*axis  ] = x;
		f[4*axis+1] = y;
		f[4*axis+2] = z;
		f[4*axis+3] = w;
	}

	vec3				getTranslation( void ) const
	{
		return vec3( f[12], f[13], f[14] );
	}
	
	void				transpose( void )
	{
		float	t;
		t = f[1]; f[1] = f[4]; f[4] = t;
		t = f[2]; f[2] = f[8]; f[8] = t;
		t = f[6]; f[6] = f[9]; f[9] = t;
		t = f[3]; f[3] = f[12]; f[12] = t;
		t = f[7]; f[7] = f[13]; f[13] = t;
		t = f[11]; f[11] = f[14]; f[14] = t;
	}
	
	friend matrix		operator+( const matrix& m1, const matrix& m2 )
	{
		matrix m3(m1); m3 += m2; return m3;
	}
	
	matrix&				operator+=( const matrix& m )
	{
		f[0] += m.f[0]; f[1] += m.f[1];
		f[2] += m.f[2]; f[3] += m.f[3];
		f[4] += m.f[4]; f[5] += m.f[5];
		f[6] += m.f[6]; f[7] += m.f[7];
		f[8] += m.f[8]; f[9] += m.f[9];
		f[10] += m.f[10]; f[11] += m.f[11];
		f[12] += m.f[12]; f[13] += m.f[13];
		f[14] += m.f[14]; f[15] += m.f[15];
		return *this;
	}
	
	friend matrix		operator-( const matrix& m1, const matrix& m2 )
	{
		matrix m3(m1); m3 -= m2; return m3;
	}
	
	matrix&				operator-=( const matrix& m )
	{
		f[0] -= m.f[0]; f[1] -= m.f[1];
		f[2] -= m.f[2]; f[3] -= m.f[3];
		f[4] -= m.f[4]; f[5] -= m.f[5];
		f[6] -= m.f[6]; f[7] -= m.f[7];
		f[8] -= m.f[8]; f[9] -= m.f[9];
		f[10] -= m.f[10]; f[11] -= m.f[11];
		f[12] -= m.f[12]; f[13] -= m.f[13];
		f[14] -= m.f[14]; f[15] -= m.f[15];
		return *this;
	}
	
	matrix&				mul( float s )
	{
		for( int i=0; i<16; i+=4 )
		{
			f[i  ] *= s;
			f[i+1] *= s;
			f[i+2] *= s;
			f[i+3] *= s;
		}
		return *this;
	}
	
	friend matrix		operator*( const matrix& m, float s )
	{
		matrix m2(m); m2.mul(s); return m2;
	}
	
	friend matrix		operator*( float s, const matrix& m )
	{
		matrix m2(m); m2.mul(s); return m2;
	}
	
	matrix&				operator*=( float s )
	{
		mul(s); return *this;
	}
	
	void				translate( float x, float y, float z )
	{
		f[12] += f[0]*x + f[4]*y + f[8]*z;
		f[13] += f[1]*x + f[5]*y + f[9]*z;
		f[14] += f[2]*x + f[6]*y + f[10]*z;
		f[15] += f[3]*x + f[7]*y + f[11]*z;
	}

	void				translate( const vec3& dir )
	{
		translate( dir.x, dir.y, dir.z );
	}
	
	void				setTranslation( float x, float y, float z )
	{
		f[12] = x; f[13] = y; f[14] = z;
	}

	void				setTranslation( const vec3& pos )
	{
		setTranslation( pos[0], pos[1], pos[2] );
	}
	
	void				scale( float dx, float dy, float dz )
	{
		scale( vec3(dx,dy,dz) );
	}

	void				scale( const vec3& s )
	{
		f[0] *= s.x;	f[4] *= s.y;	f[8] *= s.z;
		f[1] *= s.x;	f[5] *= s.y;	f[9] *= s.z;
		f[2] *= s.x;	f[6] *= s.y;	f[10] *= s.z;
		f[3] *= s.x;	f[7] *= s.y;	f[11] *= s.z;
	}
	
	void				setScale( float sx, float sy, float sz )
	{
		vec3 bx = ye::normalize( getBasis(0) ); bx *= sx; setBasis(0, bx.x, bx.y, bx.z);
		vec3 by = ye::normalize( getBasis(1) ); by *= sy; setBasis(1, by.x, by.y, by.z);
		vec3 bz = ye::normalize( getBasis(2) ); bz *= sz; setBasis(2, bz.x, bz.y, bz.z);
	}

	void				rotate( float angle_deg, unsigned axis_basis )
	{
		matrix m;
		m.setRotation( angle_deg, axis_basis );
		*this = this->mul44( m );
	}
	
	void				rotate( float angle_deg, float x, float y, float z )
	{
		rotate( angle_deg, vec3(x,y,z) );
	}
	
	void				rotate( float angle_deg, const vec3& axis )
	{
		const vec3 a = ye::normalize( axis );
		const float	s = sinf( angle_deg * 0.0174532925f );
		const float	c = cosf( angle_deg * 0.0174532925f );
		const float	xx = a.x * a.x;
		const float	yy = a.y * a.y;
		const float	zz = a.z * a.z;
		const float	xy = a.x * a.y;
		const float	yz = a.y * a.z;
		const float	zx = a.z * a.x;
		const float	xs = a.x * s;
		const float	ys = a.y * s;
		const float	zs = a.z * s;
		const float	one_c = 1.0f - c;
		matrix	rm;
		rm[0][0] = (one_c * xx) + c;
		rm[1][0] = (one_c * xy) - zs;
		rm[2][0] = (one_c * zx) + ys;
		
		rm[0][1] = (one_c * xy) + zs;
		rm[1][1] = (one_c * yy) + c;
		rm[2][1] = (one_c * yz) - xs;
		
		rm[0][2] = (one_c * zx) - ys;
		rm[1][2] = (one_c * yz) + xs;
		rm[2][2] = (one_c * zz) + c;
		*this = this->mul44( rm );
	}

	void				setRotation( const matrix& m )
	{
		f[0] = m.f[0]; f[4] = m.f[4]; f[8]  = m.f[8];
		f[1] = m.f[1]; f[5] = m.f[5]; f[9]  = m.f[9];
		f[2] = m.f[2]; f[6] = m.f[6]; f[10] = m.f[10];
	}
	
	void				setRotation( float angle_deg, float x, float y, float z )
	{
		matrix m;
		m.rotate( angle_deg, vec3(x,y,z) );
		setRotation( m );
	}
	
	void				setRotation( float angle_deg, const vec3& axis )
	{
		matrix m;
		m.rotate( angle_deg, axis );
		setRotation( m );
	}
	
	void				setRotation( float angle_deg, unsigned axis_basis )
	{
		float sn = sinf( angle_deg * 0.0174532925f );
		float cs = cosf( angle_deg * 0.0174532925f );
		switch( axis_basis )
		{
		case 0:		*this = matrix(	1, 0,   0, f[12],
									0, cs,-sn, f[13],
									0, sn, cs, f[14],
									f[3],f[7],f[11],f[15] );
					break;
					
		case 1:		*this = matrix(	cs,  0,	sn, f[12],
									0,	 1,	0,  f[13],
									-sn, 0,	cs, f[14],
									f[3],f[7],f[11],f[15] );
					break;

		case 2:		*this = matrix(	cs,-sn,	0, f[12],
									sn, cs,	0, f[13],
									0,	0,	1, f[14],
									f[3],f[7],f[11],f[15] );
					break;
		}
	}
	
	float				getRotationAboutAxis( unsigned axis ) const
	{
		unsigned otheraxisindex = (axis == 1 ? 0 : 1);
		
		vec3 a = getBasis(otheraxisindex);
		a[axis] = 0.f;
		a = ye::normalize(a);

		float angle_radians;
		float dp = a[otheraxisindex];
		
		vec3 otheraxis; otheraxis[otheraxisindex] = 1.f;
		float cp = cross( a, otheraxis )[axis];
		if( cp < 0.f )
		{
			angle_radians = acosf( dp );
		}
		else
		{
			angle_radians = 6.28318530718f - acosf(dp);
		}
		
		return angle_radians;
	}

	void				getEulerAngles( float* xyzRadians ) const
	{
		//X-Y-Z rotation order
		const matrix& m = *this;
		const float pi = 3.14159265358979323846f;
		if( fabsf(m[0][2]) < 1.f )
		{
			//two possible values for y rotation
			float y[2];
			y[0] = -asinf( m[0][2] );
			y[1] = pi - y[0];
		
			//two possible values for x rotation
			float x[2];
			x[0] = atan2f( m[1][2], m[2][2] );
			x[1] = x[0] - pi; //atan2f( -m[1][2], -m[2][2] );

			//two possible values for z rotation
			float z[2];
			z[0] = atan2f( m[0][1], m[0][0] );
			z[1] = z[0] - pi; //atan2f( -m[0][1], -m[0][0] );

			//we pick between the two solutions like so because it
			//provides a nice continuous rotation about the y axis. -jdr
			unsigned chosen = (x[0] >= pi && z[0] >= pi) ? 1 : 0;
			xyzRadians[0] = x[chosen];
			xyzRadians[1] = y[chosen];
			xyzRadians[2] = z[chosen];
		}
		else
		{
			if( m[0][2] < 0.f )
			{
				xyzRadians[1] = pi * 0.5f;
				xyzRadians[0] = atan2f( m[1][0], m[2][0] );
			}
			else
			{
				xyzRadians[1] = -pi * 0.5f;
				xyzRadians[0] = atan2f( -m[1][0], -m[2][0] );
			}
			xyzRadians[2] = 0.f;
		}
	}

	void				getEulerAnglesYXZ( float* xyzRadians ) const
	{
		//Y-X-Z rotation order
		const matrix& m = *this;
		const float pi = 3.14159265358979323846f;
		if( fabsf(m[2][1]) < 1.f )
		{
			//two possible values for x rotation
			xyzRadians[0] = asinf( -m[2][1] );

			//two possible values for y rotation
			xyzRadians[1] = atan2f( m[2][0], m[2][2] );

			//two possible values for z rotation
			xyzRadians[2] = atan2f( m[0][1], m[1][1] );
		}
		else
		{
			if( m[2][1] < 0.f )
			{
				xyzRadians[0] = pi * 0.5f;
				xyzRadians[1] = atan2f( m[1][0], m[0][0] );
			}
			else
			{
				xyzRadians[0] = -pi * 0.5f;
				xyzRadians[1] = atan2f( -m[1][0], m[0][0] );
			}
			xyzRadians[2] = 0.f;
		}
	}

	vec3				mulPoint( const vec3& right ) const
	{
		const float* m = f;
		float x = right.x, y = right.y, z = right.z;
		return vec3(	m[0]*x + m[4]*y + m[8]*z + m[12],
						m[1]*x + m[5]*y + m[9]*z + m[13],
						m[2]*x + m[6]*y + m[10]*z + m[14]	);
	}
	
	vec3				mulVector( const vec3& right ) const
	{
		const float* m = f;
		float x = right.x, y = right.y, z = right.z;
		return vec3(	m[0]*x + m[4]*y + m[8]*z,
						m[1]*x + m[5]*y + m[9]*z,
						m[2]*x + m[6]*y + m[10]*z	);
	}

	matrix				operator*( const matrix& right ) const
	{
		return mul44( right );
	}
	
	matrix				mul44( const matrix& right ) const
	{
		const float*	A = f;
		const float*	B = right.f;
		matrix			c;
		float*			C = c.f;
		
		{
			float b0 = B[0]; float b1 = B[1]; float b2 = B[2]; float b3 = B[3];
			C[0] = A[0]*b0 + A[4]*b1 + A[8]*b2 + A[12]*b3;
			C[1] = A[1]*b0 + A[5]*b1 + A[9]*b2 + A[13]*b3;
			C[2] = A[2]*b0 + A[6]*b1 + A[10]*b2 + A[14]*b3;
			C[3] = A[3]*b0 + A[7]*b1 + A[11]*b2 + A[15]*b3;
		}
		
		{
			float b4 = B[4]; float b5 = B[5]; float b6 = B[6]; float b7 = B[7];
			C[4] = A[0]*b4 + A[4]*b5 + A[8]*b6 + A[12]*b7;
			C[5] = A[1]*b4 + A[5]*b5 + A[9]*b6 + A[13]*b7;
			C[6] = A[2]*b4 + A[6]*b5 + A[10]*b6 + A[14]*b7;
			C[7] = A[3]*b4 + A[7]*b5 + A[11]*b6 + A[15]*b7;
		}
		
		{
			float b8 = B[8]; float b9 = B[9]; float b10 = B[10]; float b11 = B[11];
			C[8] = A[0]*b8 + A[4]*b9 + A[8]*b10 + A[12]*b11;
			C[9] = A[1]*b8 + A[5]*b9 + A[9]*b10 + A[13]*b11;
			C[10] = A[2]*b8 + A[6]*b9 + A[10]*b10 + A[14]*b11;
			C[11] = A[3]*b8 + A[7]*b9 + A[11]*b10 + A[15]*b11;
		}
		
		{
			float b12 = B[12]; float b13 = B[13]; float b14 = B[14]; float b15 = B[15];
			C[12] = A[0]*b12 + A[4]*b13 + A[8]*b14 + A[12]*b15;
			C[13] = A[1]*b12 + A[5]*b13 + A[9]*b14 + A[13]*b15;
			C[14] = A[2]*b12 + A[6]*b13 + A[10]*b14 + A[14]*b15;
			C[15] = A[3]*b12 + A[7]*b13 + A[11]*b14 + A[15]*b15;
		}
		
		return c;
	}
	
	matrix				mul34( const matrix& right ) const
	{
		/*
			A and B must be regular transforms, for example:
				x  x  x  x
				x  x  x  x
				x  x  x  x
				0  0  0  1
			
			The product will also be of the same form.
			
			Almost twice as fast as regular multiply:
				- 36 multiplies instead of 64
				- 28 adds instead of 48
		*/
		
		const float* 	A = f;
		const float* 	B = right.f;
		matrix			c;
		float* 			C = c.f;
		
		{
			float b0 = B[0]; float b1 = B[1]; float b2 = B[2];
			C[0] = A[0]*b0 + A[4]*b1 + A[8]*b2;
			C[1] = A[1]*b0 + A[5]*b1 + A[9]*b2;
			C[2] = A[2]*b0 + A[6]*b1 + A[10]*b2;
		}
		
		{
			float b4 = B[4]; float b5 = B[5]; float b6 = B[6];
			C[4] = A[0]*b4 + A[4]*b5 + A[8]*b6;
			C[5] = A[1]*b4 + A[5]*b5 + A[9]*b6;
			C[6] = A[2]*b4 + A[6]*b5 + A[10]*b6;
		}
		
		{
			float b8 = B[8]; float b9 = B[9]; float b10 = B[10];
			C[8] = A[0]*b8 + A[4]*b9 + A[8]*b10;
			C[9] = A[1]*b8 + A[5]*b9 + A[9]*b10;
			C[10] = A[2]*b8 + A[6]*b9 + A[10]*b10;
		}
		
		{
			float b12 = B[12]; float b13 = B[13]; float b14 = B[14];
			C[12] = A[0]*b12 + A[4]*b13 + A[8]*b14 + A[12];
			C[13] = A[1]*b12 + A[5]*b13 + A[9]*b14 + A[13];
			C[14] = A[2]*b12 + A[6]*b13 + A[10]*b14 + A[14];
		}

		C[3] = C[7] = C[11] = 0.0f; C[15] = 1.0f;
		return c;
	}
	
	matrix				mul33( const matrix& right ) const
	{
		/*
			A and B must be regular rotation transforms, for example:
				x  x  x  0
				x  x  x  0
				x  x  x  0
				0  0  0  1
			
			The product will also be of the same form.
			
			Almost 3x as fast as regular multiply:
				- 27 multiplies instead of 64
				- 18 adds instead of 48
		*/
		
		const float*	A = f;
		const float*	B = right.f;
		matrix			c;
		float*			C = c.f;
		
		C[0] = A[0]*B[0] + A[4]*B[1] + A[8]*B[2];
		C[1] = A[1]*B[0] + A[5]*B[1] + A[9]*B[2];
		C[2] = A[2]*B[0] + A[6]*B[1] + A[10]*B[2];
		
		C[4] = A[0]*B[4] + A[4]*B[5] + A[8]*B[6];
		C[5] = A[1]*B[4] + A[5]*B[5] + A[9]*B[6];
		C[6] = A[2]*B[4] + A[6]*B[5] + A[10]*B[6];
		
		C[8] = A[0]*B[8] + A[4]*B[9] + A[8]*B[10];
		C[9] = A[1]*B[8] + A[5]*B[9] + A[9]*B[10];
		C[10] = A[2]*B[8] + A[6]*B[9] + A[10]*B[10];
	
		C[12] = C[13] = C[14] = 0.0f;
		C[3] = C[7] = C[11] = 0.0f; C[15] = 1.0f;
	
		return c;
	}

	matrix				getInverseTranspose( void ) const
	{
		matrix m( *this );
		m.invert();
		m.transpose();
		return m;
	}

	matrix				getInverse( void ) const
	{
		matrix m( *this );
		m.invert();
		return m;
	}

	bool				invert( void )
	{
		//src is transpose of this matrix
		const double src[16] = {
			f[0], f[4], f[8], f[12],
			f[1], f[5], f[9], f[13],
			f[2], f[6], f[10],f[14],
			f[3], f[7], f[11],f[15]
		};
		double	dst[16];
		double	tmp[12];
		
		//calculate pairs for first 8 elements (cofactors)
		tmp[0]  = src[10] * src[15];
		tmp[1]  = src[11] * src[14];
		tmp[2]  = src[9]  * src[15];
		tmp[3]  = src[11] * src[13];
		tmp[4]  = src[9]  * src[14];
		tmp[5]  = src[10] * src[13];
		tmp[6]  = src[8]  * src[15];
		tmp[7]  = src[11] * src[12];
		tmp[8]  = src[8]  * src[14];
		tmp[9]  = src[10] * src[12];
		tmp[10] = src[8]  * src[13];
		tmp[11] = src[9]  * src[12];
		
		//calculate first 8 elements (cofactors)
		dst[0]  = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
		dst[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
		dst[1]  = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
		dst[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
		dst[2]  = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
		dst[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
		dst[3]  = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
		dst[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
		
		//calculate determinant; is the matrix invertible?
		double det = src[0]*dst[0]+src[1]*dst[1]+src[2]*dst[2]+src[3]*dst[3];
		if( det != 0.0f )
		{
			//continue calculating first 8 elements
			dst[4]  = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
			dst[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
			dst[5]  = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
			dst[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
			dst[6]  = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
			dst[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
			dst[7]  = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
			dst[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
			
			//calculate pairs for second 8 elements (cofactors)
			tmp[0]  = src[2]*src[7];	tmp[1]  = src[3]*src[6];	tmp[2]  = src[1]*src[7];
			tmp[3]  = src[3]*src[5];	tmp[4]  = src[1]*src[6];	tmp[5]  = src[2]*src[5];
			tmp[6]  = src[0]*src[7];	tmp[7]  = src[3]*src[4];	tmp[8]  = src[0]*src[6];
			tmp[9]  = src[2]*src[4];	tmp[10] = src[0]*src[5];	tmp[11] = src[1]*src[4];
			
			//calculate second 8 elements (cofactors)
			dst[8]  = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
			dst[8] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
			dst[9]  = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
			dst[9] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
			dst[10] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
			dst[10]-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
			dst[11] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
			dst[11]-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
			dst[12] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
			dst[12]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
			dst[13] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
			dst[13]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
			dst[14] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
			dst[14]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
			dst[15] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
			dst[15]-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
			
			//calculate matrix inverse
			det = 1.0f/det;
			for( int j=0; j<16; j++ )
			{
				f[j] = float( dst[j] * det );
			}
			
			return true;
		}
		
		return false;
	}

	matrix				getInverseAffineTranspose( void ) const
	{
		matrix m( *this );
		m.invertAffine();
		m.transpose();
		return m;
	}

	matrix				getInverseAffine( void ) const
	{
		matrix m( *this );
		m.invertAffine();
		return m;
	}

	bool				invertAffine( void )
	{
		/*
			This is a speedier inversion, for affine matrices.
			Basically it ignores the bottom row:

				x  x  x  x
				x  x  x  x
				x  x  x  x
				0  0  0  1

			This is an algebraic reduction of matrix::invert(), and
			runs about 2x faster. It will produce identical results
			provided the matrix is in the correct form.
		*/

		//src is transpose of this matrix
		const double src[12] = {
			f[0], f[4], f[8], f[12],
			f[1], f[5], f[9], f[13],
			f[2], f[6], f[10],f[14]
		};
		double	dst[16];
		
		dst[0] = src[10]*src[5] - src[9]*src[6];
		dst[1] = src[8]*src[6] - src[10]*src[4];
		dst[2] = src[9]*src[4] - src[8]*src[5];
		
		//calculate determinant; is the matrix invertible?
		double det = src[0]*dst[0] + src[1]*dst[1] + src[2]*dst[2];
		if( det != 0.0 )
		{
			dst[4] = src[9]*src[2] - src[10]*src[1];
			dst[5] = src[10]*src[0] - src[8]*src[2];
			dst[6] = src[8]*src[1] - src[9]*src[0];
			
			double	tmp[12];
			tmp[0]  = src[2]*src[7];	tmp[1]  = src[3]*src[6];	tmp[2]  = src[1]*src[7];
			tmp[3]  = src[3]*src[5];	tmp[4]  = src[1]*src[6];	tmp[5]  = src[2]*src[5];
			tmp[6]  = src[0]*src[7];	tmp[7]  = src[3]*src[4];	tmp[8]  = src[0]*src[6];
			tmp[9]  = src[2]*src[4];	tmp[10] = src[0]*src[5];	tmp[11] = src[1]*src[4];
			
			dst[8] = tmp[4] - tmp[5];
			dst[9] = tmp[9] - tmp[8];
			dst[10] = tmp[10] - tmp[11];

			dst[12] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
			dst[12]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
			dst[13] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
			dst[13]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
			dst[14] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
			dst[14]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
			
			det = 1.0/det;

			f[0] = float( dst[0] * det );
			f[1] = float( dst[1] * det );
			f[2] = float( dst[2] * det );
			f[3] = 0.f;

			f[4] = float( dst[4] * det );
			f[5] = float( dst[5] * det );
			f[6] = float( dst[6] * det );
			f[7] = 0.f;

			f[ 8] = float( dst[8] * det );
			f[ 9] = float( dst[9] * det );
			f[10] = float( dst[10] * det );
			f[11] = 0.f;

			f[12] = float( dst[12] * det );
			f[13] = float( dst[13] * det );
			f[14] = float( dst[14] * det );
			f[15] = 1.f;
			
			return true;
		}
		
		return false;
	}
	
	void				normalizeBasis( unsigned basis )
	{
		float c = 1.0f / sqrtf(	f[ basis*4 ] * f[ basis*4 ] +
								f[ basis*4+1 ]*f[ basis*4+1 ] +
								f[ basis*4+2 ]*f[ basis*4+2 ] );
		f[basis*4] *= c;
		f[basis*4+1] *= c;
		f[basis*4+2] *= c;
	}

	void				normalize( void )
	{
		normalizeBasis(0);
		normalizeBasis(1);
		normalizeBasis(2);
	}

	void				orthoNormalize( void )
	{
		vec3 x( f );
		vec3 y( f+4 );
		vec3 z( f+8 );
		
		y = ye::normalize(y);
		x = x - dot(x,y)*y;
		x = ye::normalize(x);
		z = z - dot(z,x)*x - dot(z,y)*y;
		z = ye::normalize(z);

		f[0] = x[0];
		f[1] = x[1];
		f[2] = x[2];

		f[4] = y[0];
		f[5] = y[1];
		f[6] = y[2];

		f[8] = z[0];
		f[9] = z[1];
		f[10]= z[2];
	}

	void				lookAt( vec3 target )
	{
		setRotation(0, 0);
		float xd = target.x - f[12];
		float yd = target.y - f[13];
		float zd = target.z - f[14];
		rotate((float)(atan2(xd, zd)*57.295779513082320876846364344191) + 180, 1);
		rotate((float)(atan2(yd, sqrt((xd * xd) + (zd * zd)))*57.295779513082320876846364344191), 0);
	}

	//stream operators
	template <typename stream_t> friend
	stream_t&		operator>>( stream_t& stm, matrix& m )
	{
		for( unsigned i=0; i<16; ++i )
		{ stm >> m.data()[i]; }
		return stm;
	}

	template <typename stream_t> friend
	stream_t&		operator<<( stream_t& stm, const matrix& m )
	{
		for( unsigned i=0; i<16; ++i )
		{ stm << m.data()[i] << ' '; }
		return stm;
	}
};

}
