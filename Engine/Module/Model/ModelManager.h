#pragma once

class Model;
class GameObject;
class Scene;

//モデル管理
class ModelManager
{
	//モデル配列
	using ModelIndex = std::map<std::string, std::shared_ptr<Model>>;
private:
	//モデル保管
	static ModelIndex modelIndex;

	ModelManager() = default;
	~ModelManager() = default;
public:
	//解放
	static void Release();
	//モデル読み込み
	static Model* LoadAssetForAssimp(std::string fileName);
	//モデル取得
	static std::weak_ptr<Model> GetModel(std::string name);

	static GameObject* AddSceneModel(std::string name,Scene* scene);
};