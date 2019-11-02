#include"Common.h"

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include"Module\Texture\texture.h"
#include"Module\AssetData\AssetData.h"
#include"Module\Texture\TextureManager.h"
#include"Module\ECSEngine.h"

#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"
#include"Module\Transform\Transform.h"

#include"Rigidbody.h"
#include"Collision.h"

using namespace DirectX;

Collider::ComponentIndex Collider::ComponentType::Index;

DirectX::Bounds::Bounds(Vector3 center, Vector3 size)
{
	this->m_center = center;
	this->m_size = size;
}

//--- Collider --------------------------------------------

//Constrcutor
DirectX::Collider::Collider(EntityID OwnerID)
:
	Component(OwnerID),
	bound(Vector3::zero(),Vector3::one())
{

}

//void DirectX::Collider::SendBehaviourMessage(Message message)
//{
//	if (message != Message::Render) return;
//	Render();
//}

void DirectX::Collider::Hitjudgment(GameObject * gameObject, GameObject * otherObject)
{
	//for(auto collider:gameObject->colliders) {
	//	if (collider->IsEnable) continue;	//Enable
	//	for (auto othercollider : otherObject->colliders) {
	//		if (othercollider->IsEnable) continue;	//Enable
	//		collider->Judgment(othercollider.get());	//判定
	//	}
	//}
}

bool DirectX::Collider::BoxVsBox(Collider * collider, Collider * other)
{
	Bounds& bound1 = collider->bound;
	Bounds& bound2 = other->bound;

	Vector3 pos1 = collider->transform().lock()->position() + bound1.GetCenter();
	Vector3 pos2 = other->transform().lock()->position() + bound2.GetCenter();

	Vector3 scale1 = collider->transform().lock()->scale() * 2.0f;
	Vector3 scale2 = other->transform().lock()->scale() * 2.0f;

	Vector3 pos1_max = pos1 + Vector3(bound1.GetMax() * scale1);
	Vector3 pos1_min = pos1 + Vector3(bound1.GetMin() * scale1);

	Vector3 pos2_max = pos2 + Vector3(bound2.GetMax() * scale2);
	Vector3 pos2_min = pos2 + Vector3(bound2.GetMin() * scale2);

	//衝突
	if ((pos2_min.x <= pos1_min.x && pos1_min.x <= pos2_max.x || pos2_min.x <= pos1_max.x && pos1_max.x <= pos2_max.x) &&
		(pos2_min.y <= pos1_min.y && pos1_min.y <= pos2_max.y || pos2_min.y <= pos1_max.y && pos1_max.y <= pos2_max.y) &&
		(pos2_min.z <= pos1_min.z && pos1_min.z <= pos2_max.z || pos2_min.z <= pos1_max.z && pos1_max.z <= pos2_max.z)
		)
	{
		//Rigidbodyを所持
		//if(collider->gameObject.lock()->rigidbody)
		//{
		//	Rigidbody& rigidbody = *collider->gameObject.lock()->rigidbody.get();

		//	Vector3 Intrusion;
		//	Vector3 velocity = rigidbody.GetVelocity();
		//	Vector3 direction = (pos1 - pos2).normalize();

		//	if (direction.y < 0.0f) {
		//		Intrusion.y = pos1_max.y - pos2_min.y;
		//		velocity.y = velocity.y > 0.0f ? 0.0f : velocity.y;
		//	}
		//	else{
		//		Intrusion.y = pos1_min.y - pos2_max.y;
		//		velocity.y = velocity.y < 0.0f ? 0.0f : velocity.y;
		//	}
		//	rigidbody.SetVelocity(velocity);
		//	collider->transform().lock()->position(collider->transform().lock()->position() - Intrusion);
		//}
		return true;
	}
	return false;
}

bool DirectX::Collider::BoxVsShpere(Collider * collider, Collider * other)
{
	Bounds& BoxBound = collider->bound;
	Bounds& SphereBound = other->bound;

	Vector3 BoxPos = collider->transform().lock()->position() + BoxBound.GetCenter();
	Vector3 SphPos = other->transform().lock()->position() + SphereBound.GetCenter();

	Vector3 pos1_max = BoxPos + Vector3(BoxBound.GetMax() * other->transform().lock()->scale());
	Vector3 pos1_min = BoxPos + Vector3(BoxBound.GetMin() * other->transform().lock()->scale());

	float radius = SphereBound.GetSize().x;

	Vector3 posMax,posMin,pos;

	//X軸
	posMax = pos1_max + Vector3(1.0f, 0.0f, 0.0f) * radius;
	posMin = pos1_min - Vector3(1.0f, 0.0f, 0.0f) * radius;
	if (posMin.x <= SphPos.x && SphPos.x <= posMax.x &&
		posMin.y <= SphPos.y && SphPos.y <= posMax.y &&
		posMin.z <= SphPos.z && SphPos.z <= posMax.z)
		return true;

	//Y軸
	posMax = pos1_max + Vector3(0.0f, 0.0f, 1.0f) * radius;
	posMin = pos1_min - Vector3(0.0f, 0.0f, 1.0f) * radius;
	if (posMin.x <= SphPos.x && SphPos.x <= posMax.x &&
		posMin.y <= SphPos.y && SphPos.y <= posMax.y &&
		posMin.z <= SphPos.z && SphPos.z <= posMax.z)
		return true;

	//Z軸
	posMax = pos1_max + Vector3(0.0f, 0.0f, 1.0f) * radius;
	posMin = pos1_min - Vector3(0.0f, 0.0f, 1.0f) * radius;
	if (posMin.x <= SphPos.x && SphPos.x <= posMax.x &&
		posMin.y <= SphPos.y && SphPos.y <= posMax.y &&
		posMin.z <= SphPos.z && SphPos.z <= posMax.z)
		return true;

	pos.x = SphPos.x - pos1_max.x < SphPos.x - pos1_min.x ? pos1_max.x : pos1_min.x;
	pos.y = SphPos.y - pos1_max.y < SphPos.y - pos1_min.y ? pos1_max.y : pos1_min.y;
	pos.z = SphPos.z - pos1_max.z < SphPos.z - pos1_min.z ? pos1_max.z : pos1_min.z;

	pos = pos - SphPos;
	if (pos.lengthSq() < radius * radius)
		return true;

	return false;
}

bool DirectX::Collider::SphereVsSphere(Collider * collider, Collider * other)
{
	Bounds& bound1 = collider->bound;
	Bounds& bound2 = other->bound;

	float radius1 = bound1.GetSize().x * collider->transform().lock()->scale().MaxElement();
	float radius2 = bound2.GetSize().x * other->transform().lock()->scale().MaxElement();

	Vector3 pos1 = XMVector3TransformCoord(bound1.GetCenter(),collider->transform().lock()->WorldMatrix());
	Vector3 pos2 = XMVector3TransformCoord(bound2.GetCenter(),other->transform().lock()->WorldMatrix());

	Vector3 distance = pos1 - pos2;
	//衝突
	if (distance.lengthSq() < (radius1 + radius2) * (radius1 + radius2))
		return true;

	return false;
}

//--- SphereCollider ----------------------------------------------------------

ID3D11Buffer* SphereCollider::m_VertexBuffer = nullptr;
ID3D11Buffer* SphereCollider::m_IndexBuffer = nullptr;
VERTEX_3D* SphereCollider::m_pVertex = nullptr;
int SphereCollider::m_IndexNum = 0;
Texture* SphereCollider::m_Texture = nullptr;

SphereCollider::SphereCollider(EntityID OwnerID)
:
	Collider(OwnerID)
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

	D3DApp::Renderer::SetTexture(m_Texture->GetShaderResourceView());
	float rad = this->transform().lock()->scale().MaxElement();
	Vector3 scale = bound.GetSize() * rad;
	Vector3 pos = bound.GetCenter();

	XMMATRIX world;
	XMMATRIX local;
	world = XMMatrixScaling(scale.x, scale.y, scale.z) *  XMMatrixTranslation(pos.x, pos.y, pos.z);
	world *= this->transform().lock()->MatrixQuaternion()* this->transform().lock()->MatrixTranslation();

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

bool DirectX::SphereCollider::Judgment(Collider * other)
{
	IsHit = false;
	switch (other->GetShapeType())
	{
	case ShapeType::Box:
		IsHit = BoxVsShpere(this,other);
		break;
	case ShapeType::Sphere:
		IsHit = SphereVsSphere(this,other);
		break;
	}

	return IsHit;
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

DirectX::BoxCollider::BoxCollider(EntityID OwnerID)
:
	Collider(OwnerID)
{
}

void DirectX::BoxCollider::SetSize(Vector3 size)
{
	this->bound.SetSize(size);
}

void DirectX::BoxCollider::Render()
{
	D3DApp::Renderer::SetVertexBuffer(m_VertexBuffer);
	D3DApp::Renderer::SetIndexBuffer(m_IndexBuffer);

	D3DApp::Renderer::SetTexture(m_Texture->GetShaderResourceView());

	Vector3 scale = bound.GetSize() * 2.0f;
	Vector3 pos = this->transform().lock()->position() + bound.GetCenter();

	XMMATRIX world = XMMatrixIdentity();
	world *= XMMatrixScaling(scale.x, scale.y, scale.z);
	world *= this->transform().lock()->MatrixScaling();
	world *= XMMatrixTranslation(pos.x, pos.y, pos.z);

	D3DApp::Renderer::SetWorldMatrix(&world);

	D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	D3DApp::GetDeviceContext()->DrawIndexed(m_IndexNum,0,0);
}

bool DirectX::BoxCollider::Judgment(Collider * other)
{
	switch (other->GetShapeType())
	{
	case ShapeType::Box:
		IsHit = BoxVsShpere(this,other);
		break;
	case ShapeType::Sphere:
		IsHit = SphereVsSphere(this,other);
	default:
		break;
	}
	return IsHit;
}




