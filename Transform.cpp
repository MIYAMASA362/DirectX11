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
	pParent = parent;
	parent.lock().get()->pChildren.push_back(transform);
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
	worldMatrix  = XMMatrixScaling(m_Scale.x,m_Scale.y,m_Scale.z);
	worldMatrix *= this->m_Rotation.toMatrix();
	worldMatrix *= XMMatrixTranslation(m_Position.x, m_Position.y,m_Position.z);

	if (!pParent.expired())
		worldMatrix *= pParent.lock()->WorldMatrix();

	return worldMatrix;
}