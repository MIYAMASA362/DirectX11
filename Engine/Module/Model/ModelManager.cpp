#define _CRT_SECURE_NO_WARNINGS
#include"Common.h"

#include<assimp\cimport.h>
#include<assimp\scene.h>
#include<assimp\postprocess.h>
#include<assimp\matrix4x4.h>

#pragma comment(lib,"assimp.lib")

//DirectX
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include"Module\Texture\texture.h"
#include"Module\Material\Material.h"

#include"Module\AssetData\AssetData.h"

//ECS
#include"Module\ECSEngine.h"

// Component
#include"Module\Mesh\Mesh.h"
#include"Module\Renderer\Renderer.h"

#include"Module\Texture\texture.h"
#include"Module\Texture\TextureManager.h"

//Model
#include"model.h"
#include"ModelManager.h"

const std::string ModelManager::AssetDataBase = DirectX::AssetManager::AssetPath+ "Model/";
std::map<std::string, std::shared_ptr<Model>> ModelManager::ModelIndex;

const aiScene* ModelManager::_aiScene;

void DirectX::ModelManager::LoadAsset(ModelAsset asset)
{
	Model* data = new Model();

	MODEL model;
	LoadObj(asset, &model);

	// ���_�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * model.VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.VertexArray;

		D3DApp::GetDevice()->CreateBuffer(&bd, &sd, &data->VertexBuffer);
	}

	// �C���f�b�N�X�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short) * model.IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.IndexArray;

		D3DApp::GetDevice()->CreateBuffer(&bd, &sd, &data->IndexBuffer);
	}

	// �T�u�Z�b�g�ݒ�
	{
		data->SubsetArray = new DX11_SUBSET[model.SubsetNum];
		data->SubsetNum = model.SubsetNum;

		for (unsigned short i = 0; i < model.SubsetNum; i++)
		{
			data->SubsetArray[i].StartIndex = model.SubsetArray[i].StartIndex;
			data->SubsetArray[i].IndexNum = model.SubsetArray[i].IndexNum;

			data->SubsetArray[i].Material.Material = model.SubsetArray[i].Material._Material;
			if (strlen(model.SubsetArray[i].Material._TextureName) == 0) 
				data->SubsetArray[i].Material.Texture = TextureManager::GetTexture("sky");
			else
				data->SubsetArray[i].Material.Texture = TextureManager::LoadTexture(model.SubsetArray[i].Material._TextureName);
		}
	}

	//�������폜
	{
		delete[] model.VertexArray;
		delete[] model.IndexArray;
		delete[] model.SubsetArray;
	}

	ModelIndex.emplace(asset.name,std::shared_ptr<Model>(data));
}

void DirectX::ModelManager::LoadAssetForAssimp(const char * fileName)
{
	_aiScene = aiImportFile(fileName,aiProcessPreset_TargetRealtime_MaxQuality);

	//���f��
	AssimpModel* model = new AssimpModel();

	//���b�V����
	model->_MeshNum = _aiScene->mNumMeshes;
	//���b�V���z��
	model->_MeshArray = new ModelMesh[model->_MeshNum];

	//���b�V���擾
	for (int m = 0; m < model->_MeshNum; m++)
	{
		aiMesh* mesh = _aiScene->mMeshes[m];	//AssimpMesh
		ModelMesh& modelMesh = model->_MeshArray[m];

		//�}�e���A���擾
		aiMaterial& material = *_aiScene->mMaterials[mesh->mMaterialIndex];

		//���_�o�b�t�@����
		{
			//���_�z��
			modelMesh._VertexArray = new VERTEX_3D[mesh->mNumVertices];

			//���_�擾
			for (int v = 0; v < mesh->mNumVertices; v++)
			{
				VERTEX_3D& vertex = modelMesh._VertexArray[v];

				//Position
				aiVector3D aiPosition(0.0f,0.0f,0.0f);
				if(mesh->HasPositions())
					aiPosition = mesh->mVertices[v];

				//Normal
				aiVector3D aiNormal(0.0f,1.0f,0.0f);
				if (mesh->HasNormals())
					aiNormal = mesh->mNormals[v];

				//TexCoord
				aiVector3D aiTexCoord(1.0f,1.0f,0.0f);
				if(mesh->HasTextureCoords(0))
					aiTexCoord = mesh->mTextureCoords[0][v];

				//Color Diffuse
				aiColor4D aiDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
				if (mesh->HasVertexColors(0))
					aiDiffuse = mesh->mColors[0][v];

				//���_�ݒ�
				vertex.Position = XMFLOAT3(aiPosition.x, aiPosition.y, aiPosition.z);
				vertex.Normal	= XMFLOAT3(aiNormal.x, aiNormal.y, aiNormal.z);
				vertex.TexCoord = XMFLOAT2(aiTexCoord.x, aiTexCoord.y);
				vertex.Diffuse	= XMFLOAT4(aiDiffuse.r, aiDiffuse.g, aiDiffuse.b, aiDiffuse.a);
			}

			//�o�b�t�@����
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.Usage = D3D11_USAGE_DEFAULT;

			//���\�[�X����
			D3D11_SUBRESOURCE_DATA sd;
			sd.pSysMem = modelMesh._VertexArray;

			//���_�o�b�t�@�쐬
			D3DApp::GetDevice()->CreateBuffer(&bd, &sd, &modelMesh._VertexBuffer);
		}

		//�C���f�b�N�X�o�b�t�@�쐬
		{
			//�O�p��
			modelMesh._IndexNum = mesh->mNumFaces * 3;
			//�C���f�b�N�X
			modelMesh._IndexArray = new unsigned short[modelMesh._IndexNum];

			//�C���f�b�N�X�o�^
			for (int f = 0; f < mesh->mNumFaces; f++)
				for (int n = 0; n < 3; n++)
					modelMesh._IndexArray[f * 3 + n] = mesh->mFaces[f].mIndices[n];

			//�o�b�t�@����
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.ByteWidth = sizeof(unsigned short) * modelMesh._IndexNum;
			bd.Usage = D3D11_USAGE_DEFAULT;

			//���\�[�X����
			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = modelMesh._IndexArray;

			D3DApp::GetDevice()->CreateBuffer(&bd, &sd, &modelMesh._IndexBuffer);
		}
	}
	
	//Material
	if(_aiScene->HasMaterials())
	{
		//for(int m =0;m < _aiScene->mNumMaterials; m++)
		//{
		//	aiMaterial* material = _aiScene->mMaterials[m];
		//	std::string assetPath = std::string(fileName).substr(0,std::string(fileName).find_last_of("\\/")+1);
		//
		//	unsigned int texNum = material->GetTextureCount(aiTextureType_DIFFUSE);
		//	model->_TextureArray = new Texture[texNum];

		//	//Texture
		//	for(int t = 0; t < texNum; t++)
		//	{
		//		aiString texturePath;
		//		if (material->GetTexture(aiTextureType_DIFFUSE, t, &texturePath) != AI_SUCCESS) continue;
		//		
		//		HRESULT hr = D3DX11CreateShaderResourceViewFromFileA(D3DApp::GetDevice(),texturePath.C_Str(),NULL,NULL,&model->_TextureArray[t].srv,NULL);
		//		if (FAILED(hr)) assert(false);
		//	}
		//}
	}
}


void DirectX::ModelManager::LoadObj(ModelAsset asset, MODEL * Model)
{
	std::string FileName = AssetDataBase + asset.folderPath + "/" + asset.modelPath;

	XMFLOAT3	*positionArray;
	XMFLOAT3	*normalArray;
	XMFLOAT2	*texcoordArray;

	unsigned short	positionNum = 0;
	unsigned short	normalNum = 0;
	unsigned short	texcoordNum = 0;
	unsigned short	vertexNum = 0;
	unsigned short	indexNum = 0;
	unsigned short	in = 0;
	unsigned short	subsetNum = 0;

	ModelMaterial	*materialArray = nullptr;
	unsigned short	materialNum = 0;

	char str[256];
	char *s;
	char c;


	FILE *file;
	file = fopen(FileName.c_str(), "rt");
	assert(file);



	//�v�f���J�E���g
	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "v") == 0)
		{
			positionNum++;
		}
		else if (strcmp(str, "vn") == 0)
		{
			normalNum++;
		}
		else if (strcmp(str, "vt") == 0)
		{
			texcoordNum++;
		}
		else if (strcmp(str, "usemtl") == 0)
		{
			subsetNum++;
		}
		else if (strcmp(str, "f") == 0)
		{
			in = 0;

			do
			{
				fscanf(file, "%s", str);
				vertexNum++;
				in++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');

			//�l�p�͎O�p�ɕ���
			if (in == 4)
				in = 6;

			indexNum += in;
		}
	}


	//�������m��
	positionArray = new XMFLOAT3[positionNum];
	normalArray = new XMFLOAT3[normalNum];
	texcoordArray = new XMFLOAT2[texcoordNum];


	Model->VertexArray = new VERTEX_3D[vertexNum];
	Model->VertexNum = vertexNum;

	Model->IndexArray = new unsigned short[indexNum];
	Model->IndexNum = indexNum;

	Model->SubsetArray = new SUBSET[subsetNum];
	Model->SubsetNum = subsetNum;




	//�v�f�Ǎ�
	XMFLOAT3 *position = positionArray;
	XMFLOAT3 *normal = normalArray;
	XMFLOAT2 *texcoord = texcoordArray;

	unsigned short vc = 0;
	unsigned short ic = 0;
	unsigned short sc = 0;


	fseek(file, 0, SEEK_SET);

	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "mtllib") == 0)
		{
			//�}�e���A���t�@�C��
			fscanf(file, "%s", str);

			char path[256];
			
			strcpy(path, AssetDataBase.c_str());
			strcat(path, asset.folderPath.c_str());
			strcat(path,"/");

			LoadMaterial(path,str, &materialArray, &materialNum);
		}
		else if (strcmp(str, "o") == 0)
		{
			//�I�u�W�F�N�g��
			fscanf(file, "%s", str);
		}
		else if (strcmp(str, "v") == 0)
		{
			//���_���W
			fscanf(file, "%f", &position->x);
			fscanf(file, "%f", &position->y);
			fscanf(file, "%f", &position->z);
			//position->x *= SCALE_MODEL;
			//position->y *= SCALE_MODEL;
			//position->z *= SCALE_MODEL;
			position++;
		}
		else if (strcmp(str, "vn") == 0)
		{
			//�@��
			fscanf(file, "%f", &normal->x);
			fscanf(file, "%f", &normal->y);
			fscanf(file, "%f", &normal->z);
			normal++;
		}
		else if (strcmp(str, "vt") == 0)
		{
			//�e�N�X�`�����W
			fscanf(file, "%f", &texcoord->x);
			fscanf(file, "%f", &texcoord->y);
			//texcoord->y = 1.0f - texcoord->y;
			texcoord++;
		}
		else if (strcmp(str, "usemtl") == 0)
		{
			//�}�e���A��
			fscanf(file, "%s", str);

			if (sc != 0)
				Model->SubsetArray[sc - 1].IndexNum = ic - Model->SubsetArray[sc - 1].StartIndex;

			Model->SubsetArray[sc].StartIndex = ic;


			for (unsigned short i = 0; i < materialNum; i++)
			{
				if (strcmp(str, materialArray[i]._Name) == 0)
				{
					Model->SubsetArray[sc].Material._Material = materialArray[i]._Material;
					strcpy(Model->SubsetArray[sc].Material._TextureName, materialArray[i]._TextureName);
					strcpy(Model->SubsetArray[sc].Material._Name, materialArray[i]._Name);

					break;
				}
			}

			sc++;

		}
		else if (strcmp(str, "f") == 0)
		{
			//��
			in = 0;

			do
			{
				fscanf(file, "%s", str);

				s = strtok(str, "/");
				Model->VertexArray[vc].Position = positionArray[atoi(s) - 1];
				if (s[strlen(s) + 1] != '/')
				{
					//�e�N�X�`�����W�����݂��Ȃ��ꍇ������
					s = strtok(NULL, "/");
					Model->VertexArray[vc].TexCoord = texcoordArray[atoi(s) - 1];
				}
				s = strtok(NULL, "/");
				Model->VertexArray[vc].Normal = normalArray[atoi(s) - 1];

				Model->VertexArray[vc].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				Model->IndexArray[ic] = vc;
				ic++;
				vc++;

				in++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');

			//�l�p�͎O�p�ɕ���
			if (in == 4)
			{
				Model->IndexArray[ic] = vc - 4;
				ic++;
				Model->IndexArray[ic] = vc - 2;
				ic++;
			}
		}
	}


	if (sc != 0)
		Model->SubsetArray[sc - 1].IndexNum = ic - Model->SubsetArray[sc - 1].StartIndex;


	delete[] positionArray;
	delete[] normalArray;
	delete[] texcoordArray;
	delete[] materialArray;
}

void DirectX::ModelManager::LoadMaterial(const char * FileName,const char* MaterialName, ModelMaterial ** MaterialArray, unsigned short * MaterialNum)
{
	char str[256];
	char MaterialPath[256];

	FILE *file;

	strcpy(MaterialPath,FileName);
	strcat(MaterialPath,MaterialName);
	file = fopen(MaterialPath, "rt");
	if (file == NULL)
	{
		assert(false);
		return;
	}

	ModelMaterial *materialArray;
	unsigned short materialNum = 0;

	//�v�f���J�E���g
	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;


		if (strcmp(str, "newmtl") == 0)
		{
			materialNum++;
		}
	}


	//�������m��
	materialArray = new ModelMaterial[materialNum];


	//�v�f�Ǎ�
	int mc = -1;

	fseek(file, 0, SEEK_SET);

	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;


		if (strcmp(str, "newmtl") == 0)
		{
			//�}�e���A����
			mc++;
			fscanf(file, "%s", materialArray[mc]._Name);
			strcpy(materialArray[mc]._TextureName, "");
		}
		else if (strcmp(str, "Ka") == 0)
		{
			//�A���r�G���g
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Ambient.r);
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Ambient.g);
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Ambient.b);
			materialArray[mc]._Material._constant.Ambient.a = 1.0f;
		}
		else if (strcmp(str, "Kd") == 0)
		{
			//�f�B�t���[�Y
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Diffuse.r);
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Diffuse.g);
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Diffuse.b);
			materialArray[mc]._Material._constant.Diffuse.a = 1.0f;
		}
		else if (strcmp(str, "Ks") == 0)
		{
			//�X�y�L����
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Specular.r);
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Specular.g);
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Specular.b);
			materialArray[mc]._Material._constant.Specular.a = 1.0f;
		}
		else if (strcmp(str, "Ns") == 0)
		{
			//�X�y�L�������x
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Shininess);
		}
		else if (strcmp(str, "d") == 0)
		{
			//�A���t�@
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Diffuse.a);
		}
		else if (strcmp(str, "map_Kd") == 0)
		{
			//�e�N�X�`��
			fscanf(file, "%s", str);

			char path[256];
			strcpy(path, FileName);
			strcat(path, str);

			strcat(materialArray[mc]._TextureName, path);
		}
	}


	*MaterialArray = materialArray;
	*MaterialNum = materialNum;
}

std::weak_ptr<Model> DirectX::ModelManager::GetModel(std::string name)
{
	return ModelIndex[name];
}

void DirectX::ModelManager::Release()
{
	ModelIndex.clear();
}

ModelMesh::~ModelMesh()
{
	if (_VertexArray) delete[] _VertexArray;
	if (_IndexArray) delete[] _IndexArray;

	if (_VertexBuffer) _VertexBuffer->Release();
	if (_IndexBuffer) _IndexBuffer->Release();
}

AssimpModel::~AssimpModel()
{
	delete[] _MeshArray;
	delete[] _TextureArray;
}
