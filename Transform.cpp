#include"main.h"
#include"texture.h"
#include"DirectXStruct.h"
#include"Tag.h"
#include"Object.h"
#include"Component.h"
#include"SceneManager.h"
#include"Transform.h"
#include"GameObject.h"
#include"Behaviour.h"
#include"camera.h"

//Constrcutor
Transform::Transform(Vector3 position, Quaternion rotation, Vector3 scale)
	:m_Position(position), m_Rotation(rotation), m_Scale(scale){}
Transform::Transform(Vector3 position,Vector3 rotation,Vector3 scale)
	:Transform(position,Quaternion::Euler(rotation),scale){}
Transform::Transform()
	:Transform(Vector3::zero(), Vector3::zero(), Vector3::one()){}

//SetParent
void Transform::SetParent(std::weak_ptr<Transform> parent)
{
	//���ɐe
	if (pParent.lock() == parent.lock()) return;

	XMMATRIX ParentMatrix = parent.lock()->WorldMatrix();
	XMMATRIX MyMatrix = this->WorldMatrix();

	pParent = parent;
	parent.lock().get()->pChildren.push_back(this->transform);
	
	XMMATRIX matrix = MyMatrix * XMMatrixInverse(nullptr,ParentMatrix);
	this->localPosition(Vector3(matrix.r[3]));
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
	m_WorldMatrix *= this->rotation().toMatrix();
	m_WorldMatrix *= XMMatrixTranslation(m_Position.x, m_Position.y,m_Position.z);

	if (this->m_Rotation.isIdentity())
		bool istrue = true;

	if (!pParent.expired())
		m_WorldMatrix *= pParent.lock()->WorldMatrix();

	return m_WorldMatrix;
}