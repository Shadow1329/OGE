#pragma once




//Struct of Vertex
struct OGEVertex
{
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Nornal;
	D3DXVECTOR2 m_TexCoords;

	OGEVertex() {}
	OGEVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		m_Pos.x = x;
		m_Pos.y = y;
		m_Pos.z = z;
		m_Nornal.x = nx;
		m_Nornal.y = ny;
		m_Nornal.z = nz;
		m_TexCoords.x = u;
		m_TexCoords.y = v;
	}
};
#define OGEVERTEX_FVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
