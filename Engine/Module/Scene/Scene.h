#pragma once

//*********************************************************************************************************************
//
//	Scene
//
//*********************************************************************************************************************
class Scene : public Object
{
	friend cereal::access;
private:
	//�V�[����
	std::string _name;
	//�ǂݍ��݂���Ă�
	bool IsLoaded = false;		

	//�t�@�C���p�X
	std::string _filePath;

	//�I�u�W�F�N�g�K�w
	HierarchyUtility _hierarchyUtility;	

	//�R���X�g���N�^
	Scene();


public:
	//�R���X�g���N�^
	Scene(std::string name, std::string path);
	//�f�X�g���N�^
	virtual ~Scene();


	//�V�[�����擾
	std::string GetSceneName() { return this->_name; };
	//���O
	bool CompareName(std::string name);

	//�I�u�W�F�N�g�ǉ�
	GameObject* AddSceneObject(std::string name, TagName tag = TagName::Default);
	GameObject* CreateInstance(GameObject * gameObject);
	//�I�u�W�F�N�g�폜
	void RemoveSceneObject(GameObject* gameobject);

	//�A�N�e�B�u��
	void AttachActiveScene();
	//��A�N�e�B�u��
	void DetachActiveScene();

	//�f�o�b�O�\��
	void DebugGUI();

	//�I�u�W�F�N�g�K�w�̎擾
	HierarchyUtility* GetHierarchyUtility() { return &_hierarchyUtility; };


	//�ǂݍ��݊֐�
	virtual void Load();
	//�������݊֐�
	virtual void Save();
protected:

	//
	void UnLoad();

private:
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(_name),
			CEREAL_NVP(_hierarchyUtility)
		);

		std::map<EntityID, ComponentList> ComponentIndex;
		auto utility = _hierarchyUtility;
		for(auto hierarchy : utility.GetHierarchyMap())
		{
			auto components = ComponentManager::GetInstance()->GetComponents(hierarchy.second.GetSelf().lock().get());
			ComponentIndex.emplace(hierarchy.first,*components.lock());
		}
		archive(CEREAL_NVP(ComponentIndex));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			CEREAL_NVP(_name),
			CEREAL_NVP(_hierarchyUtility)
		);
		std::map<EntityID, ComponentList> ComponentIndex;
		archive(CEREAL_NVP(ComponentIndex));

		for(auto hierarchy : _hierarchyUtility.GetHierarchyMap())
		{
			auto gameObject = std::dynamic_pointer_cast<GameObject>(hierarchy.second.GetSelf().lock()).get();
			auto instance = this->CreateInstance(gameObject);

			auto find = ComponentIndex.find(hierarchy.first);
			if (find == ComponentIndex.end()) continue;
			auto components = instance->GetComponents();
			for(auto component : find->second._components)
			{
				auto obj = component.lock();
				obj = obj;
			}
		}
	}
};