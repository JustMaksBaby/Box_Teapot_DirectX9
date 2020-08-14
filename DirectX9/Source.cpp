#include <Windows.h>
#include <stdint.h>
#include <d3dx9.h>
#include <d3d9.h>

#include "Vertex.h"

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
uint32 g_ScreenWidth = 640; //window width
uint32 g_ScreenHeight = 480; // width height

// DELETE DIRECT3D OBJECTS 
#define RELEASE(obj) if(obj != nullptr){obj->Release();obj = nullptr;}

//GLOBAL DIRECT3D VARIABLES
static IDirect3DDevice9*  g_Device		= nullptr;
static IDirect3DVertexBuffer9* g_Vertex = nullptr;
static IDirect3DTexture9* g_Texture      = nullptr; 
 
void ReleaseDirectObjects()
{
	// usign to free Direct obj
	g_Vertex->Release(); 
	g_Device->Release();
	g_Texture->Release(); 
}

void SetStates()
{
	g_Device->SetRenderState(D3DRS_ZENABLE, TRUE);  // Z BUFFER ON/ TURNDER ON BY DEFAUT
	g_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); // HOW TO SHOW THE VERTICES
	g_Device->SetRenderState(D3DRS_LIGHTING, FALSE); // TURN ON THE LIGHT/ TURNED ON BY DEFAULT

	// STATES FOR TEXTURES FILTRATION
	g_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR); 
	g_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); 
	g_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR); 

}

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
	param.AutoDepthStencilFormat = D3DFMT_D16; // z-buffer
	

	//create device
	DWORD vertexProcessing = supportsHardwareVertexProcessing ?  D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING; 
	directObj->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		param.hDeviceWindow,
		vertexProcessing,
		&param,
		&g_Device); 

	if (!directObj)
	{
		MessageBox(0,  L"InitDirect3D -> FAILED initialize a device",NULL, MB_OK); 
		return false; 
	}

	// release Direct object
	RELEASE(directObj)

	return true; 
	
}

void SetCamera()
{

	 //  set camera position	
	 D3DXVECTOR3 position = { 0.0f, 0.0f, -3.0f};
	 D3DXVECTOR3 lookAt = { 0,0,0 };
	 D3DXVECTOR3 up = { 0.0f, 1.0f, 0.0f };
	 
	 D3DXMATRIX camera{};
	 D3DXMatrixLookAtLH(&camera, &position, &lookAt, &up);
	 g_Device->SetTransform(D3DTS_VIEW, &camera);


	//set  camera projection 
	D3DXMATRIX projection; 
	D3DXMatrixPerspectiveFovLH(
		&projection,
		D3DXToRadian(90),
		(float)g_ScreenWidth / (float)g_ScreenHeight,
		1.0f,
		100.0f); 
	
	g_Device->SetTransform(D3DTS_PROJECTION, &projection); 

	// SETUP DIRECT3D ENVIRONMENT
		/*
		-> SETUP  VIEW TRANSFORMATION  +
		-> SETUP  PROJECTION TRANSFORMATION+
		->
		*/
} 



void SetGraphics()
{

	g_Device->CreateVertexBuffer(
		sizeof(Vertex) * 6,
		0,
		VERTEX_FVF,
		D3DPOOL_MANAGED,
		&g_Vertex,
		NULL
	);


	Vertex* vertices = nullptr;
	g_Vertex->Lock(0, 0, (void**)&vertices, 0);


	vertices[0] = Vertex(-10.0f, -10.0f, 8.0f, 0.0f, 1.0f);
	vertices[1] = Vertex(-10.0f,  10.0f, 8.0f, 0.0f, 0.0f);
	vertices[2] = Vertex(10.0f,   10.0f, 8.0f, 1.0f, 0.0f);

	vertices[3] = Vertex(-10.0f, -10.0f, 8.0f, 0.0f, 1.0f);
	vertices[4] = Vertex(10.0f,   10.0f, 8.0f, 1.0f, 0.0f);
	vertices[5] = Vertex(10.0f,  -10.0f, 8.0f, 1.0f, 1.0f);


	g_Vertex->Unlock(); 
	//SETUP GRAPHIC OBJECTS 
		/*
		-> create and fill vertex buffer + 
		-> create teapot ojb
		*/
}

void RenderFrame()
{
	// SET WORLD MATRIX
	D3DXMATRIX world{};
	D3DXMatrixIdentity(&world);
	g_Device->SetTransform(D3DTS_WORLD, &world);

	//START DRAWING
	g_Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0F, 0);
	g_Device->BeginScene();

	g_Device->SetFVF(VERTEX_FVF);
	g_Device->SetStreamSource(0, g_Vertex, 0, sizeof(Vertex));
	g_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	g_Device->EndScene();
	g_Device->Present(0, 0, 0, 0);

// DRAW FUNCTION
	/*
	-> SET WORLD MATRIX +
	-> CLEAN BUFFER +
	-> DRAW OBJECTS
		-> DRAW SQUARE
		-> DRAW A TEAPOT
	*/

}

void LoadTexture(LPCWSTR textureFile)
{
	if (g_Texture != nullptr)
	{
		RELEASE(g_Texture)
	}

	if (SUCCEEDED(D3DXCreateTextureFromFile(g_Device, textureFile, &g_Texture)))
	{
		g_Device->SetTexture(0, g_Texture); 
	}
	else
	{
		MessageBox(0, L"Couldn`t load a texture", 0, MB_OK); 
	}
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
		MessageBox(0, L"InitWindow ->  FAILED register the class", NULL, 0);
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
		::MessageBox(0, L"InitWindow  -> FAILED create window", NULL, 0);
		return false;
	}

	ShowWindow(g_hWnd, SW_SHOW);
	//END CREATE WINDOW

	return true; 
}

int WINAPI wWinMain(HINSTANCE hinstance,
					HINSTANCE prevInstance,
					PWSTR cmdLine,
					int showCmd)
{
	
	if (InitWindow(g_ScreenWidth, g_ScreenHeight))
	{
		if (InitDirect3D())
		{
			SetStates(); 
			SetCamera(); 
			SetGraphics(); 

			LoadTexture(L"crate.jpg"); 

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
					RenderFrame();
				}

			}

			ReleaseDirectObjects(); 
		} 
	} 

	RELEASE(g_Device)
	return 0;
}

