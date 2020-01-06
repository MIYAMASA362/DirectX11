#include"Common.h"

#include"Module\DirectX\DirectX.h"

#include "Light.h"

#include"Module\Shader\Shader.h"

Light::Light()
{
	_constant.Direction = Vector4(0.0f, -1.0f, 0.0f, 0.0f);
	_constant.Diffuse = COLOR(0.9f, 0.9f, 0.9f, 1.0f);
	_constant.Ambient = COLOR(0.1f, 0.1f, 0.1f, 1.0f);
}

Light::~Light()
{

}

void Light::SetResource()
{
	D3DRenderer::GetInstance()->GetConstantBuffer()->UpdateSubresource(CONSTANT_BUFFER_LIGHT,&_constant);
}
