#pragma once
#ifndef FIELD_H
#define FIELD_H

#include<d3d11.h>
#include"texture.h"
#include"DirectX.h"

using namespace DirectX;

class CField:public Behaviour
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	CTexture* m_Texture = NULL;
public:
	void Initialize()override;
	void Update() override;
	void Render() override;
	void Finalize()override;
};

class CWallField:public Behaviour
{
private:
	ID3D11Buffer* m_FieldVertexBuffer = NULL;
	CTexture* m_FieldTexture = NULL;

	ID3D11Buffer* m_WallVertexBuffer = NULL;
	CTexture* m_WallTexture = NULL;
public:
	int n = 0;
public:
	void Initialize()override;
	void Update()override;
	void Render()override;
	void Finalize()override;
};

class CMeshField
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;
	DX11_SUBSET* m_SubsetArray = NULL;
	unsigned short m_SubsetNum;
public:
	void Init(XMFLOAT2 GridScale, unsigned int WidthNum, unsigned int HeightNum);
	void UnInit();
	void Update();
	void Draw(XMFLOAT3 Position, XMFLOAT3 Rotation, XMFLOAT3 Scale);
};


#endif // !FIELD_H
