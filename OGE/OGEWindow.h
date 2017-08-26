#pragma once
#include <Windows.h>
#include <cstdio>




// Window class
class OGEWindow
{
	private:
		static HINSTANCE     m_HInst;
		static HWND          m_HWnd;
		static wchar_t       m_Class[MAX_PATH];
		static wchar_t       m_Caption[MAX_PATH];
		static DWORD         m_Style;
		static DWORD         m_XPos;
		static DWORD         m_YPos;
		static DWORD         m_Width;
		static DWORD         m_Height;
		static WNDCLASSEX    m_WCEx;
		static bool			 (*m_UpdateCallback)(float);

		static long CALLBACK AppWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


	public:
		static bool Init(DWORD x, DWORD y, DWORD width, DWORD height, wchar_t* classname, wchar_t* caption, bool (*updatecallback)(float));
		static void EnterMsgLoop();
		static void Release();

		static HWND      GetHWnd();
		static HINSTANCE GetHInstance();
		static bool ShowMouse(BOOL show);
};
