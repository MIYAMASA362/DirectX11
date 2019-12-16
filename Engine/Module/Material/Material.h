#pragma once

#include"Module\DirectX\DirectXStruct.h"

class Texture;
struct aiMaterial;

//��}�e���A��
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
		float Dummy[3];	//16bit�T�C�Y�ɍ��킹��
	};
public:
	Material::Constant _constant;

	Material();
	virtual ~Material();

	void SetResource();
};

//�e�N�X�`���}�e���A��
struct TextureMaterial final
{
	Material _Material;
	Texture* _Texture;

	virtual ~TextureMaterial();

	void SetResource();
};