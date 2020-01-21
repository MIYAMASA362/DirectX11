#define INCLUDE_CEREAL
#include"Common.h"
#include<memory>

//DirectX
#include"Module\DirectX\DirectX.h"

#include"Module\IMGUI\GUI_ImGui.h"

//ECS
#include"Module\ECSEngine.h"

//Component
#include"Transform.h"
#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"

#include"Module\Hierarchy\Hierarchy.h"

#include"Module\Scene\Scene.h"
#include"Module\Scene\SceneManager.h"

using namespace DirectX;

//*********************************************************************************************************************
//
//	Transform
//
//*********************************************************************************************************************

//Transform
//	コンストラクタ
//
Transform::Transform()
	:
	Component()
{

}

//Transform
//	コンストラクタ
//
Transform::Transform(EntityID OwnerID)
	:
	Component(OwnerID),
	_Position(Vector3::zero()),
	_Rotation(Quaternion::Identity()),
	_Scale(Vector3::one())
{
	//階層取得
	_hierarchy = gameObject()->GetScene()->GetHierarchyUtility()->GetHierarchy(OwnerID);
}

//~Transform
//	デストラクタ
//
Transform::~Transform()
{

}



//SetParent
//	親設定
//
void Transform::SetParent(std::weak_ptr<Transform> parent)
{
	gameObject()->GetScene()->GetHierarchyUtility()->AttachParent(gameObject()->GetEntityID(),parent.lock()->gameObject());

	//向きなどを保持したまま子になる
	XMMATRIX matrix;
	matrix = this->WorldMatrix() * XMMatrixInverse(nullptr, parent.lock()->WorldMatrix());
	this->localPosition({ matrix.r[3] });
	this->localScale(this->_Scale / parent.lock()->_Scale);
	this->localRotation(Quaternion::AtMatrix(matrix));
}

//SetParent
//	親設定
//
void Transform::SetParent(std::weak_ptr<GameObject> parent)
{
	SetParent(parent.lock()->transform());
}

//detachParent
//	親を解放
//
void Transform::detachParent() 
{
	Vector3 position = this->position();
	Vector3 scale = this->scale();
	Quaternion q = Quaternion::AtMatrix(this->MatrixQuaternion());

	gameObject()->GetScene()->GetHierarchyUtility()->DetachParent(gameObject()->GetEntityID());

	this->position(position);
	this->localScale(scale);
	this->rotation(q);
}

void Transform::detachChild(std::weak_ptr<Transform> target)
{
	gameObject()->GetScene()->GetHierarchyUtility()->DetachChild(gameObject()->GetEntityID(),target.lock()->gameObject());
}

void Transform::childTransformUpdate()
{
	auto children = _hierarchy->GetChildren();
	if (children.size() != 0)
		for (auto child : children)
			std::dynamic_pointer_cast<GameObject>(child.lock())->transform().lock()->WorldMatrix();
}

void Transform::DetachParent()
{
	if (_hierarchy->GetParent().expired()) return;
	gameObject()->GetScene()->GetHierarchyUtility()->DetachParent(gameObject()->GetEntityID());
	detachParent();
}

std::weak_ptr<Transform> Transform::GetParent() 
{
	if (_hierarchy->GetParent().expired()) return std::weak_ptr<Transform>();
	return std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform();
}

std::list<std::weak_ptr<Transform>> Transform::GetChildren()
{
	std::list<std::weak_ptr<Transform>> list;
	for (auto child : _hierarchy->GetChildren())
		list.push_back(std::dynamic_pointer_cast<GameObject>(child.lock())->transform());
	return list;
}

void Transform::SendComponentMessageChildren(std::string message)
{
	//自身に送信
	ComponentManager::GetInstance()->SendComponentMessage(message,this->GetOwnerID());
	//子に送信
	for (auto child : _hierarchy->GetChildren())
		std::dynamic_pointer_cast<GameObject>(child.lock())->transform().lock()->SendComponentMessageChildren(message);
}

std::weak_ptr<IComponent> Transform::GetComponentInParent(ComponentTypeID componentTypeID)
{
	if (_hierarchy->GetParent().expired()) return  std::weak_ptr<IComponent>();

	//TypeID参照
	if (ComponentIndex.size()) return  std::weak_ptr<IComponent>();

	//親IDの物を検索
	for (auto component : ComponentIndex)
		if (component.second.lock()->GetOwnerID() == _hierarchy->GetParent().lock()->GetEntityID())
			return component.second;

	//親のGetComponentInParentを実行
	return std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform().lock()->GetComponentInParent(componentTypeID);
}

std::weak_ptr<IComponent> Transform::GetComponentInChildren(ComponentTypeID componentTypeID)
{
	if (_hierarchy->GetChildren().size() == 0) return std::weak_ptr<IComponent>();
	//子IDの物を検索
	for (auto child : _hierarchy->GetChildren()) 
	{
		auto id = child.lock()->GetEntityID();
		for (auto component : ComponentIndex)
			if (component.second.lock()->GetOwnerID() == id)
				return component.second;
	}
	return std::weak_ptr<IComponent>();
}

ComponentList Transform::GetComponentsInChildren(ComponentTypeID componentTypeID)
{
	ComponentList list;

	if (_hierarchy->GetChildren().size() == 0) return list;

	//子Component検索
	for (auto child : _hierarchy->GetChildren())
	{
		auto id = child.lock()->GetEntityID();
		for(auto component:ComponentIndex)
		{
			if (component.second.lock()->GetOwnerID() == id)
				list.Add(component.second);
		}

		auto childlist = std::dynamic_pointer_cast<GameObject>(child.lock())->transform().lock()->GetComponentsInChildren(componentTypeID);
		if(childlist.Size() != 0) 
			list.Add(&childlist);
	}

	return list;
}

void Transform::DetachChildren()
{
	gameObject()->GetScene()->GetHierarchyUtility()->DetachChildren(gameObject()->GetEntityID());
}

//--- 方向処理 ----------------------------------------------------------------

Vector3 Transform::TransformDirection(Vector3 direction) {
	return XMVector3Normalize(XMVector3Transform(direction, this->rotation().toMatrix()));
}

//OnDebugImGui
//	デバッグ表示
//
void Transform::OnDebugImGui()
{
	Vector3 position = this->position();
	Vector3 rotation = Quaternion::ToEulerAngles(this->rotation());
	Vector3 scale = this->scale();

	Component::OnDebugImGui();
	ImGui::InputFloat3("Position", &position.x);
	ImGui::InputFloat3("Rotation", &rotation.x);
	ImGui::InputFloat3("Scale", &scale.x);

	this->position(position);
	this->rotation(Quaternion::Euler(rotation));
	this->localScale(scale);
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
	Quaternion q = this->_Rotation;
	if (!_hierarchy->GetParent().expired())
		q *= std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform().lock()->rotation();
	return q;
}

Vector3 Transform::scale() {
	Vector3 scale = this->_Scale;
	if (!_hierarchy->GetParent().expired())
		scale *= std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform().lock()->scale();
	return scale;
}

Vector3 Transform::localPosition() {
	return _Position;
}

Quaternion Transform::localRotation() {
	return _Rotation;
}

Vector3 Transform::localScale() {
	return _Scale;
}

void Transform::position(Vector3 position) {
	this->_Position += position - this->position();
}

void Transform::rotation(Quaternion rotation) {
	if (!_hierarchy->GetParent().expired())
		rotation = std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform().lock()->rotation().conjugate() * rotation;
	this->_Rotation = rotation;
	Quaternion::Normalize(this->_Rotation);
}

void Transform::localPosition(Vector3 position) {
	this->_Position = position;
}

void Transform::localRotation(Quaternion rotation) {
	this->_Rotation = rotation;
}

void Transform::localScale(Vector3 scale) {
	this->_Scale = scale;
}

void Transform::LookAt(std::weak_ptr<Transform> target) {
	Vector3 mPos = this->position();
	Vector3 pPos = target.lock()->position();
	this->_Rotation = Quaternion::QuaternionLookRotation(pPos - mPos, Vector3::up());
	if (!_hierarchy->GetParent().expired())
		this->_Rotation = std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform().lock()->rotation().conjugate() * this->_Rotation;
}

void Transform::OnDestroy()
{

}


XMMATRIX Transform::MatrixQuaternion()
{
	XMMATRIX matrix;
	matrix = this->_Rotation.toMatrix();

	if (!_hierarchy->GetParent().expired())
		matrix *= std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform().lock()->MatrixQuaternion();

	return matrix;
}

XMMATRIX Transform::MatrixTranslation()
{
	XMMATRIX matrix;
	matrix = XMMatrixTranslation(_Position.x,_Position.y,_Position.z);

	if (!_hierarchy->GetParent().expired())
		matrix *= std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform().lock()->MatrixTranslation();

	return matrix;
}

XMMATRIX Transform::MatrixScaling()
{
	XMMATRIX matrix;
	matrix = XMMatrixScaling(_Scale.x,_Scale.y,_Scale.z);

	if (!_hierarchy->GetParent().expired())
		matrix *= std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform().lock()->MatrixScaling();

	return matrix;
}

XMMATRIX Transform::WorldMatrix()
{
	XMMATRIX WorldMatrix = XMMatrixIdentity();

	WorldMatrix *= XMMatrixScaling(_Scale.x,_Scale.y,_Scale.z);
	WorldMatrix *= XMMatrixRotationQuaternion(_Rotation);
	WorldMatrix *= XMMatrixTranslation(_Position.x, _Position.y, _Position.z);

	if (!_hierarchy->GetParent().expired())
		WorldMatrix *= std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform().lock()->WorldMatrix();
	
	return WorldMatrix;
}