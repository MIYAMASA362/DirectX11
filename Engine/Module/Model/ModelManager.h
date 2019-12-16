#pragma once

struct MODEL_MATERIAL;
struct MODEL;
struct ModelMaterial;

struct aiScene;

class Texture;

//サブセット マテリアル区分け
struct ModelSubset
{
	unsigned short _StartIndex;
	unsigned short _IndexNum;
	TextureMaterial _Material;
};

//Assimp読み込み用のモデルデータ
struct AssimpModel final
{
	using MeshType = Mesh<VERTEX_3D>;

	//ノードメッシュ
	struct NodeMesh final
	{
		//行列
		XMMATRIX _OffsetMatrix;

		//メッシュ
		MeshType* _Mesh;

		//サブセット
		ModelSubset *_SubsetArray;
		unsigned short _SubsetNum = 0;

		~NodeMesh();
	};
	std::vector<NodeMesh*> _NodeMeshArray;

	~AssimpModel();

	void Render(Vector3 Position);
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
		static const std::string AssetDataBase;								//AssetフォルダまでのPath
		static std::map<std::string, std::shared_ptr<Model>> ModelIndex;	//Model保管
	private:
		ModelManager() = default;
		~ModelManager() = default;
	public:
		static AssimpModel* LoadAssetForAssimp(std::string fileName);
		static void Release();
		static std::weak_ptr<Model> GetModel(std::string name);	
	};
}