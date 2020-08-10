#include <Windows.h>
#include <stdint.h>

// TYPE DEFINITIONS
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;

typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t  uint8;

//LIBS
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib") 
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Gdi32.lib")

//- 
#define global_var static

//GLOBAL VARIABLES
HWND hWnd_g = nullptr; // window handler 
uint32 screenWidth_g = 600; //window width
uint32 screenHeight_g = 600; // width height


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{	::PostQuitMessage(0);
		break;
	}
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hWnd);
	}break;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

bool InitWindow(uint32 width, uint32 height)
{
	
	//CREATE CLASS
	WNDCLASS wc{};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"Direct3D9App";

	if (!RegisterClass(&wc))
	{
		::MessageBox(0, L"RegisterClass() - FAILED", 0, 0);
		return false;
	}
	//END CREATE CLASS


	//CREATE WINDOW
	hWnd_g = CreateWindow(
		L"Direct3D9App", 
		L"Direct3D9App",
		WS_EX_TOPMOST|WS_SYSMENU,
		0, 0,
		width, height,
		0 /*parent hwnd*/, 
		0 /* menu */, 
		GetModuleHandle(NULL), 
		0 /*extra*/);

	if (!hWnd_g)
	{
		::MessageBox(0, L"CreateWindow() - FAILED", 0, 0);
		return false;
	}

	ShowWindow(hWnd_g, SW_SHOW);
	//END CREATE WINDOW

	return true; 
}
//
// WinMain
//
int WINAPI wWinMain(HINSTANCE hinstance,
					HINSTANCE prevInstance,
					PWSTR cmdLine,
					int showCmd)
{
	
	if (InitWindow(screenWidth_g, screenHeight_g))
	{
		MSG msg{};
		//GAME LOOP 
		while (msg.message != WM_QUIT)
		{

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				// 
			}

		}
	} 

	return 0;
}

