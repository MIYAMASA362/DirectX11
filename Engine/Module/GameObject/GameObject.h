#pragma once

class GameObject;
class Scene;

namespace cereal
{
	class access;
}

//*********************************************************************************************************************
//
//	GameObject Entity
//
//*********************************************************************************************************************
class GameObject final :public Entity<GameObject>
{
	friend Scene;
	friend cereal::access;
	friend Entity<GameObject>;
private:
	//�����V�[��
	Scene* _Scene;			

	//�I�u�W�F�N�g��
	std::string _Name;
	//�^�O
	Tag _Tag;

	//�폜�t���O
	bool _IsDestroy = false;
	//�A�N�e�B�u�t���O
	bool _IsActive = true;

	std::weak_ptr<GameObject> _gameObject;
	std::weak_ptr<Transform> _transform;

public:
	//�R���X�g���N�^
	GameObject();
	GameObject(std::string name, Scene* scene, TagName tagName);
	//�f�X�g���N�^
	virtual ~GameObject();

	//�����V�[���̎擾
	Scene* GetScene() { return _Scene; };

	//�I�u�W�F�N�g���擾
	std::string GetName() { return _Name; };
	//�I�u�W�F�N�g���ݒ�
	void SetName(std::string name) { _Name = name; }

	//�^�O�m�F
	bool CompareTag(TagName tag) { return _Tag.Compare(tag); }

	//�폜�t���O�擾
	bool GetIsDestroy() { return _IsDestroy; };

	//�A�N�e�B�u�ύX
	void SetActive(bool IsActive) { _IsActive = IsActive; }

	//�e�̉e�����󂯂邩
	//	TODO : �e�K�w���Ė߂�l�����߂鏈����ǋL����
	bool GetActive() { return _IsActive; }

	//���g�̂�
	bool GetActiveSelf() { return _IsActive; };

	std::weak_ptr<Transform> transform();
	std::weak_ptr<GameObject> gameObject();

	//�f�o�b�O�\��
	void OnDebugGUI();

	void Register(std::shared_ptr<Object> instance) override;

private:
	//�V���A���C�Y
	template<class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Entity<GameObject>>(this),
			cereal::base_class<Object>(this),
			CEREAL_NVP(_Name),
			CEREAL_NVP(_IsDestroy),
			CEREAL_NVP(_IsActive)
		);
	}

	//�f�V���A���C�Y
	template<class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Entity<GameObject>>(this),
			cereal::base_class<Object>(this),
			CEREAL_NVP(_Name),
			CEREAL_NVP(_IsDestroy),
			CEREAL_NVP(_IsActive)
		);
	}


};

CEREAL_REGISTER_TYPE(Entity<GameObject>)
CEREAL_REGISTER_TYPE(GameObject)