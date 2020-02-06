#define INCLUDE_CEREAL
#include"Common.h"

#include"Module\DirectX\DirectX.h"

#include"Module\Texture\texture.h"
#include"Module\Texture\TextureManager.h"
#include"Module\ECSEngine.h"

#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"
#include"Module\Transform\Transform.h"

#include"Module\Mesh\Mesh.h"
#include"Module\Field\field.h"

#include"Rigidbody.h"
#include"Collision.h"

#include"Module\Shader\Shader.h"

using namespace DirectX;

//*********************************************************************************************************************
//
//	Collision
//
//*********************************************************************************************************************

Bounds::Bounds(Vector3 center, Vector3 size)
{
	this->m_center = center;
	this->m_size = size;
}

Vector3 Bounds::GetCenter()
{
	return this->m_center;
}

Vector3 Bounds::GetExtents()
{
	return this->m_size * 0.5f;
}

Vector3 Bounds::GetMax()
{
	return this->m_center + this->GetExtents();
}

Vector3 Bounds::GetMin()
{
	return this->m_center - this->GetExtents();
}

Vector3 Bounds::GetSize()
{
	return this->m_size;
}

float Bounds::GetRadius()
{
	return m_size.x;
}

void Bounds::SetCenter(Vector3 center)
{
	this->m_center = center;
}

void Bounds::SetSize(Vector3 size)
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
Collider::Collider(EntityID OwnerID)
:
	Component(OwnerID),
	bound(Vector3::zero(),Vector3::one())
{

}

Collider::~Collider()
{

}

void Collider::Start()
{
	//Rigidbodyを探す
	auto rigidbody = this->gameObject()->GetComponent<Rigidbody>();
	if (rigidbody.expired())
	{
		auto component = this->transform()->GetComponentInParent(typeid(Rigidbody).hash_code());
		if (component.expired()) return;
		rigidbody = std::dynamic_pointer_cast<Rigidbody>(component.lock());
	}
	_rigidbody = rigidbody.lock();
	//_rigidbody.lock()->RegisterCollider(std::dynamic_pointer_cast<Collider>(GetSelf().lock()));
}

void Collider::IsHitReset()
{
	for (auto col : ComponentIndex) col.second.lock()->_IsHit = false;
}

void Collider::Hitjudgment()
{
	for(auto collider : ComponentIndex)
		for(auto other : ComponentIndex)
		{
			if (collider.second.lock() == other.second.lock()) continue;
			collider.second.lock()->Judgment(other.second.lock().get());
		}
}

void Collider::Hitjudgment(std::list<std::weak_ptr<Collider>>& colliderlist)
{
	for(auto collider : colliderlist)
		for(auto other : ComponentIndex)
		{
			if (collider.lock() == other.second.lock()) continue;
			collider.lock()->Judgment(other.second.lock().get());
		}
}

void Collider::Hitjudgment(std::list<std::weak_ptr<Collider>>& colliderlist, std::list<std::weak_ptr<Collider>>& otherlist)
{
	for(auto collider: colliderlist)
	{
		for (auto other : otherlist) 
		{
			if (collider.lock() == other.lock()) continue;
			collider.lock()->Judgment(other.lock().get());
		}
	}
}

bool Collider::BoxVsBox(Collider * collider, Collider * other)
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
		return true;
	}
	return false;
}

bool Collider::OBBVsOBB(Collider * colA, Collider * colB)
{
	Bounds& boundA = colA->bound;
	Bounds& boundB = colB->bound;

	//空間行列
	XMMATRIX worldA = colA->transform()->WorldMatrix();
	XMMATRIX worldB = colB->transform()->WorldMatrix();

	//中心
	Vector3 centerA = XMVector3TransformCoord(boundA.GetCenter(),worldA);
	Vector3 centerB = XMVector3TransformCoord(boundB.GetCenter(),worldB);

	//Boxのサイズ
	Vector3 sizeA = boundA.GetSize() * colA->transform()->scale();
	Vector3 sizeB = boundB.GetSize() * colB->transform()->scale();

	Vector3 distance = centerA - centerB;

	auto DotSepAxis = [](Vector3* SepAxis,Vector3* right,Vector3* forward,Vector3* up = 0)
	{
		float r1 = fabsf(Vector3::Dot(*right, *SepAxis));
		float r2 = fabsf(Vector3::Dot(*forward, *SepAxis));
		float r3 = up ? (fabsf(Vector3::Dot(*up,*SepAxis))) : 0.0f;
		return r1 + r2 + r3;
	};

	//分離軸
	Vector3 SepAxis;

	//分離軸上の長さ
	float rA;	//ColAの長さ
	float rB;	//ColBの長さ
	float L;	//ColAとColB間の長さ

	//Axis
	Vector3 colA_Right	 = colA->transform()->right();
	Vector3 colA_Forward = colA->transform()->forward();
	Vector3 colA_Up		 = colA->transform()->up();

	Vector3 colB_Right	 = colB->transform()->right();
	Vector3 colB_Forward = colB->transform()->forward();
	Vector3 colB_Up		 = colB->transform()->up();

	//AxisLength
	Vector3 colA_RightLen	= colA_Right * sizeA.x;
	Vector3 colA_ForwardLen = colA_Forward * sizeA.z;
	Vector3 colA_UpLen		= colA_Up * sizeA.y;

	Vector3 colB_RightLen	= colB_Right * sizeB.x;
	Vector3 colB_ForwardLen = colB_Forward * sizeB.z;
	Vector3 colB_UpLen		= colB_Up * sizeB.y;

	//ColA
	{
		//ColA Right
		{
			SepAxis = colA_Right;

			rA = Vector3::Length(SepAxis * sizeA.x);
			rB = DotSepAxis(&SepAxis, &colB_RightLen, &colB_ForwardLen,&colB_UpLen);

			L = fabsf(Vector3::Dot(distance, SepAxis));
			if (L > rA + rB) return false;
			
		}

		//ColA Forward
		{
			SepAxis = colA_Forward;

			rA = Vector3::Length(SepAxis * sizeA.z);
			rB = DotSepAxis(&SepAxis, &colB_RightLen, &colB_ForwardLen, &colB_UpLen);

			L = fabsf(Vector3::Dot(distance, SepAxis));
			if (L > rA + rB) return false;
		}

		//ColA Up
		{
			SepAxis = colA_Up;

			rA = Vector3::Length(SepAxis * sizeA.y);
			rB = DotSepAxis(&SepAxis, &colB_RightLen, &colB_ForwardLen, &colB_UpLen);

			L = fabsf(Vector3::Dot(distance, SepAxis));
			if (L > rA + rB) return false;
		}
	}
	
	//ColB
	{
		//ColB right
		{
			SepAxis = colB_Right;

			rA = Vector3::Length(SepAxis * sizeB.x);
			rB = DotSepAxis(&SepAxis,&colA_RightLen,&colA_ForwardLen,&colA_UpLen);

			L = fabsf(Vector3::Dot(distance,SepAxis));
			if (L > rA + rB) return false;	
		}

		//ColB forward
		{
			SepAxis = colB->transform()->forward();

			rA = Vector3::Length(SepAxis * sizeB.z);
			rB = DotSepAxis(&SepAxis, &colA_RightLen, &colA_ForwardLen, &colA_UpLen);

			L = fabsf(Vector3::Dot(distance,SepAxis));
			if (L > rA + rB) return false;
		}

		//ColB up
		{
			SepAxis = colB_Up;

			rA = Vector3::Length(SepAxis * sizeB.y);
			rB = DotSepAxis(&SepAxis, &colA_RightLen, &colA_ForwardLen, &colA_UpLen);

			L = fabsf(Vector3::Dot(distance,SepAxis));
			if (L > rA + rB) return false;
		}
	}

	//外積
	Vector3 Cross;

	{
		//Forward vs Forward
		{
			Cross = Vector3::Cross(colA_Forward, colB_Forward);
			rA = DotSepAxis(&Cross,&colA_RightLen,&colA_UpLen);
			rB = DotSepAxis(&Cross,&colB_RightLen,&colB_UpLen);

			L = fabsf(Vector3::Dot(distance,Cross));
			if (L > rA + rB) return false;
		}

		//Forward vs Right
		{
			Cross = Vector3::Cross(colA_Forward,colB_Right);
			rA = DotSepAxis(&Cross,&colA_RightLen,&colA_UpLen);
			rB = DotSepAxis(&Cross,&colB_ForwardLen,&colB_UpLen);

			L = fabsf(Vector3::Dot(distance,Cross));
			if (L > rA + rB) return false;
		}
		
		//Forward vs Up
		{
			Cross = Vector3::Cross(colA_Forward,colB_Up);
			rA = DotSepAxis(&Cross,&colA_RightLen,&colA_UpLen);
			rB = DotSepAxis(&Cross,&colB_RightLen,&colB_ForwardLen);

			L = fabsf(Vector3::Dot(distance,Cross));
			if (L > rA + rB) return false;
		}

		//Right vs Forward
		{
			Cross = Vector3::Cross(colA_Right,colB_Forward);
			rA = DotSepAxis(&Cross,&colA_ForwardLen,&colA_UpLen);
			rB = DotSepAxis(&Cross,&colA_RightLen,&colB_UpLen);

			L = fabsf(Vector3::Dot(distance,Cross));
			if (L > rA + rB) return false;
		}

		//Right vs Right
		{
			Cross = Vector3::Cross(colA_Right,colB_Right);
			rA = DotSepAxis(&Cross, &colA_ForwardLen, &colA_UpLen);
			rB = DotSepAxis(&Cross, &colB_ForwardLen, &colB_UpLen);

			L = fabsf(Vector3::Dot(distance,Cross));
			if(L > rA + rB) return false;
		}

		//Right vs Up
		{
			Cross = Vector3::Cross(colA_Right,colB_Up);
			rA = DotSepAxis(&Cross, &colA_ForwardLen, &colA_UpLen);
			rB = DotSepAxis(&Cross, &colB_ForwardLen, &colB_RightLen);

			L = fabsf(Vector3::Dot(distance,Cross));
			if (L > rA + rB) return false;
		}

		//Up vs Forward
		{
			Cross = Vector3::Cross(colA_Up,colB_Forward);
			rA = DotSepAxis(&Cross,&colA_RightLen,&colA_ForwardLen);
			rB = DotSepAxis(&Cross,&colB_RightLen,&colB_UpLen);

			L = fabsf(Vector3::Dot(distance,Cross));
			if (L > rA + rB) return false;
		}

		//Up vs Right
		{
			Cross = Vector3::Cross(colA_Up,colB_Right);
			rA = DotSepAxis(&Cross, &colA_RightLen, &colA_ForwardLen);
			rB = DotSepAxis(&Cross, &colB_ForwardLen,&colB_UpLen);

			L = fabsf(Vector3::Dot(distance,Cross));
			if (L > rA + rB) return false;
		}

		//Up vs Up
		{
			Cross = Vector3::Cross(colA_Up, colB_Up);
			rA = DotSepAxis(&Cross, &colA_RightLen, &colA_ForwardLen);
			rB = DotSepAxis(&Cross, &colB_ForwardLen,&colB_RightLen);

			L = fabsf(Vector3::Dot(distance,Cross));
			if (L > rA + rB) return false;
		}
	}

	//物理判定
	if (!colA->IsTrigger || !colB->IsTrigger)
	{
		if(!colA->_rigidbody.expired())
		{
			//向きを分離軸に設定
			SepAxis = distance.normalize();

			rA = DotSepAxis(&SepAxis,&colA_RightLen,&colA_ForwardLen,&colA_UpLen);
			rB = DotSepAxis(&SepAxis,&colB_RightLen,&colB_ForwardLen,&colB_UpLen);

			Vector3 Rep = SepAxis * ((rA + rB) - distance.length());

			auto rigidbody = colA->_rigidbody.lock();
			rigidbody->transform()->position(rigidbody->transform()->position() + Rep);
		}
	}

	return true;
}

bool Collider::OBBVsShpere(Collider * box, Collider * sphere)
{
	Bounds& BoxB = box->bound;
	Bounds& SphB = sphere->bound;

	//空間行列
	XMMATRIX BoxWorld = box->transform()->WorldMatrix();
	XMMATRIX SphWorld = sphere->transform()->WorldMatrix();

	//中心
	Vector3 BoxC = XMVector3TransformCoord(BoxB.GetCenter(), BoxWorld);
	Vector3 SphC = XMVector3TransformCoord(SphB.GetCenter(), SphWorld);

	//Boxのサイズ
	Vector3 boxSize = BoxB.GetSize() * box->transform()->scale();
	float radius = SphB.GetRadius();

	Vector3 distance = BoxC - SphC;

	Vector3 Box_Right = box->transform()->right();
	Vector3 Box_Forward = box->transform()->forward();
	Vector3 Box_Up = box->transform()->up();
	
	Vector3 Box_RightLen = Box_Right * boxSize.x;
	Vector3 Box_ForwardLen = Box_Forward * boxSize.z;
	Vector3 Box_UpLen = Box_Up * boxSize.y;

	float rA,rB,L;
	
	{
		rA = Vector3::Length(Box_RightLen);
		rB = radius;

		L = fabsf(Vector3::Dot(distance, Box_Right));

		if (L > rA + rB)
			return false;
	}

	{
		rA = Vector3::Length(Box_ForwardLen);
		rB = radius;
	
		L = fabsf(Vector3::Dot(distance,Box_Forward));
		if (L > rA + rB)
			return false;
	}

	{
		rA = Vector3::Length(Box_UpLen);
		rB = radius;

		L = fabsf(Vector3::Dot(distance,Box_Up));
		if (L > rA + rB)
			return false;
	}

	return true;
}



bool Collider::BoxVsShpere(Collider * collider, Collider * other)
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

bool Collider::SphereVsSphere(Collider * collider, Collider * other)
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
		//物理判定しない
		if (collider->IsTrigger || other->IsTrigger)
		{
			return true;
		}
		//
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

bool Collider::SphereVsMesh(Collider * sphCol, Collider* meshCol)
{
	Bounds& sphB = sphCol->bound;
	
	float radius = sphB.GetRadius();

	Vector3 sphC = XMVector3TransformCoord(sphB.GetCenter(),sphCol->transform()->WorldMatrix());

	auto Meshworld = meshCol->transform()->WorldMatrix();

	//物理処理
	auto Extrusion = [sphCol](Vector3 sub)->bool
	{
		if (!sphCol->_rigidbody.expired())
		{
			auto rigidbody = sphCol->_rigidbody.lock();
			rigidbody->transform()->position(rigidbody->transform()->position() + sub);
		}
		return true;
	};

	for(unsigned int i = 0; i < meshCol->_mesh->_SurfaceNum; i++)
	{
		//面判定

		auto surface = meshCol->_mesh->_Surface[i];

		//三角面
		Vector3 p1;
		p1 = surface._p1;
		p1 = XMVector3TransformCoord(XMVectorSet(p1.x,p1.y,p1.z,1.0f),Meshworld);
		Vector3 p2;
		p2 = surface._p2;
		p2 = XMVector3TransformCoord(XMVectorSet(p2.x,p2.y,p2.z,1.0f),Meshworld);
		Vector3 p3;
		p3 = surface._p3;
		p3 = XMVector3TransformCoord(XMVectorSet(p3.x,p3.y,p3.z,1.0f),Meshworld);

		//法線
		Vector3 normal = Vector3::Cross((p2 - p1),(p3 - p1));
		normal = normal.normalize();

		if (normal == Vector3::zero()) continue;

		Vector3 dir = p1 - sphC;
		float length = fabsf(Vector3::Dot(dir,normal));

		if(length < radius) return Extrusion(normal *(radius - length));

		//辺判定

		float t; //比率
		Vector3 vsphC; //頂点からの距離

		Vector3 vp12 = p2 - p1;
		vsphC = sphC - p1;
		t = Vector3::Dot(vp12.normalize(), vsphC) / vp12.length();

		length = (vp12 * t - vsphC).length();

		if (length < radius) return Extrusion(normal * length);

		Vector3 vp23 = p3 - p2;
		vsphC = sphC - p2;

		length = (vp23 * t - sphC).length();

		if (length < radius) return Extrusion(normal * length);

		Vector3 vp31 = p1 - p3;
		vsphC = sphC - p3;

		length = (vp31 * t- sphC).length();

		if (length < radius) return Extrusion(normal * length);
	}

	return false;
}

void Collider::SendComponentMessage(std::string message)
{
	if (message == "Start") return Start();
	if (message == "Render") return Render();
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



SphereCollider::~SphereCollider()
{
}

void SphereCollider::SetRenderBuffer()
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

		hr = D3DRenderer::GetInstance()->GetDevice()->CreateBuffer(&bd,&sd,&m_VertexBuffer);
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

		hr = D3DRenderer::GetInstance()->GetDevice()->CreateBuffer(&bd,&sd,&m_IndexBuffer);
		if (FAILED(hr))
			MessageBox(NULL,"頂点インデックスバッファの生成に失敗しました。","SphereCollider",MB_OK);
	
		delete[] pIndex;
	}

	m_Texture = TextureManager::GetTexture("sky").lock().get();
}

void SphereCollider::ReleaseRenderBuffer()
{
	if (m_IndexBuffer)
		m_IndexBuffer->Release();
	if (m_VertexBuffer)
		m_VertexBuffer->Release();
	delete[] m_pVertex;
}

void SphereCollider::SetRadius(float radius)
{
	this->bound.SetSize(Vector3::one() * radius);
}

void SphereCollider::Render()
{
	D3DRenderer::GetInstance()->SetVertexBuffer(m_VertexBuffer,sizeof(VERTEX_3D),0);
	D3DRenderer::GetInstance()->SetIndexBuffer(m_IndexBuffer);

	m_Texture->SetResource();
	float rad = this->transform()->scale().MaxElement();
	float radius = bound.GetRadius();
	Vector3 scale = bound.GetSize() * rad;
	Vector3 pos = bound.GetCenter();

	XMMATRIX world;
	XMMATRIX local;
	world = XMMatrixScaling(radius,radius,radius);
	world *= XMMatrixTranslation(pos.x,pos.y,pos.z);
	world *= this->transform()->MatrixQuaternion()* this->transform()->MatrixTranslation();

	D3DRenderer::GetInstance()->SetWorldMatrix(&world);
	D3DRenderer::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	D3DRenderer::GetInstance()->GetDeviceContext()->DrawIndexed(m_IndexNum, 0, 0);

	local = XMMatrixRotationZ(Mathf::PI_2) * world;
	D3DRenderer::GetInstance()->SetWorldMatrix(&local);
	D3DRenderer::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	D3DRenderer::GetInstance()->GetDeviceContext()->DrawIndexed(m_IndexNum, 0, 0);

	local = XMMatrixRotationX(Mathf::PI_2) * world;
	D3DRenderer::GetInstance()->SetWorldMatrix(&local);
	D3DRenderer::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	D3DRenderer::GetInstance()->GetDeviceContext()->DrawIndexed(m_IndexNum, 0, 0);
}

bool SphereCollider::Judgment(Collider * other)
{
	switch (other->GetShapeType())
	{
	case ShapeType::ST_Box:
		_IsHit = BoxVsShpere(this,other) || _IsHit;
		break;
	case ShapeType::ST_Sphere:
		_IsHit = SphereVsSphere(this,other) || _IsHit;
		break;
	case ShapeType::ST_Mesh:
		_IsHit = SphereVsMesh(this,other) || _IsHit;
		break;
	default:
		break;
	}

	return _IsHit;
}

void SphereCollider::OnDebugImGui()
{
	ImGui::Checkbox("IsTrigger", &this->IsTrigger);
	ImGui::Checkbox("IsHit", &this->_IsHit);
}

//--- BoxCollider -------------------------------------------------------------

ID3D11Buffer* BoxCollider::m_IndexBuffer = nullptr;
ID3D11Buffer* BoxCollider::m_VertexBuffer = nullptr;
VERTEX_3D* BoxCollider::m_pVertex = nullptr;
int BoxCollider::m_IndexNum = 0;
Texture* BoxCollider::m_Texture;

void BoxCollider::SetRenderBuffer()
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

		hr = D3DRenderer::GetInstance()->GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
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

		hr = D3DRenderer::GetInstance()->GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
		if (FAILED(hr))
			MessageBox(NULL, "頂点インデックスバッファの生成に失敗しました。", "BoxCollider", MB_OK);
	
		delete[] pIndex;
	}
	m_Texture = TextureManager::GetTexture("sky").lock().get();
}

void BoxCollider::ReleaseRenderBuffer()
{
	if (m_IndexBuffer)
		m_IndexBuffer->Release();
	if (m_VertexBuffer)
		m_VertexBuffer->Release();
	delete[] m_pVertex;
}

BoxCollider::BoxCollider(EntityID OwnerID)
:
	Collider(OwnerID)
{
}

void BoxCollider::SetSize(Vector3 size)
{
	this->bound.SetSize(size);
}

void BoxCollider::Render()
{
	D3DRenderer::GetInstance()->SetVertexBuffer(m_VertexBuffer,sizeof(VERTEX_3D),0);
	D3DRenderer::GetInstance()->SetIndexBuffer(m_IndexBuffer);

	m_Texture->SetResource();

	Vector3 scale = bound.GetSize() * 2.0f;
	Vector3 pos = bound.GetCenter();

	XMMATRIX world = XMMatrixIdentity();
	world *= XMMatrixScaling(scale.x, scale.y, scale.z);
	world *= XMMatrixTranslation(pos.x, pos.y, pos.z);
	world *= this->transform()->WorldMatrix();
	

	D3DRenderer::GetInstance()->SetWorldMatrix(&world);
	D3DRenderer::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	D3DRenderer::GetInstance()->GetDeviceContext()->DrawIndexed(m_IndexNum,0,0);
}

bool BoxCollider::Judgment(Collider * other)
{
	switch (other->GetShapeType())
	{
	case ShapeType::ST_Box:
		_IsHit = OBBVsOBB(this,other) || _IsHit;
		break;
	case ShapeType::ST_Sphere:
		_IsHit = OBBVsShpere(this, other) || _IsHit;
	default:
		break;
	}
	return _IsHit;
}

void BoxCollider::OnDebugImGui()
{
	ImGui::Checkbox("IsTrigger", &this->IsTrigger);
	ImGui::Checkbox("IsHit", &this->_IsHit);
	if (!_rigidbody.expired())
		ImGui::Text(("RigidbodyName:" + _rigidbody.lock()->gameObject()->GetName()).c_str());
	this->bound.DebugImGui();
}

FieldCollider::FieldCollider(EntityID OwnerID)
	:
	Collider(OwnerID)
{

}

void FieldCollider::SetMesh(MeshField * field)
{
	this->_field = field;
}

bool FieldCollider::IsOnGround(Vector3 Position)
{
	float width = this->_field->m_Width * this->transform()->scale().x;
	float depth = this->_field->m_Depth * this->transform()->scale().z;
	float x = ( Position.x + (width * this->_field->m_WidthNum * 0.5f)) / width;
	float z = (-Position.z + (depth * this->_field->m_DepthNum * 0.5f)) / depth;
	return 0.0 < x && x < (float)this->_field->m_WidthNum && 0.0f < z && z < (float)this->_field->m_DepthNum;
}

float FieldCollider::GetHeight(Vector3 Position)
{
	Transform* trans = this->transform().get();
	Vector3 scale = trans->scale();
	auto VertexIndex = this->_field->_VertexArray;

	unsigned int widthNum = this->_field->m_WidthNum;
	unsigned int depthNum = this->_field->m_DepthNum;

	float width = this->_field->m_Width * scale.x;
	float depth = this->_field->m_Depth * scale.z;

	int x = (int)(( Position.x + width * widthNum * 0.5) / width);
	int z = (int)((-Position.z + depth * depthNum * 0.5) / depth);

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

Vector3 FieldCollider::GetNormal(Vector3 Position)
{
	Transform* trans = this->transform().get();
	Vector3 scale = trans->scale();
	auto VertexIndex = this->_field->_VertexArray;

	unsigned int widthNum = this->_field->m_WidthNum;
	unsigned int depthNum = this->_field->m_DepthNum;

	float width = this->_field->m_Width * scale.x;
	float depth = this->_field->m_Depth * scale.z;

	int x = (int)(( Position.x + width * widthNum * 0.5) / width);
	int z = (int)((-Position.z + depth * depthNum * 0.5) / depth);

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

MeshCollider::MeshCollider(EntityID OwnerID)
:
	Collider(OwnerID)
{

}

void MeshCollider::SetMesh(Mesh * mesh)
{
	_mesh = mesh;
}

