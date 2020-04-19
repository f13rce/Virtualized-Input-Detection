#pragma once

#include <windows.h>
#include <string>

namespace Input
{
	// Mouse
	void MoveMouse(int aX, int aY);
	void LeftClick(int aX, int aY);
	void LeftDrag(int aStartX, int aStartY, int aEndX, int aEndY);
	void RightClick(int aX, int aY);

	// Keyboard
	enum Key : WORD
	{
		// https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		A = 0x41,
		Backspace = VK_TAB,
		Tab = VK_TAB,
		Enter = VK_RETURN,
		Shift = VK_SHIFT, // Only hits this - doesn't hold it!
		CTRL = VK_CONTROL,
		Alt = VK_MENU,
		Capslock = VK_CAPITAL,
		Spacebar = VK_SPACE,
		Left = VK_LEFT,
		Right = VK_RIGHT,
		Up = VK_UP,
		Down = VK_DOWN,
		Printscreen = VK_SNAPSHOT,
		Delete = VK_DELETE,
		Period = VK_OEM_PERIOD,
		Comma = VK_OEM_COMMA,
		Plus = VK_OEM_PLUS,
		Minus = VK_OEM_MINUS,
		Dash = VK_OEM_MINUS,
		SingleQuote = VK_OEM_7
	};

	void Type(const std::string& acString);
	void PressKey(WORD aKey);
	void PressKeyDown(WORD aKey);
	void PressKeyUp(WORD aKey);
}
