#include"Common.h"

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include "Light.h"

Light::Light()
{
	D3DApp::Renderer::CreateConstantBuffer<Light::Constant>(&_LightBuffer);

	_constant.Direction = Vector4(0.0f, -1.0f, 0.0f, 0.0f);
	_constant.Diffuse = COLOR(0.9f, 0.9f, 0.9f, 1.0f);
	_constant.Ambient = COLOR(0.1f, 0.1f, 0.1f, 1.0f);
}

Light::~Light()
{
	if (_LightBuffer) _LightBuffer->Release();
}

void Light::SetResource()
{
	D3DApp::GetDeviceContext()->VSSetConstantBuffers(4, 1, &_LightBuffer);
	D3DApp::GetDeviceContext()->UpdateSubresource(_LightBuffer, 0, NULL,&_constant, 0, 0);
}
