#pragma once

struct MODEL_MATERIAL;
struct MODEL;
struct ModelMaterial;

struct aiScene;

class Texture;

//�T�u�Z�b�g �}�e���A���敪��
struct ModelSubset
{
	unsigned short _StartIndex;
	unsigned short _IndexNum;
	TextureMaterial _Material;
};

//Assimp�ǂݍ��ݗp�̃��f���f�[�^
struct AssimpModel final
{
	using MeshType = Mesh<VERTEX_3D>;

	//�m�[�h���b�V��
	struct NodeMesh final
	{
		//�s��
		XMMATRIX _OffsetMatrix;

		//���b�V��
		MeshType* _Mesh;

		//�T�u�Z�b�g
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

	//Model����
	class ModelAsset
	{
		friend class ModelManager;
	private:
		const std::string name;			//KeyName
		const std::string folderPath;	//�t�H���_
		const std::string modelPath;	//�t�H���_���̃��f��
	public:
		ModelAsset(std::string name, std::string folder,std::string model) :name(name), folderPath(folder),modelPath(model) {};
		~ModelAsset() = default;
	};

	//Model�Ǘ�
	class ModelManager:public AssetManager
	{
	private:
		static const std::string AssetDataBase;								//Asset�t�H���_�܂ł�Path
		static std::map<std::string, std::shared_ptr<Model>> ModelIndex;	//Model�ۊ�
	private:
		ModelManager() = default;
		~ModelManager() = default;
	public:
		static AssimpModel* LoadAssetForAssimp(std::string fileName);
		static void Release();
		static std::weak_ptr<Model> GetModel(std::string name);	
	};
}