#define _CRT_SECURE_NO_WARNINGS
#include"Common.h"
#include"Module\DirectX\DirectX.h"

#include<assimp\cimport.h>
#include<assimp\scene.h>
#include<assimp\postprocess.h>
#include<assimp\matrix4x4.h>

#pragma comment(lib,"assimp.lib")

#include"Module\Texture\texture.h"
#include"Module\Material\Material.h"

#include"Module\ECSEngine.h"
#include"Module\Transform\Transform.h"
#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"

// Component
#include"Module\Mesh\Mesh.h"

#include"Module\Texture\texture.h"
#include"Module\Texture\TextureManager.h"

#include"Module\Mesh\Mesh.h"
#include"Module\Scene\SceneManager.h"

#include"Module\Renderer\Renderer.h"
#include"Module\Mesh\MeshRender.h"

#include"Module\Shader\Shader.h"
#include"Module\Shader\ShaderManager.h"

//Model
#include"model.h"
#include"ModelManager.h"

//*********************************************************************************************************************
//
//	ModelManager
//
//*********************************************************************************************************************

ModelManager::ModelIndex ModelManager::modelIndex;

//SetMaterial
//	�}�e���A���ݒ�
//
void SetMaterial(Material* m, aiMaterial* material)
{
	//Diffuse
	aiColor3D diffuse(0.0f, 0.0f, 0.0f);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);

	//Ambient
	aiColor3D ambient(0.0f, 0.0f, 0.0f);
	material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);

	//Specular
	aiColor3D specular(0.0f, 0.0f, 0.0f);
	material->Get(AI_MATKEY_COLOR_SPECULAR, specular);

	//Emmision
	aiColor3D emission(0.0f, 0.0f, 0.0f);
	material->Get(AI_MATKEY_COLOR_EMISSIVE, emission);

	//Shininess
	float shininess = 0.0f;
	material->Get(AI_MATKEY_SHININESS, shininess);

	//�}�e���A���ݒ�
	m->_constant.Diffuse = Color(diffuse.r, diffuse.g, diffuse.b, 1.0f);
	m->_constant.Ambient = Color(ambient.r, ambient.g, ambient.b, 1.0f);
	m->_constant.Specular = Color(specular.r, specular.g, specular.b, 1.0f);
	m->_constant.Emission = Color(emission.r, emission.g, emission.b, 1.0f);
	m->_constant.Shininess = shininess;
}

//GetNodeMesh
//	�m�[�h���b�V���擾
//
void GetNodeMesh(aiNode* node, Model* model, const aiScene* scene, std::string folderPath,const aiMatrix4x4& nodeMtx)
{
	aiMatrix4x4 rootMtx = nodeMtx * node->mTransformation;

	if (node->mNumMeshes > 0)
	{
		//���b�V��
		Mesh* mesh = new Mesh();

		//�m�[�h���b�V��
		NodeMesh* nodeMesh = new NodeMesh();
		model->_NodeMeshArray.push_back(nodeMesh);

		//�m�[�h���ݒ�
		nodeMesh->_NodeName = node->mName.C_Str();

		//�s��ݒ�
		aiMatrix4x4 matrix = node->mTransformation;
		nodeMesh->_OffsetMatrix.r[0] = XMVectorSet(matrix.a1,matrix.a2,matrix.a3,matrix.a4);
		nodeMesh->_OffsetMatrix.r[1] = XMVectorSet(matrix.b1,matrix.b2,matrix.b3,matrix.b4);
		nodeMesh->_OffsetMatrix.r[2] = XMVectorSet(matrix.c1,matrix.c2,matrix.c3,matrix.c4);
		nodeMesh->_OffsetMatrix.r[3] = XMVectorSet(matrix.d1,matrix.d2,matrix.d3,matrix.d4);

		//�z�񒷐ݒ�
		for (unsigned int m = 0; m < node->mNumMeshes; m++)
		{
			aiMesh* aimesh = scene->mMeshes[node->mMeshes[m]];
			mesh->_VertexNum += aimesh->mNumVertices;
			mesh->_IndexNum  += aimesh->mNumFaces * 3;
		}
		nodeMesh->_SubsetNum = node->mNumMeshes;

		//�z��ݒ�
		mesh->_VertexArray = new NodeMesh::MeshType::VertexType[mesh->_VertexNum];	//���_�z��
		mesh->_IndexArray = new unsigned short[mesh->_IndexNum];	//�C���f�b�N�X�z��
		nodeMesh->_SubsetArray = new ModelSubset[nodeMesh->_SubsetNum];		//�T�u�Z�b�g�z��

		//�C���f�b�N�X�J�E���g
		unsigned int iCount = 0;

		//���b�V��(���ʑ�)
		for (unsigned int m = 0; m < node->mNumMeshes; m++)
		{
			//�T�u�Z�b�g
			ModelSubset* subset = &nodeMesh->_SubsetArray[m];

			//���b�V�����擾
			aiMesh* aimesh = scene->mMeshes[node->mMeshes[m]];
			{
				//�}�e���A���̎擾 1�̃��b�V���ɑ΂��ă}�e���A�����
				aiMaterial* material;
				material = scene->mMaterials[aimesh->mMaterialIndex];

				//�}�e���A���ݒ�
				SetMaterial(&subset->_Material,material);

				//�e�N�X�`���ݒ�
				aiString texName;

				if (material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texName) == AI_SUCCESS)
				{
					subset->_Material._Texture  =TextureManager::LoadTexture(folderPath + texName.C_Str());
				}
				//���_�ݒ�
#pragma region SetVertex

				for (unsigned int v = 0; v < aimesh->mNumVertices; v++)
				{
					VERTEX_3D* vertex = &mesh->_VertexArray[v];

					//���_
					aiVector3D position(0.0f, 0.0f, 0.0f);
					if (aimesh->HasPositions()) position = aimesh->mVertices[v];
					vertex->Position = XMFLOAT3(position.x, position.y, position.z);

					//�@��
					aiVector3D normal(0.0f, 1.0f, 0.0f);
					if (aimesh->HasNormals()) normal = aimesh->mNormals[v];
					vertex->Normal = XMFLOAT3(normal.x, normal.y, normal.z);

					//�ڐ�
					aiVector3D tangent(1.0f, 0.0f, 0.0f);
					if (aimesh->HasTangentsAndBitangents()) tangent = aimesh->mTangents[v];

					//�]�@��
					aiVector3D binormal(0.0f, 0.0f, 1.0f);
					if (aimesh->HasTangentsAndBitangents()) binormal = aimesh->mBitangents[v];

					//UV���W
					aiVector3D texCoord(0.0f, 0.0f, 0.0f);
					if (aimesh->HasTextureCoords(0)) texCoord = aimesh->mTextureCoords[0][v];
					vertex->TexCoord = XMFLOAT2(texCoord.x, texCoord.y);

					//���_�F
					aiColor4D diffuse(1.0f, 1.0f, 1.0f, 1.0f);
					if (aimesh->HasVertexColors(0)) diffuse = aimesh->mColors[0][v];
					vertex->Diffuse = XMFLOAT4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
				}
#pragma endregion

				//�C���f�b�N�X�ݒ�
#pragma region SetIndex

				//�T�u�Z�b�g �J�n�C���f�b�N�X
				subset->_StartIndex = iCount;

				//�C���f�b�N�X�ݒ�
				for (unsigned int f = 0; f < aimesh->mNumFaces; f++)
				{
					aiFace* face = &aimesh->mFaces[f];
					for (unsigned int i = 0; i < face->mNumIndices; i++)
					{
						mesh->_IndexArray[iCount] = face->mIndices[i];
						iCount++;
					}
				}

				//�C���f�b�N�X��
				subset->_IndexNum = iCount - subset->_StartIndex;

#pragma endregion
			}
		}

		//���_�o�b�t�@����
		mesh->CreateVertexBuffer();

		// �C���f�b�N�X�o�b�t�@����
		mesh->CreateIndexBuffer();

		nodeMesh->_Mesh = MeshManager::RegisterIndex(nodeMesh->_NodeName,mesh);
	}

	//�q�̃m�[�h���擾
	for (unsigned int child = 0; child < node->mNumChildren; child++)
		GetNodeMesh(node->mChildren[child], model,scene,folderPath,rootMtx);
}

//LoadAssetForAssimp
//
//
Model* ModelManager::LoadAssetForAssimp(std::string fileName)
{
	//�V�[��
	const aiScene* scene;
	
	//aiImportFile aiProcessPreset_TargetRealtime_MaxQuality �O�p�|���S���ǂݍ���
	scene = aiImportFile(
		fileName.c_str(),
		aiProcessPreset_TargetRealtime_MaxQuality |	//���A���^�C�������_�����O�p�Ƀf�[�^���œK������
		aiProcess_ConvertToLeftHanded				//DirectX3D�x�[�X�̃V���[�g�J�b�g�t���O
	);

	if(!scene)
	{
		MessageBox(NULL,("NotFind:"+ fileName).c_str(),"�G���[",MB_OK);
	}

	//�t�H���_�p�X�ݒ�
	size_t pos = fileName.find_last_of("\\/") + 1;
	std::string folderPath;
	if (pos == std::string::npos)
		folderPath = "";
	else
		folderPath = fileName.substr(0, pos);

	//���f��
	Model* model = new Model();

	aiMatrix4x4 mtx;

	//�m�[�h�̎擾
	GetNodeMesh(scene->mRootNode,model,scene,folderPath,mtx);
	fileName = fileName.substr(pos, fileName.find_last_of(".") - pos);

	//�ǉ�
	modelIndex.emplace(fileName,std::shared_ptr<Model>(model));

	return model;
}

//GetModel
//
//
std::weak_ptr<Model> ModelManager::GetModel(std::string name)
{
	return modelIndex[name];
}

//AddSceneNodeModel
//
//
void AddSceneNodeModel(NodeMesh* nodeMesh, GameObject* parent,Scene* scene)
{
	for(int i = 0; i < nodeMesh->_SubsetNum; i++)
	{
		auto subset = nodeMesh->_SubsetArray[i];

		GameObject* child = scene->AddSceneObject(nodeMesh->_NodeName + std::to_string(i),TagName::Default);
		child->transform().lock()->SetParent(parent->transform());
		auto renderer = child->AddComponent<MeshRender>().lock();
		auto meshfilter = child->AddComponent<MeshFilter>().lock();
		meshfilter->SetMesh(nodeMesh->_Mesh);
		meshfilter->SetIndexNum(subset._IndexNum, subset._StartIndex);

		renderer->SetMaterial(&subset._Material);
		renderer->_Material._Shader = ShaderManager::GetShader();
	}
}

//AddSceneModel
//
//
GameObject * ModelManager::AddSceneModel(std::string name, Scene * scene)
{
	auto model = modelIndex.at(name);

	GameObject* result = scene->AddSceneObject(name, TagName::Default);

	GameObject* parent = result;
	for(auto nodeMesh : model->_NodeMeshArray)
	{
		AddSceneNodeModel(nodeMesh,parent,scene);
	}

	return result;
}

void ModelManager::EditorWindow()
{
	ImGui::Begin("ModelManager");

	for(auto model : modelIndex)
	{
		ImGui::Text(model.first.c_str());
	}

	ImGui::End();
}

void ModelManager::Release()
{
	modelIndex.clear();
}

//
//void AssimpModel::Render(Vector3 Position)
//{
//	XMMATRIX worldMtx;
//	worldMtx = XMMatrixRotationY(rot);
//	worldMtx *= XMMatrixTranslation(Position.x,Position.y,Position.z);
//	worldMtx *= XMMatrixScaling(1.0f,1.0f,1.0f);
//	rot += 0.01f;
//
//	D3DApp::Renderer::SetRasterize(D3D11_FILL_SOLID,D3D11_CULL_NONE);
//
//	for(auto nodeMesh : this->_NodeMeshArray)
//	{
//		nodeMesh->_Mesh->SetVertexBuffer();
//		nodeMesh->_Mesh->SetIndexBuffer();
//
//		XMMATRIX local;
//		local = nodeMesh->_OffsetMatrix * worldMtx;
//
//		D3DApp::Renderer::SetWorldMatrix(&local);
//
//		for (unsigned int s = 0; s < nodeMesh->_SubsetNum; s++)
//		{
//			ModelSubset& subset = nodeMesh->_SubsetArray[s];
//			D3DApp::GetShader()->SetShader();
//
//			subset._Material._Material.SetResource();
//			D3DApp::GetConstantBuffer()->SetVSConstantBuffer(CONSTANT_BUFFER_MATERIAL,3);
//			subset._Material._Texture[0].SetResource();
//
//			D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//			D3DApp::GetDeviceContext()->DrawIndexed(subset._IndexNum,subset._StartIndex,0);
//		}
//	}
//}
//
