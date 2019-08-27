#include<List>
#include<typeinfo>
#include<string>
#include<memory>
#include<d3d11.h>
#include<DirectXMath.h>

//DirectX
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

//Component
#include"Module\Object\Object.h"
#include"Module\Component\Component.h"
#include"Module\Behaviour\Behaviour.h"

#include"Module\Tag\Tag.h"

#include"Module\Transform\Transform.h"
#include"Module\GameObject\GameObject.h"
#include"Module\Renderer\Renderer.h"
#include"Module\Camera\camera.h"
#include"Module\Physics\Rigidbody.h"

#include"Module\UI\UI.h"

#include"SceneManager.h"
#include"Module\Physics\Collision.h"

#include"../IMGUI/GUI_ImGui.h"

using namespace DirectX;

SceneManager* SceneManager::pInstance = nullptr;

//Scene�̕ۊǌ�
std::list<std::shared_ptr<Scene>> SceneManager::pSceneIndex;
//���݂�Scene
std::weak_ptr<Scene> SceneManager::pActiveScene;
//����Scene
std::weak_ptr<Scene> SceneManager::pNextScene;
//�J�ڂ��L����
bool SceneManager::IsChangeScene = false;

//--- SceneManager ------------------------------------------------------------
SceneManager::SceneManager()
{
	pSceneIndex.clear();
}
SceneManager::~SceneManager()
{
	if (!pActiveScene.expired())
		pActiveScene.reset();
	if (!pNextScene.expired())
		pNextScene.reset();

	if (pSceneIndex.size() != 0)
		pSceneIndex.clear();
}

//--- Singleton Method ----------------------------------------------
//�C���X�^���X����
void SceneManager::Create()
{
	if (pInstance != nullptr) return;
	pInstance = new SceneManager();
}
//�C���X�^���X�폜
void SceneManager::Destroy()
{
	if (pInstance == nullptr) return;
	delete pInstance;
}

//--- Scene Method --------------------------------------------------

//Scene������v���Ă���Γǂݍ���
void SceneManager::LoadScene(std::string SceneName)
{
	//Index����
	for (auto wp_Scene : pSceneIndex)
		if (wp_Scene->name == SceneName){
			LoadScene(wp_Scene);
			return;
		}
}

//Scene�ǂݍ���
void DirectX::SceneManager::LoadScene(std::weak_ptr<Scene> scene)
{
	if(pActiveScene.expired()){
		AttachActiveScene(scene);
		return;
	}
	//����Scene�֓o�^
	SetIsChangeScene(scene);
}

//ActiveScene��Component���փ��b�Z�[�W�𑗐M����
void DirectX::SceneManager::RunActiveScene(Component::Message message,BYTE option)
{
	//Message�𑗂�I�u�W�F�N�g�Q
	std::list<std::shared_ptr<GameObject>> RunObjectIndex;

	//�S�Ă�GameObject�ɍX�V��Message�𑗐M
	if (option == SCENE_OPTION_UPDATE_DEFAULT || option == SCENE_OPTION_UPDATE_ALL)
		RunObjectIndex = pActiveScene.lock()->GameObjectIndex;
	//���o����
	else {
		//���o����
		bool IsActive,	//GameObject��Active�ł�����̂𒊏o
			 IsParent;	//GameObject���e�ł�����̂𒊏o

		//�t���O�v�Z
		{
			IsActive = { SCENE_OPTION_UPDATE_ACTIVE == (option & SCENE_OPTION_UPDATE_ACTIVE) };
			IsParent = { SCENE_OPTION_UPDATE_PARENT == (option & SCENE_OPTION_UPDATE_PARENT) };
		}

		//���o
		for (auto gameObject : pActiveScene.lock()->GameObjectIndex) {
			if (IsActive)
				if (!gameObject->IsActive)
					continue;
			if (IsParent)
				if (!gameObject->transform->GetParent().expired())
					continue;
			RunObjectIndex.push_back(gameObject);
		}
	}

	//Message�𑗂�
	{
		//�q��Component�𑖂炷����
		bool IsChild = SCENE_OPTION_UPDATE_PARENT == (option & SCENE_OPTION_UPDATE_PARENT);

		for (std::shared_ptr<GameObject> gameObject : RunObjectIndex) {
			gameObject->RunComponent(message);
			if (IsChild)
				//�q��Component�𑖂炷
				gameObject->transform->SendComponentMessageChildren(message);
		}
	}
}

//	RunActiveScene_Render()
//	
//	Canvas���A�^�b�`����Ă���ꍇ�A���̃I�u�W�F�N�g�Ǝq��2D�`��Ƃ��Ĉ���
//
void DirectX::SceneManager::RunActiveScene_Render()
{
	//2D�`�悷��I�u�W�F�N�g�̃C���f�b�N�X
	std::list<std::weak_ptr<GameObject>> RenderIndex_2D;

	//3D�`��
	{
		for (auto gameObject : pActiveScene.lock()->GameObjectIndex) {
			//�A�N�e�B�u�łȂ�
			if (!gameObject->IsActive) continue;
			//Canvas���A�^�b�`����Ă���
			if (gameObject->canvas) {
				RenderIndex_2D.push_back(gameObject);
				continue;
			}
			//�e�����Ȃ�
			if (gameObject->transform->GetParent().expired()){
				gameObject->RunComponent(Component::Render);
				gameObject->transform->SendComponentMessageChildren(Component::Render);
			}
		}
	}

	//2D�`��
	{
		//2D���(�X�N���[�����)�ɕϊ�
		D3DApp::Renderer::SetWorldViewProjection2D();

		//2D�`��
		for (auto gameObject : RenderIndex_2D)
			//�e�����Ȃ�
			if (gameObject.lock()->transform->GetParent().expired()) {
				gameObject.lock()->RunComponent(Component::Render);
				gameObject.lock()->transform->SendComponentMessageChildren(Component::Render);
			}
	}
}

//ActiveScene��GameObject��Collider�̍X�V
void DirectX::SceneManager::ColliderUpdate()
{
	//GameObject
	for(auto gameObject : pActiveScene.lock()->GameObjectIndex){
		//Active:false
		if (!gameObject.get()->IsActive)
			continue;
		//collider
		for (auto collider : gameObject->colliders) {
			//Enable:false
			if (!collider.get()->GetEnable())
				continue;
			//otherObject
			for(auto otherObject:pActiveScene.lock()->GameObjectIndex){
				//OtherObject:self
				if (gameObject == otherObject)
					continue;
				//Active:false
				if (!otherObject.get()->IsActive)
					continue;
				//otherCollider
				for(auto otherCollider:otherObject->colliders){
					//Enable:false
					if (!otherCollider.get()->GetEnable())
						continue;
					bool IsHit = false;
					Vector3 Intrusion = Vector3::zero();

					//Box_vs_Box
					auto Box_Vs_Box =[gameObject,otherObject,collider,otherCollider](Vector3& Intrusion)
					{
						Bounds& bound1 = collider.get()->bound;
						Bounds& bound2 = otherCollider.get()->bound;

						Vector3 pos1 = gameObject->transform.get()->position() + bound1.GetCenter();
						Vector3 pos2 = otherObject->transform.get()->position() + bound2.GetCenter();

						Vector3 pos1_max = pos1 + Vector3(bound1.GetMax() * gameObject->transform->scale() * 2.0f);
						Vector3 pos1_min = pos1 + Vector3(bound1.GetMin() * gameObject->transform->scale() * 2.0f);

						Vector3 pos2_max = pos2 + Vector3(bound2.GetMax() * otherObject->transform->scale() * 2.0f);
						Vector3 pos2_min = pos2 + Vector3(bound2.GetMin() * otherObject->transform->scale() * 2.0f);

						//�Փ�
						if ((pos2_min.x <= pos1_min.x && pos1_min.x <= pos2_max.x || pos2_min.x <= pos1_max.x && pos1_max.x <= pos2_max.x) &&
							(pos2_min.y <= pos1_min.y && pos1_min.y <= pos2_max.y || pos2_min.y <= pos1_max.y && pos1_max.y <= pos2_max.y) &&
							(pos2_min.z <= pos1_min.z && pos1_min.z <= pos2_max.z || pos2_min.z <= pos1_max.z && pos1_max.z <= pos2_max.z)
							)
						{
							if (gameObject->rigidbody) {

								Vector3 velocity = gameObject->rigidbody->GetVelocity();
								Vector3 direction = pos1 - pos2;

								//OtherGameObject���猩��GameObject�����ɂ���
								if (direction.y < 0.0f){
									Intrusion.y = pos1_max.y - pos2_min.y;
									velocity.y = velocity.y > 0.0f ? 0.0f : velocity.y;
								}
								//OtherGameObject���猩��GameObject����ɂ���
								else{
									Intrusion.y = pos1_min.y - pos2_max.y;
									velocity.y = velocity.y < 0.0f ? 0.0f : velocity.y;
								}

								gameObject->rigidbody->SetVelocity(velocity);
							}
							return true;
						}
						return false;
					};

					//Box_vs_Sphere
					auto Box_Vs_Sphere = [gameObject,otherObject,collider,otherCollider]()
					{
						Bounds& BoxBound = collider.get()->bound;
						Bounds& SphBound = otherCollider.get()->bound;

						Vector3 BoxPos = gameObject->transform.get()->position() + BoxBound.GetCenter();
						Vector3 SphPos = otherObject->transform.get()->position() + SphBound.GetCenter();

						Vector3 pos1_max = BoxPos + Vector3(BoxBound.GetMax() * gameObject->transform->scale());
						Vector3 pos1_min = BoxPos + Vector3(BoxBound.GetMin() * gameObject->transform->scale());

						float radius = SphBound.GetSize().x;

						Vector3 posMax; 
						Vector3 posMin; 

						//X��
						posMax = pos1_max + Vector3(1.0f, 0.0f, 0.0f)*radius;
						posMin = pos1_min - Vector3(1.0f, 0.0f, 0.0f)*radius;
						if (posMin.x <= SphPos.x && SphPos.x <= posMax.x &&
							posMin.y <= SphPos.y && SphPos.y <= posMax.y &&
							posMin.z <= SphPos.z && SphPos.z <= posMax.z)
						{
							return true;
						}

						//Y��
						posMax = pos1_max + Vector3(0.0f, 1.0f, 0.0f) * radius;
						posMin = pos1_min - Vector3(0.0f, 1.0f, 0.0f) * radius;
						if (posMin.x <= SphPos.x && SphPos.x <= posMax.x &&
							posMin.y <= SphPos.y && SphPos.y <= posMax.y &&
							posMin.z <= SphPos.z && SphPos.z <= posMax.z)
						{
							return true;
						}

						//Z��
						posMax = pos1_max + Vector3(0.0f, 0.0f, 1.0f) * radius;
						posMin = pos1_min - Vector3(0.0f, 0.0f, 1.0f) * radius;
						if (posMin.x <= SphPos.x && SphPos.x <= posMax.x &&
							posMin.y <= SphPos.y && SphPos.y <= posMax.y &&
							posMin.z <= SphPos.z && SphPos.z <= posMax.z)
						{
							return true;
						}
						
						Vector3 pos;
						pos.x = SphPos.x - pos1_max.x < SphPos.x - pos1_min.x ? pos1_max.x : pos1_min.x;
						pos.y = SphPos.y - pos1_max.y < SphPos.y - pos1_min.y ? pos1_max.y : pos1_min.y;
						pos.z = SphPos.z - pos1_max.z < SphPos.z - pos1_min.z ? pos1_max.z : pos1_min.z;

						pos = pos - SphPos;
						if (pos.lengthSq() < radius * radius)
							return true;

						return false;
					};

					//Sphere_vs_Sphere
					auto Sphere_Vs_Sphere =[gameObject, otherObject, collider, otherCollider](Vector3& Intrusion){

						Bounds& bound1 = collider.get()->bound;
						Bounds& bound2 = otherCollider.get()->bound;

						float radius1 = bound1.GetSize().x * gameObject->transform.get()->scale().MaxElement();
						float radius2 = bound2.GetSize().x * otherObject->transform.get()->scale().MaxElement();

						Vector3 pos1 = XMVector3TransformCoord(bound1.GetCenter(), gameObject->transform.get()->WorldMatrix());
						Vector3 pos2 = XMVector3TransformCoord(bound2.GetCenter(), otherObject->transform.get()->WorldMatrix()); //otherObject->transform.get()->position() + bound2.GetCenter();



						Vector3 distance = pos1 - pos2;
						//�Փ�
						if (distance.lengthSq() < (radius1 + radius2) * (radius1 + radius2)) {
							Intrusion = distance.normalize() * (distance.length() - (radius1 + radius2));
							return true;
						}
						return false;
					};

					//Is Box
					if(collider.get()->GetShapeType() == Collider::ShapeType::Box)
					{
						//Box vs Box
						if (otherCollider.get()->GetShapeType() == Collider::ShapeType::Box)
							IsHit = Box_Vs_Box(Intrusion);
						//Box vs Sphere
						else
							IsHit = Box_Vs_Sphere();
					}
					//Is Sphere
					else if(collider.get()->GetShapeType() == Collider::ShapeType::Sphere)
					{
						//Sphere vs Sphere
						if (otherCollider.get()->GetShapeType() == Collider::ShapeType::Sphere)
							IsHit = Sphere_Vs_Sphere(Intrusion);
						else
							IsHit = Box_Vs_Box(Intrusion);
					}

					//�Փ˃��b�Z�[�W
					if(IsHit){
						//����
						if (collider->IsTrigger) {
							for (auto component : gameObject->Components) {
								component->OnTriggerEnter(otherCollider);
							}
						}
						//����
						else if (gameObject->rigidbody) {
							if(!otherCollider->IsTrigger)
								gameObject->transform.get()->position(gameObject->transform.get()->position() - Intrusion);
						}
					}
				}
			}
		}
	}
}


//ActiveScene���I�u�W�F�N�g��ImGUI�\��
void DirectX::SceneManager::DebugGUI_ActiveScene()
{
	std::string label = "Scene:";
	label += pActiveScene.lock()->name;
	ImGui::Begin("Scene");
	ImGui::Text(label.c_str());
	
	for(auto gameObject : pActiveScene.lock()->GameObjectIndex)
	{
		ImGui::SetNextTreeNodeOpen(false,ImGuiCond_Once);
		if(ImGui::TreeNode(gameObject->name.c_str())){
			Vector3 position = gameObject->transform.get()->position();
			Vector3 rotation = Quaternion::ToEulerAngles(gameObject->transform.get()->rotation());
			Quaternion q = gameObject->transform.get()->rotation();
			Vector3 scale = gameObject->transform.get()->scale();
			ImGui::InputFloat3("Position",&position.x);
			ImGui::InputFloat3("Rotation",&rotation.x);
			ImGui::InputFloat3("Scale",&scale.x);

			if(ImGui::TreeNode("Component")){
				for(auto component : gameObject->Components){
					component->DebugImGui();
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

//�폜�w�肳�ꂽ�I�u�W�F�N�g���폜
void DirectX::SceneManager::CleanUp()
{
	//CeanUp���Ȃ��Ă���
	if (!pActiveScene._Get()->IsCeanUp) return;
	//�폜
	pActiveScene._Get()->GameObjectIndex.remove_if([](std::shared_ptr<GameObject> gameObject) { return gameObject->IsDestroy; });
	//CeanUp����
	pActiveScene._Get()->IsCeanUp = false;
}

//Scene�J��
void DirectX::SceneManager::ChangeScene()
{
	//�J�ڃt���O���L��
	if (!IsChangeScene) return;
	//ActiveScene������
	DetachActiveScene();
	//����Scene��ActiveScene�֕ύX
	AttachActiveScene(pNextScene);
	IsChangeScene = false;
}

//ActiveScene�̎擾
std::weak_ptr<Scene> DirectX::SceneManager::GetActiveScene()
{
	return pActiveScene;
}

//Scene������Scene���擾
std::weak_ptr<Scene> DirectX::SceneManager::GetSceneByName(std::string SceneName)
{
	for (auto wp_Scene : pSceneIndex)
		if (wp_Scene->name == SceneName)
			return wp_Scene;
	return std::weak_ptr<Scene>();
}

void DirectX::SceneManager::ApplyRigidbody()
{
	for(auto gameObject:pActiveScene.lock()->GameObjectIndex){
		if (!gameObject->IsActive)
			continue;
		if (!gameObject->rigidbody)
			continue;
		if (!gameObject->rigidbody->GetEnable())
			continue;
		gameObject->rigidbody->ApplyRigidbody();
	}
}


void DirectX::SceneManager::SetIsChangeScene(std::weak_ptr<Scene> scene)
{
	pNextScene = scene;
	IsChangeScene = true;
}

//ActiveScene��ݒ�
void DirectX::SceneManager::AttachActiveScene(std::weak_ptr<Scene> scene)
{
	pActiveScene.reset();
	pActiveScene = scene;
	scene.lock()->IsLoaded = true;
	scene.lock()->IsCeanUp = false;
	scene.lock()->Load();
	//����������
	RunActiveScene(Component::Initialize);
}

//ActiveScene��j��
void DirectX::SceneManager::DetachActiveScene()
{
	pActiveScene.lock()->IsLoaded = false;
	pActiveScene.lock()->IsCeanUp = false;
	//�I������
	RunActiveScene(Component::Finalize);
	pActiveScene.lock()->GameObjectIndex.clear();
	pActiveScene.reset();
}

//--- Scene -------------------------------------------------------------------

Scene::Scene(std::string name):name(name)
{
	GameObjectIndex.clear();
}

Scene::~Scene()
{
	GameObjectIndex.clear();
}

//GameObject��Tag�w��Œǉ�
GameObject* Scene::AddSceneObject(std::string name,TagName tag)
{
	//GameObject�̐���
	std::shared_ptr<GameObject> object = std::shared_ptr<GameObject>(new GameObject(name,this,tag));
	GameObjectIndex.push_back(object);
	object->self = object;
	object->AddComponent<Transform>();
	return object.get();
}

void Scene::SetIsCeanUp(bool IsEnable) {
	this->IsCeanUp = IsEnable;
};