#include"Common.h"

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include "Material.h"


Material::Material()
{
	D3DApp::Renderer::CreateConstantBuffer<Material::Constant>(&_MaterialBuffer);

	_constant.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	_constant.Ambient = Color(0.6f, 0.6f, 0.6f, 1.0f);
	_constant.Specular = Color();
	_constant.Emission = Color();
	float Shininess = 1.0f;
}

Material::~Material()
{
	if (_MaterialBuffer) _MaterialBuffer->Release();
}

void Material::SetResource()
{
	D3DApp::GetDeviceContext()->VSSetConstantBuffers(3,1,&_MaterialBuffer);
	D3DApp::GetDeviceContext()->UpdateSubresource(_MaterialBuffer,0,NULL,&_constant,0,0);
}
