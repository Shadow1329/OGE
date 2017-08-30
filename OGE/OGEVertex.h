#pragma once
#include <d3dx9.h>




//Struct of Vertex
struct OGEVertex
{
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Tangent;
	D3DXVECTOR3 m_Binormal;
	D3DXVECTOR3 m_Nornal;
	D3DXVECTOR2 m_TexCoords;

	static IDirect3DVertexDeclaration9* OGEVertexDeclaration;
	static void InitVertexDeclaration(IDirect3DDevice9* device);
	static void ReleaseVertexDeclaration();

	OGEVertex() {}
	OGEVertex(float x, float y, float z, float nx, float ny, float nz, float tx, float ty, float tz, float bx, float by, float bz, float u, float v)
	{
		m_Pos.x = x;
		m_Pos.y = y;
		m_Pos.z = z;
		m_Tangent.x = tx;
		m_Tangent.y = ty;
		m_Tangent.z = tz;
		m_Binormal.x = bx;
		m_Binormal.y = by;
		m_Binormal.z = bz;
		m_Nornal.x = nx;
		m_Nornal.y = ny;
		m_Nornal.z = nz;
		m_TexCoords.x = u;
		m_TexCoords.y = v;
	}
};
