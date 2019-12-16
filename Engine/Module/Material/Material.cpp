#include"Common.h"

#include"Module\DirectX\DirectX.h"

#include "Material.h"
#include"Module\Shader\Shader.h"


Material::Material()
{
	_constant.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	_constant.Ambient = Color(0.6f, 0.6f, 0.6f, 1.0f);
	_constant.Specular = Color();
	_constant.Emission = Color();
	float Shininess = 1.0f;
}

Material::~Material()
{

}

void Material::SetResource()
{
	D3DApp::GetConstantBuffer()->UpdateSubresource(CONSTANT_BUFFER_MATERIAL,&_constant);
}

TextureMaterial::~TextureMaterial()
{
	delete _Texture;
}
