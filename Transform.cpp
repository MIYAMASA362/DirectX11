#include"main.h"
#include"texture.h"
#include"DirectXStruct.h"
#include"Tag.h"
#include"Object.h"
#include"SceneManager.h"
#include"Component.h"
#include"Transform.h"
#include"GameObject.h"
#include"Behaviour.h"
#include"camera.h"

Transform::Transform(Vector3 position, Quaternion rotation, Vector3 scale)
	:m_Position(position), m_Rotation(rotation),m_Scale(scale),hierarchy(0)
{

}

Transform::Transform(Vector3 position,Vector3 rotation,Vector3 scale)
	:Transform(position,Quaternion::Euler(rotation),scale){}
Transform::Transform() 
	:Transform(Vector3::zero(), Vector3::zero(), Vector3::one()){}

Vector3 Transform::VecDirection(Vector3 vec)
{
	XMMATRIX matrix = XMMatrixRotationQuaternion(this->m_Rotation);
	vec = XMVector3Normalize(XMVector3Transform(vec, matrix));
	return vec;
}

//SetParent
void Transform::SetParent(std::weak_ptr<Transform> parent)
{
	pParent = parent;
	parent.lock().get()->pChildren.push_back(transform);
	hierarchy++;	//ŠK‘wã‚°
}
void DirectX::Transform::SetParent(GameObject* parent)
{
	this->SetParent(parent->transform);
}

//Matrix
XMMATRIX Transform::MatrixQuaternion()
{
	return this->m_Rotation.toMatrix();
}
XMMATRIX Transform::MatrixTranslation()
{
	return XMMatrixTranslation(position().x, position().y, position().z);
}
XMMATRIX Transform::MatrixScaling()
{
	return XMMatrixScaling(scale().x, scale().y, scale().z);
}
XMMATRIX Transform::WorldMatrix()
{
	return MatrixScaling() * this->rotation().toMatrix() * MatrixTranslation();
}