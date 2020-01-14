#include"Common.h"

#include"Module\DirectX\DirectX.h"
#include"Module\ECSEngine.h"

#include"Module\Mesh\Mesh.h"
#include "Geometry.h"

//*********************************************************************************************************************
//
//	Cube
//
//*********************************************************************************************************************

Geometry::Cube::Cube()
{
	this->_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	this->_VertexNum = 24;
	this->_VertexArray = new Mesh::VertexType[this->_VertexNum]
	{
		//上面
		{ XMFLOAT3(-0.5f, 0.5f, 0.5f),XMFLOAT3(0.0f,1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(0.5f, 0.5f, 0.5f),XMFLOAT3(0.0f,1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(1.0f,0.0f) },
		{ XMFLOAT3(-0.5f, 0.5f,-0.5f),XMFLOAT3(0.0f,1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(0.0f,1.0f) },
		{ XMFLOAT3(0.5f, 0.5f,-0.5f),XMFLOAT3(0.0f,1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(1.0f,1.0f) },

		//下面
		{ XMFLOAT3(0.5f,-0.5f, 0.5f),XMFLOAT3(0.0f,-1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(-0.5f,-0.5f, 0.5f),XMFLOAT3(0.0f,-1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(1.0f,0.0f) },
		{ XMFLOAT3(0.5f,-0.5f,-0.5f),XMFLOAT3(0.0f,-1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(0.0f,1.0f) },
		{ XMFLOAT3(-0.5f,-0.5f,-0.5f),XMFLOAT3(0.0f,-1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(1.0f,1.0f) },

		//正面
		{ XMFLOAT3(-0.5f, 0.5f,-0.5f),XMFLOAT3(0.0f,0.0f,-1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(0.5f, 0.5f,-0.5f),XMFLOAT3(0.0f,0.0f,-1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(1.0f,0.0f) },
		{ XMFLOAT3(-0.5f,-0.5f,-0.5f),XMFLOAT3(0.0f,0.0f,-1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(0.0f,1.0f) },
		{ XMFLOAT3(0.5f,-0.5f,-0.5f),XMFLOAT3(0.0f,0.0f,-1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(1.0f,1.0f) },

		//右側面
		{ XMFLOAT3(0.5f, 0.5f,-0.5f),XMFLOAT3(1.0f,0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(0.5f, 0.5f, 0.5f),XMFLOAT3(1.0f,0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(1.0f,0.0f) },
		{ XMFLOAT3(0.5f,-0.5f,-0.5f),XMFLOAT3(1.0f,0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(0.0f,1.0f) },
		{ XMFLOAT3(0.5f,-0.5f, 0.5f),XMFLOAT3(1.0f,0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(1.0f,1.0f) },

		//背面
		{ XMFLOAT3(0.5f, 0.5f, 0.5f),XMFLOAT3(0.0f,0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, 0.5f),XMFLOAT3(0.0f,0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(1.0f,0.0f) },
		{ XMFLOAT3(0.5f,-0.5f, 0.5f),XMFLOAT3(0.0f,0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(0.0f,1.0f) },
		{ XMFLOAT3(-0.5f,-0.5f, 0.5f),XMFLOAT3(0.0f,0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(1.0f,1.0f) },

		//左側面
		{ XMFLOAT3(-0.5f, 0.5f, 0.5f),XMFLOAT3(-1.0f,0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(-0.5f, 0.5f,-0.5f),XMFLOAT3(-1.0f,0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(1.0f,0.0f) },
		{ XMFLOAT3(-0.5f,-0.5f, 0.5f),XMFLOAT3(-1.0f,0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(0.0f,1.0f) },
		{ XMFLOAT3(-0.5f,-0.5f,-0.5f),XMFLOAT3(-1.0f,0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(1.0f,1.0f) }
	};

	this->_IndexNum = 36;
	this->_IndexArray = new unsigned short[this->_IndexNum];
	for (int j = 0; j < 6; j++)
	{
		int n = j * 4;
		int i = j * 6;
		this->_IndexArray[i + 0] = n + 0;
		this->_IndexArray[i + 1] = n + 1;
		this->_IndexArray[i + 2] = n + 2;

		this->_IndexArray[i + 3] = n + 1;
		this->_IndexArray[i + 4] = n + 3;
		this->_IndexArray[i + 5] = n + 2;
	}


	this->_SurfaceNum = 12;
	this->_Surface = new Surface[this->_SurfaceNum];
	for (int n = 0; n < this->_SurfaceNum; n++)
	{
		this->_Surface[n]._p1 = this->_VertexArray[this->_IndexArray[n + 0]].Position;
		this->_Surface[n]._p2 = this->_VertexArray[this->_IndexArray[n + 1]].Position;
		this->_Surface[n]._p3 = this->_VertexArray[this->_IndexArray[n + 2]].Position;
	}

	this->CreateVertexBuffer();
	this->CreateIndexBuffer();
}

Geometry::Cube::~Cube()
{

}

//*********************************************************************************************************************
//
//	Sphere
//
//*********************************************************************************************************************

Geometry::Sphere::Sphere()
{
	this->_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	unsigned int Grid = 37;	//Grid数
	float Radius = 0.5f;	//半径

	//角度
	float WidthAngle = Mathf::PI2 / (Grid - 1);
	float HeightAngle = Mathf::PI2 / (Grid - 1);

	float TexWidth = (1.0f / Grid);
	float TexHeight = (1.0f / Grid);

	this->_VertexNum = Grid * Grid;
	this->_VertexArray = new Mesh::VertexType[this->_VertexNum];

	//変化値
	float x, y, z, radius;

	//頂点
	for (int h = 0; h < Grid; h++)
	{
		y = Radius * sinf(HeightAngle * h);
		radius = Radius * cosf(HeightAngle * h);

		for (int w = 0; w < Grid; w++)
		{
			x = radius * sinf(WidthAngle * w);
			z = radius * cosf(WidthAngle * w);

			auto& vertex = this->_VertexArray[h * Grid + w];
			vertex.Position = XMFLOAT3(x, y, z);
			vertex.Normal	= XMFLOAT3(1.0f * x, 1.0f * y, 1.0f * z);
			vertex.Diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex.TexCoord = XMFLOAT2(TexWidth * w, TexHeight * h);
			if (h == 0)
				vertex.TexCoord.y += 0.005f;
		}
	}

	//頂点インデックス
	this->_IndexNum = (Grid * 2) * (Grid - 1) + (Grid - 2) * 2;
	this->_IndexArray = new unsigned short[this->_IndexNum];
	int nCount = 0;
	for (int h = 0; h < Grid - 1; h++)
	{
		int w = 0;
		if (h != 0)
		{
			this->_IndexArray[nCount] = this->_IndexArray[nCount - 1];
			this->_IndexArray[nCount + 1] = Grid * h + w;
			nCount += 2;
		}

		for (; w < Grid; w++)
		{
			this->_IndexArray[nCount] = Grid * h + w;
			this->_IndexArray[nCount + 1] = Grid * (h + 1) + w;
			nCount += 2;
		}
	}

	this->_SurfaceNum = (Grid - 1) * (Grid -1);
	this->_Surface = new Surface[this->_SurfaceNum];
	for (int n = 0; n < this->_SurfaceNum; n++)
	{
		this->_Surface[n]._p1 = this->_VertexArray[this->_IndexArray[n + 0]].Position;
		this->_Surface[n]._p2 = this->_VertexArray[this->_IndexArray[n + 1]].Position;
		this->_Surface[n]._p3 = this->_VertexArray[this->_IndexArray[n + 2]].Position;
	}

	this->CreateVertexBuffer();
	this->CreateIndexBuffer();
}

Geometry::Sphere::~Sphere()
{

}

//*********************************************************************************************************************
//
//	Plane
//
//*********************************************************************************************************************
Geometry::Plane::Plane()
{
	this->_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	this->_VertexNum = 4;
	this->_VertexArray = new Mesh::VertexType[this->_VertexNum]
	{
		{ XMFLOAT3(-0.5f, 0.0f, 0.5f),XMFLOAT3(0.0f,1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(0.5f, 0.0f, 0.5f),XMFLOAT3(0.0f,1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(1.0f,0.0f) },
		{ XMFLOAT3(-0.5f, 0.0f,-0.5f),XMFLOAT3(0.0f,1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(0.0f,1.0f) },
		{ XMFLOAT3(0.5f, 0.0f,-0.5f),XMFLOAT3(0.0f,1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(1.0f,1.0f) },
	};

	this->_SurfaceNum = 2;
	this->_Surface = new Surface[this->_SurfaceNum];

	//頂点インデックス
	{
		this->_IndexNum = 4;
		this->_IndexArray = new unsigned short[this->_IndexNum];

		_IndexArray[0] = 0;
		_IndexArray[1] = 1;
		_IndexArray[2] = 2;
		_IndexArray[3] = 3;
	}

	//サーファス
	for (int n = 0; n < this->_SurfaceNum; n++)
	{
		_Surface[n]._p1 = _VertexArray[_IndexArray[n + 0]].Position;
		_Surface[n]._p2 = _VertexArray[_IndexArray[n + 1]].Position;
		_Surface[n]._p3 = _VertexArray[_IndexArray[n + 2]].Position;
	}

	this->CreateVertexBuffer();
	this->CreateIndexBuffer();
}

Geometry::Plane::~Plane()
{

}
