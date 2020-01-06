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
//	コンストラクタ
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
//	デストラクタ
//
Material::~Material()
{
	_Shader.reset();
	_Texture.reset();
}

//SetResource
//	リソース設定
//
void Material::SetResource()
{
	SetResourceShader();
	SetResourceMaterial();
	SetResourceTexture();
}

//SetResourceMaterial
//	マテリアル設定
//
void Material::SetResourceMaterial()
{
	D3DRenderer::GetInstance()->GetConstantBuffer()->UpdateSubresource(CONSTANT_BUFFER_MATERIAL,&_constant);
}

//SetResourceTexture
//	テクスチャ設定
//
void Material::SetResourceTexture()
{
	_Texture.lock()->SetResource();
}

//SetResoueceShader
//	シェーダ設定
//
void Material::SetResourceShader()
{
	_Shader.lock()->SetShader();
}

