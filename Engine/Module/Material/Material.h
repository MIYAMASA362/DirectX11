#pragma once

#include"Module\DirectX\DirectXStruct.h"

class Texture;
struct aiMaterial;

//基準マテリアル
class Material final
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

	Material();
	virtual ~Material();

	void SetResource();
};

//テクスチャマテリアル
struct TextureMaterial final
{
	Material _Material;
	Texture* _Texture;

	virtual ~TextureMaterial();

	void SetResource();
};