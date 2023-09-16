#include "DisplayWindow.h"
#include <windows.h>
#include <windowsx.h>

namespace cc
{

struct WindowProc
{
	static LRESULT CALLBACK Proc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		static DisplayWindow* dwin = nullptr;
		static bool move[4] = {};

		if( message == WM_NCCREATE )
		{
			//set the user data
			dwin = (DisplayWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		}
		else if( message >= WM_KEYFIRST && message <= WM_KEYLAST )
		{
			bool press = (message == WM_KEYDOWN);

			if( wParam == 'E' )
			{ move[0] = press; }
			else if( wParam == 'D' )
			{ move[1] = press; }
			else if( wParam == 'S' )
			{ move[2] = press; }
			else if( wParam == 'F' )
			{ move[3] = press; }

			if( dwin->moveFunc )
			{ dwin->moveFunc( move ); }
		}
		else if( message == WM_MOUSEMOVE )
		{
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);

			if( dwin->lookFunc )
			{ dwin->lookFunc( float(x), float(y) ); }
		}

		return DefWindowProc( hWnd, message, wParam, lParam );
	}
};

DisplayWindow::DisplayWindow( long x, long y, long width, long height )
{
	moveFunc = nullptr;
	lookFunc = nullptr;
	mWidth = width;
	mHeight = height;

	WNDCLASSEX wc;
	ZeroMemory( &wc, sizeof(wc) );
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc::Proc;
	wc.hInstance = 0;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.lpszClassName = L"DisplayWindowWin32";
	RegisterClassEx(&wc);

	DWORD windowFlags = WS_OVERLAPPEDWINDOW;
	RECT rect = { x, y, x+width, y+height };
	AdjustWindowRect( &rect, windowFlags, FALSE );
	
	DWORD dwExStyle = 0;
	HWND hwnd =
	CreateWindowExA(	dwExStyle, "DisplayWindowWin32", "",
						windowFlags,
						rect.left, rect.top,
						rect.right-rect.left, rect.bottom-rect.top,
						0, 0, GetModuleHandle( 0 ),
						(LPVOID)this	);
	mHWND = hwnd;
	mHDC = GetDC(hwnd);

	ShowWindow( hwnd, SW_SHOW );
}

DisplayWindow::~DisplayWindow()
{
	ReleaseDC( (HWND)mHWND, (HDC)mHDC );
	DestroyWindow( (HWND)mHWND );
}

void	DisplayWindow::processEvents( void )
{
	SetActiveWindow( (HWND)mHWND );

	MSG m;
	while( PeekMessage( &m, 0, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage(&m);
		DispatchMessage(&m);
	}
}

void	DisplayWindow::repaint( const Texture& t )
{
	BITMAPINFO binfo = {};
	auto& hdr = binfo.bmiHeader;
	hdr.biSize = sizeof(hdr);
	hdr.biWidth = t.width;
	hdr.biHeight = t.height; //negative for opposite line order
	hdr.biPlanes = 1;
	hdr.biBitCount = t.channels * 8;
	hdr.biCompression = BI_RGB;
	hdr.biSizeImage = t.width*t.height*t.channels;
	
	StretchDIBits(	(HDC)mHDC,
					0, 0, mWidth, mHeight,
					0, 0, t.width, t.height,
					t.pixels,
					&binfo, DIB_RGB_COLORS, SRCCOPY );
}

}