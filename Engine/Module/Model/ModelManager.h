#pragma once

struct MODEL_MATERIAL;
struct MODEL;
struct ModelMaterial;

struct aiScene;

class Texture;

//Assimp読み込み用のモデルデータ
struct AssimpModel final
{
	using VERTEX_TYPE = VERTEX_3D;

	//サブセット
	struct Subset
	{
		unsigned short _StartIndex;
		unsigned short _IndexNum;
		Material _Material;
		Texture* _Texture;		//テクスチャ
		unsigned int _TexNum;	//テクスチャ数
	};

	//ノードメッシュ
	struct NodeMesh
	{
		//行列
		XMMATRIX _OffsetMatrix;

		//バッファ
		ID3D11Buffer *_VertexBuffer;	//頂点バッファ
		ID3D11Buffer *_IndexBuffer;		//インデックスバッファ

		//頂点
		VERTEX_TYPE *_VertexArray;
		unsigned int _VertexNum = 0;

		//インデックス
		unsigned short *_IndexArray;
		unsigned int _IndexNum = 0;

		//サブセット
		Subset *_SubsetArray;
		unsigned short _SubsetNum = 0;

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
		static void LoadAsset(ModelAsset asset);
		static AssimpModel* LoadAssetForAssimp(std::string fileName);
		static void Release();
		static std::weak_ptr<Model> GetModel(std::string name);	
	private:
		static void LoadObj(ModelAsset asset,MODEL* Model);
		static void LoadMaterial(const char* FileName,const char* MaterialName,ModelMaterial **MaterialArray,unsigned short* MaterialNum);
	};
}