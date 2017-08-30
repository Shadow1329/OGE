#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include "OGEGraphics.h"




// Members
IDirect3D9*				OGEGraphics::m_Direct3D = NULL;
IDirect3DDevice9*		OGEGraphics::m_Direct3DDevice = NULL;
IDirect3DCubeTexture9*	OGEGraphics::m_Direct3DTextureReflect = NULL;
IDirect3DTexture9*		OGEGraphics::m_Direct3DTexture1 = NULL;
IDirect3DTexture9*		OGEGraphics::m_Direct3DTexture1_Specular = NULL;
IDirect3DTexture9*		OGEGraphics::m_Direct3DTexture1_Normal = NULL;
IDirect3DTexture9*		OGEGraphics::m_Direct3DTexture2 = NULL;
IDirect3DTexture9*		OGEGraphics::m_Direct3DTexture2_Specular = NULL;
IDirect3DTexture9*		OGEGraphics::m_Direct3DTexture2_Normal = NULL;
ID3DXSprite*			OGEGraphics::m_Direct3DSprite = NULL;
ID3DXFont*				OGEGraphics::m_Direct3dFont = NULL;
ID3DXEffect*			OGEGraphics::m_Direct3dEffect = NULL;
OGECube*				OGEGraphics::m_Box = NULL;
OGEGround*				OGEGraphics::m_Ground = NULL;
OGECamera*				OGEGraphics::m_Camera = NULL;

D3DXMATRIX				OGEGraphics::m_ProjectionMatrix;
D3DXMATRIX				OGEGraphics::m_ViewMatrix;

D3DXVECTOR3				OGEGraphics::m_LightVec;
D3DXCOLOR				OGEGraphics::m_DiffuseLight;
D3DXCOLOR				OGEGraphics::m_AmbientLight;
D3DXCOLOR				OGEGraphics::m_SpecularLight;
float					OGEGraphics::m_SpecularPower;

D3DXCOLOR				OGEGraphics::m_DiffuseMtrl;
D3DXCOLOR				OGEGraphics::m_AmbientMtrl;
D3DXCOLOR				OGEGraphics::m_SpecularMtrl;




// Init
bool OGEGraphics::Init(HWND hwnd, bool windowed, int width, int height)
{
	// Release existing resources
	Release();


	// Check that window is not null
	if (hwnd == NULL) return false;


	// Step 1: Create IDirect3D9 object
	m_Direct3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_Direct3D == NULL) return false;


	// Step 2: Check device capabilities
	D3DCAPS9 caps;
	m_Direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	// Check for hardware vertex processing
	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Check for vertex shader version 2.0 support.
	if (caps.VertexShaderVersion < D3DVS_VERSION(3, 0))
		return false;

	// Check for pixel shader version 2.0 support.
	if (caps.PixelShaderVersion < D3DPS_VERSION(3, 0))
		return false;



	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = windowed;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


	// Step 4: Create the device.
	if (FAILED(m_Direct3D->CreateDevice(
		D3DADAPTER_DEFAULT,		// primary adapter
		D3DDEVTYPE_HAL,			// device type
		hwnd,					// window associated with device
		vp,						// vertex processing type
		&d3dpp,					// present parameters
		&m_Direct3DDevice		// return created device
		))) return false;


	// Step 5: Create transform matrices
	// Set projection matrix
	D3DXMatrixPerspectiveFovLH(
		&m_ProjectionMatrix,
		D3DX_PI * 0.5f,
		(float)width / (float)height,
		0.1f,
		1000.0f);


	// Step 6: Set render states
	m_Direct3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);			// Z-Buffer enabled
	m_Direct3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);			// Set nomalizing of normals

	
	return true;
}




// Setup
void OGEGraphics::Test_Setup()
{
	// Create light
	m_LightVec = D3DXVECTOR3(0.0, 0.7f, -0.7f);
	m_DiffuseLight = D3DXCOLOR(0.7f, 0.6f, 0.5f, 1.0f);
	m_AmbientLight = D3DXCOLOR(0.7f, 0.6f, 0.5f, 1.0f);
	m_SpecularLight = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpecularPower = 5.0f;


	// Create material
	m_DiffuseMtrl = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_AmbientMtrl = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpecularMtrl = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


	// Create textures
	D3DXCreateCubeTextureFromFile(m_Direct3DDevice, L"Data/Textures/Grass.dds", &m_Direct3DTextureReflect);
	D3DXCreateTextureFromFile(m_Direct3DDevice, L"Data/Textures/Floor1.png" ,&m_Direct3DTexture1);
	D3DXCreateTextureFromFile(m_Direct3DDevice, L"Data/Textures/Floor1_Specular.png", &m_Direct3DTexture1_Specular);
	D3DXCreateTextureFromFile(m_Direct3DDevice, L"Data/Textures/Floor1_Normal.png", &m_Direct3DTexture1_Normal);
	D3DXCreateTextureFromFile(m_Direct3DDevice, L"Data/Textures/Floor.png", &m_Direct3DTexture2);
	D3DXCreateTextureFromFile(m_Direct3DDevice, L"Data/Textures/Floor_Specular.png", &m_Direct3DTexture2_Specular);
	D3DXCreateTextureFromFile(m_Direct3DDevice, L"Data/Textures/Floor_Normal.png", &m_Direct3DTexture2_Normal);
	
	


	// Create sprite
	D3DXCreateSprite(m_Direct3DDevice, &m_Direct3DSprite);


	// Create font
	D3DXCreateFontA(m_Direct3DDevice, 30, 10, 1, 0, FALSE, 0, 30, 0, 0, "Arial", &m_Direct3dFont);


	// Create effects
	ID3DXBuffer* errors = 0;
	D3DXCreateEffectFromFile(m_Direct3DDevice, L"Data/Shaders/Test.fx", 0, 0, D3DXSHADER_DEBUG, 0, &m_Direct3dEffect, &errors);
	if (errors)
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0);


	// Init vertex declaration
	OGEVertex::InitVertexDeclaration(m_Direct3DDevice);


	// Create cube
	m_Box = new OGECube(m_Direct3DDevice);


	// Create ground
	m_Ground = new OGEGround(m_Direct3DDevice);


	// Create camera
	m_Camera = new OGECamera();
}




// Release
void OGEGraphics::Release()
{
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = NULL;
	}

	if (m_Ground)
	{
		delete m_Ground;
		m_Ground = NULL;
	}

	if (m_Box)
	{
		delete m_Box;
		m_Box = NULL;
	}

	OGEVertex::ReleaseVertexDeclaration();

	if (m_Direct3dFont)
	{
		m_Direct3dFont->Release();
		m_Direct3dFont = NULL;
	}

	if (m_Direct3DSprite)
	{
		m_Direct3DSprite->Release();
		m_Direct3DSprite = NULL;
	}

	if (m_Direct3DTexture2_Normal)
	{
		m_Direct3DTexture2_Normal->Release();
		m_Direct3DTexture2_Normal = NULL;
	}

	if (m_Direct3DTexture1_Normal)
	{
		m_Direct3DTexture1_Normal->Release();
		m_Direct3DTexture1_Normal = NULL;
	}

	if (m_Direct3DTexture2_Specular)
	{
		m_Direct3DTexture2_Specular->Release();
		m_Direct3DTexture2_Specular = NULL;
	}

	if (m_Direct3DTexture1_Specular)
	{
		m_Direct3DTexture1_Specular->Release();
		m_Direct3DTexture1_Specular = NULL;
	}

	if (m_Direct3DTexture2)
	{
		m_Direct3DTexture2->Release();
		m_Direct3DTexture2 = NULL;
	}

	if (m_Direct3DTexture1)
	{
		m_Direct3DTexture1->Release();
		m_Direct3DTexture1 = NULL;
	}

	if (m_Direct3DTextureReflect)
	{
		m_Direct3DTextureReflect->Release();
		m_Direct3DTextureReflect = NULL;
	}

	if (m_Direct3DDevice) 
	{ 
		m_Direct3DDevice->Release();
		m_Direct3DDevice = NULL;
	}

	if (m_Direct3D) 
	{ 
		m_Direct3D->Release();
		m_Direct3D = NULL;
	}
}




// Clear color and z-buffer
void OGEGraphics::Clear(long color, float zBuffer)
{
	if (m_Direct3DDevice)
	{
		// Clear display and zbuffer
		m_Direct3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, zBuffer, 0);
	}
}




// Begin drawing scene
void OGEGraphics::BeginScene()
{
	if (m_Direct3DDevice)
	{
		m_Direct3DDevice->BeginScene();
	}
}




// Test draw cube
void OGEGraphics::Test_DrawCube(float timeDelta)
{
	if (m_Direct3DDevice)
	{
		D3DXMATRIX World;

		// Rotate Y-axis
		static float y = 0.0f;
		D3DXMatrixRotationY(&World, y);
		//y += timeDelta;
		if (y >= 6.28f)
			y = 0.0f;


		D3DXMATRIX WorldInverseTranspose;
		D3DXMatrixIdentity(&WorldInverseTranspose);
		//D3DXMatrixInverse(&WorldInverseTranspose, 0, &World);
		//D3DXMatrixTranspose(&WorldInverseTranspose, &WorldInverseTranspose);


		D3DXVECTOR3 EyePos = m_Camera->GetPosition();


		// Setup the rendering FX
		D3DXHANDLE hTech = m_Direct3dEffect->GetTechniqueByName("PhongTech");
		D3DXHANDLE hWorld = m_Direct3dEffect->GetParameterByName(0, "gWorld");
		D3DXHANDLE hView = m_Direct3dEffect->GetParameterByName(0, "gView");
		D3DXHANDLE hProjection = m_Direct3dEffect->GetParameterByName(0, "gProjection");
		D3DXHANDLE hWorldInvTrans = m_Direct3dEffect->GetParameterByName(0, "gWorldInverseTranspose");
		D3DXHANDLE hEyePos = m_Direct3dEffect->GetParameterByName(0, "gEyePosisiton");
		D3DXHANDLE hTexReflect = m_Direct3dEffect->GetParameterByName(0, "gTexReflect");
		D3DXHANDLE hTexDiffuse = m_Direct3dEffect->GetParameterByName(0, "gTexDiffuse");
		D3DXHANDLE hTexSpecular = m_Direct3dEffect->GetParameterByName(0, "gTexSpecular");
		D3DXHANDLE hTexNormal = m_Direct3dEffect->GetParameterByName(0, "gTexNormal");
		D3DXHANDLE hDiffuseMtrl = m_Direct3dEffect->GetParameterByName(0, "gDiffuseMtrl");
		D3DXHANDLE hDiffuseLight = m_Direct3dEffect->GetParameterByName(0, "gDiffuseLight");
		D3DXHANDLE hAmbientMtrl = m_Direct3dEffect->GetParameterByName(0, "gAmbientMtrl");
		D3DXHANDLE hAmbientLight = m_Direct3dEffect->GetParameterByName(0, "gAmbientLight");
		D3DXHANDLE hSpecularMtrl = m_Direct3dEffect->GetParameterByName(0, "gSpecularMtrl");
		D3DXHANDLE hSpecularLight = m_Direct3dEffect->GetParameterByName(0, "gSpecularLight");
		D3DXHANDLE hSpecularPower = m_Direct3dEffect->GetParameterByName(0, "gSpecularPower");
		D3DXHANDLE hLightVec = m_Direct3dEffect->GetParameterByName(0, "gLightVec");
		m_Direct3dEffect->SetTechnique(hTech);
		m_Direct3dEffect->SetMatrix(hWorld, &World);
		m_Direct3dEffect->SetMatrix(hView, &m_ViewMatrix);
		m_Direct3dEffect->SetMatrix(hProjection, &m_ProjectionMatrix);
		m_Direct3dEffect->SetMatrix(hWorldInvTrans, &WorldInverseTranspose);
		m_Direct3dEffect->SetValue(hEyePos, &EyePos, sizeof(D3DXVECTOR3));
		m_Direct3dEffect->SetValue(hDiffuseMtrl, &m_DiffuseMtrl, sizeof(D3DXCOLOR));
		m_Direct3dEffect->SetValue(hDiffuseLight, &m_DiffuseLight, sizeof(D3DXCOLOR));
		m_Direct3dEffect->SetValue(hAmbientMtrl, &m_AmbientMtrl, sizeof(D3DXCOLOR));
		m_Direct3dEffect->SetValue(hAmbientLight, &m_AmbientLight, sizeof(D3DXCOLOR));
		m_Direct3dEffect->SetValue(hSpecularLight, &m_SpecularLight, sizeof(D3DXCOLOR));
		m_Direct3dEffect->SetValue(hSpecularMtrl, &m_SpecularMtrl, sizeof(D3DXCOLOR));
		m_Direct3dEffect->SetFloat(hSpecularPower, m_SpecularPower);
		m_Direct3dEffect->SetValue(hLightVec, &m_LightVec, sizeof(D3DXVECTOR3));
		m_Direct3dEffect->SetTexture(hTexReflect, m_Direct3DTextureReflect);
		m_Direct3dEffect->SetTexture(hTexDiffuse, m_Direct3DTexture1);
		m_Direct3dEffect->SetTexture(hTexSpecular, m_Direct3DTexture1_Specular);
		m_Direct3dEffect->SetTexture(hTexNormal, m_Direct3DTexture1_Normal);

		// Begin passes
		UINT numPasses = 0;
		m_Direct3dEffect->Begin(&numPasses, 0);
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_Direct3dEffect->BeginPass(i);

			// Draw
			m_Box->Draw(m_Direct3DDevice);

			m_Direct3dEffect->EndPass();
		}
		m_Direct3dEffect->End();
	}
}




// Test draw ground
void OGEGraphics::Test_DrawGround(float timeDelta)
{
	if (m_Direct3DDevice)
	{
		D3DXMATRIX Ty;
		D3DXMATRIX Sy;
		D3DXMATRIX World;

		// Transform
		D3DXMatrixScaling(&Sy, 10, 1, 10);
		D3DXMatrixTranslation(&Ty, 0, -1, 0);
		World = Sy * Ty;


		D3DXMATRIX WorldInverseTranspose;
		D3DXMatrixIdentity(&WorldInverseTranspose);
		//D3DXMatrixInverse(&WorldInverseTranspose, 0, &World);
		//D3DXMatrixTranspose(&WorldInverseTranspose, &WorldInverseTranspose);


		D3DXVECTOR3 EyePos = m_Camera->GetPosition();


		// Setup the rendering FX
		D3DXHANDLE hTech = m_Direct3dEffect->GetTechniqueByName("PhongTech");
		D3DXHANDLE hWorld = m_Direct3dEffect->GetParameterByName(0, "gWorld");
		D3DXHANDLE hView = m_Direct3dEffect->GetParameterByName(0, "gView");
		D3DXHANDLE hProjection = m_Direct3dEffect->GetParameterByName(0, "gProjection");
		D3DXHANDLE hWorldInvTrans = m_Direct3dEffect->GetParameterByName(0, "gWorldInverseTranspose");
		D3DXHANDLE hEyePos = m_Direct3dEffect->GetParameterByName(0, "gEyePosisiton");
		D3DXHANDLE hTexReflect = m_Direct3dEffect->GetParameterByName(0, "gTexReflect");
		D3DXHANDLE hTexDiffuse = m_Direct3dEffect->GetParameterByName(0, "gTexDiffuse");
		D3DXHANDLE hTexSpecular = m_Direct3dEffect->GetParameterByName(0, "gTexSpecular");
		D3DXHANDLE hTexNormal = m_Direct3dEffect->GetParameterByName(0, "gTexNormal");
		D3DXHANDLE hDiffuseMtrl = m_Direct3dEffect->GetParameterByName(0, "gDiffuseMtrl");
		D3DXHANDLE hDiffuseLight = m_Direct3dEffect->GetParameterByName(0, "gDiffuseLight");
		D3DXHANDLE hAmbientMtrl = m_Direct3dEffect->GetParameterByName(0, "gAmbientMtrl");
		D3DXHANDLE hAmbientLight = m_Direct3dEffect->GetParameterByName(0, "gAmbientLight");
		D3DXHANDLE hSpecularMtrl = m_Direct3dEffect->GetParameterByName(0, "gSpecularMtrl");
		D3DXHANDLE hSpecularLight = m_Direct3dEffect->GetParameterByName(0, "gSpecularLight");
		D3DXHANDLE hSpecularPower = m_Direct3dEffect->GetParameterByName(0, "gSpecularPower");
		D3DXHANDLE hLightVec = m_Direct3dEffect->GetParameterByName(0, "gLightVec");
		m_Direct3dEffect->SetTechnique(hTech);
		m_Direct3dEffect->SetMatrix(hWorld, &World);
		m_Direct3dEffect->SetMatrix(hView, &m_ViewMatrix);
		m_Direct3dEffect->SetMatrix(hProjection, &m_ProjectionMatrix);
		m_Direct3dEffect->SetMatrix(hWorldInvTrans, &WorldInverseTranspose);
		m_Direct3dEffect->SetValue(hEyePos, &EyePos, sizeof(D3DXVECTOR3));
		m_Direct3dEffect->SetValue(hDiffuseMtrl, &m_DiffuseMtrl, sizeof(D3DXCOLOR));
		m_Direct3dEffect->SetValue(hDiffuseLight, &m_DiffuseLight, sizeof(D3DXCOLOR));
		m_Direct3dEffect->SetValue(hAmbientMtrl, &m_AmbientMtrl, sizeof(D3DXCOLOR));
		m_Direct3dEffect->SetValue(hAmbientLight, &m_AmbientLight, sizeof(D3DXCOLOR));
		m_Direct3dEffect->SetValue(hSpecularLight, &m_SpecularLight, sizeof(D3DXCOLOR));
		m_Direct3dEffect->SetValue(hSpecularMtrl, &m_SpecularMtrl, sizeof(D3DXCOLOR));
		m_Direct3dEffect->SetFloat(hSpecularPower, m_SpecularPower);
		m_Direct3dEffect->SetValue(hLightVec, &m_LightVec, sizeof(D3DXVECTOR3));
		m_Direct3dEffect->SetTexture(hTexReflect, m_Direct3DTextureReflect);
		m_Direct3dEffect->SetTexture(hTexDiffuse, m_Direct3DTexture2);
		m_Direct3dEffect->SetTexture(hTexSpecular, m_Direct3DTexture2_Specular);
		m_Direct3dEffect->SetTexture(hTexNormal, m_Direct3DTexture2_Normal);

		// Begin passes
		UINT numPasses = 0;
		m_Direct3dEffect->Begin(&numPasses, 0);
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_Direct3dEffect->BeginPass(i);

			// Draw
			m_Ground->Draw(m_Direct3DDevice);

			m_Direct3dEffect->EndPass();
		}
		m_Direct3dEffect->End();
	}
}




// Test draw sprite
void OGEGraphics::Test_DrawSprite(float timeDelta)
{
	if (m_Direct3DDevice)
	{
		D3DXMATRIX T;
		D3DXMatrixTranslation(&T, 0, 0, 2);
		m_Direct3DDevice->SetTransform(D3DTS_WORLD, &T);

		m_Direct3DSprite->Begin(D3DXSPRITE_OBJECTSPACE | D3DXSPRITE_DONOTMODIFY_RENDERSTATE);

		m_Direct3DSprite->SetTransform(&T);
		m_Direct3DSprite->Draw(m_Direct3DTexture1, NULL, &D3DXVECTOR3(128, 128, 0), NULL, D3DCOLOR_XRGB(255, 255, 255));
		m_Direct3DSprite->Flush();

		m_Direct3DSprite->End();

	}
}




// Test draw text
void OGEGraphics::Test_DrawText(std::string text, int x1, int y1, int x2, int y2, D3DCOLOR color, float timeDelta)
{
	if (m_Direct3DDevice)
	{
		RECT rect = { x1, y1, x2, y2 };
		m_Direct3dFont->DrawTextA(
			NULL,
			text.c_str(),
			-1,
			&rect,
			DT_TOP | DT_LEFT | DT_WORDBREAK,
			color);
	}
}




// End of drawing scene
void OGEGraphics::EndScene()
{
	if (m_Direct3DDevice)
	{
		m_Direct3DDevice->EndScene();
	}
}




// Display frame
void OGEGraphics::Display()
{
	if (m_Direct3DDevice)
	{
		// View frame
		m_Direct3DDevice->Present(0, 0, 0, 0);
	}
}




// Set camera position
void OGEGraphics::Test_SetCameraPosition(float x, float y, float z)
{
	m_Camera->SetPosition(x, y, z);
}




// Set camera rotation
void OGEGraphics::Test_SetCameraRotation(float rx, float ry, float rz)
{

}




// Move camera
void OGEGraphics::Test_MoveCamera(float x, float y, float z)
{
	m_Camera->Walk(z);
	m_Camera->Strafe(x);
	m_Camera->Fly(y);
}




// Rotate camera
void OGEGraphics::Test_RotateCamera(float rx, float ry, float rz)
{
	m_Camera->Yaw(ry);
	m_Camera->Pitch(rx);
	m_Camera->Roll(rz);
}




// Update view
void OGEGraphics::Test_UpdateView()
{
	m_Camera->GetViewMatrix(&m_ViewMatrix);
}