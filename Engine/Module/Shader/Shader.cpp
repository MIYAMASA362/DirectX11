#define _CRT_SECURE_NO_WARNINGS
#include<io.h>

#include"Common.h"
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include "Shader.h"
#include"Module\Light\Light.h"
#include"Module\Material\Material.h"

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
	D3DApp::GetDeviceContext()->UpdateSubresource(_ConstantBuffer[bufferElement],0,NULL,&data,0,0);
}

void ConstantBuffer::SetVSConstantBuffer(CONSTANT_BUFFER_ELEMENT bufferElement, UINT slot, UINT NumBuffer)
{
	D3DApp::GetDeviceContext()->VSSetConstantBuffers(slot, NumBuffer, &_ConstantBuffer[(int)bufferElement]);
}

void ConstantBuffer::SetPSConstantBuffer(CONSTANT_BUFFER_ELEMENT bufferElement, UINT slot, UINT NumBuffer)
{
	D3DApp::GetDeviceContext()->PSSetConstantBuffers(slot, NumBuffer, &_ConstantBuffer[(int)bufferElement]);
}





Shader::Shader()
{

}

Shader::~Shader()
{

}

void Shader::LoadShader(const char * VertexShader, const char * PixelShader,VERTEX_INPUT_LAYOUT* InputLayout,unsigned int LayoutSize)
{
	//頂点シェーダ
	{
		long int fsize;
		unsigned char* buffer = nullptr;
		LoadShaderFile(VertexShader,&buffer,&fsize);

		D3DApp::GetDevice()->CreateVertexShader(buffer,fsize,NULL,&_VertexShader);

		//入力レイアウト設定
		{
			D3D11_INPUT_ELEMENT_DESC* layout = new D3D11_INPUT_ELEMENT_DESC[LayoutSize];

			for (int i = 0; i < LayoutSize; i++)
			{
				D3D11_INPUT_ELEMENT_DESC* target = &layout[i];
				switch (InputLayout[i])
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

			D3DApp::GetDevice()->CreateInputLayout(layout, LayoutSize, buffer, fsize, &_VertexLayout);

			delete[] layout;
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

	_ConstantBuffer = new ConstantBuffer();
	_ConstantBuffer->CreateBuffer();

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

	if (_ConstantBuffer) delete _ConstantBuffer;

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

	/*_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_WORLD,0);
	_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_VIEW,1);
	_ConstantBuffer->SetVSConstantBuffer(CONSTANT_BUFFER_PROJECTION,2);*/

	_Light->SetResource();
	_Material->SetResource();
}

void Shader::SetWorldMatrix(XMMATRIX * WorldMatrix)
{
	_WorldMatrix = *WorldMatrix;
	//_ConstantBuffer->UpdateSubresource(CONSTANT_BUFFER_WORLD,&XMMatrixTranspose(_WorldMatrix));
	D3DApp::GetDeviceContext()->UpdateSubresource(_WorldBuffer, 0, NULL, &XMMatrixTranspose(_WorldMatrix), 0, 0);
}

void Shader::SetViewMatrix(XMMATRIX * ViewMatrix)
{
	_ViewMatrix = *ViewMatrix;
	//_ConstantBuffer->UpdateSubresource(CONSTANT_BUFFER_VIEW,&XMMatrixTranspose(_ViewMatrix));
	D3DApp::GetDeviceContext()->UpdateSubresource(_ViewBuffer, 0, NULL, &XMMatrixTranspose(_ViewMatrix), 0, 0);
}

void Shader::SetProjectionMatrix(XMMATRIX * ProjectionMatrix)
{
	_ProjectionMatrix = *ProjectionMatrix;
	//_ConstantBuffer->UpdateSubresource(CONSTANT_BUFFER_PROJECTION,&XMMatrixTranspose(_ProjectionMatrix));
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
