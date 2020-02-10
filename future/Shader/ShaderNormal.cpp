#include"main.h"
#include"renderer.h"

#include"shader.h"
#include"ShaderNormal.h"

#include<io.h>

#include"game_object.h"
#include"CLight.h"

void CShaderNormal::Init(const char * VertexShader, const char * PixelShader)
{
	// ���_�V�F�[�_����
	{
		FILE* file;
		long int fsize;

		file = fopen(VertexShader, "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		CRenderer::GetDevice()->CreateVertexShader(buffer, fsize, NULL, &m_VertexShader);


		// ���̓��C�A�E�g����
		{
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TANGENT",	  0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			//COLOR �� 12byte���ꂽ
			UINT numElements = ARRAYSIZE(layout);

			CRenderer::GetDevice()->CreateInputLayout(layout,
				numElements,
				buffer,
				fsize,
				&m_VertexLayout);
		}

		delete[] buffer;
	}



	// �s�N�Z���V�F�[�_����
	{
		FILE* file;
		long int fsize;

		file = fopen(PixelShader, "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		CRenderer::GetDevice()->CreatePixelShader(buffer, fsize, NULL, &m_PixelShader);

		delete[] buffer;
	}



	// �萔�o�b�t�@����
	{
		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		hBufferDesc.ByteWidth = sizeof(CONSTANT);
		CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_ConstantBuffer);
	}
}

void CShaderNormal::Uninit()
{
	if (m_ConstantBuffer)	m_ConstantBuffer->Release();

	if (m_VertexLayout)		m_VertexLayout->Release();
	if (m_VertexShader)		m_VertexShader->Release();
	if (m_PixelShader)		m_PixelShader->Release();
}

void CShaderNormal::Set()
{
	// �V�F�[�_�ݒ�
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// ���̓��C�A�E�g�ݒ�
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);


	// �萔�o�b�t�@�X�V
	CRenderer::GetDeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, NULL, &m_Constant, 0, 0);

	// �萔�o�b�t�@�ݒ�
	CRenderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	CRenderer::GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);
}

void CShaderNormal::SetLight(CLight * light)
{
	CRenderer::GetDeviceContext()->VSSetConstantBuffers(1,1,light->GetBuffer());
	CRenderer::GetDeviceContext()->PSSetConstantBuffers(1,1,light->GetBuffer());
}

