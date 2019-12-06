#pragma once

#include"Module\DirectX\DirectXStruct.h"

class Material
{
public:
	struct Constant
	{
		Color Ambient;
		Color Diffuse;
		Color Specular;
		Color Emission;
		float Shininess;
		float Dummy[3];	//16bitサイズに合わせる
	};
public:
	Material::Constant _constant;
	ID3D11Buffer* _MaterialBuffer;

	Material();
	virtual ~Material();

	void SetResource();
};

namespace DirectX
{
	class Texture;

	//Model用のマテリアル
	typedef struct tagDX11_MODEL_MATERIAL
	{
		Material Material;
		Texture* Texture;
	}DX11_MODEL_MATERIAL;

	//SubSet
	typedef struct tagDX11_SUBSET
	{
		unsigned short	StartIndex;
		unsigned short	IndexNum;
		DX11_MODEL_MATERIAL	Material;
	}DX11_SUBSET;
}
