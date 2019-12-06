#define _CRT_SECURE_NO_WARNINGS
#include<io.h>

#include"Common.h"
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include "Shader.h"
#include"Module\Light\Light.h"
#include"Module\Material\Material.h"

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
		long int fsize;
		unsigned char* buffer = nullptr;
		LoadShaderFile(VertexShader,&buffer,&fsize);

		D3DApp::GetDevice()->CreateVertexShader(buffer,fsize,NULL,&_VertexShader);

		//入力レイアウト
		{
			D3D11_INPUT_ELEMENT_DESC InputLayout[] = 
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			
			UINT numElements = ARRAYSIZE(InputLayout);

			D3DApp::GetDevice()->CreateInputLayout(InputLayout,numElements,buffer,fsize,&_VertexLayout);
		}

		delete[] buffer;
	}

	//ピクセルシェーダ
	{
		long int fsize;
		unsigned char* buffer = nullptr;
		LoadShaderFile(PixelShader,&buffer,&fsize);

		D3DApp::GetDevice()->CreatePixelShader(buffer,fsize,NULL,&_PixelShader);

		delete[] buffer;
	}

	//定数バッファ生成
	{
		D3DApp::Renderer::CreateConstantBuffer<XMMATRIX>(&_WorldBuffer);
		D3DApp::Renderer::CreateConstantBuffer<XMMATRIX>(&_ViewBuffer);
		D3DApp::Renderer::CreateConstantBuffer<XMMATRIX>(&_ProjectionBuffer);
	}

	_Light = new Light();
	_Material = new Material();
}

void Shader::Release()
{
	if (_Light) delete _Light;
	if (_Material) delete _Material;

	if (_ProjectionBuffer) _ProjectionBuffer->Release();
	if (_ViewBuffer)_ViewBuffer->Release();
	if (_WorldBuffer)_WorldBuffer->Release();

	if (_VertexLayout) _VertexLayout->Release();
	if (_VertexShader) _VertexShader->Release();
	if (_PixelShader) _PixelShader->Release();
}

void Shader::SetShader()
{
	D3DApp::GetDeviceContext()->IASetInputLayout(_VertexLayout);

	D3DApp::GetDeviceContext()->VSSetShader(_VertexShader,NULL,0);
	D3DApp::GetDeviceContext()->PSSetShader(_PixelShader,NULL,0);

	//SetBuffer
	//VSSetConstantBuffer:Slot番号 配列番号
	D3DApp::GetDeviceContext()->VSSetConstantBuffers(0, 1, &_WorldBuffer);
	D3DApp::GetDeviceContext()->VSSetConstantBuffers(1, 1, &_ViewBuffer);
	D3DApp::GetDeviceContext()->VSSetConstantBuffers(2, 1, &_ProjectionBuffer);

	_Light->SetResource();
	_Material->SetResource();
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

void Shader::LoadShaderFile(const char * filename, unsigned char** buffer, long int * fsize)
{
	FILE* file;
	file = fopen(filename,"rb");
	*fsize = _filelength(_fileno(file));

	*buffer = new unsigned char[*fsize];
	fread(*buffer,*fsize,1,file);
	fclose(file);
}
