#define _CRT_SECURE_NO_WARNINGS
#include<io.h>

#include"Common.h"
#include"Module\DirectX\DirectX.h"

#include "Shader.h"
#include"Module\Light\Light.h"
#include"Module\Material\Material.h"


//*********************************************************************************************************************
//
//	ConstBuffer
//
//*********************************************************************************************************************

//ConstBuffer
//	�R���X�g���N�^
//
ConstantBuffer::ConstantBuffer(D3DRenderer * renderer)
	:
	_Renderer(renderer)
{

}


void ConstantBuffer::CreateBuffer()
{
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = sizeof(float);

	bd.ByteWidth = sizeof(XMMATRIX);
	_Renderer->GetDevice()->CreateBuffer(&bd, NULL, &_ConstantBuffer[CONSTANT_BUFFER_WORLD]);
	_Renderer->GetDevice()->CreateBuffer(&bd, NULL, &_ConstantBuffer[CONSTANT_BUFFER_VIEW]);
	_Renderer->GetDevice()->CreateBuffer(&bd, NULL, &_ConstantBuffer[CONSTANT_BUFFER_PROJECTION]);

	bd.ByteWidth = sizeof(Material::Constant);
	_Renderer->GetDevice()->CreateBuffer(&bd, NULL, &_ConstantBuffer[CONSTANT_BUFFER_MATERIAL]);

	bd.ByteWidth = sizeof(Light::Constant);
	_Renderer->GetDevice()->CreateBuffer(&bd, NULL, &_ConstantBuffer[CONSTANT_BUFFER_LIGHT]);
}


ID3D11Buffer* ConstantBuffer::GetConstantBuffer(CONSTANT_BUFFER_ELEMENT bufferElement)
{
	return _ConstantBuffer[bufferElement];
}

void ConstantBuffer::UpdateSubresource(CONSTANT_BUFFER_ELEMENT bufferElement, const void* data)
{
	_Renderer->GetDeviceContext()->UpdateSubresource(_ConstantBuffer[bufferElement], 0, NULL, data, 0, 0);
}

void ConstantBuffer::SetVSConstantBuffer(CONSTANT_BUFFER_ELEMENT bufferElement, UINT slot, UINT NumBuffer)
{
	_Renderer->GetDeviceContext()->VSSetConstantBuffers(slot, NumBuffer, &_ConstantBuffer[(int)bufferElement]);
}

void ConstantBuffer::SetPSConstantBuffer(CONSTANT_BUFFER_ELEMENT bufferElement, UINT slot, UINT NumBuffer)
{
	_Renderer->GetDeviceContext()->PSSetConstantBuffers(slot, NumBuffer, &_ConstantBuffer[(int)bufferElement]);
}




//���_�V�F�[�_

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

	D3DRenderer::GetInstance()->GetDevice()->CreateVertexShader(buffer, fsize, NULL, &_VertexShader);

	//���̓��C�A�E�g�ݒ�
	{
		D3D11_INPUT_ELEMENT_DESC* layout = new D3D11_INPUT_ELEMENT_DESC[layoutSize];

		for (unsigned int i = 0; i < layoutSize; i++)
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
				MessageBox(NULL, "���̓��C�A�E�g�̗v�f���������Ă܂��B", "�G���[", MB_OK);
				break;
			}
		}

		D3DRenderer::GetInstance()->GetDevice()->CreateInputLayout(layout, layoutSize, buffer, fsize, &_InputLayout);

		delete[] layout;

	}

	delete[] buffer;
}

void VertexShaderData::SetShader()
{
	D3DRenderer::GetInstance()->GetDeviceContext()->VSSetShader(_VertexShader, NULL, 0);
	D3DRenderer::GetInstance()->GetDeviceContext()->IASetInputLayout(_InputLayout);
}




//�s�N�Z���V�F�[�_

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

	D3DRenderer::GetInstance()->GetDevice()->CreatePixelShader(buffer, fsize, NULL, &_PixelShader);

	delete[] buffer;
}

void PixelShaderData::SetShader()
{
	D3DRenderer::GetInstance()->GetDeviceContext()->PSSetShader(_PixelShader, NULL, 0);
}




//�V�F�[�_

Shader::Shader()
{

}

Shader::~Shader()
{
	if (_PixelShader) delete _PixelShader;
	if (_VertexShader) delete _VertexShader;
}

void Shader::SetVertexShader(const char * VertexShader, VERTEX_INPUT_LAYOUT * layout, unsigned int size)
{
	_VertexShader = new VertexShaderData();
	_VertexShader->LoadShader(VertexShader, layout, size);
}

void Shader::SetPixelShader(const char * PixelShader)
{
	_PixelShader = new PixelShaderData();
	_PixelShader->LoadShader(PixelShader);
}

void Shader::Release()
{
	if (_PixelShader) delete _PixelShader;
	if (_VertexShader) delete _VertexShader;
}

void Shader::SetShader()
{
	_VertexShader->SetShader();
	_PixelShader->SetShader();
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