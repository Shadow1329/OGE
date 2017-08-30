#include "OGEApplication.h"




// Run
void OGEApplication::Run()
{
	// Init application
	if (!Init()) return;

	// Enter loop
	EnterLoop();

	// Shutdown application
	Release();
}




// Init
bool OGEApplication::Init()
{
	DWORD width = 800;
	DWORD height = 600;


	// Init window
	if (!OGEWindow::Init(0, 0, width, height, L"OGEClass", L"OGE", &OGEApplication::Update))
	{
		MessageBoxA(0, "Init window - FAILED", 0, 0);
		return false;
	}
	OGEWindow::ShowMouse(true);


	// Init input
	if (!OGEInput::Init(OGEWindow::GetHInstance(), OGEWindow::GetHWnd()))
	{
		MessageBoxA(0, "Init input - FAILED", 0, 0);
		return false;
	}


	// Init graphics
	if (!OGEGraphics::Init(OGEWindow::GetHWnd(), true, width, height))
	{
		MessageBoxA(0, "Init graphics - FAILED", 0, 0);
		return false;
	}
	OGEGraphics::Test_Setup();
	OGEGraphics::Test_SetCameraPosition(0, 1, -5);
	SetCursorPos(640, 480);
	

	return true;
}




// Enter loop
void OGEApplication::EnterLoop()
{
	// Enter window message loop
	OGEWindow::EnterMsgLoop();
}




// Update callback
bool OGEApplication::Update(float timeDelta)
{
	// Update input devices
	OGEInput::UpdateDevices();


	// Do test actions
	float tx = 0;
	float ty = 0;
	float tz = 0;
	float rx = 0;
	float ry = 0;
	float rz = 0;
	float tspeed = 6;
	float rspeed = 0.7;

	if (OGEInput::KeyDown(DIK_ESCAPE))
		return false;

	if (OGEInput::KeyDown(DIK_W))
		tz = tspeed * timeDelta;

	if (OGEInput::KeyDown(DIK_S))
		tz = -tspeed * timeDelta;

	if (OGEInput::KeyDown(DIK_A))
		tx = -tspeed * timeDelta;

	if (OGEInput::KeyDown(DIK_D))
		tx = tspeed * timeDelta;

	if (OGEInput::KeyDown(DIK_SPACE))
		ty = tspeed * timeDelta;

	if (OGEInput::KeyDown(DIK_LSHIFT))
		ty = -tspeed * timeDelta;

	rx = (float)OGEInput::MouseDY() / 200.f;
	ry = (float)OGEInput::MouseDX() / 200.f;

	/*if (OGEInput::KeyDown(VK_UP))
		rx = -rspeed * timeDelta;

	if (OGEInput::KeyDown(VK_DOWN))
		rx = rspeed * timeDelta;

	if (OGEInput::KeyDown(VK_LEFT))
		ry = -rspeed * timeDelta;

	if (OGEInput::KeyDown(VK_RIGHT))
		ry = rspeed * timeDelta;*/

	OGEGraphics::Test_MoveCamera(tx, ty, tz);
	OGEGraphics::Test_RotateCamera(rx, ry, rz);


	// Render
	OGEGraphics::Test_UpdateView();
	OGEGraphics::Clear(0xff000000, 1.0f);
	OGEGraphics::BeginScene();
	OGEGraphics::Test_DrawGround(timeDelta);
	OGEGraphics::Test_DrawCube(timeDelta);
	//OGEGraphics::Test_DrawSprite(timeDelta);
	OGEGraphics::Test_DrawText("Hi!", 0, 0, 200, 100, D3DCOLOR_XRGB(0, 0, 0), timeDelta);
	OGEGraphics::EndScene();
	OGEGraphics::Display();


	return true;
}




// Shutdown
void OGEApplication::Release()
{
	OGEGraphics::Release();
	OGEInput::Release();
	OGEWindow::Release();
}