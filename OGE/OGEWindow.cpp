#pragma comment(lib, "winmm.lib")
#include "OGEWindow.h"




// Members
HINSTANCE     OGEWindow::m_HInst;
HWND          OGEWindow::m_HWnd;
wchar_t       OGEWindow::m_Class[MAX_PATH];
wchar_t       OGEWindow::m_Caption[MAX_PATH];
DWORD         OGEWindow::m_Style;
DWORD         OGEWindow::m_XPos;
DWORD         OGEWindow::m_YPos;
DWORD         OGEWindow::m_Width;
DWORD         OGEWindow::m_Height;
WNDCLASSEX    OGEWindow::m_WCEx;
bool		  (*OGEWindow::m_UpdateCallback)(float);




// Constructor
bool OGEWindow::Init(DWORD x, DWORD y, DWORD width, DWORD height, wchar_t* classname, wchar_t* caption, bool(*updatecallback)(float))
{
	// Get the instance handle
	m_HInst = GetModuleHandle(NULL);

	// Set a default window class and caption
	lstrcpyW(m_Class, classname);
	lstrcpyW(m_Caption, caption);

	// Set default window style, position, width, height
	m_Style = WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	m_XPos = x;
	m_YPos = y;
	m_Width = width;
	m_Height = height;
	m_UpdateCallback = updatecallback;

	// Set default WNDCLASSEX structure
	m_WCEx.cbSize = sizeof(WNDCLASSEX);
	m_WCEx.style = CS_HREDRAW | CS_VREDRAW;
	m_WCEx.lpfnWndProc = &OGEWindow::AppWindowProc;
	m_WCEx.cbClsExtra = NULL;
	m_WCEx.cbWndExtra = NULL;
	m_WCEx.hInstance = m_HInst;
	m_WCEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	m_WCEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_WCEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	m_WCEx.lpszMenuName = NULL;
	m_WCEx.lpszClassName = m_Class;
	m_WCEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Register window class
	if (!RegisterClassEx(&m_WCEx))
		return false;

	// Create window
	m_HWnd = CreateWindow(
		m_Class,
		m_Caption,
		m_Style,
		m_XPos, m_YPos,
		m_Width, m_Height,
		NULL, NULL, m_HInst, NULL);
	if (!m_HWnd)
		return false;

	// Show and update window
	ShowWindow(m_HWnd, SW_NORMAL);
	UpdateWindow(m_HWnd);

	return true;
}




// Enter loop
void OGEWindow::EnterMsgLoop()
{
	// Enter loop
	MSG Msg;
	ZeroMemory(&Msg, sizeof(MSG));

	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);
	while (Msg.message != WM_QUIT) 
	{
		// Handle Windows messages (if any)
		if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		else 
		{
			// Calculate delta time
			__int64 currTimeStamp = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			float dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;

			// Do per-frame processing, break on FALSE return value
			if (m_UpdateCallback(dt) == false) break;

			// Remember last time
			prevTimeStamp = currTimeStamp;
		}
	}
}



void OGEWindow::Release()
{
	// Unregister the window class
	UnregisterClass(m_Class, m_HInst);
}




// Get window handle
HWND OGEWindow::GetHWnd()
{
	return m_HWnd;
}




// Get instance handle
HINSTANCE OGEWindow::GetHInstance()
{
	return m_HInst;
}




// Show or hide mouse
bool OGEWindow::ShowMouse(BOOL show)
{
	ShowCursor(show);
	return true;
}




// The message procedure - empty except for destroy message
long CALLBACK OGEWindow::AppWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			break;

		default: 
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}