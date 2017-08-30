#pragma once
#include <d3dx9.h>
#include <string>
#include <iostream>

#include "OGEVertex.h"
#include "OGECube.h"
#include "OGEGround.h"
#include "OGECamera.h"

using namespace std;




// Graphics class
class OGEGraphics
{
	private:
		static IDirect3D9*				m_Direct3D;
		static IDirect3DDevice9*		m_Direct3DDevice;
		static IDirect3DCubeTexture9*	m_Direct3DTextureReflect;
		static IDirect3DTexture9*		m_Direct3DTexture1;
		static IDirect3DTexture9*		m_Direct3DTexture1_Specular;
		static IDirect3DTexture9*		m_Direct3DTexture1_Normal;
		static IDirect3DTexture9*		m_Direct3DTexture2;
		static IDirect3DTexture9*		m_Direct3DTexture2_Specular;
		static IDirect3DTexture9*		m_Direct3DTexture2_Normal;
		static ID3DXSprite*				m_Direct3DSprite;
		static ID3DXFont*				m_Direct3dFont;
		static ID3DXEffect*				m_Direct3dEffect;
		static OGECube*					m_Box;
		static OGEGround*				m_Ground;
		static OGECamera*				m_Camera;

		static D3DXMATRIX				m_ProjectionMatrix;
		static D3DXMATRIX				m_ViewMatrix;

		static D3DXVECTOR3				m_LightVec;
		static D3DXCOLOR				m_DiffuseLight;
		static D3DXCOLOR				m_AmbientLight;
		static D3DXCOLOR				m_SpecularLight;
		static float					m_SpecularPower;

		static D3DXCOLOR				m_DiffuseMtrl;
		static D3DXCOLOR				m_AmbientMtrl;
		static D3DXCOLOR				m_SpecularMtrl;



	public:
		// Utils block
		static bool Init(HWND hwnd, bool windowed, int width, int height);
		static void Release();

		// Rendering block
		static void Clear(long color, float zBuffer);
		static void BeginScene();
		static void EndScene();
		static void Display();

		// Test block
		static void Test_Setup();
		static void Test_DrawCube(float timeDelta);
		static void Test_DrawGround(float timeDelta);
		static void Test_DrawSprite(float timeDelta);
		static void Test_DrawText(std::string text, int x1, int y1, int x2, int y2, D3DCOLOR color, float timeDelta);
		static void Test_SetCameraPosition(float x, float y, float z);
		static void Test_SetCameraRotation(float rx, float ry, float rz);
		static void Test_MoveCamera(float x, float y, float z);
		static void Test_RotateCamera(float rx, float ry, float rz);
		static void Test_UpdateView();
};

