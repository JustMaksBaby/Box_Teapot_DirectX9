#include <Windows.h>
#include <stdint.h>
#include <d3dx9.h>
#include <d3d9.h>

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


//GLOBAL  COMMON VARIABLES
HWND g_hWnd = nullptr; // window handler 
uint32 g_ScreenWidth = 600; //window width
uint32 g_ScreenHeight = 600; // width height

// DELETE DIRECT3D OBJECTS 
#define RELEASE(obj) if(obj != nullptr){obj->Release();obj = nullptr;}
//GLOBAL DIRECT3D VARIABLES
static IDirect3DDevice9*  g_Device = nullptr;
 
bool InitDirect3D()
{
	// create Direct object
	IDirect3D9* directObj = Direct3DCreate9(D3D_SDK_VERSION);

	//check capabilities
	D3DCAPS9 devCaps{}; 
	directObj->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &devCaps); 
	bool supportsHardwareVertexProcessing = devCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT;

	// set paramaters
	D3DPRESENT_PARAMETERS param{};
	param.Windowed = TRUE; 
	param.SwapEffect = D3DSWAPEFFECT_DISCARD;  
	param.hDeviceWindow = g_hWnd; 
	param.EnableAutoDepthStencil = TRUE; // z-buffer
	param.AutoDepthStencilFormat = D3DFMT_A8R8G8B8; // z-buffer

	//create device
	DWORD vertexProcessing = supportsHardwareVertexProcessing?  D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING; 
	directObj->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		param.hDeviceWindow,
		vertexProcessing,
		&param,
		&g_Device); 

	// release Direct object
	RELEASE(directObj)

	
}

// CREATE VERTEX

// SETUP DIRECT3D ENVIRONMENT
	/* 
	-> SETUP  VIEW TRANSFORMATION
	-> SETUP  PROJECTION TRANSFORMATION 
	-> 
	*/

//SETUP GRAPHIC OBJECTS 
	/*
	-> create and fill vertex buffer
	-> create teapot ojb
	*/

// DRAW FUNCTION
	/*
	-> CLEAN BUFFER
	-> SET WORLD MATRIX
	-> DRAW OBJECTS
	*/


void ReleaseDirectObjects()
{
	// usign to free Direct obj
}


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
	g_hWnd= CreateWindow(
		L"Direct3D9App", 
		L"Direct3D9App",
		WS_EX_TOPMOST|WS_SYSMENU|WS_SIZEBOX, 
		0, 0,
		width, height,
		0 /*parent hwnd*/, 
		0 /* menu */, 
		GetModuleHandle(NULL), 
		0 /*extra*/);

	if (!g_hWnd)
	{
		::MessageBox(0, L"CreateWindow() - FAILED", 0, 0);
		return false;
	}

	ShowWindow(g_hWnd, SW_SHOW);
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
	
	if (InitWindow(g_ScreenWidth, g_ScreenHeight))
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

