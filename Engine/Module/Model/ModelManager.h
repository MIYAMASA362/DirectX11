#pragma once

class Model;
class GameObject;
class Scene;

//���f���Ǘ�
class ModelManager
{
	//���f���z��
	using ModelIndex = std::map<std::string, std::shared_ptr<Model>>;
private:
	//���f���ۊ�
	static ModelIndex modelIndex;

	ModelManager() = default;
	~ModelManager() = default;
public:
	//���
	static void Release();
	//���f���ǂݍ���
	static Model* LoadAssetForAssimp(std::string fileName);
	//���f���擾
	static std::weak_ptr<Model> GetModel(std::string name);

	static GameObject* AddSceneModel(std::string name,Scene* scene);
};