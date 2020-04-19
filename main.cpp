// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <cstdint>
#include <iostream>
#include <string>       // std::wstring
#include <sstream>      // std::wostringstream

#include "VirtualizedInput.h"

#undef UNICODE
#define UNICODE

int32_t g_screenWidth = 640;
int32_t g_screenHeight = 480;

INPUT g_input;

HWND g_hwnd = NULL;
INPUT_MESSAGE_SOURCE g_inputSource;

HRESULT Initialize(HINSTANCE hInstance);
HRESULT OnRender(HDC hdc, const RECT& rcPaint);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void GetDesktopResolution(int32_t& aWidth, int32_t& aHeight)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	aWidth = desktop.right;
	aHeight = desktop.bottom;
}

_Use_decl_annotations_
int
WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	HRESULT hr = E_FAIL;

	ZeroMemory(&g_inputSource, sizeof(g_inputSource));

	if (SUCCEEDED(hr = Initialize(hInstance)))
	{
		MSG msg;

		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return SUCCEEDED(hr);
}

int main(int aArgc, wchar_t* apArgv[])
{
	GetDesktopResolution(g_screenWidth, g_screenHeight);

	WinMain(
		GetModuleHandle(NULL),
		NULL,
		LPSTR(""),
		0);
}

HRESULT Initialize(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	ATOM atom;

	// Register window class
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT("InputSourceApp");
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	atom = RegisterClassEx(&wcex);

	SetProcessDPIAware();

	// Create window
	g_hwnd = CreateWindow(
		TEXT("InputSourceApp"),
		TEXT("Input Source Identification Sample"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		g_screenWidth,
		g_screenHeight,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (g_hwnd)
	{
		ShowWindow(
			g_hwnd,
			SW_SHOWNORMAL
		);

		UpdateWindow(
			g_hwnd
		);
	}

	return g_hwnd ? S_OK : E_FAIL;
}


HRESULT OnRender(HDC hdc, const RECT& rcPaint)
{
	WCHAR wzText[512];

	FillRect(hdc, &rcPaint, (HBRUSH)GetStockObject(WHITE_BRUSH));

	// Source
	//StringCchCopyW(wzText, ARRAYSIZE(wzText), L"Source: ");
	//std::cout << "Source input ID: " << wzText << std::endl;

	switch (g_inputSource.deviceType)
	{
		case IMDT_UNAVAILABLE:
			Input::MoveMouse(rand() % g_screenWidth, rand() % g_screenHeight);
			break;

		case IMDT_KEYBOARD:
			break;

		case IMDT_MOUSE:
			break;

		case IMDT_TOUCH:
			break;

		case IMDT_PEN:
			break;
		case IMDT_TOUCHPAD:
			break;
	}

	// Origin
	//StringCchCatW(wzText, ARRAYSIZE(wzText), L"Origin: ");
	//std::cout << "Origin: " << wzText << std::endl;

	switch (g_inputSource.originId)
	{
		case IMO_UNAVAILABLE:
			break;

		case IMO_HARDWARE:
			std::cout << "Input came from hardware" << std::endl;
			break;

		case IMO_INJECTED:
			std::cout << "Injected input detected!" << std::endl;
			break;

		case IMO_SYSTEM:
			break;
	}

	DrawText(hdc, wzText, (int)wcslen(wzText), (LPRECT)&rcPaint, DT_TOP | DT_LEFT);

	return S_OK;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if ((message >= WM_MOUSEFIRST && message <= WM_MOUSELAST) ||
		(message >= WM_KEYFIRST && message <= WM_KEYLAST) ||
		(message >= WM_TOUCH && message <= WM_POINTERWHEEL))
	{
		GetCurrentInputMessageSource(&g_inputSource);
		InvalidateRect(g_hwnd, NULL, FALSE);
	}

	switch (message)
	{
		case WM_PAINT:
		case WM_DISPLAYCHANGE:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			OnRender(hdc, ps.rcPaint);
			EndPaint(hwnd, &ps);
		}
		return 0;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		return 1;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
