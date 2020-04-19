#include "VirtualizedInput.h"

#define WIN32_LEAN_AND_MEAN
//#define _WIN32_WINNT 0x0500

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <vector>

#include <chrono>
#include <thread>

namespace Input
{
	// TODO - Make this dynamic
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

	INPUT g_input;

	void MouseSetup()
	{
		g_input.type = INPUT_MOUSE;
		g_input.mi.dx = (0 * (0xFFFF / SCREEN_WIDTH));
		g_input.mi.dy = (0 * (0xFFFF / SCREEN_HEIGHT));
		g_input.mi.mouseData = 0;
		g_input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
		g_input.mi.time = 0;
		g_input.mi.dwExtraInfo = 0;
	}

	void MoveMouse(int aX, int aY)
	{
		MouseSetup();

		std::vector<int32_t> xs;
		std::vector<int32_t> ys;

		POINT initialPos;
		if (GetCursorPos(&initialPos))
		{
			float stepCount = 6.f;
			int32_t stepsX = int32_t(fabsf(aX - initialPos.x) / stepCount);
			int32_t stepsY = int32_t(fabsf(aY - initialPos.y) / stepCount);

			int32_t steps = stepsX > stepsY ? stepsX : stepsY;

			if (steps > 0)
			{
				for (int32_t i = 0; i <= steps; ++i)
				{
					xs.push_back(initialPos.x + ((aX - initialPos.x) / steps * i));
					ys.push_back(initialPos.y + ((aY - initialPos.y) / steps * i));
				}
			}
		}

		xs.push_back(aX);
		ys.push_back(aY);

		for (int i = 0; i < xs.size(); ++i)
		{
			g_input.mi.dx = (xs[i] * (0xFFFF / SCREEN_WIDTH));
			g_input.mi.dy = (ys[i] * (0xFFFF / SCREEN_HEIGHT));
			g_input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);

			SendInput(1, &g_input, sizeof(INPUT));

			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1ms);
		}
	}

	void LeftClick(int aX, int aY)
	{
		MouseSetup();
		MoveMouse(aX, aY);
		g_input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN);
		SendInput(1, &g_input, sizeof(INPUT));

		Sleep(25);

		g_input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP);
		SendInput(1, &g_input, sizeof(INPUT));
	}

	void LeftDrag(int aStartX, int aStartY, int aEndX, int aEndY)
	{
		// Setup
		MouseSetup();

		// Move to start
		MoveMouse(aStartX, aStartY);

		Sleep(150);

		// Click
		g_input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN);
		SendInput(1, &g_input, sizeof(INPUT));
		Sleep(150);

		// Move
		MoveMouse(aEndX, aEndY);

		Sleep(150);

		// Release
		g_input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP);
		SendInput(1, &g_input, sizeof(INPUT));
	}

	void RightClick(int aX, int aY)
	{
		MouseSetup();
		MoveMouse(aX, aY);
		g_input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN);
		SendInput(1, &g_input, sizeof(INPUT));

		Sleep(10);

		g_input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP);
		SendInput(1, &g_input, sizeof(INPUT));
	}

	void Type(const std::string& acString)
	{
		for (auto i = 0; i < acString.size(); ++i)
		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(35ms);

			if (acString[i] >= 'A' && acString[i] <= 'Z')
			{
				PressKey(acString[i]);
				continue;
			}

			if (acString[i] >= 'a' && acString[i] <= 'z')
			{
				PressKey(acString[i] - 0x20);
				continue;
			}

			switch (acString[i])
			{
			case '\n': PressKey(Key::Enter);	break;
			case ' ': PressKey(Key::Spacebar);	break;
			case '-': PressKey(Key::Dash);	break;
			case '+': PressKey(Key::Plus);	break;
			case '.': PressKey(Key::Period);	break;
			case ',': PressKey(Key::Comma);	break;
			case '\'': PressKey(Key::SingleQuote);	break;
			case '"': PressKey(Key::SingleQuote);	break; // Actually a SingleQuote but whatever
			};
		}

		//char a = 'a';
		//Log::Info(std::to_string(uint32_t(a))); // 91 dec - 0x61 hex
	}

	void PressKey(WORD aKey)
	{
		// VK_F12; see https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		g_input.type = INPUT_KEYBOARD;
		g_input.ki.wScan = MapVirtualKey(aKey, MAPVK_VK_TO_VSC);
		g_input.ki.time = 0;
		g_input.ki.dwExtraInfo = 0;
		g_input.ki.wVk = aKey;
		g_input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
		SendInput(1, &g_input, sizeof(INPUT));

		g_input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &g_input, sizeof(INPUT));
	}

	void PressKeyDown(WORD aKey)
	{
		g_input.type = INPUT_KEYBOARD;
		g_input.ki.wScan = MapVirtualKey(aKey, MAPVK_VK_TO_VSC);
		g_input.ki.time = 0;
		g_input.ki.dwExtraInfo = 0;
		g_input.ki.wVk = aKey;
		g_input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
		SendInput(1, &g_input, sizeof(INPUT));
	}

	void PressKeyUp(WORD aKey)
	{
		g_input.type = INPUT_KEYBOARD;
		g_input.ki.wScan = MapVirtualKey(aKey, MAPVK_VK_TO_VSC);
		g_input.ki.time = 0;
		g_input.ki.dwExtraInfo = 0;
		g_input.ki.wVk = aKey;
		g_input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &g_input, sizeof(INPUT));
	}

}
