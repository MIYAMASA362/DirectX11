#include"Common.h"

#include"Module\DirectX\DirectX.h"

#include"Module\Texture\texture.h"
#include"Module\Texture\TextureManager.h"

#include "Material.h"
#include"Module\Shader\Shader.h"

//*********************************************************************************************************************
//
//	Material
//
//*********************************************************************************************************************

//Material
//	�R���X�g���N�^
//
Material::Material()
{
	_constant.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	_constant.Ambient = Color(0.6f, 0.6f, 0.6f, 1.0f);
	_constant.Specular = Color();
	_constant.Emission = Color();
	float Shininess = 1.0f;
}

//~Material
//	�f�X�g���N�^
//
Material::~Material()
{
	_Shader.reset();
	_Texture.reset();
}

//SetResource
//	���\�[�X�ݒ�
//
void Material::SetResource()
{
	SetResourceShader();
	SetResourceMaterial();
	SetResourceTexture();
}

//SetResourceMaterial
//	�}�e���A���ݒ�
//
void Material::SetResourceMaterial()
{
	D3DRenderer::GetInstance()->GetConstantBuffer()->UpdateSubresource(CONSTANT_BUFFER_MATERIAL,&_constant);
}

//SetResourceTexture
//	�e�N�X�`���ݒ�
//
void Material::SetResourceTexture()
{
	_Texture.lock()->SetResource();
}

//SetResoueceShader
//	�V�F�[�_�ݒ�
//
void Material::SetResourceShader()
{
	_Shader.lock()->SetShader();
}

