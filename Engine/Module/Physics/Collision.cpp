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

#include"Module\Mesh\Mesh.h"
#include"Module\Field\field.h"

#include"Rigidbody.h"
#include"Collision.h"

using namespace DirectX;

DirectX::Bounds::Bounds(Vector3 center, Vector3 size)
{
	this->m_center = center;
	this->m_size = size;
}

Vector3 DirectX::Bounds::GetCenter()
{
	return this->m_center;
}

Vector3 DirectX::Bounds::GetExtents()
{
	return this->m_size * 0.5f;
}

Vector3 DirectX::Bounds::GetMax()
{
	return this->m_center + this->GetExtents();
}

Vector3 DirectX::Bounds::GetMin()
{
	return this->m_center - this->GetExtents();
}

Vector3 DirectX::Bounds::GetSize()
{
	return this->m_size;
}

void DirectX::Bounds::SetCenter(Vector3 center)
{
	this->m_center = center;
}

void DirectX::Bounds::SetSize(Vector3 size)
{
	this->m_size = size;
}

void Bounds::DebugImGui()
{
	if (ImGui::TreeNode("Bounds")) {
		ImGui::InputFloat3("Center",&m_center.x);
		ImGui::InputFloat3("Size",&m_size.x);
		ImGui::TreePop();
	}
}

//--- Collider --------------------------------------------

//Constrcutor
DirectX::Collider::Collider(EntityID OwnerID)
:
	Component(OwnerID),
	bound(Vector3::zero(),Vector3::one())
{
	this->SendComponentMessage = [this](std::string message){
		if (message == "Start")	 { Start(); return; }
		if (message == "Render") { Render(); return; }
	};
}

DirectX::Collider::~Collider()
{

}

void DirectX::Collider::Start()
{
	//Rigidbodyを探す
	auto rigidbody = ComponentManager::GetComponent<Rigidbody>(this->GetOwnerID());
	if (rigidbody.expired())
	{
		auto component = this->transform()->GetComponentInParent(Rigidbody::TypeID);
		if (component.expired()) return;
		rigidbody = std::dynamic_pointer_cast<Rigidbody>(component.lock());
	}
	_rigidbody = rigidbody.lock();
	_rigidbody.lock()->RegisterCollider(std::dynamic_pointer_cast<Collider>(this->_self.lock()));
}

void DirectX::Collider::Hitjudgment()
{
	auto components = ComponentManager::GetOrCreateComponentIndex(Collider::TypeID).lock();
	auto rigidbodys = ComponentManager::GetOrCreateComponentIndex(Rigidbody::TypeID).lock();

	for(auto self:*components){
		auto collider = std::dynamic_pointer_cast<Collider>(self.lock());
		for (auto other : *components) {
			if (self.lock() == other.lock()) continue;
			auto otherCollider = std::dynamic_pointer_cast<Collider>(other.lock());

			collider->Judgment(otherCollider.get());
		}
	}
}

void DirectX::Collider::Hitjudgment(std::list<std::weak_ptr<Collider>>& colliderlist)
{
	auto components = ComponentManager::GetOrCreateComponentIndex(Collider::TypeID).lock();
	std::list<std::weak_ptr<Collider>> colliders;

	for (auto self : *components)
		colliders.push_back(std::dynamic_pointer_cast<Collider>(self.lock()));

	for(auto collider:colliderlist)
		for (auto other : colliders)
			collider.lock()->Judgment(other.lock().get());
}

void DirectX::Collider::Hitjudgment(std::list<std::weak_ptr<Collider>>& colliderlist, std::list<std::weak_ptr<Collider>>& otherlist)
{
	for(auto collider: colliderlist)
	{
		for (auto other : otherlist) {
			collider.lock()->Judgment(other.lock().get());
		}
	}
}

bool DirectX::Collider::BoxVsBox(Collider * collider, Collider * other)
{
	Bounds& bound1 = collider->bound;
	Bounds& bound2 = other->bound;

	Vector3 pos1 = collider->transform()->position() + bound1.GetCenter();
	Vector3 pos2 = other->transform()->position() + bound2.GetCenter();

	Vector3 scale1 = collider->transform()->scale() * 2.0f;
	Vector3 scale2 = other->transform()->scale() * 2.0f;

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
		////IsTrigger
		//if (!collider->IsTrigger || !other->IsTrigger) {
		//	//Rigidbodyを所持
		//	auto rigidbody = collider->gameObject()->GetComponent<Rigidbody>();
		//	if (rigidbody)
		//	{
		//		Vector3 Intrusion;
		//		Vector3 velocity = rigidbody->GetVelocity();
		//		Vector3 direction = Vector3::Normalize(pos1 - pos2);
		//		if (direction.y < 0.0f)
		//		{
		//			Intrusion.y = pos1_max.y - pos2_min.y;
		//			velocity.y = velocity.y > 0.0f ? 0.0f : velocity.y;
		//		}
		//		else
		//		{
		//			Intrusion.y = pos1_min.y - pos2_max.y;
		//			velocity.y = velocity.y < 0.0f ? 0.0f : velocity.y;
		//		}

		//		rigidbody->SetVelocity(velocity);
		//		collider->transform()->position(collider->transform()->position() - Intrusion);
		//	}
		//}
		return true;
	}
	return false;
}

bool DirectX::Collider::BoxVsShpere(Collider * collider, Collider * other)
{
	Bounds& BoxBound = collider->bound;
	Bounds& SphereBound = other->bound;

	Vector3 BoxPos = collider->transform()->position() + BoxBound.GetCenter();
	Vector3 SphPos = other->transform()->position() + SphereBound.GetCenter();

	Vector3 pos1_max = BoxPos + Vector3(BoxBound.GetMax() * other->transform()->scale());
	Vector3 pos1_min = BoxPos + Vector3(BoxBound.GetMin() * other->transform()->scale());

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

	float radius1 = bound1.GetSize().x * collider->transform()->scale().MaxElement();
	float radius2 = bound2.GetSize().x * other->transform()->scale().MaxElement();

	Vector3 pos1 = XMVector3TransformCoord(bound1.GetCenter(),collider->transform()->WorldMatrix());
	Vector3 pos2 = XMVector3TransformCoord(bound2.GetCenter(),other->transform()->WorldMatrix());

	Vector3 distance = pos1 - pos2;
	//衝突
	if (distance.lengthSq() < (radius1 + radius2) * (radius1 + radius2)) 
	{
		if(!collider->_rigidbody.expired())
		{
			auto rigidbody = collider->_rigidbody.lock();
			float length = (radius1 + radius2) - distance.length();
			Vector3 vec = distance.normalize() * length;
			rigidbody->transform()->position(rigidbody->transform()->position() + vec);
		}
		return true;
	}
	return false;
}

//--- SphereCollider ----------------------------------------------------------

ID3D11Buffer* SphereCollider::m_VertexBuffer = nullptr;
ID3D11Buffer* SphereCollider::m_IndexBuffer = nullptr;
VERTEX_3D* SphereCollider::m_pVertex = nullptr;
int SphereCollider::m_IndexNum = 0;
Texture* SphereCollider::m_Texture = nullptr;

DirectX::SphereCollider::SphereCollider(EntityID OwnerID)
:
	Collider(OwnerID)
{
	this->SetRadius(0.25f);
}

DirectX::SphereCollider::~SphereCollider()
{
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
	float rad = this->transform()->scale().MaxElement();
	Vector3 scale = bound.GetSize() * rad;
	Vector3 pos = bound.GetCenter();

	XMMATRIX world;
	XMMATRIX local;
	world = XMMatrixScaling(scale.x, scale.y, scale.z) *  XMMatrixTranslation(pos.x, pos.y, pos.z);
	world *= this->transform()->MatrixQuaternion()* this->transform()->MatrixTranslation();

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
	default:
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
	this->OnDebugImGui = [this]() {
		if (ImGui::TreeNode("BoxCollider")) {
			ImGui::Checkbox("IsTrigger",&this->IsTrigger);
			ImGui::Checkbox("IsHit",&this->IsHit);
			if(!_rigidbody.expired())
				ImGui::Text(("RigidbodyName:"+_rigidbody.lock()->gameObject()->GetName()).c_str());
			this->bound.DebugImGui();
			ImGui::TreePop();
		}
	};
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
	Vector3 pos = this->transform()->position() + bound.GetCenter();

	XMMATRIX world = XMMatrixIdentity();
	world *= XMMatrixScaling(scale.x, scale.y, scale.z);
	world *= this->transform()->MatrixScaling();
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
		IsHit = BoxVsBox(this,other);
		break;
	case ShapeType::Sphere:
		IsHit = SphereVsSphere(this,other);
	default:
		IsHit = false;
		break;
	}
	return IsHit;
}

DirectX::FieldCollider::FieldCollider(EntityID OwnerID)
	:
	Collider(OwnerID)
{

}

void DirectX::FieldCollider::SetMesh(MeshField * field)
{
	this->_field = field;
}

bool DirectX::FieldCollider::IsOnGround(Vector3 Position)
{
	float width = this->_field->m_Width * this->transform()->scale().x;
	float depth = this->_field->m_Depth * this->transform()->scale().z;
	float x = ( Position.x + (width * this->_field->m_WidthNum * 0.5f)) / width;
	float z = (-Position.z + (depth * this->_field->m_DepthNum * 0.5f)) / depth;
	return 0.0 < x && x < (float)this->_field->m_WidthNum && 0.0f < z && z < (float)this->_field->m_DepthNum;
}

float DirectX::FieldCollider::GetHeight(Vector3 Position)
{
	Transform* trans = this->transform().get();
	Vector3 scale = trans->scale();
	auto VertexIndex = this->_field->_VertexArray;

	unsigned int widthNum = this->_field->m_WidthNum;
	unsigned int depthNum = this->_field->m_DepthNum;

	float width = this->_field->m_Width * scale.x;
	float depth = this->_field->m_Depth * scale.z;

	int x = ( Position.x + (width * widthNum * 0.5f)) / width;
	int z = (-Position.z + (depth * depthNum * 0.5f)) / depth;

	Vector3 PosV(VertexIndex[z * (widthNum + 1) + x].Position);
	Vector3 PosA(VertexIndex[(z + 1) * (widthNum + 1) + (x + 1)].Position);

	Vector3 va = PosA - PosV;
	Vector3 vp = Position - PosV;
	
	Vector3 p0, p1, p2;
	if(Vector3::Cross(va,vp).y > 0.0f)
	{
		p0 = VertexIndex[(z + 1) * (widthNum + 1) +  x		].Position;
		p1 = VertexIndex[ z		 * (widthNum + 1) +  x		].Position;
		p2 = VertexIndex[(z + 1) * (widthNum + 1) + (x + 1)	].Position;
	}
	else
	{
		p0 = VertexIndex[ z		 * (widthNum + 1) + (x + 1) ].Position;
		p1 = VertexIndex[(z + 1) * (widthNum + 1) + (x + 1)	].Position;
		p2 = VertexIndex[ z		 * (widthNum + 1) +  x		].Position;
	}

	p0 *= scale;
	p1 *= scale;
	p2 *= scale;

	Vector3 normal = Vector3::Cross((p1 - p0),(p2 - p0));	//面の法線

	Vector3 v(0.0f,-10.0f,0.0f);	//プレイヤーからの垂線

	float dvn  = Vector3::Dot(v,normal);
	float dp0n = Vector3::Dot(p0,normal);
	normal.z *= -1.0;	//dpnが逆になるのを防ぐ
	float dpn = Vector3::Dot(Position,normal);

	float t;
	if (dvn == 0.0f || (dp0n - dpn) == 0.0f)
		t = 0.0f;
	else
		t = (dp0n - dpn) / dvn;

	Vector3 hp = Position + v*t;

	if (isnan(hp.y))
		return 0.0f;

	return hp.y;
}

Vector3 DirectX::FieldCollider::GetNormal(Vector3 Position)
{
	Transform* trans = this->transform().get();
	Vector3 scale = trans->scale();
	auto VertexIndex = this->_field->_VertexArray;

	unsigned int widthNum = this->_field->m_WidthNum;
	unsigned int depthNum = this->_field->m_DepthNum;

	float width = this->_field->m_Width * scale.x;
	float depth = this->_field->m_Depth * scale.z;

	int x = (Position.x + (width * widthNum * 0.5f)) / width;
	int z = (-Position.z + (depth * depthNum * 0.5f)) / depth;

	Vector3 PosV(VertexIndex[z * (widthNum + 1) + x].Position);
	Vector3 PosA(VertexIndex[(z + 1) * (widthNum + 1) + (x + 1)].Position);

	Vector3 va = PosA - PosV;
	Vector3 vp = Position - PosV;

	Vector3 p0, p1, p2;
	if (Vector3::Cross(va, vp).y > 0.0f)
	{
		p0 = VertexIndex[(z + 1) * (widthNum + 1) + x].Position;
		p1 = VertexIndex[z		 * (widthNum + 1) + x].Position;
		p2 = VertexIndex[(z + 1) * (widthNum + 1) + (x + 1)].Position;
	}
	else
	{
		p0 = VertexIndex[z		 * (widthNum + 1) + (x + 1)].Position;
		p1 = VertexIndex[(z + 1) * (widthNum + 1) + (x + 1)].Position;
		p2 = VertexIndex[z		 * (widthNum + 1) + x].Position;
	}

	p0 *= scale;
	p1 *= scale;
	p2 *= scale;

	Vector3 normal = Vector3::Cross((p1 - p0), (p2 - p0));	//面の法線

	return normal.normalize();
}
