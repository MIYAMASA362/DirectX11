#pragma once

struct MODEL_MATERIAL;
struct MODEL;
struct ModelMaterial;

struct aiScene;

class Texture;

//Assimp�ǂݍ��ݗp�̃��f���f�[�^
struct AssimpModel final
{
	using VERTEX_TYPE = VERTEX_3D;

	//�T�u�Z�b�g
	struct Subset
	{
		unsigned short _StartIndex;
		unsigned short _IndexNum;
		Material _Material;
		Texture* _Texture;		//�e�N�X�`��
		unsigned int _TexNum;	//�e�N�X�`����
	};

	//�m�[�h���b�V��
	struct NodeMesh
	{
		//�s��
		XMMATRIX _OffsetMatrix;

		//�o�b�t�@
		ID3D11Buffer *_VertexBuffer;	//���_�o�b�t�@
		ID3D11Buffer *_IndexBuffer;		//�C���f�b�N�X�o�b�t�@

		//���_
		VERTEX_TYPE *_VertexArray;
		unsigned int _VertexNum = 0;

		//�C���f�b�N�X
		unsigned short *_IndexArray;
		unsigned int _IndexNum = 0;

		//�T�u�Z�b�g
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
		static void LoadAsset(ModelAsset asset);
		static AssimpModel* LoadAssetForAssimp(std::string fileName);
		static void Release();
		static std::weak_ptr<Model> GetModel(std::string name);	
	private:
		static void LoadObj(ModelAsset asset,MODEL* Model);
		static void LoadMaterial(const char* FileName,const char* MaterialName,ModelMaterial **MaterialArray,unsigned short* MaterialNum);
	};
}