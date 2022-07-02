/*
*
*               ____  _
*              / __ \| |   By REDACTED & REDACTED
REDACTED
*
*				- REDACTED
*				-  REDACTED
*
*/


#include <Windows.h>
#include <iostream>
#include <thread>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <vector>
#include <string>
#include <thread>
#include <D3DX10math.h>
#include "Game/SDK/Classes.hpp"
//#include "Game/Render/Render.h"
/* Render.h */

#include <Windows.h>
#include <iostream>
#include <thread>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <vector>
#include <string>
#include <thread>
#include <sstream>

#include "Render/ImGui/imgui_impl_dx12.h"
#include "Render/ImGui/imgui_impl_win32.h"
#include "Render/ImGui/imgui.h"

#include "Hooking/memory_hook.h"

#include "Utilities///Log.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

/* Render.cpp */
#pragma once

#include <D3DX10math.h>

/* Classes */
#include "Game/SDK/Classes.hpp"

/* Configs */
#include "Render/Globals.hpp"

typedef struct
{
	DWORD R;
	DWORD G;
	DWORD B;
	DWORD A;
}RGBA;


std::string string_To_UTF8( const std::string &str )
{
	int nwLen = ::MultiByteToWideChar( CP_ACP, 0, str.c_str(), -1, NULL, 0 );

	wchar_t *pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory( pwBuf, nwLen * 2 + 2 );

	::MultiByteToWideChar( CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen );

	int nLen = ::WideCharToMultiByte( CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL );

	char *pBuf = new char[nLen + 1];
	ZeroMemory( pBuf, nLen + 1 );

	::WideCharToMultiByte( CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL );

	std::string retStr( pBuf );

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

/* RGBA Structure (Red, Green, Blue, Alpha) */

static void FilledRectangle( int x, int y, int w, int h, RGBA color )
{
	ImGui::GetForegroundDrawList()->AddRectFilled( ImVec2( x, y ), ImVec2( x + w, y + h ), ImGui::ColorConvertFloat4ToU32( ImVec4( color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0 ) ), 0, 0 );
}

void Box( int x, int y, int w, int h, RGBA color )
{
	ImGui::GetForegroundDrawList()->AddRect( ImVec2( x, y ), ImVec2( x + w, y + h ), ImGui::ColorConvertFloat4ToU32( ImVec4( color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0 ) ), 0, 0 );
}

void Rectangle( int x, int y, int w, int h, RGBA color, int thickness )
{
	ImGui::GetForegroundDrawList()->AddRect( ImVec2( x, y ), ImVec2( x + w, y + h ), ImGui::ColorConvertFloat4ToU32( ImVec4( color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0 ) ), 0, 0, thickness );
}

void FilledCircle( int x, int y, int radius, RGBA color, int segments )
{
	ImGui::GetForegroundDrawList()->AddCircleFilled( ImVec2( x, y ), radius, ImGui::ColorConvertFloat4ToU32( ImVec4( color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0 ) ), segments );
}

void Circle( int x, int y, int radius, RGBA color, int segments )
{
	ImGui::GetForegroundDrawList()->AddCircle( ImVec2( x, y ), radius, ImGui::ColorConvertFloat4ToU32( ImVec4( color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0 ) ), segments );
}

void Triangle( int x1, int y1, int x2, int y2, int x3, int y3, RGBA color, float thickne )
{
	ImGui::GetForegroundDrawList()->AddTriangle( ImVec2( x1, y1 ), ImVec2( x2, y2 ), ImVec2( x3, y3 ), ImGui::ColorConvertFloat4ToU32( ImVec4( color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0 ) ), thickne );
}

void FilledTriangle( int x1, int y1, int x2, int y2, int x3, int y3, RGBA color )
{
	ImGui::GetForegroundDrawList()->AddTriangleFilled( ImVec2( x1, y1 ), ImVec2( x2, y2 ), ImVec2( x3, y3 ), ImGui::ColorConvertFloat4ToU32( ImVec4( color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0 ) ) );
}

void Line( int x1, int y1, int x2, int y2, RGBA color, int thickness )
{
	if (x2 != 0 && y2 != 0)
	{
		ImGui::GetForegroundDrawList()->AddLine( ImVec2( x1, y1 ), ImVec2( x2, y2 ), ImGui::ColorConvertFloat4ToU32( ImVec4( color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0 ) ), thickness );
	}
}

void BottomHealthBar( int x, int y, int w, int h, int borderPx, RGBA color )
{
	FilledRectangle( x + borderPx, y + h + borderPx, w, borderPx, color ); //bottom 
	FilledRectangle( x + w - w + borderPx, y + h + borderPx, w, borderPx, color ); //bottom 
}

void CornerBox( int x, int y, int w, int h, int borderPx, RGBA color )
{
	int WidthThird = w / 4;
	int HeightThird = WidthThird;

	//topleft 
	FilledRectangle( x - w / 2 - (1), y - (1), WidthThird + 2, 3, { 0, 0,0, 255 } ); //h
	FilledRectangle( x - w / 2 - (1), y - (1), 3, HeightThird + 2, { 0, 0,0, 255 } ); //v

	//topright
	FilledRectangle( (x + w / 2) - WidthThird - (1), y - (1), WidthThird + 2, 3, { 0, 0,0, 255 } ); //h
	FilledRectangle( x + w / 2 - (1), y - (1), 3, HeightThird + 2, { 0, 0,0, 255 } ); //v

	//bottom left
	FilledRectangle( x - w / 2 - (1), y + h - 4, WidthThird + 2, 3, { 0, 0,0, 255 } ); //h
	FilledRectangle( x - w / 2 - (1), (y + h) - HeightThird - 4, 3, HeightThird + 2, { 0, 0,0, 255 } );//v

	//bottom right
	FilledRectangle( (x + w / 2) - WidthThird - (1), y + h - 4, WidthThird + 2, 3, { 0, 0,0, 255 } );//h
	FilledRectangle( x + w / 2 - (1), (y + h) - HeightThird - 4, 3, HeightThird + 3, { 0, 0,0, 255 } ); //v

	/* draw eight lines */
	//topleft 
	FilledRectangle( x - w / 2, y, WidthThird, 1, color ); //h
	FilledRectangle( x - w / 2, y, 1, HeightThird, color ); //v

	//topright
	FilledRectangle( (x + w / 2) - WidthThird, y, WidthThird, 1, color ); //h
	FilledRectangle( x + w / 2, y, 1, HeightThird, color ); //v

	//bottom left
	FilledRectangle( x - w / 2, y + h - 3, WidthThird, 1, color ); //h
	FilledRectangle( x - w / 2, (y + h) - HeightThird - 3, 1, HeightThird, color );//v

	//bottom right
	FilledRectangle( (x + w / 2) - WidthThird, y + h - 3, WidthThird, 1, color );//h
	FilledRectangle( x + w / 2, (y + h) - HeightThird - 3, 1, HeightThird + 1, color ); //v		
}

void BorderBox( int x, int y, int x2, int y2, int thickness, RGBA color )
{
	FilledRectangle( x, y, x2, thickness, color ); // top
	FilledRectangle( x, y + y2, x2, thickness, color ); // bottom
	FilledRectangle( x, y, thickness, y2, color ); // left
	FilledRectangle( x + x2, y, thickness, y2 + thickness, color ); // right
}

void NormalBox( int x, int y, int w, int h, int borderPx, RGBA color )
{
	FilledRectangle( x + borderPx, y, w, borderPx, color ); //top 
	FilledRectangle( x + w - w + borderPx, y, w, borderPx, color ); //top 
	FilledRectangle( x, y, borderPx, h, color ); //left 
	FilledRectangle( x, y + h - h + borderPx * 2, borderPx, h, color ); //left 
	FilledRectangle( x + borderPx, y + h + borderPx, w, borderPx, color ); //bottom 
	FilledRectangle( x + w - w + borderPx, y + h + borderPx, w, borderPx, color ); //bottom 
	FilledRectangle( x + w + borderPx, y, borderPx, h, color );//right 
	FilledRectangle( x + w + borderPx, y + h - h + borderPx * 2, borderPx, h, color );//right 
}

void NormalFilledBox( int x, int y, int w, int h, int borderPx, RGBA color )
{
	FilledRectangle( x + borderPx, y, w, borderPx, color ); //top 
	FilledRectangle( x + w - w + borderPx, y, w, borderPx, color ); //top 
	FilledRectangle( x, y, borderPx, h, color ); //left 
	FilledRectangle( x, y + h - h + borderPx * 2, borderPx, h, color ); //left 
	FilledRectangle( x + borderPx, y + h + borderPx, w, borderPx, color ); //bottom 
	FilledRectangle( x + w - w + borderPx, y + h + borderPx, w, borderPx, color ); //bottom 
	FilledRectangle( x + w + borderPx, y, borderPx, h, color );//right 
	FilledRectangle( x + w + borderPx, y + h - h + borderPx * 2, borderPx, h, color );//right 
}
//
void OutlinedText( ImFont *pFont, const std::string &text, const ImVec2 &pos, float size, RGBA color, bool center )
{
	// = font;

	if (!pFont)
		return;

	std::stringstream stream( text );
	std::string line;


	float y = 0.0f;
	int i = 0;

	ImGui::PushFont( pFont );
	while (std::getline( stream, line ))
	{
		ImVec2 textSize = pFont->CalcTextSizeA( size, FLT_MAX, 0.0f, line.c_str() );

		if (center)
		{

			ImGui::GetForegroundDrawList()->AddText( pFont, size, ImVec2( (pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1 ), ImGui::GetColorU32( ImVec4( 0, 0, 0, 255 ) ), line.c_str() );
			ImGui::GetForegroundDrawList()->AddText( pFont, size, ImVec2( (pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) - 1 ), ImGui::GetColorU32( ImVec4( 0, 0, 0, 255 ) ), line.c_str() );
			//	ImGui::GetForegroundDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			//	ImGui::GetForegroundDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

			ImGui::GetForegroundDrawList()->AddText( pFont, size, ImVec2( pos.x - textSize.x / 2.0f, pos.y + textSize.y * i ), ImGui::ColorConvertFloat4ToU32( ImVec4( color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0 ) ), line.c_str() );
		}
		else
		{//
			ImGui::GetForegroundDrawList()->AddText( pFont, size, ImVec2( (pos.x) + 1, (pos.y + textSize.y * i) + 1 ), ImGui::GetColorU32( ImVec4( 0, 0, 0, 255 ) ), line.c_str() );
			ImGui::GetForegroundDrawList()->AddText( pFont, size, ImVec2( (pos.x) - 1, (pos.y + textSize.y * i) - 1 ), ImGui::GetColorU32( ImVec4( 0, 0, 0, 255 ) ), line.c_str() );
			//	ImGui::GetForegroundDrawList()->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				//ImGui::GetForegroundDrawList()->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

			ImGui::GetForegroundDrawList()->AddText( pFont, size, ImVec2( pos.x, pos.y + textSize.y * i ), ImGui::ColorConvertFloat4ToU32( ImVec4( color.R / 255.0, color.G / 255.0, color.B / 255.0, color.A / 255.0 ) ), line.c_str() );

		}

		y = pos.y + textSize.y * (i + 1);
		i++;
	}
	ImGui::PopFont();
	//return y;
}


inline uintptr_t GameBase = (uintptr_t)GetModuleHandle( NULL );

bool WorldToScreen( D3DXVECTOR3 WorldPosition, D3DXVECTOR3 *ScreenPosition )
{
	uint64_t GameRender = *(uint64_t *)(GameBase + 0x62CF960);
	uintptr_t p_RenderView = *(uintptr_t *)(GameRender + 0x60);

	if (!p_RenderView)
		return false;

	D3DXMATRIX c_ViewProjection = *(D3DXMATRIX *)(p_RenderView + 0x480);

	float mX = render::window.width * 0.5f;
	float mY = render::window.height * 0.5f;

	float w = c_ViewProjection.m[0][3] * WorldPosition.x + c_ViewProjection.m[1][3] * WorldPosition.y + c_ViewProjection.m[2][3] * WorldPosition.z + c_ViewProjection.m[3][3];

	if (w < 0.01)
		return false;

	ScreenPosition->x = c_ViewProjection.m[0][0] * WorldPosition.x + c_ViewProjection.m[1][0] * WorldPosition.y + c_ViewProjection.m[2][0] * WorldPosition.z + c_ViewProjection.m[3][0];
	ScreenPosition->y = c_ViewProjection.m[0][1] * WorldPosition.x + c_ViewProjection.m[1][1] * WorldPosition.y + c_ViewProjection.m[2][1] * WorldPosition.z + c_ViewProjection.m[3][1];

	float invw = 1.0f / w;

	ScreenPosition->x *= invw;
	ScreenPosition->y *= invw;

	float x = (float)render::window.width / 2;
	float y = (float)render::window.height / 2;


	x += 0.5 * ScreenPosition->x * (float)render::window.width + 0.5;
	y -= 0.5 * ScreenPosition->y * (float)render::window.height + 0.5;

	ScreenPosition->x = x;
	ScreenPosition->y = y;

	if (ScreenPosition->x > render::window.width || ScreenPosition->y > render::window.height || ScreenPosition->x < 0 || ScreenPosition->y < 0)
		return false;

	return true;
}

//
//bool WorldToScreen( D3DXVECTOR3 WorldPosition, D3DXVECTOR3 *ScreenPosition )
//{
//	uint64_t GameRender = *(uint64_t *)(GameBase + 0x62CF960);
//	uintptr_t p_RenderView = *(uintptr_t *)(GameRender + 0x60);
//
//	if (!p_RenderView)
//		return false;
//
//	D3DXMATRIX c_ViewProjection = *(D3DXMATRIX *)(p_RenderView + 0x480);
//
//	float mX = render::window.width * 0.5f;
//	float mY = render::window.height * 0.5f;
//
//	float w = c_ViewProjection.m[0][3] * WorldPosition.x + c_ViewProjection.m[1][3] * WorldPosition.y + c_ViewProjection.m[2][3] * WorldPosition.z + c_ViewProjection.m[3][3];
//
//	if (w < 0.01)
//		return false;
//
//	ScreenPosition->x = c_ViewProjection.m[0][0] * WorldPosition.x + c_ViewProjection.m[1][0] * WorldPosition.y + c_ViewProjection.m[2][0] * WorldPosition.z + c_ViewProjection.m[3][0];
//	ScreenPosition->y = c_ViewProjection.m[0][1] * WorldPosition.x + c_ViewProjection.m[1][1] * WorldPosition.y + c_ViewProjection.m[2][1] * WorldPosition.z + c_ViewProjection.m[3][1];
//
//	float invw = 1.0f / w;
//
//	ScreenPosition->x *= invw;
//	ScreenPosition->y *= invw;
//
//	float x = (float)render::window.width / 2;
//	float y = (float)render::window.height / 2;
//
//
//	x += 0.5 * ScreenPosition->x * (float)render::window.width + 0.5;
//	y -= 0.5 * ScreenPosition->y * (float)render::window.height + 0.5;
//
//	ScreenPosition->x = x;
//	ScreenPosition->y = y;
//
//	if (ScreenPosition->x > render::window.width || ScreenPosition->y > render::window.height || ScreenPosition->x < 0 || ScreenPosition->y < 0)
//		return false;
//
//	return true;
//}

ULONG64 GetRootComponent( uintptr_t Entity )
{
	ULONG64 c_RootComponent = *(ULONG64 *)(Entity + 0xc8);
	if (c_RootComponent)
		return c_RootComponent;
}

auto Text( std::string Letters, int size, RGBA color )
{
	OutlinedText( font, Letters, ImVec2( 15, 15 ), size, color, false );
}

std::string ReadASCII( uint64_t adr )
{
	char buf[31];
	ZeroMemory( buf, sizeof( buf ) );
	memcpy( buf, (void *)adr, 32 );
	return buf;
}

void entity()
{
	//	Circle(render::window.width / 2, render::window.height / 2, cfg::FOV, { 255, 255, 255, 255 }, 150);
//OutlinedText(font, "REDACTED", ImVec2(15, 15), 14, { 255, 255, 255, 255 }, false);
	//Text( "REDACTED", 14, { 255, 255, 255, 255 } );

	OutlinedText( font, "Interpreter#7777", ImVec2( 15, 15 ), 14, { 255, 255, 255, 255 }, false );

	std::vector<uintptr_t> EntList;

	ULONG64 GameRender = *(ULONG64 *)(GameBase + 0x62CF960);
	//std::cout << "GameRender -> " << GameRender << std::endl;

	ULONG64 RenderView = *(ULONG64 *)(GameRender + 0x60);
	//std::cout << "RenderView -> " << RenderView << std::endl;

	D3DXMATRIX Matrix2 = *(D3DXMATRIX *)(RenderView + 0x480);
	//std::cout << "Matrix -> " << Matrix2 << std::endl;

	auto GameContext = *(uint64_t *)(GameBase + 0x621BE48);
	//std::cout << "GameContext -> " << GameContext << std::endl;

	auto ClientPlayerManager = *(uint64_t *)(GameContext + 0xA8);
	//std::cout << "ClientPlayerManager -> " << ClientPlayerManager << std::endl;

	auto LocalPlayerPtr = *(uint64_t *)(ClientPlayerManager + 0x51);
	//printf("LocalPlayer -> %x\n", LocalPlayer);

	LocalPlayerPtr = *(uint64_t *)(ClientPlayerManager + 0x00);
	auto ClientPlayersArray = *(uint64_t *)(ClientPlayerManager + 0x570);
	//std::cout << "ClientPlayersArray -> " << ClientPlayersArray << std::endl;
	ULONG64 Count = *(ULONG64 *)(ClientPlayersArray + 0x10);
	D3DXVECTOR3 ScreenPosition;



	for (int i = 0; i < 128; i++)
	{
		uintptr_t ClientPlayerPtr = *(uint64_t *)(ClientPlayersArray + (0x18 + (i * 0x8)));
		auto LocalPlayer = *(uint64_t *)(*(uint64_t *)(ClientPlayerManager + 0x560) + 0x00);

		if (ClientPlayerPtr != 0x00)
		{
			if (LocalPlayer == ClientPlayerPtr)continue;
			auto LocalTeamId = *(int *)(LocalPlayer + 0x60);
			auto TeamId = *(int *)(ClientPlayerPtr + 0x60);
			if (LocalTeamId == TeamId)continue;

			D3DXVECTOR3 relative_location = *(D3DXVECTOR3 *)(GetRootComponent( ClientPlayerPtr ) + 0x20E0);
			if (!relative_location)
				continue;

			if (LocalPlayerPtr && relative_location)
			{
				if (relative_location != D3DXVECTOR3( 0, 0, 0 ))
				{

					if (WorldToScreen( relative_location, &ScreenPosition ))
					{
						std::string Namee;

						//Namee += Distance;

						//Namee += "m - ";
						Namee += ReadASCII( ClientPlayerPtr + 0x20 );
						const ImVec2 text_dimension = ImGui::CalcTextSize( Namee.c_str() );
						const float text_width = text_dimension.x + 5.f;
						const float mid_width = ScreenPosition.x - (text_width / 2.f);

						FilledRectangle( mid_width + 2.f, ScreenPosition.y - 25.f, text_width + 10.f, text_dimension.y + 5.f, { 49, 47, 48, 90 } );
						Line( mid_width, ScreenPosition.y - 25.f, mid_width, (ScreenPosition.y - 25.f) + text_dimension.y + 5.f, { 18, 217, 17, 100 }, 3.f );
						OutlinedText( font, Namee.c_str(), ImVec2( mid_width + 6.f, (ScreenPosition.y - 25.f) + 2.f ), 14.6, { 255, 255, 255, 255 }, false );
					}
				}
			}
		}
	}
}

/* Namespace contains a definition for all window information */
namespace Process
{
	DWORD ID;
	HANDLE Handle;
	HWND Hwnd;
	HMODULE Module;
	WNDPROC WndProc;
	int WindowWidth;
	int WindowHeight;
	LPCSTR Title;
	LPCSTR ClassName;
	LPCSTR Path;
}


WNDCLASSEX WindowClass;
HWND WindowHwnd;

bool InitWindow()
{

	WindowClass.cbSize = sizeof( WNDCLASSEX );
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = DefWindowProc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetModuleHandle( NULL );
	WindowClass.hIcon = NULL;
	WindowClass.hCursor = NULL;
	WindowClass.hbrBackground = NULL;
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = "MJ";
	WindowClass.hIconSm = NULL;
	RegisterClassEx( &WindowClass );
	WindowHwnd = CreateWindow( WindowClass.lpszClassName, "DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, WindowClass.hInstance, NULL );
	if (WindowHwnd == NULL)
	{
		return false;
	}
	return true;
}

bool DeleteWindow()
{
	DestroyWindow( WindowHwnd );
	UnregisterClass( WindowClass.lpszClassName, WindowClass.hInstance );
	if (WindowHwnd != NULL)
	{
		return false;
	}
	return true;
}

#if defined _M_X64
typedef uint64_t uintx_t;
#elif defined _M_IX86
typedef uint32_t uintx_t;
#endif

static uintx_t *MethodsTable = NULL;

//=========================================================================================================================//


namespace DirectX12
{
	bool Init()
	{

		if (InitWindow() == false)
		{
			return false;
		}

		HMODULE D3D12Module = GetModuleHandle( "d3d12.dll" );
		HMODULE DXGIModule = GetModuleHandle( "dxgi.dll" );

		//Log::DirectX( "D3D12Module: %x", D3D12Module );
		//Log::DirectX( "DXGIModule: %x", DXGIModule );
		if (D3D12Module == NULL || DXGIModule == NULL)
		{
			DeleteWindow();
			return false;
		}

		void *CreateDXGIFactory = GetProcAddress( DXGIModule, "CreateDXGIFactory" );
		//Log::DirectX( "CreateDXGIFactory: %x", CreateDXGIFactory );

		if (CreateDXGIFactory == NULL)
		{
			DeleteWindow();
			return false;
		}

		IDXGIFactory *Factory;
		if (((long( __stdcall * )(const IID &, void **))(CreateDXGIFactory))(__uuidof(IDXGIFactory), (void **)&Factory) < 0)
		{
			DeleteWindow();
			return false;
		}
		//Log::DirectX( "IDXGIFactory" );


		IDXGIAdapter *Adapter;
		if (Factory->EnumAdapters( 0, &Adapter ) == DXGI_ERROR_NOT_FOUND)
		{
			DeleteWindow();
			return false;
		}
		//Log::DirectX( "Factory->EnumAdapters (IDXGIAdapter)" );

		void *D3D12CreateDevice = GetProcAddress( D3D12Module, "D3D12CreateDevice" );
		if (D3D12CreateDevice == NULL)
		{
			DeleteWindow();
			return false;
		}
		//Log::DirectX( "D3D12CreateDevice" );

		ID3D12Device *Device;
		if (((long( __stdcall * )(IUnknown *, D3D_FEATURE_LEVEL, const IID &, void **))(D3D12CreateDevice))(Adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void **)&Device) < 0)
		{
			DeleteWindow();
			return false;
		}
		//Log::DirectX( "ID3D12Device" );

		D3D12_COMMAND_QUEUE_DESC QueueDesc;
		QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		QueueDesc.Priority = 0;
		QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		QueueDesc.NodeMask = 0;
		//Log::DirectX( "D3D12_COMMAND_QUEUE_DESC" );

		ID3D12CommandQueue *CommandQueue;
		if (Device->CreateCommandQueue( &QueueDesc, __uuidof(ID3D12CommandQueue), (void **)&CommandQueue ) < 0)
		{
			DeleteWindow();
			return false;
		}
		//Log::DirectX( "ID3D12CommandQueue" );

		ID3D12CommandAllocator *CommandAllocator;
		if (Device->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void **)&CommandAllocator ) < 0)
		{
			DeleteWindow();
			return false;
		}
		//Log::DirectX( "ID3D12CommandAllocator" );

		ID3D12GraphicsCommandList *CommandList;
		if (Device->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void **)&CommandList ) < 0)
		{
			DeleteWindow();
			return false;
		}
		//Log::DirectX( "ID3D12GraphicsCommandList" );

		DXGI_RATIONAL RefreshRate;
		RefreshRate.Numerator = 60;
		RefreshRate.Denominator = 1;
		//Log::DirectX( "DXGI_RATIONAL" );

		DXGI_MODE_DESC BufferDesc;
		BufferDesc.Width = 100;
		BufferDesc.Height = 100;
		BufferDesc.RefreshRate = RefreshRate;
		BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		//Log::DirectX( "DXGI_MODE_DESC" );

		DXGI_SAMPLE_DESC SampleDesc;
		SampleDesc.Count = 1;
		SampleDesc.Quality = 0;
		//Log::DirectX( "DXGI_SAMPLE_DESC" );

		DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
		SwapChainDesc.BufferDesc = BufferDesc;
		SwapChainDesc.SampleDesc = SampleDesc;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = 2;
		SwapChainDesc.OutputWindow = WindowHwnd;
		SwapChainDesc.Windowed = 1;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		//Log::DirectX( "DXGI_SWAP_CHAIN_DESC" );
		
		IDXGISwapChain *SwapChain;
		if (Factory->CreateSwapChain( CommandQueue, &SwapChainDesc, &SwapChain ) < 0)
		{
			DeleteWindow();
			return false;
		}
		//Log::DirectX( "IDXGISwapChain : Factory->CreateSwapChain" );

		MethodsTable = (uintx_t *)::calloc( 150, sizeof( uintx_t ) );

		memcpy( MethodsTable, *(uintx_t **)Device, 44 * sizeof( uintx_t ) );
		memcpy( MethodsTable + 44, *(uintx_t **)CommandQueue, 19 * sizeof( uintx_t ) );
		memcpy( MethodsTable + 44 + 19, *(uintx_t **)CommandAllocator, 9 * sizeof( uintx_t ) );
		memcpy( MethodsTable + 44 + 19 + 9, *(uintx_t **)CommandList, 60 * sizeof( uintx_t ) );
		memcpy( MethodsTable + 44 + 19 + 9 + 60, *(uintx_t **)SwapChain, 18 * sizeof( uintx_t ) );
		//Log::DirectX( "MethodsTable : %x", MethodsTable );

		Device->Release();
		Device = NULL;
		//Log::DirectX( "Device->Release()" );

		CommandQueue->Release();
		CommandQueue = NULL;
		//Log::DirectX( "CommandQueue->Release()" );

		CommandAllocator->Release();
		CommandAllocator = NULL;
		//Log::DirectX( "CommandAllocator->Release()" );

		CommandList->Release();
		CommandList = NULL;
		//Log::DirectX( "CommandList->Release()" );

		SwapChain->Release();
		SwapChain = NULL;
		//Log::DirectX( "SwapChain->Release()" );

		DeleteWindow();
		return true;
	}
}


//=========================================================================================================================//


bool CreateHook( uint16_t Index, void **Original, void *Function )
{
	//Log::Hooking( "Index: %x Original: %x Function: %x", Index, Original, Function);
	assert( _index >= 0 && _original != NULL && _function != NULL );
	void *target = (void *)MethodsTable[Index];
	auto nigger = memory_hook::create_context( target );
	*Original = (void *)nigger->create_hook( (uintptr_t)Function, true );
	nigger->enable_hook();
	return true;
}

void DisableHook( uint16_t Index )
{
	assert( Index >= 0 );
	void *target = ((void *)MethodsTable[Index]);

	auto it = memory_hook::hook_list.begin();
	while (it != memory_hook::hook_list.end())
	{
		if ((*it)->target == (uintptr_t)target) break;
		it++;
	}

	(*it)->disable_hook();
}

void DisableAll()
{
	auto it = memory_hook::hook_list.begin();
	while (it != memory_hook::hook_list.end())
	{
		(*it)->disable_hook();
		it++;
	}//pickel pines
	free( MethodsTable );
	MethodsTable = NULL;
}

int countnum = -1;
bool nopants_enabled = true;

//=========================================================================================================================//

typedef HRESULT( APIENTRY *Present12 ) (IDXGISwapChain *pSwapChain, UINT SyncInterval, UINT Flags);
Present12 oPresent = NULL;

typedef void(APIENTRY *DrawInstanced)(ID3D12GraphicsCommandList *dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
DrawInstanced oDrawInstanced = NULL;

typedef void(APIENTRY *DrawIndexedInstanced)(ID3D12GraphicsCommandList *dCommandList, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
DrawIndexedInstanced oDrawIndexedInstanced = NULL;

typedef void(APIENTRY *ExecuteCommandLists)(ID3D12CommandQueue *queue, UINT NumCommandLists, ID3D12CommandList *ppCommandLists);
ExecuteCommandLists oExecuteCommandLists = NULL;

//=========================================================================================================================//

bool ShowMenu = false;
bool ImGui_Initialised = false;

namespace DirectX12Interface
{
	ID3D12Device *Device = nullptr;
	ID3D12DescriptorHeap *DescriptorHeapBackBuffers;
	ID3D12DescriptorHeap *DescriptorHeapImGuiRender;
	ID3D12GraphicsCommandList *CommandList;
	ID3D12CommandQueue *CommandQueue;

	struct _FrameContext
	{
		ID3D12CommandAllocator *CommandAllocator;
		ID3D12Resource *Resource;
		D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle;
	};

	uintx_t BuffersCounts = -1;
	_FrameContext *FrameContext;
}

//=========================================================================================================================//
bool resized = false;
extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK WndProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	if (ImGui_ImplWin32_WndProcHandler( hWnd, Msg, wParam, lParam ))
		return true;

	switch (Msg)
	{
		case WM_SIZE:
		{
			if (wParam != SIZE_MINIMIZED)
			{
				resized = true;
				return CallWindowProc( Process::WndProc, hWnd, Msg, wParam, lParam );
			}
			break;
		}
	}

	return CallWindowProc( Process::WndProc, hWnd, Msg, wParam, lParam );
}

//=========================================================================================================================//


HRESULT APIENTRY hkPresent( IDXGISwapChain3 *pSwapChain, UINT SyncInterval, UINT Flags )
{
	if (!ImGui_Initialised)
	{
		if (SUCCEEDED( pSwapChain->GetDevice( __uuidof(ID3D12Device), (void **)&DirectX12Interface::Device ) ))
		{
			ImGui::CreateContext();
			ImGuiIO &io = ImGui::GetIO(); (void)io;
			ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

			DXGI_SWAP_CHAIN_DESC Desc;
			pSwapChain->GetDesc( &Desc );
			Desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			Desc.OutputWindow = Process::Hwnd;
			Desc.Windowed = ((GetWindowLongPtr( Process::Hwnd, GWL_STYLE ) & WS_POPUP) != 0) ? false : true;
			//Log::DirectX( "DXGI_SWAP_CHAIN_DESC" );

			DirectX12Interface::BuffersCounts = Desc.BufferCount;
			DirectX12Interface::FrameContext = new DirectX12Interface::_FrameContext[DirectX12Interface::BuffersCounts];
			//Log::DirectX( "DirectX12Interface::BuffersCounts, irectX12Interface::FrameContext" );

			D3D12_DESCRIPTOR_HEAP_DESC DescriptorImGuiRender = {};
			DescriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			DescriptorImGuiRender.NumDescriptors = DirectX12Interface::BuffersCounts;
			DescriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			//Log::DirectX( "D3D12_DESCRIPTOR_HEAP_DESC" );

			if (DirectX12Interface::Device->CreateDescriptorHeap( &DescriptorImGuiRender, IID_PPV_ARGS( &DirectX12Interface::DescriptorHeapImGuiRender ) ) != S_OK)
				return oPresent( pSwapChain, SyncInterval, Flags );
			//Log::DirectX( "DirectX12Interface::Device->CreateDescriptorHeap" );

			ID3D12CommandAllocator *Allocator;
			if (DirectX12Interface::Device->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( &Allocator ) ) != S_OK)
				return oPresent( pSwapChain, SyncInterval, Flags );
			//Log::DirectX( "DirectX12Interface::Device->CreateCommandAllocator" );

			for (size_t i = 0; i < DirectX12Interface::BuffersCounts; i++)
			{
				DirectX12Interface::FrameContext[i].CommandAllocator = Allocator;
				//Log::DirectX( "DirectX12Interface::BuffersCounts" );
			}

			if (DirectX12Interface::Device->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, NULL, IID_PPV_ARGS( &DirectX12Interface::CommandList ) ) != S_OK ||
				DirectX12Interface::CommandList->Close() != S_OK)
				return oPresent( pSwapChain, SyncInterval, Flags );
			//Log::DirectX( "DirectX12Interface::Device->CreateCommandList" );

			D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffers;
			DescriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			DescriptorBackBuffers.NumDescriptors = DirectX12Interface::BuffersCounts;
			DescriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			DescriptorBackBuffers.NodeMask = 1;
			//Log::DirectX( "D3D12_DESCRIPTOR_HEAP_DESC" );

			if (DirectX12Interface::Device->CreateDescriptorHeap( &DescriptorBackBuffers, IID_PPV_ARGS( &DirectX12Interface::DescriptorHeapBackBuffers ) ) != S_OK)
				return oPresent( pSwapChain, SyncInterval, Flags );
			//Log::DirectX( "DirectX12Interface::Device->CreateDescriptorHeap" );

			const auto RTVDescriptorSize = DirectX12Interface::Device->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_RTV );
			D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = DirectX12Interface::DescriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart();
			//Log::DirectX( "DirectX12Interface::DescriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart" );

			for (size_t i = 0; i < DirectX12Interface::BuffersCounts; i++)
			{
				ID3D12Resource *pBackBuffer = nullptr;
				DirectX12Interface::FrameContext[i].DescriptorHandle = RTVHandle;
				pSwapChain->GetBuffer( i, IID_PPV_ARGS( &pBackBuffer ) );
				DirectX12Interface::Device->CreateRenderTargetView( pBackBuffer, nullptr, RTVHandle );
				DirectX12Interface::FrameContext[i].Resource = pBackBuffer;
				RTVHandle.ptr += RTVDescriptorSize;
				//Log::DirectX( "DirectX12Interface::FrameContext: %x", i);
			}

			RECT rect;
			if (GetWindowRect( Desc.OutputWindow, &rect ))
			{

				int width = rect.right - rect.left;
				int height = rect.bottom - rect.top;
				render::window = render::window_size( width, height );
			}

			ImGui_ImplWin32_Init( Process::Hwnd );
			//Log::ImGui( "ImGui_ImplWin32_Init" );

			ImGui_ImplDX12_Init( DirectX12Interface::Device, DirectX12Interface::BuffersCounts, DXGI_FORMAT_R8G8B8A8_UNORM, DirectX12Interface::DescriptorHeapImGuiRender, DirectX12Interface::DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(), DirectX12Interface::DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart() );
			//Log::ImGui( "ImGui_ImplDX12_Init" );

			ImGui_ImplDX12_CreateDeviceObjects();
			//Log::ImGui( "ImGui_ImplDX12_CreateDeviceObjects" );

			ImGui::GetIO().ImeWindowHandle = Process::Hwnd;
			font = io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\Arial.ttf", 13.0f );
			//Log::ImGui( "Font: Arial.ttf, 13.0f" );

			Process::WndProc = (WNDPROC)SetWindowLongPtr( Process::Hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc );
			ImGui_Initialised = true;

			//Log::DirectX( "DirectX12 initialized successfully" );
		}
		//else
			//Log::DirectX( "DirectX12 could not be initialized successfully" );
	}

	if (DirectX12Interface::CommandQueue == nullptr)
		return oPresent( pSwapChain, SyncInterval, Flags );


	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	entity();
	//ImGui::ShowDemoWindow();

	ImGui::EndFrame();

	DirectX12Interface::_FrameContext &CurrentFrameContext = DirectX12Interface::FrameContext[pSwapChain->GetCurrentBackBufferIndex()];
	CurrentFrameContext.CommandAllocator->Reset();

	D3D12_RESOURCE_BARRIER Barrier;
	Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	Barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	Barrier.Transition.pResource = CurrentFrameContext.Resource;
	Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	DirectX12Interface::CommandList->Reset( CurrentFrameContext.CommandAllocator, nullptr );
	DirectX12Interface::CommandList->ResourceBarrier( 1, &Barrier );
	DirectX12Interface::CommandList->OMSetRenderTargets( 1, &CurrentFrameContext.DescriptorHandle, FALSE, nullptr );
	DirectX12Interface::CommandList->SetDescriptorHeaps( 1, &DirectX12Interface::DescriptorHeapImGuiRender );

	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData( ImGui::GetDrawData(), DirectX12Interface::CommandList );
	Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	DirectX12Interface::CommandList->ResourceBarrier( 1, &Barrier );
	DirectX12Interface::CommandList->Close();
	DirectX12Interface::CommandQueue->ExecuteCommandLists( 1, reinterpret_cast<ID3D12CommandList *const *>(&DirectX12Interface::CommandList) );
	return oPresent( pSwapChain, SyncInterval, Flags );
}

//=========================================================================================================================//

void hkExecuteCommandLists( ID3D12CommandQueue *queue, UINT NumCommandLists, ID3D12CommandList *ppCommandLists )
{
	if (!DirectX12Interface::CommandQueue)
		DirectX12Interface::CommandQueue = queue;

	oExecuteCommandLists( queue, NumCommandLists, ppCommandLists );
}

//=========================================================================================================================//

void APIENTRY hkDrawInstanced( ID3D12GraphicsCommandList *dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation )
{

	return oDrawInstanced( dCommandList, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation );
}

//=========================================================================================================================//

void APIENTRY hkDrawIndexedInstanced( ID3D12GraphicsCommandList *dCommandList, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation )
{
	return oDrawIndexedInstanced( dCommandList, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation );
}

auto Swapchain() -> bool
{
	/* Find and store all process info */
	Process::ID = GetCurrentProcessId( );
	Process::Handle = GetCurrentProcess( );
	Process::Hwnd = FindWindow( "Battlefield™ 2042", NULL );

	bool InitializeHook = false;

	while (InitializeHook == false)
	{
		if (DirectX12::Init() == true)
		{
			CreateHook( 54, (void **)&oExecuteCommandLists, hkExecuteCommandLists );
			CreateHook( 140, (void **)&oPresent, hkPresent );
			CreateHook( 84, (void **)&oDrawInstanced, hkDrawInstanced );
			CreateHook( 85, (void **)&oDrawIndexedInstanced, hkDrawIndexedInstanced );
			//Log::DirectX( "All hooks have been initialized" );
			InitializeHook = true;
		}
	}
	return 0;
}

auto DllMain( void *, std::uint32_t call_reason, void * ) -> bool
{
	if ( call_reason != 1 )
		return false;

	AllocConsole( );
	freopen( "conout$", "w", stdout );
	freopen( "conin$", "r", stdin );
	
	std::thread( Swapchain ).detach();
	//std::thread( entity ).detach();
}