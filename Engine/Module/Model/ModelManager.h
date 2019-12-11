#pragma once

struct MODEL_MATERIAL;
struct MODEL;
struct ModelMaterial;

struct aiScene;

class Texture;

struct ModelMesh final
{
	//バッファ
	ID3D11Buffer* _VertexBuffer;
	ID3D11Buffer* _IndexBuffer;

	//頂点
	VERTEX_3D* _VertexArray;
	unsigned int _VertexNum;

	//インデックス
	unsigned short* _IndexArray;
	unsigned int _IndexNum;

	~ModelMesh();
};

struct AssimpModel final
{
	ModelMesh* _MeshArray;	//メッシュ配列
	unsigned int _MeshNum;	//メッシュ数

	Texture* _TextureArray;		//テクスチャ

	~AssimpModel();
};

namespace DirectX
{
	class Model;

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
		static const aiScene* _aiScene;
		static const std::string AssetDataBase;								//AssetフォルダまでのPath
		static std::map<std::string, std::shared_ptr<Model>> ModelIndex;	//Model保管
	private:
		ModelManager() = default;
		~ModelManager() = default;
	public:
		static void LoadAsset(ModelAsset asset);
		static void LoadAssetForAssimp(const char* fileName);
		static void Release();
		static std::weak_ptr<Model> GetModel(std::string name);	
	private:
		static void LoadObj(ModelAsset asset,MODEL* Model);
		static void LoadMaterial(const char* FileName,const char* MaterialName,ModelMaterial **MaterialArray,unsigned short* MaterialNum);
	};
}