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

#include"Module\Scene\SceneManager.h"

using namespace DirectX;

//--- Constrcutor -------------------------------------------------------------

//other
Transform::Transform(EntityID OwnerID)
	:
	Component(OwnerID),
	m_Position(Vector3::zero()),
	m_Rotation(Quaternion::Identity()),
	m_Scale(Vector3::one())
{
	//階層取得
	_hierarchy = gameObject()->GetScene()->GetHierarchy(OwnerID);

	this->OnDebugImGui =[this]()
	{
		if (ImGui::TreeNode("Transform")) {
			ImGui::Text(("ID:" + std::to_string(this->GetInstanceID())).c_str());
			Vector3 position = this->position();
			Vector3 rotation = Quaternion::ToEulerAngles(this->rotation());
			Vector3 scale = this->scale();

			ImGui::InputFloat3("Position", &position.x);
			ImGui::InputFloat3("Rotation", &rotation.x);
			ImGui::InputFloat3("Scale", &scale.x);
			if(!this->GetParent().expired())
				ImGui::Text(("Parent:"+this->GetParent().lock()->gameObject()->GetName()).c_str());
			this->position(position);
			this->rotation(Quaternion::Euler(rotation));
			this->localScale(scale);

			ImGui::TreePop();
		}
	};
}

//--- 親子関係　---------------------------------------------------------------

//SetParent
void Transform::SetParent(std::weak_ptr<Transform> parent)
{
	_hierarchy->GetUtility().AttachParent(gameObject(),parent.lock()->gameObject());

	//向きなどを保持したまま子になる
	XMMATRIX matrix;
	matrix = this->WorldMatrix() * XMMatrixInverse(nullptr, parent.lock()->WorldMatrix());
	this->localPosition({ matrix.r[3] });
	this->localScale(this->m_Scale / parent.lock()->m_Scale);
	this->localRotation(Quaternion::AtMatrix(matrix));
}

void Transform::SetParent(std::weak_ptr<GameObject> parent)
{
	SetParent(parent.lock()->transform());
}

void Transform::detachParent() 
{
	Vector3 position = this->position();
	Vector3 scale = this->scale();
	Quaternion q = Quaternion::AtMatrix(this->MatrixQuaternion());

	_hierarchy->GetUtility().DetachParent(this->gameObject()->GetEntityID());

	this->position(position);
	this->localScale(scale);
	this->rotation(q);
}

void Transform::detachChild(std::weak_ptr<Transform> target)
{
	_hierarchy->GetUtility().DetachChild(target.lock()->gameObject()->GetEntityID(),target.lock()->gameObject());
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
	_hierarchy->GetUtility().DetachParent(this->gameObject()->GetEntityID());
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
	ComponentManager::SendComponentMessage(message,this->GetOwnerID());
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
		if (component.second->GetOwnerID() == _hierarchy->GetParent().lock()->GetEntityID())
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
			if (component.second->GetOwnerID() == id)
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
			if (component.second->GetOwnerID() == id)
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
	_hierarchy->GetUtility().DetachChildren(this->gameObject()->GetEntityID());
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
	if (!_hierarchy->GetParent().expired())
		q *= std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform().lock()->rotation();
	return q;
}

Vector3 Transform::scale() {
	Vector3 scale = this->m_Scale;
	if (!_hierarchy->GetParent().expired())
		scale *= std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform().lock()->scale();
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
	if (!_hierarchy->GetParent().expired())
		rotation = std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform().lock()->rotation().conjugate() * rotation;
	this->m_Rotation = rotation;
	Quaternion::Normalize(this->m_Rotation);
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
	if (!_hierarchy->GetParent().expired())
		this->m_Rotation = std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform().lock()->rotation().conjugate() * this->m_Rotation;
}

void Transform::OnDestroy()
{

}

//--- 行列処理 ----------------------------------------------------------------

XMMATRIX Transform::MatrixQuaternion()
{
	XMMATRIX matrix;
	matrix = this->m_Rotation.toMatrix();

	if (!_hierarchy->GetParent().expired())
		matrix *= std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform().lock()->MatrixQuaternion();

	return matrix;
}

XMMATRIX Transform::MatrixTranslation()
{
	XMMATRIX matrix;
	matrix = XMMatrixTranslation(m_Position.x,m_Position.y,m_Position.z);

	if (!_hierarchy->GetParent().expired())
		matrix *= std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform().lock()->MatrixTranslation();

	return matrix;
}

XMMATRIX Transform::MatrixScaling()
{
	XMMATRIX matrix;
	matrix = XMMatrixScaling(m_Scale.x,m_Scale.y,m_Scale.z);

	if (!_hierarchy->GetParent().expired())
		matrix *= std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform().lock()->MatrixScaling();

	return matrix;
}

XMMATRIX Transform::WorldMatrix()
{
	XMMATRIX m_WorldMatrix = XMMatrixIdentity();

	m_WorldMatrix *= XMMatrixScaling(m_Scale.x,m_Scale.y,m_Scale.z);
	m_WorldMatrix *= XMMatrixRotationQuaternion(m_Rotation);
	m_WorldMatrix *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	if (!_hierarchy->GetParent().expired())
		m_WorldMatrix *= std::dynamic_pointer_cast<GameObject>(_hierarchy->GetParent().lock())->transform().lock()->WorldMatrix();
	
	return m_WorldMatrix;
}