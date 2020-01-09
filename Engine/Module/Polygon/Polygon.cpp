
#include"Common.h"
#include"Module\DirectX\DirectX.h"

#include"Module\IMGUI\GUI_ImGui.h"

#include"Module\Object\Object.h"
#include"Module\Texture\texture.h"

//ECS
#include"Module\ECSEngine.h"

#include"Polygon.h"

#include"Module\Shader\Shader.h"

using namespace DirectX;

void CPolygon::Start()
{
	//���_���
	VERTEX_3D vertex[4] = {
		{ XMFLOAT3(  0.0f,   0.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(200.0f,   0.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(  0.0f, 200.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(200.0f, 200.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f) },
	};

	//���_�o�b�t�@�̐���
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(VERTEX_3D) * (sizeof(vertex)/sizeof(VERTEX_3D));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subResourceData;
	ZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = vertex;

	D3DRenderer::GetInstance()->GetDevice()->CreateBuffer(&bufferDesc, &subResourceData, &this->m_VertexBuffer);
}

void CPolygon::OnDestroy()
{
	if (m_Texture)
		delete m_Texture;
	if (m_VertexBuffer)
		m_VertexBuffer->Release();
}

void CPolygon::Update()
{

}

void CPolygon::Render()
{
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	//���_�o�b�t�@�ݒ� �����o�^���ł���
	D3DRenderer::GetInstance()->GetDeviceContext()->IASetVertexBuffers(0,1,&m_VertexBuffer,&stride,&offset);
	(*this->m_Texture)->SetResource();
	D3DRenderer::GetInstance()->SetWorldViewProjection2D();

	//�ǂ�����Ē��_���q����̂����_�̐ݒ���g�|���W�Ƃ���
	D3DRenderer::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//Draw(n,i)�@n:���� i:�ǂ�����n�߂邩
	D3DRenderer::GetInstance()->GetDeviceContext()->Draw(4,0);
}