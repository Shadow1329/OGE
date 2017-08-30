#pragma once
#include <d3dx9.h>
#include "OGEVertex.h"




// Ground class
class OGEGround
{
public:
	OGEGround(IDirect3DDevice9* device);
	~OGEGround();

	void Draw(IDirect3DDevice9* device);


private:
	IDirect3DVertexBuffer9* m_VB;
	IDirect3DIndexBuffer9*  m_IB;
};

