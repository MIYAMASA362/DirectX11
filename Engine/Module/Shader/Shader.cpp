#define _CRT_SECURE_NO_WARNINGS
#include<io.h>

#include"Common.h"
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include "Shader.h"

Shader::Shader()
{

}

Shader::~Shader()
{

}

void Shader::LoadShader(const char * VertexShader, const char * PixelShader)
{
	//頂点シェーダ
	{
		FILE* file;
		long int fsize;

		file = fopen(VertexShader,"rb");
		fsize = _filelength(_fileno(file));

		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer,fsize,1,file);
		fclose(file);

		D3DApp::GetDevice()->CreateVertexShader(buffer,fsize,NULL,&_VertexShader);

		//入力レイアウト
		{
			D3D11_INPUT_ELEMENT_DESC InputLayout[] = 
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		 0, 30, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			
			UINT numElements = ARRAYSIZE(InputLayout);

			D3DApp::GetDevice()->CreateInputLayout(InputLayout,numElements,buffer,fsize,&_VertexLayout);
		}

		delete[] buffer;
	}

	//ピクセルシェーダ
	{
		FILE* file;
		long int fsize;

		file = fopen(PixelShader,"rb");
		fsize = _filelength(_fileno(file));
		
		unsigned char* buffer = new unsigned char[fsize];

		fread(buffer,fsize,1,file);
		fclose(file);

		D3DApp::GetDevice()->CreatePixelShader(buffer,fsize,NULL,&_PixelShader);

		delete[] buffer;
	}

	//定数バッファ生成
	{
		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		hBufferDesc.ByteWidth = sizeof(XMMATRIX);
		D3DApp::GetDevice()->CreateBuffer(&hBufferDesc,NULL,&_WorldBuffer);
		D3DApp::GetDevice()->CreateBuffer(&hBufferDesc,NULL,&_ViewBuffer);
		D3DApp::GetDevice()->CreateBuffer(&hBufferDesc,NULL,&_ProjectionBuffer);


		hBufferDesc.ByteWidth = sizeof(MATERIAL);
		D3DApp::GetDevice()->CreateBuffer(&hBufferDesc,NULL,&_MaterialBuffer);


		hBufferDesc.ByteWidth = sizeof(LIGHT);
		D3DApp::GetDevice()->CreateBuffer(&hBufferDesc,NULL,&_LightBuffer);
		
	}
}

void Shader::Release()
{
	if (_LightBuffer) _LightBuffer->Release();
	if (_MaterialBuffer) _MaterialBuffer->Release();

	if (_ProjectionBuffer) _ProjectionBuffer->Release();
	if (_ViewBuffer)_ViewBuffer->Release();
	if (_WorldBuffer)_WorldBuffer->Release();

	if (_VertexLayout) _VertexLayout->Release();
	if (_VertexShader) _VertexShader->Release();
	if (_PixelShader) _PixelShader->Release();
}

void Shader::SetShader()
{
	D3DApp::GetDeviceContext()->VSSetShader(_VertexShader,NULL,0);
	D3DApp::GetDeviceContext()->PSSetShader(_PixelShader,NULL,0);

	D3DApp::GetDeviceContext()->IASetInputLayout(_VertexLayout);

	//UpdateSubResource

	//SetBuffer
	D3DApp::GetDeviceContext()->VSSetConstantBuffers(0, 1, &_WorldBuffer);
	D3DApp::GetDeviceContext()->VSSetConstantBuffers(1, 1, &_ViewBuffer);
	D3DApp::GetDeviceContext()->VSSetConstantBuffers(2, 1, &_ProjectionBuffer);
	D3DApp::GetDeviceContext()->VSSetConstantBuffers(3, 1, &_MaterialBuffer);
	D3DApp::GetDeviceContext()->VSSetConstantBuffers(4, 1, &_LightBuffer);
}

void Shader::SetWorldMatrix(XMMATRIX * WorldMatrix)
{
	_WorldMatrix = *WorldMatrix;
	D3DApp::GetDeviceContext()->UpdateSubresource(_WorldBuffer, 0, NULL, &XMMatrixTranspose(_WorldMatrix), 0, 0);
}

void Shader::SetViewMatrix(XMMATRIX * ViewMatrix)
{
	_ViewMatrix = *ViewMatrix;
	D3DApp::GetDeviceContext()->UpdateSubresource(_ViewBuffer, 0, NULL, &XMMatrixTranspose(_ViewMatrix), 0, 0);
}

void Shader::SetProjectionMatrix(XMMATRIX * ProjectionMatrix)
{
	_ProjectionMatrix = *ProjectionMatrix;
	D3DApp::GetDeviceContext()->UpdateSubresource(_ProjectionBuffer, 0, NULL, &XMMatrixTranspose(_ProjectionMatrix), 0, 0);
}

void Shader::SetMaterial(MATERIAL* material)
{
	_Material = *material;
	D3DApp::GetDeviceContext()->UpdateSubresource(_MaterialBuffer, 0, NULL, &_Material, 0, 0);
}

void Shader::SetLight(LIGHT* light)
{
	_Light = *light;
	D3DApp::GetDeviceContext()->UpdateSubresource(_LightBuffer, 0, NULL, &_Light, 0, 0);
}
