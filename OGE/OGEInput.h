#pragma once
#include <dinput.h>




// Input class
class OGEInput
{
private:
	static IDirectInput8*		m_DirectInput;

	static IDirectInputDevice8* m_Keyboard;
	static char					m_KeyboardState[256];

	static IDirectInputDevice8* m_Mouse;
	static DIMOUSESTATE2        m_MouseState;


public:
	// Utils block
	static bool Init(HINSTANCE hInstance, HWND hWnd);
	static void Release();

	// Controls block
	static bool KeyDown(char key);
	static bool MouseButtonDown(int button);
	static float MouseDX();
	static float MouseDY();
	static float MouseDZ();
	static void UpdateDevices();
};

