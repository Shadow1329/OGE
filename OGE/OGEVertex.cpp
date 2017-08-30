#include "OGEVertex.h"




IDirect3DVertexDeclaration9* OGEVertex::OGEVertexDeclaration = 0;




// Init vertex declaration
void OGEVertex::InitVertexDeclaration(IDirect3DDevice9* device)
{
	D3DVERTEXELEMENT9 NMapVertexElements[] =
	{
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	device->CreateVertexDeclaration(NMapVertexElements, &OGEVertex::OGEVertexDeclaration);
}




// Release vertex declaration
void OGEVertex::ReleaseVertexDeclaration()
{
	if (OGEVertex::OGEVertexDeclaration)
	{
		OGEVertex::OGEVertexDeclaration->Release();
		OGEVertex::OGEVertexDeclaration = NULL;
	}
}
