#pragma once

namespace DirectX
{
	class Texture;

	typedef struct tagMaterial
	{
		Color Ambient;
		Color Diffuse;
		Color Specular;
		Color Emission;
		float Shininess;
		float Dummy[3];	//16bit

		tagMaterial() = default;
	}Material,MATERIAL;

	//Model�p�̃}�e���A��
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