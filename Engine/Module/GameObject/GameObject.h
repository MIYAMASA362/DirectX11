#pragma once

class GameObject;
class Scene;

//*********************************************************************************************************************
//
//	GameObject Entity
//
//*********************************************************************************************************************
class GameObject final:public Entity<GameObject>
{
	friend Scene;
private:
	Scene* const scene;			//�Ή��V�[��

	const std::string name;		//�I�u�W�F�N�g��
	const Tag tag;				//�^�O

	bool IsDestroy = false;		//�폜�t���O
	bool IsActive  = true;		//�A�N�e�B�u�t���O

	std::weak_ptr<GameObject> _gameObject;
	std::weak_ptr<Transform> _transform;

public:
	//�R���X�g���N�^
	GameObject(std::string name,Scene* scene, TagName tagName);
	GameObject() :GameObject("Hoge",,) {}
	//�f�X�g���N�^
	virtual ~GameObject();

	void SetActive(bool IsActive);	//�A�N�e�B�u�ύX
	bool GetActive();				//�e�̉e�����󂯂邩
	
	bool CompareTag(TagName tag);	//�^�O�m�F

	bool GetIsDestroy();		//�폜�t���O�擾
	bool GetActiveSelf();		//���g�̂�
	Scene* GetScene();

	std::weak_ptr<Transform> transform();
	std::weak_ptr<GameObject> gameObject();

	void OnDebugGUI();
	std::string GetName();

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(name),
			CEREAL_NVP(IsDestroy),
			CEREAL_NVP(IsActive)
		);

		auto components = ComponentManager::GetComponents(this).lock();
		archive(
			CEREAL_NVP(components)
		);
	}
};

CEREAL_REGISTER_TYPE(GameObject)

CEREAL_REGISTER_POLYMORPHIC_RELATION(Entity<GameObject>,GameObject)