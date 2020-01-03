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

// Component
#include"Module\Mesh\Mesh.h"

#include"Module\Texture\texture.h"
#include"Module\Texture\TextureManager.h"

#include"Module\Mesh\Mesh.h"

//Model
#include"model.h"
#include"ModelManager.h"

ModelManager::ModelIndex ModelManager::modelIndex;

Material SetMaterial(aiMaterial* material)
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
	Material m;
	m._constant.Diffuse = Color(diffuse.r, diffuse.g, diffuse.b, 1.0f);
	m._constant.Ambient = Color(ambient.r, ambient.g, ambient.b, 1.0f);
	m._constant.Specular = Color(specular.r, specular.g, specular.b, 1.0f);
	m._constant.Emission = Color(emission.r, emission.g, emission.b, 1.0f);
	m._constant.Shininess = shininess;

	return m;
}

void GetNodeMesh(aiNode* node, Model* model, const aiScene* scene, std::string folderPath,const aiMatrix4x4& nodeMtx)
{
	aiMatrix4x4 rootMtx = nodeMtx * node->mTransformation;

	if (node->mNumMeshes > 0)
	{
		//�m�[�h���b�V��
		NodeMesh* nodeMesh = new NodeMesh();
		model->_NodeMeshArray.push_back(nodeMesh);

		nodeMesh->_Mesh = new NodeMesh::MeshType();

		//�s��ݒ�
		aiMatrix4x4 matrix = node->mTransformation;
		nodeMesh->_OffsetMatrix.r[0] = XMVectorSet(matrix.a1,matrix.a2,matrix.a3,matrix.a4);
		nodeMesh->_OffsetMatrix.r[1] = XMVectorSet(matrix.b1,matrix.b2,matrix.b3,matrix.b4);
		nodeMesh->_OffsetMatrix.r[2] = XMVectorSet(matrix.c1,matrix.c2,matrix.c3,matrix.c4);
		nodeMesh->_OffsetMatrix.r[3] = XMVectorSet(matrix.d1,matrix.d2,matrix.d3,matrix.d4);

		//�z�񒷐ݒ�
		for (unsigned int m = 0; m < node->mNumMeshes; m++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[m]];
			nodeMesh->_Mesh->_VertexNum += mesh->mNumVertices;
			nodeMesh->_Mesh->_IndexNum  += mesh->mNumFaces * 3;
		}
		nodeMesh->_SubsetNum = node->mNumMeshes;

		//�z��ݒ�
		nodeMesh->_Mesh->_VertexArray = new NodeMesh::MeshType::VertexType[nodeMesh->_Mesh->_VertexNum];	//���_�z��
		nodeMesh->_Mesh->_IndexArray = new unsigned short[nodeMesh->_Mesh->_IndexNum];	//�C���f�b�N�X�z��
		nodeMesh->_SubsetArray = new ModelSubset[nodeMesh->_SubsetNum];		//�T�u�Z�b�g�z��

		//�C���f�b�N�X�J�E���g
		unsigned int iCount = 0;

		//���b�V��(���ʑ�)
		for (unsigned int m = 0; m < node->mNumMeshes; m++)
		{
			//�T�u�Z�b�g
			ModelSubset* subset = &nodeMesh->_SubsetArray[m];

			//���b�V�����擾
			aiMesh* mesh = scene->mMeshes[node->mMeshes[m]];
			{
				//�}�e���A���̎擾 1�̃��b�V���ɑ΂��ă}�e���A�����
				aiMaterial* material;
				material = scene->mMaterials[mesh->mMaterialIndex];

				//�}�e���A���ݒ�
				subset->_Material._Material = SetMaterial(material);

				//�e�N�X�`���ݒ�
				aiString texName;
				subset->_Material._Texture = new Texture();

				if (material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texName) == AI_SUCCESS)
					subset->_Material._Texture->LoadTexture(folderPath + texName.C_Str());

				//���_�ݒ�
#pragma region SetVertex

				for (unsigned int v = 0; v < mesh->mNumVertices; v++)
				{
					VERTEX_3D* vertex = &nodeMesh->_Mesh->_VertexArray[v];

					//���_
					aiVector3D position(0.0f, 0.0f, 0.0f);
					if (mesh->HasPositions()) position = mesh->mVertices[v];
					vertex->Position = XMFLOAT3(position.x, position.y, position.z);

					//�@��
					aiVector3D normal(0.0f, 1.0f, 0.0f);
					if (mesh->HasNormals()) normal = mesh->mNormals[v];
					vertex->Normal = XMFLOAT3(normal.x, normal.y, normal.z);

					//�ڐ�
					aiVector3D tangent(1.0f, 0.0f, 0.0f);
					if (mesh->HasTangentsAndBitangents()) tangent = mesh->mTangents[v];

					//�]�@��
					aiVector3D binormal(0.0f, 0.0f, 1.0f);
					if (mesh->HasTangentsAndBitangents()) binormal = mesh->mBitangents[v];

					//UV���W
					aiVector3D texCoord(0.0f, 0.0f, 0.0f);
					if (mesh->HasTextureCoords(0)) texCoord = mesh->mTextureCoords[0][v];
					vertex->TexCoord = XMFLOAT2(texCoord.x, texCoord.y);

					//���_�F
					aiColor4D diffuse(1.0f, 1.0f, 1.0f, 1.0f);
					if (mesh->HasVertexColors(0)) diffuse = mesh->mColors[0][v];
					vertex->Diffuse = XMFLOAT4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
				}
#pragma endregion

				//�C���f�b�N�X�ݒ�
#pragma region SetIndex

				//�T�u�Z�b�g �J�n�C���f�b�N�X
				subset->_StartIndex = iCount;

				//�C���f�b�N�X�ݒ�
				for (unsigned int f = 0; f < mesh->mNumFaces; f++)
				{
					aiFace* face = &mesh->mFaces[f];
					for (unsigned int i = 0; i < face->mNumIndices; i++)
					{
						nodeMesh->_Mesh->_IndexArray[iCount] = face->mIndices[i];
						iCount++;
					}
				}

				//�C���f�b�N�X��
				subset->_IndexNum = iCount - subset->_StartIndex;

#pragma endregion
			}
		}

		//���_�o�b�t�@����
		D3DApp::Renderer::GetD3DAppDevice()->CreateBuffer(
			D3D11_BIND_VERTEX_BUFFER,
			sizeof(VERTEX_3D) * nodeMesh->_Mesh->_VertexNum,
			nodeMesh->_Mesh->_VertexArray,
			&nodeMesh->_Mesh->_VertexBuffer
		);

		// �C���f�b�N�X�o�b�t�@����
		D3DApp::Renderer::GetD3DAppDevice()->CreateBuffer(
			D3D11_BIND_INDEX_BUFFER,
			sizeof(unsigned short) * nodeMesh->_Mesh->_IndexNum,
			nodeMesh->_Mesh->_IndexArray,
			&nodeMesh->_Mesh->_IndexBuffer
		);
	}

	//�q�̃m�[�h���擾
	for (unsigned int child = 0; child < node->mNumChildren; child++)
		GetNodeMesh(node->mChildren[child], model,scene,folderPath,rootMtx);
}

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
	size_t pos = fileName.find_last_of("\\/");
	std::string folderPath;
	if (pos == std::string::npos)
		folderPath = "";
	else
		folderPath = fileName.substr(0, pos+1);

	//���f��
	Model* model = new Model();

	aiMatrix4x4 mtx;

	//�m�[�h�̎擾
	GetNodeMesh(scene->mRootNode,model,scene,folderPath,mtx);

	//�ǉ�
	modelIndex.emplace(fileName.substr(pos,fileName.find_last_of(".")),std::shared_ptr<Model>(model));

	return model;
}

std::weak_ptr<Model> ModelManager::GetModel(std::string name)
{
	return modelIndex[name];
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
