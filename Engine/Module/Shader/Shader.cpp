#define _CRT_SECURE_NO_WARNINGS
#include<io.h>

#include"Common.h"
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include "Shader.h"
#include"Module\Light\Light.h"
#include"Module\Material\Material.h"



//コンスタントバッファ

void ConstantBuffer::CreateBuffer()
{
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = sizeof(float);
	
	bd.ByteWidth = sizeof(XMMATRIX);
	D3DApp::GetDevice()->CreateBuffer(&bd, NULL, &_ConstantBuffer[CONSTANT_BUFFER_WORLD]);
	D3DApp::GetDevice()->CreateBuffer(&bd, NULL, &_ConstantBuffer[CONSTANT_BUFFER_VIEW]);
	D3DApp::GetDevice()->CreateBuffer(&bd, NULL, &_ConstantBuffer[CONSTANT_BUFFER_PROJECTION]);

	bd.ByteWidth = sizeof(Material::Constant);
	D3DApp::GetDevice()->CreateBuffer(&bd, NULL, &_ConstantBuffer[CONSTANT_BUFFER_MATERIAL]);

	bd.ByteWidth = sizeof(Light::Constant);
	D3DApp::GetDevice()->CreateBuffer(&bd, NULL, &_ConstantBuffer[CONSTANT_BUFFER_LIGHT]);
}

ID3D11Buffer* ConstantBuffer::GetConstantBuffer(CONSTANT_BUFFER_ELEMENT bufferElement)
{
	return _ConstantBuffer[bufferElement];
}

void ConstantBuffer::UpdateSubresource(CONSTANT_BUFFER_ELEMENT bufferElement,const void* data)
{
	D3DApp::GetDeviceContext()->UpdateSubresource(_ConstantBuffer[bufferElement],0,NULL,data,0,0);
}

void ConstantBuffer::SetVSConstantBuffer(CONSTANT_BUFFER_ELEMENT bufferElement, UINT slot, UINT NumBuffer)
{
	D3DApp::GetDeviceContext()->VSSetConstantBuffers(slot, NumBuffer, &_ConstantBuffer[(int)bufferElement]);
}

void ConstantBuffer::SetPSConstantBuffer(CONSTANT_BUFFER_ELEMENT bufferElement, UINT slot, UINT NumBuffer)
{
	D3DApp::GetDeviceContext()->PSSetConstantBuffers(slot, NumBuffer, &_ConstantBuffer[(int)bufferElement]);
}





//頂点シェーダ

VertexShaderData::VertexShaderData()
{

}

VertexShaderData::~VertexShaderData()
{
	if (_InputLayout) _InputLayout->Release();
	if (_VertexShader) _VertexShader->Release();
}

void VertexShaderData::LoadShader(const char * vertexShader, VERTEX_INPUT_LAYOUT * inputLayout, unsigned int layoutSize)
{
	long int fsize;
	unsigned char* buffer = nullptr;

	LoadShaderFile(vertexShader, &buffer, &fsize);

	D3DApp::GetDevice()->CreateVertexShader(buffer,fsize,NULL,&_VertexShader);

	//入力レイアウト設定
	{
		D3D11_INPUT_ELEMENT_DESC* layout = new D3D11_INPUT_ELEMENT_DESC[layoutSize];

		for (int i = 0; i < layoutSize; i++)
		{
			D3D11_INPUT_ELEMENT_DESC* target = &layout[i];
			switch (inputLayout[i])
			{
			case VERTEX_INPUT_LAYOUT::VSIL_POSITION:
			{
				target->SemanticName = "POSITION";
				target->SemanticIndex = 0;
				target->Format = DXGI_FORMAT_R32G32B32_FLOAT;
				target->InputSlot = 0;
				target->AlignedByteOffset = 0;
				target->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				target->InstanceDataStepRate = 0;
			}
			break;
			case VERTEX_INPUT_LAYOUT::VSIL_NORMAL:
			{
				target->SemanticName = "NORMAL";
				target->SemanticIndex = 0;
				target->Format = DXGI_FORMAT_R32G32B32_FLOAT;
				target->InputSlot = 0;
				target->AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				target->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				target->InstanceDataStepRate = 0;
			}
			break;
			case VERTEX_INPUT_LAYOUT::VSIL_COLOR:
			{
				target->SemanticName = "COLOR";
				target->SemanticIndex = 0;
				target->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				target->InputSlot = 0;
				target->AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				target->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				target->InstanceDataStepRate = 0;
			}
			break;
			case VERTEX_INPUT_LAYOUT::VSIL_TEXCOORD:
			{
				target->SemanticName = "TEXCOORD";
				target->SemanticIndex = 0;
				target->Format = DXGI_FORMAT_R32G32_FLOAT;
				target->InputSlot = 0;
				target->AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				target->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				target->InstanceDataStepRate = 0;
			}
			break;
			default:
				MessageBox(NULL, "入力レイアウトの要素が欠損してます。", "エラー", MB_OK);
				break;
			}
		}

		D3DApp::GetDevice()->CreateInputLayout(layout, layoutSize, buffer, fsize, &_InputLayout);

		delete[] layout;

	}

	delete[] buffer;
}

void VertexShaderData::SetShader()
{
	D3DApp::GetDeviceContext()->VSSetShader(_VertexShader,NULL,0);
	D3DApp::GetDeviceContext()->IASetInputLayout(_InputLayout);
}




//ピクセルシェーダ

PixelShaderData::PixelShaderData()
{
}

PixelShaderData::~PixelShaderData()
{
	if (_PixelShader) _PixelShader->Release();
}

void PixelShaderData::LoadShader(const char * pixelShader)
{
	long int fsize;
	unsigned char* buffer = nullptr;

	LoadShaderFile(pixelShader, &buffer, &fsize);

	D3DApp::GetDevice()->CreatePixelShader(buffer,fsize,NULL,&_PixelShader);

	delete[] buffer;
}

void PixelShaderData::SetShader()
{
	D3DApp::GetDeviceContext()->PSSetShader(_PixelShader,NULL,0);
}




//シェーダ

Shader::Shader()
{

}

Shader::~Shader()
{

}

void Shader::LoadShader(const char * VertexShader, const char * PixelShader,VERTEX_INPUT_LAYOUT* InputLayout,unsigned int LayoutSize)
{
	//頂点シェーダ
	_VertexShader = new VertexShaderData();
	_VertexShader->LoadShader(VertexShader,InputLayout,LayoutSize);

	//ピクセルシェーダ
	_PixelShader = new PixelShaderData();
	_PixelShader->LoadShader(PixelShader);

	//定数バッファ生成

	_ConstantBuffer = new ConstantBuffer();
	_ConstantBuffer->CreateBuffer();

	_Light = new Light();
	_Material = new Material();
}

void Shader::Release()
{
	if (_Material) delete _Material;
	if (_Light) delete _Light;

	if (_ConstantBuffer) delete _ConstantBuffer;

	if (_PixelShader) delete _PixelShader;
	if (_VertexShader) delete _VertexShader;
}

void Shader::SetShader()
{
	_VertexShader->SetShader();
	_PixelShader->SetShader();

	//SetBuffer
	_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_WORLD,0);
	_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_VIEW,1);
	_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_PROJECTION,2);

	_Light->SetResource();
	_Material->SetResource();
}

void Shader::SetWorldMatrix(XMMATRIX * WorldMatrix)
{
	_ConstantBuffer->UpdateSubresource(CONSTANT_BUFFER_WORLD,&XMMatrixTranspose(*WorldMatrix));
}

void Shader::SetViewMatrix(XMMATRIX * ViewMatrix)
{
	_ConstantBuffer->UpdateSubresource(CONSTANT_BUFFER_VIEW,&XMMatrixTranspose(*ViewMatrix));
}

void Shader::SetProjectionMatrix(XMMATRIX * ProjectionMatrix)
{
	_ConstantBuffer->UpdateSubresource(CONSTANT_BUFFER_PROJECTION,&XMMatrixTranspose(*ProjectionMatrix));
}




void LoadShaderFile(const char * filename, unsigned char** buffer, long int * fsize)
{
	FILE* file;
	file = fopen(filename, "rb");
	*fsize = _filelength(_fileno(file));

	*buffer = new unsigned char[*fsize];
	fread(*buffer, *fsize, 1, file);
	fclose(file);
}