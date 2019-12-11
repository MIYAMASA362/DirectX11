#pragma once

struct MODEL_MATERIAL;
struct MODEL;
struct ModelMaterial;

struct aiScene;

class Texture;

struct ModelMesh final
{
	//�o�b�t�@
	ID3D11Buffer* _VertexBuffer;
	ID3D11Buffer* _IndexBuffer;

	//���_
	VERTEX_3D* _VertexArray;
	unsigned int _VertexNum;

	//�C���f�b�N�X
	unsigned short* _IndexArray;
	unsigned int _IndexNum;

	~ModelMesh();
};

struct AssimpModel final
{
	ModelMesh* _MeshArray;	//���b�V���z��
	unsigned int _MeshNum;	//���b�V����

	Texture* _TextureArray;		//�e�N�X�`��

	~AssimpModel();
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
		static const aiScene* _aiScene;
		static const std::string AssetDataBase;								//Asset�t�H���_�܂ł�Path
		static std::map<std::string, std::shared_ptr<Model>> ModelIndex;	//Model�ۊ�
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