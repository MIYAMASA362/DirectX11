#include<memory>
#include<list>
#include<string>
#include<d3d11.h>
#include<DirectXMath.h>

//DirectX
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

//Component
#include"Module\Object\Object.h"
#include"Module\Component\Component.h"

#include"Transform.h"

#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"

using namespace DirectX;

//Constrcutor
Transform::Transform(Vector3 position, Quaternion rotation, Vector3 scale)
	:m_Position(position), m_Rotation(rotation), m_Scale(scale) {}
Transform::Transform(Vector3 position,Vector3 rotation,Vector3 scale)
	:Transform(position,Quaternion::Euler(rotation),scale){}
Transform::Transform()
	:Transform(Vector3::zero(), Vector3::zero(), Vector3::one()){}

//SetParent
void Transform::SetParent(std::weak_ptr<Transform> parent)
{
	//既に親
	if (pParent.lock() == parent.lock()) return;

	this->WorldMatrix();

	//親設定
	pParent = parent;
	//親の子に設定
	parent.lock().get()->pChildren.push_back(this->transform);
	
	//向きなどを保持したまま子になる
	XMMATRIX matrix = this->m_WorldMatrix * XMMatrixInverse(nullptr, parent.lock()->WorldMatrix());
	this->localPosition({ matrix.r[3] });
	this->localScale(this->m_Scale / parent.lock()->m_Scale);
	this->localRotation(Quaternion::AtMatrix(matrix));
}
void DirectX::Transform::SetParent(GameObject* parent)
{
	this->SetParent(parent->transform);
}

XMMATRIX Transform::MatrixQuaternion()
{
	XMMATRIX matrix;
	matrix = this->m_Rotation.toMatrix();

	if (!pParent.expired())
		matrix *= pParent.lock()->MatrixQuaternion();

	return matrix;
}
XMMATRIX Transform::MatrixTranslation()
{
	XMMATRIX matrix;
	matrix = XMMatrixTranslation(m_Position.x,m_Position.y,m_Position.z);

	if (!pParent.expired())
		matrix *= pParent.lock()->MatrixTranslation();

	return matrix;
}
XMMATRIX Transform::MatrixScaling()
{
	XMMATRIX matrix;
	matrix = XMMatrixScaling(m_Scale.x,m_Scale.y,m_Scale.z);

	if (!pParent.expired())
		matrix *= pParent.lock()->MatrixScaling();

	return matrix;
}
XMMATRIX Transform::WorldMatrix()
{
	m_WorldMatrix  = XMMatrixScaling(m_Scale.x,m_Scale.y,m_Scale.z);
	m_WorldMatrix *= this->m_Rotation.toMatrix();
	m_WorldMatrix *= XMMatrixTranslation(m_Position.x, m_Position.y,m_Position.z);

	if (this->m_Rotation.isIdentity())
		bool istrue = true;

	if (!pParent.expired())
		m_WorldMatrix *= pParent.lock()->WorldMatrix();

	return m_WorldMatrix;
}