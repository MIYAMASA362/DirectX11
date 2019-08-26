#include"Module\Module.h"

DirectX::Bounds::Bounds(Vector3 center, Vector3 size)
{
	this->m_center = center;
	this->m_size = size;
}

DirectX::Collider::Collider()
:
	bound(Vector3::zero(),Vector3::one()),
	DirectX::Component("Collider")
{

}

void DirectX::Collider::SendBehaviourMessage(Message message)
{
	if (message != Message::Render) return;
	Render();
}

//--- SphereCollider ----------------------------------------------------------

ID3D11Buffer* SphereCollider::m_VertexBuffer = nullptr;
ID3D11Buffer* SphereCollider::m_IndexBuffer = nullptr;
VERTEX_3D* SphereCollider::m_pVertex = nullptr;
int SphereCollider::m_IndexNum = 0;
Texture* SphereCollider::m_Texture = nullptr;

SphereCollider::SphereCollider()
{
	this->SetRadius(0.25f);
}

void DirectX::SphereCollider::SetRenderBuffer()
{
	HRESULT hr;
	const int vertexCircle = 13;

	{
		const int vertexNum = vertexCircle;
		m_pVertex = new VERTEX_3D[vertexNum];

		const float c_value = Mathf::PI2 / (vertexCircle-1);
		const float radian = 1.0f;
		float angle = 0.0f;

		int n;
		for (n = 0; n < vertexCircle; n++) {
			VERTEX_3D& vertex = m_pVertex[n];
			vertex.Position =
				XMFLOAT3(
					radian * sinf(angle),
					0.0f,
					radian * cosf(angle)
				);
			vertex.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertex.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex.TexCoord = XMFLOAT2(0.0f, 0.0f);
			angle += c_value;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd,sizeof(bd));
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.ByteWidth = sizeof(VERTEX_3D) * vertexNum;
		bd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd,sizeof(sd));
		sd.pSysMem = m_pVertex;

		hr = D3DApp::GetDevice()->CreateBuffer(&bd,&sd,&m_VertexBuffer);
		if (FAILED(hr))
			MessageBox(NULL,"頂点バッファの生成に失敗しました。","SphereCollider",MB_OK);
	}

	{
		m_IndexNum = vertexCircle;
		WORD* pIndex = new WORD[m_IndexNum];

		for (int n = 0; n < m_IndexNum; n++) {
			pIndex[n] = n;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd,sizeof(bd));
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.ByteWidth = sizeof(WORD) * m_IndexNum;
		bd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd,sizeof(sd));
		sd.pSysMem = pIndex;

		hr = D3DApp::GetDevice()->CreateBuffer(&bd,&sd,&m_IndexBuffer);
		if (FAILED(hr))
			MessageBox(NULL,"頂点インデックスバッファの生成に失敗しました。","SphereCollider",MB_OK);
	
		delete[] pIndex;
	}

	m_Texture = TextureManager::GetTexture("sky");
}

void DirectX::SphereCollider::ReleaseRenderBuffer()
{
	if (m_IndexBuffer)
		m_IndexBuffer->Release();
	if (m_VertexBuffer)
		m_VertexBuffer->Release();
	delete[] m_pVertex;
}

void DirectX::SphereCollider::SetRadius(float radius)
{
	this->bound.SetSize(Vector3::one() * radius * 2.0f);
}

void DirectX::SphereCollider::Render()
{
	D3DApp::Renderer::SetVertexBuffer(m_VertexBuffer);
	D3DApp::Renderer::SetIndexBuffer(m_IndexBuffer);

	D3DApp::Renderer::SetTexture(m_Texture);
	float rad = this->transform.lock()->scale().MaxElement();
	Vector3 scale = bound.GetSize() * rad;
	Vector3 pos = bound.GetCenter();

	XMMATRIX world;
	XMMATRIX local;
	world = XMMatrixScaling(scale.x, scale.y, scale.z) *  XMMatrixTranslation(pos.x, pos.y, pos.z);
	world *= this->transform.lock()->MatrixQuaternion()* this->transform.lock()->MatrixTranslation();

	D3DApp::Renderer::SetWorldMatrix(&world);
	D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	D3DApp::GetDeviceContext()->DrawIndexed(m_IndexNum, 0, 0);

	local = XMMatrixRotationZ(Mathf::PI_2) * world;
	D3DApp::Renderer::SetWorldMatrix(&local);
	D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	D3DApp::GetDeviceContext()->DrawIndexed(m_IndexNum, 0, 0);

	local = XMMatrixRotationX(Mathf::PI_2) * world;
	D3DApp::Renderer::SetWorldMatrix(&local);
	D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	D3DApp::GetDeviceContext()->DrawIndexed(m_IndexNum, 0, 0);
}

//--- BoxCollider -------------------------------------------------------------

ID3D11Buffer* BoxCollider::m_IndexBuffer = nullptr;
ID3D11Buffer* BoxCollider::m_VertexBuffer = nullptr;
VERTEX_3D* BoxCollider::m_pVertex = nullptr;
int BoxCollider::m_IndexNum = 0;
Texture* BoxCollider::m_Texture;

void DirectX::BoxCollider::SetRenderBuffer()
{
	HRESULT hr;

	int nCount = 0;
	{
		const int vertex = 8;
		m_pVertex = new VERTEX_3D[vertex];
		for (int n = 0; n < 2; n++) {
			float y = -0.5f + (float)n * 1.0f;
			m_pVertex[nCount].Position = XMFLOAT3(-0.5f, y, 0.5f);
			m_pVertex[nCount + 1].Position = XMFLOAT3(0.5f, y, 0.5f);
			m_pVertex[nCount + 2].Position = XMFLOAT3(0.5f, y, -0.5f);
			m_pVertex[nCount + 3].Position = XMFLOAT3(-0.5f, y, -0.5f);
			for (int i = 0; i < 4; i++) {
				m_pVertex[nCount + i].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				m_pVertex[nCount + i].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				m_pVertex[nCount + i].TexCoord = XMFLOAT2(0.0f, 0.0f);
			}
			nCount += 4;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.ByteWidth = sizeof(VERTEX_3D) * vertex;
		bd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_pVertex;

		hr = D3DApp::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
		if (FAILED(hr))
			MessageBox(NULL, "頂点バッファの生成に失敗しました。", "BoxCollider", MB_OK);
	}

	{
		m_IndexNum = 24;
		WORD* pIndex = new WORD[m_IndexNum];

		nCount = 0;
		for (int n = 0; n < 4; n++) {
			pIndex[nCount] = n;
			pIndex[nCount + 1] = n + 1;
			nCount += 2;
		}
		pIndex[nCount - 1] = 0;

		for (int n = 0; n < 4; n++) {
			pIndex[nCount] = n;
			pIndex[nCount + 1] = n + 4;
			nCount += 2;
		}

		for (int n = 4; n < 8; n++) {
			pIndex[nCount] = n;
			pIndex[nCount + 1] = n + 1;
			nCount += 2;
		}
		pIndex[nCount - 1] = 4;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.ByteWidth = sizeof(WORD) * m_IndexNum;
		bd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = pIndex;

		hr = D3DApp::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
		if (FAILED(hr))
			MessageBox(NULL, "頂点インデックスバッファの生成に失敗しました。", "BoxCollider", MB_OK);
	
		delete[] pIndex;
	}
	m_Texture = TextureManager::GetTexture("sky");
}

void DirectX::BoxCollider::ReleaseRenderBuffer()
{
	if (m_IndexBuffer)
		m_IndexBuffer->Release();
	if (m_VertexBuffer)
		m_VertexBuffer->Release();
	delete[] m_pVertex;
}

void DirectX::BoxCollider::SetSize(Vector3 size)
{
	this->bound.SetSize(size);
}

void DirectX::BoxCollider::Render()
{
	D3DApp::Renderer::SetVertexBuffer(m_VertexBuffer);
	D3DApp::Renderer::SetIndexBuffer(m_IndexBuffer);

	D3DApp::Renderer::SetTexture(m_Texture);

	Vector3 scale = bound.GetSize() * 2.0f;
	Vector3 pos = this->transform.lock()->position() + bound.GetCenter();

	XMMATRIX world = XMMatrixIdentity();
	world *= XMMatrixScaling(scale.x, scale.y, scale.z);
	world *= this->transform.lock()->MatrixScaling();
	world *= XMMatrixTranslation(pos.x, pos.y, pos.z);

	D3DApp::Renderer::SetWorldMatrix(&world);

	D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	D3DApp::GetDeviceContext()->DrawIndexed(m_IndexNum,0,0);
}




