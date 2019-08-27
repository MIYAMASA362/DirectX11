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

//--- Constrcutor -------------------------------------------------------------

//Constrcutor
Transform::Transform(Vector3 position, Quaternion rotation, Vector3 scale)
:
	Component("Transform"),
	m_Position(position),
	m_Rotation(rotation),
	m_Scale(scale)
{

}

//other
Transform::Transform(Vector3 position,Vector3 rotation,Vector3 scale)
	:Transform(position,Quaternion::Euler(rotation),scale){}
Transform::Transform()
	:Transform(Vector3::zero(), Vector3::zero(), Vector3::one()){}

//--- 親子関係　---------------------------------------------------------------

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

void Transform::SetParent(GameObject* parent)
{
	this->SetParent(parent->transform);
}

void Transform::detachParent() {
	Vector3 position = this->position();
	Vector3 scale = this->scale();
	Quaternion q = Quaternion::AtMatrix(this->MatrixQuaternion());
	pParent.reset();

	this->position(position);
	this->localScale(scale);
	this->rotation(q);
}

void Transform::detachChildSearch(Transform* target) {
	for (auto itr = pChildren.begin(); itr != pChildren.end(); itr++)
		if (itr->lock().get() == target)
		{
			pChildren.erase(itr);
			return;
		}
}

void Transform::childTransformUpdate() {
	if (pChildren.size() != 0)
		for (auto child : pChildren)
			child.lock()->WorldMatrix();
}

void Transform::DetachParent() {
	if (pParent.expired()) return;
	pParent.lock()->detachChildSearch(this);
	detachParent();
}

std::weak_ptr<Transform> Transform::GetParent() {
	return pParent;
}

std::list<std::weak_ptr<Transform>> DirectX::Transform::GetChildren()
{
	return this->pChildren;
}

void DirectX::Transform::SendComponentMessageChildren(Component::Message message)
{
	for(auto child:pChildren)
	{
		child.lock()->gameObject.lock()->RunComponent(message);
		if (child.lock()->pChildren.size() != 0)
			child.lock()->SendComponentMessageChildren(message);
	}
}

void Transform::DetachChildren() {
	//子を参照
	for (auto itr = pChildren.begin(); itr != pChildren.end(); itr++) {
		//親と離す
		itr->lock()->detachParent();
	}
	//子リストを削除
	pChildren.clear();
}

//--- 方向処理 ----------------------------------------------------------------

Vector3 Transform::TransformDirection(Vector3 direction) {
	return XMVector3Normalize(XMVector3Transform(direction, this->rotation().toMatrix()));
}

Vector3 Transform::right() { 
	return TransformDirection(Vector3::right()); 
}

Vector3 Transform::left() {
	return right()*-1.0f; 
}

Vector3 Transform::up() {
	return TransformDirection(Vector3::up()); 
}

Vector3 Transform::down() { 
	return up() * -1.0f;
}

Vector3 Transform::forward() { 
	return TransformDirection(Vector3::forward()); 
}

Vector3 Transform::back() { 
	return forward() * -1.0f; 
}

Vector3 Transform::position() {
	Vector3 position = this->WorldMatrix().r[3];
	return position;
}

Quaternion Transform::rotation() {
	Quaternion q = this->m_Rotation;
	if (!pParent.expired())
		q *= pParent.lock()->rotation();
	return q;
}

Vector3 Transform::scale() {
	Vector3 scale = this->m_Scale;
	if (!pParent.expired())
		scale *= pParent.lock()->scale();
	return scale;
}

Vector3 Transform::localPosition() {
	return m_Position;
}

Quaternion Transform::localRotation() {
	return m_Rotation;
}

Vector3 Transform::localScale() {
	return m_Scale;
}

void Transform::position(Vector3 position) {
	this->m_Position += position - this->position();
}

void Transform::rotation(Quaternion rotation) {
	if (!pParent.expired())
		rotation = pParent.lock()->rotation().conjugate() * rotation;
	this->m_Rotation = rotation;
}

void Transform::localPosition(Vector3 position) {
	this->m_Position = position;
}

void Transform::localRotation(Quaternion rotation) {
	this->m_Rotation = rotation;
}

void Transform::localScale(Vector3 scale) {
	this->m_Scale = scale;
}

void Transform::LookAt(std::weak_ptr<Transform> target) {
	Vector3 mPos = this->position();
	Vector3 pPos = target.lock()->position();
	this->m_Rotation = Quaternion::QuaternionLookRotation(pPos - mPos, Vector3::up());
	if (!pParent.expired())
		this->m_Rotation = pParent.lock()->rotation().conjugate() * this->m_Rotation;
}

//--- 行列処理 ----------------------------------------------------------------

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
	m_WorldMatrix = XMMatrixIdentity();

	m_WorldMatrix *= XMMatrixScaling(m_Scale.x,m_Scale.y,m_Scale.z);
	m_WorldMatrix *= XMMatrixRotationQuaternion(m_Rotation);
	m_WorldMatrix *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	if (!pParent.expired())
		m_WorldMatrix *= pParent.lock()->transform.lock()->WorldMatrix();

	return m_WorldMatrix;
}