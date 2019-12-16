#pragma once

#include"Module\DirectX\DirectXStruct.h"

class Texture;

//��}�e���A��
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
		float Dummy[3];	//16bit�T�C�Y�ɍ��킹��
	};
public:
	Material::Constant _constant;

	Material();
	virtual ~Material();

	void SetResource();
};

//�e�N�X�`���}�e���A��
struct TextureMaterial
{
	Material _Material;
	Texture* _Texture;

	virtual ~TextureMaterial();
};