#include "OGECamera.h"




OGECamera::OGECamera()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}




void OGECamera::SetPosition(float x, float y, float z)
{
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = z;
}




D3DXVECTOR3 OGECamera::GetPosition()
{
	return m_Pos;
}




void OGECamera::Walk(float units)
{
	m_Pos += m_Look * units;
}




void OGECamera::Strafe(float units)
{
	m_Pos += m_Right * units;
}




void OGECamera::Fly(float units)
{
	m_Pos.y += units;
}




void OGECamera::Pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &m_Right, angle);
	D3DXVec3TransformCoord(&m_Up, &m_Up, &T);
	D3DXVec3TransformCoord(&m_Look, &m_Look, &T);
}




void OGECamera::Yaw(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationY(&T, angle);
	D3DXVec3TransformCoord(&m_Right, &m_Right, &T);
	D3DXVec3TransformCoord(&m_Look, &m_Look, &T);
}




void OGECamera::Roll(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &m_Look, angle);
	D3DXVec3TransformCoord(&m_Right, &m_Right, &T);
	D3DXVec3TransformCoord(&m_Up, &m_Up, &T);
}




void OGECamera::GetViewMatrix(D3DXMATRIX* matrix)
{
	// Normalize Look vector
	D3DXVec3Normalize(&m_Look, &m_Look);

	// Find Up vector
	D3DXVec3Cross(&m_Up, &m_Look, &m_Right);
	D3DXVec3Normalize(&m_Up, &m_Up);

	// Find Right vector
	D3DXVec3Cross(&m_Right, &m_Up, &m_Look);
	D3DXVec3Normalize(&m_Right, &m_Right);

	// Build the view matrix:
	float x = -D3DXVec3Dot(&m_Right, &m_Pos);
	float y = -D3DXVec3Dot(&m_Up, &m_Pos);
	float z = -D3DXVec3Dot(&m_Look, &m_Pos);

	(*matrix)(0, 0) = m_Right.x;	(*matrix)(0, 1) = m_Up.x;	(*matrix)(0, 2) = m_Look.x;		(*matrix)(0, 3) = 0.0f;
	(*matrix)(1, 0) = m_Right.y;	(*matrix)(1, 1) = m_Up.y;	(*matrix)(1, 2) = m_Look.y;		(*matrix)(1, 3) = 0.0f;
	(*matrix)(2, 0) = m_Right.z;	(*matrix)(2, 1) = m_Up.z;	(*matrix)(2, 2) = m_Look.z;		(*matrix)(2, 3) = 0.0f;
	(*matrix)(3, 0) = x;			(*matrix)(3, 1) = y;		(*matrix)(3, 2) = z;			(*matrix)(3, 3) = 1.0f;
}