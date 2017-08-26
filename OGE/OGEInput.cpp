#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include "OGEInput.h"




// Members
IDirectInput8*			OGEInput::m_DirectInput;
IDirectInputDevice8*	OGEInput::m_Keyboard;
char					OGEInput::m_KeyboardState[256];
IDirectInputDevice8*	OGEInput::m_Mouse;
DIMOUSESTATE2			OGEInput::m_MouseState;




// Init
bool OGEInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// Clear states
	ZeroMemory(m_KeyboardState, sizeof(m_KeyboardState));
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));


	// Create DirectInput8 interface
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInput, 0))) return false;


	// Create keyboard
	if (FAILED(m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_Keyboard, 0))) return false;
	if (FAILED(m_Keyboard->SetDataFormat(&c_dfDIKeyboard))) return false;
	if (FAILED(m_Keyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) return false;
	if (FAILED(m_Keyboard->Acquire())) return false;


	// Create mouse
	if (FAILED(m_DirectInput->CreateDevice(GUID_SysMouse, &m_Mouse, 0))) return false;
	if (FAILED(m_Mouse->SetDataFormat(&c_dfDIMouse2))) return false;
	if (FAILED(m_Mouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) return false;
	if (FAILED(m_Mouse->Acquire())) return false;


	return true;
}




// Release
void OGEInput::Release()
{
	if (m_Mouse)
	{
		m_Mouse->Unacquire();
		m_Mouse->Release();
		m_Mouse = NULL;
	}
	if (m_Keyboard)
	{
		m_Keyboard->Unacquire();
		m_Keyboard->Release();
		m_Keyboard = NULL;
	}
	if (m_DirectInput)
	{
		m_DirectInput->Release();
		m_DirectInput = NULL;
	}
}




// Check key
bool OGEInput::KeyDown(char key)
{
	return (m_KeyboardState[key] & 0x80);
}




// Check mouse button
bool OGEInput::MouseButtonDown(int button)
{
	return (m_MouseState.rgbButtons[button] & 0x80) != 0;
}




// Mouse deltaX
float OGEInput::MouseDX()
{
	return (float)m_MouseState.lX;
}




// Mouse deltaY
float OGEInput::MouseDY()
{
	return (float)m_MouseState.lY;
}




// Mouse deltaZ
float OGEInput::MouseDZ()
{
	return (float)m_MouseState.lZ;
}




// Update inputs states
void OGEInput::UpdateDevices()
{
	// Update keyboard
	if (FAILED(m_Keyboard->GetDeviceState(sizeof(m_KeyboardState), (void**)&m_KeyboardState)))
	{
		// Keyboard lost, zero out keyboard data structure.
		ZeroMemory(m_KeyboardState, sizeof(m_KeyboardState));

		// Try to acquire for next time we poll.
		m_Keyboard->Acquire();
	}

	// Poll mouse.
	if (FAILED(m_Mouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&m_MouseState)))
	{
		// Mouse lost, zero out mouse data structure.
		ZeroMemory(&m_MouseState, sizeof(m_MouseState));

		// Try to acquire for next time we poll.
		m_Mouse->Acquire();
	}
}