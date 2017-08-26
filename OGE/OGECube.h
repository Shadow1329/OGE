#pragma once
#include <d3dx9.h>
#include "OGEVertex.h"




// Cube class
class OGECube
{
public:
	OGECube(IDirect3DDevice9* device);
	~OGECube();

	void Draw(IDirect3DDevice9* device, D3DMATERIAL9* mtrl);


private:
	IDirect3DVertexBuffer9* m_VB;
	IDirect3DIndexBuffer9*  m_IB;
};

