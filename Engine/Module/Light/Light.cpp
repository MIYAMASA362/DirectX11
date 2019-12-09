#include"Common.h"

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include"Module\Shader\Shader.h"
#include "Light.h"

Light::Light()
{
	_constant.Direction = Vector4(0.0f, -1.0f, 0.0f, 0.0f);
	_constant.Diffuse = DirectX::COLOR(0.9f, 0.9f, 0.9f, 1.0f);
	_constant.Ambient = DirectX::COLOR(0.1f, 0.1f, 0.1f, 1.0f);
}

Light::~Light()
{

}

void Light::SetResource()
{
	D3DApp::GetConstBuffer()->UpdateSubresource(CONSTANT_BUFFER_LIGHT,&_constant);
}
