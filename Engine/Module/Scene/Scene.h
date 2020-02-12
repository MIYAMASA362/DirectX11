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
	std::shared_ptr<GameObject> AddSceneObject(std::string name, TagName tag = TagName::Default);
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

	virtual void Destroy() override {};
protected:

	//
	void UnLoad();

	void OrderlySceneObject(HierarchyUtility* utility);

private:
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(_name),
			CEREAL_NVP(_hierarchyUtility)
		);

		/*std::map<EntityID, ComponentList> ComponentIndex;
		auto utility = _hierarchyUtility;
		for(auto hierarchy : utility.GetHierarchyMap())
		{
			auto components = ComponentManager::GetInstance()->GetComponents(hierarchy.second.GetSelf().lock().get());
			ComponentIndex.emplace(hierarchy.first,*components.lock());
		}
		archive(CEREAL_NVP(ComponentIndex));
		*/
	}

	template<class Archive>
	void load(Archive& archive)
	{
		HierarchyUtility utility;

		archive(
			CEREAL_NVP(_name),
			utility
		);

		
		this->_hierarchyUtility = utility;
		OrderlySceneObject(&this->_hierarchyUtility);
		this->_hierarchyUtility = this->_hierarchyUtility;

		//std::map<EntityID, ComponentList> ComponentIndex;
		//archive(CEREAL_NVP(ComponentIndex));

		//for(auto hierarchy : _hierarchyUtility.GetHierarchyMap())
		//{
		//	auto gameObject = std::dynamic_pointer_cast<GameObject>(hierarchy.second.GetSelf().lock()).get();
		//	auto instance = this->CreateInstance(gameObject);
		//	this->_hierarchyUtility = this->_hierarchyUtility;

		//	auto find = ComponentIndex.find(hierarchy.first);
		//	if (find == ComponentIndex.end()) continue;
		//	for(auto component : find->second._components)
		//	{
		//		//�C���X�^���X����
		//		ComponentManager::GetInstance()->AddComponentInstance(instance,component.lock().get());

		//		auto obj = component.lock().get();
		//		size_t c_obj = typeid(*obj).hash_code();
		//		size_t transform = typeid(Transform).hash_code();

		//		std::string name = typeid(*obj).name();
		//		obj = obj;
		//	}
		//}
	}
};

#define STATIC_ATTRIBUTE static