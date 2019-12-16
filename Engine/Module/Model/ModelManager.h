#pragma once

struct MODEL_MATERIAL;
struct MODEL;
struct ModelMaterial;

struct aiScene;

class Texture;
class Model;

namespace DirectX
{
	//Model資源
	class ModelAsset
	{
		friend class ModelManager;
	private:
		const std::string name;			//KeyName
		const std::string folderPath;	//フォルダ
		const std::string modelPath;	//フォルダ内のモデル
	public:
		ModelAsset(std::string name, std::string folder,std::string model) :name(name), folderPath(folder),modelPath(model) {};
		~ModelAsset() = default;
	};

	//Model管理
	class ModelManager:public AssetManager
	{
	private:
		static const std::string AssetDataBase;								//AssetフォルダまでのPath
		static std::map<std::string, std::shared_ptr<Model>> ModelIndex;	//Model保管
	private:
		ModelManager() = default;
		~ModelManager() = default;
	public:
		static Model* LoadAssetForAssimp(std::string fileName);
		static void Release();
		static std::weak_ptr<Model> GetModel(std::string name);	
	};
}