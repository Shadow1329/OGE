#pragma once
#include <d3dx9.h>




//Struct of Camera
class OGECamera
{
private:
	D3DXVECTOR3 m_Right;
	D3DXVECTOR3 m_Up;
	D3DXVECTOR3 m_Look;
	D3DXVECTOR3 m_Pos;


public:
	OGECamera();

	void Walk(float units);
	void Strafe(float units);
	void Fly(float units);
	void Pitch(float angle);
	void Yaw(float angle);
	void Roll(float angle);

	void SetPosition(float x, float y, float z);
	D3DXVECTOR3 GetPosition();

	void GetViewMatrix(D3DXMATRIX* matrix);
};

