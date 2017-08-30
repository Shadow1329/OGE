#include "OGEGround.h"




OGEGround::OGEGround(IDirect3DDevice9* device)
{
	device->CreateVertexBuffer(
		4 * sizeof(OGEVertex),
		D3DUSAGE_WRITEONLY,
		NULL,
		D3DPOOL_MANAGED,
		&m_VB,
		NULL);

	OGEVertex* v;
	m_VB->Lock(0, 0, (void**)&v, NULL);

	// fill face vertex data
	v[0] = OGEVertex(-1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[1] = OGEVertex(-1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 10.0f);
	v[2] = OGEVertex(1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 10.0f, 10.0f);
	v[3] = OGEVertex(1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 10.0f, 0.0f);

	m_VB->Unlock();

	device->CreateIndexBuffer(
		6 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_IB,
		NULL);

	WORD* i = 0;
	m_IB->Lock(0, 0, (void**)&i, NULL);

	// fill face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	m_IB->Unlock();
}




OGEGround::~OGEGround()
{
	if (m_VB) { m_VB->Release(); m_VB = 0; }
	if (m_IB) { m_IB->Release(); m_IB = 0; }
}




void OGEGround::Draw(IDirect3DDevice9* device)
{
	device->SetStreamSource(0, m_VB, 0, sizeof(OGEVertex));
	device->SetIndices(m_IB);
	device->SetVertexDeclaration(OGEVertex::OGEVertexDeclaration);
	device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		4,
		0,
		2);
}
