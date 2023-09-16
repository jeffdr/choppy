#pragma once

#include "Texture.h"

namespace ye
{

class	DisplayWindow
{
public:
	DisplayWindow( long x, long y, long width, long height );
	~DisplayWindow();

	void	repaint( const Texture& d );

	void	processEvents( void );

	void	(*moveFunc)( bool* dirs );
	void	(*lookFunc)( float x, float y );

private:
	long	mWidth, mHeight;
	void*	mHWND;
	void*	mHDC;

	friend struct WindowProc;
};


}