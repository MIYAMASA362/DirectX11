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

	//Entity�̕ۊǌ�
	std::unordered_map<EntityID, std::weak_ptr<IEntity>> _EntityIndex;

	//�R���X�g���N�^
	Scene();

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(_name),
			CEREAL_NVP(_hierarchyUtility)
		);
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			CEREAL_NVP(_name),
			CEREAL_NVP(_hierarchyUtility)
		);
	}

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
	GameObject * AddSceneObject(GameObject * gameObject);
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

};