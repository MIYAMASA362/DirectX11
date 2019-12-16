#define _CRT_SECURE_NO_WARNINGS
#include"Common.h"
#include"Module\DirectX\DirectX.h"

#include<assimp\cimport.h>
#include<assimp\scene.h>
#include<assimp\postprocess.h>
#include<assimp\matrix4x4.h>

#pragma comment(lib,"assimp.lib")

#define STB_IMAGE_IMPLEMENTATION
#include<stb\stb_image.h>

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

#include"Module\Shader\Shader.h"

#include"Module\Mesh\Mesh.h"

//Model
#include"model.h"
#include"ModelManager.h"

const std::string ModelManager::AssetDataBase = DirectX::AssetManager::AssetPath+ "Model/";
std::map<std::string, std::shared_ptr<Model>> ModelManager::ModelIndex;

void GetNodeMesh(aiNode* node, AssimpModel* model, const aiScene* scene, std::string folderPath,const aiMatrix4x4& nodeMtx)
{
	aiMatrix4x4 rootMtx = nodeMtx * node->mTransformation;

	if (node->mNumMeshes > 0)
	{
		//�m�[�h���b�V��
		AssimpModel::NodeMesh* NodeMesh = new AssimpModel::NodeMesh();
		model->_NodeMeshArray.push_back(NodeMesh);

		NodeMesh->_Mesh = new AssimpModel::MeshType();

		//�s��ݒ�
		aiMatrix4x4 matrix = node->mTransformation;
		NodeMesh->_OffsetMatrix.r[0] = XMVectorSet(matrix.a1,matrix.a2,matrix.a3,matrix.a4);
		NodeMesh->_OffsetMatrix.r[1] = XMVectorSet(matrix.b1,matrix.b2,matrix.b3,matrix.b4);
		NodeMesh->_OffsetMatrix.r[2] = XMVectorSet(matrix.c1,matrix.c2,matrix.c3,matrix.c4);
		NodeMesh->_OffsetMatrix.r[3] = XMVectorSet(matrix.d1,matrix.d2,matrix.d3,matrix.d4);

		//�z�񒷐ݒ�
		for (unsigned int m = 0; m < node->mNumMeshes; m++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[m]];
			NodeMesh->_Mesh->_VertexNum += mesh->mNumVertices;
			NodeMesh->_Mesh->_IndexNum  += mesh->mNumFaces * 3;
		}
		NodeMesh->_SubsetNum = node->mNumMeshes;

		//�z��ݒ�
		NodeMesh->_Mesh->_VertexArray = new AssimpModel::MeshType::VertexType[NodeMesh->_Mesh->_VertexNum];	//���_�z��
		NodeMesh->_Mesh->_IndexArray = new unsigned short[NodeMesh->_Mesh->_IndexNum];	//�C���f�b�N�X�z��
		NodeMesh->_SubsetArray = new ModelSubset[NodeMesh->_SubsetNum];		//�T�u�Z�b�g�z��

		//�C���f�b�N�X�J�E���g
		unsigned int iCount = 0;

		//���b�V��(���ʑ�)
		for (unsigned int m = 0; m < node->mNumMeshes; m++)
		{
			//�T�u�Z�b�g
			ModelSubset* subset = &NodeMesh->_SubsetArray[m];

			//���b�V�����擾
			aiMesh* mesh = scene->mMeshes[node->mMeshes[m]];
			{
				//�}�e���A���ݒ�
#pragma region GetMaterial

				//�}�e���A���̎擾 1�̃��b�V���ɑ΂��ă}�e���A�����
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

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

				//�T�u�Z�b�g�ݒ�
				subset->_Material._Material._constant.Diffuse = Color(diffuse.r, diffuse.g, diffuse.b, 1.0f);
				subset->_Material._Material._constant.Ambient = Color(ambient.r, ambient.g, ambient.b, 1.0f);
				subset->_Material._Material._constant.Specular = Color(specular.r, specular.g, specular.b, 1.0f);
				subset->_Material._Material._constant.Emission = Color(emission.r, emission.g, emission.b, 1.0f);
				subset->_Material._Material._constant.Shininess = shininess;

#pragma endregion

				//�e�N�X�`���ݒ�
#pragma region SetTexture

				aiString texName;
				subset->_Material._Texture = new Texture();

				if (material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texName) == AI_SUCCESS)
				{
					//�t�@�C���p�X
					std::string filePath = folderPath + texName.C_Str();
					filePath = filePath;

					//�e�N�X�`���ǂݍ���
					unsigned char* pixels;	//��f
					int width;
					int height;
					int bpp;

					pixels = stbi_load(filePath.c_str(), &width, &height, &bpp, STBI_rgb_alpha);

					D3D11_TEXTURE2D_DESC td;
					td.Width = width;
					td.Height = height;
					td.MipLevels = 1;
					td.ArraySize = 1;
					td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					td.SampleDesc.Count = 1;
					td.SampleDesc.Quality = 0;
					td.Usage = D3D11_USAGE_DEFAULT;
					td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
					td.CPUAccessFlags = 0;
					td.MiscFlags = 0;

					D3D11_SUBRESOURCE_DATA sd;
					sd.pSysMem = pixels;
					sd.SysMemPitch = width * 4;
					sd.SysMemSlicePitch = width * height * bpp;

					HRESULT hr = D3DApp::GetDevice()->CreateTexture2D(&td, &sd, &subset->_Material._Texture->texture);
					if (FAILED(hr))
						assert(false);

					D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
					srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					srv.Texture2D.MipLevels = 1;

					hr = D3DApp::GetDevice()->CreateShaderResourceView(subset->_Material._Texture->texture, &srv, &subset->_Material._Texture->srv);

					if (FAILED(hr))
						assert(false);

					stbi_image_free(pixels);
				}

#pragma endregion

				//���_�ݒ�
#pragma region SetVertex

				for (unsigned int v = 0; v < mesh->mNumVertices; v++)
				{
					VERTEX_3D* vertex = &NodeMesh->_Mesh->_VertexArray[v];

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
						NodeMesh->_Mesh->_IndexArray[iCount] = face->mIndices[i];
						iCount++;
					}
				}

				//�C���f�b�N�X��
				subset->_IndexNum = iCount - subset->_StartIndex;

#pragma endregion
			}
		}

		//���_�o�b�t�@����
		D3DApp::CreateBuffer(
			D3D11_BIND_VERTEX_BUFFER,
			sizeof(VERTEX_3D) * NodeMesh->_Mesh->_VertexNum,
			NodeMesh->_Mesh->_VertexArray,
			&NodeMesh->_Mesh->_VertexBuffer
		);

		// �C���f�b�N�X�o�b�t�@����
		D3DApp::CreateBuffer(
			D3D11_BIND_INDEX_BUFFER,
			sizeof(unsigned short) * NodeMesh->_Mesh->_IndexNum,
			NodeMesh->_Mesh->_IndexArray,
			&NodeMesh->_Mesh->_IndexBuffer
		);
	}

	//�q�̃m�[�h���擾
	for (unsigned int child = 0; child < node->mNumChildren; child++)
		GetNodeMesh(node->mChildren[child], model,scene,folderPath,rootMtx);
}

AssimpModel* DirectX::ModelManager::LoadAssetForAssimp(std::string fileName)
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
	AssimpModel* model = new AssimpModel();

	aiMatrix4x4 mtx;
	//�m�[�h�̎擾
	GetNodeMesh(scene->mRootNode,model,scene,folderPath,mtx);

	return model;
}

std::weak_ptr<Model> DirectX::ModelManager::GetModel(std::string name)
{
	return ModelIndex[name];
}

void DirectX::ModelManager::Release()
{
	ModelIndex.clear();
}

AssimpModel::~AssimpModel()
{
	for(auto mesh : _NodeMeshArray) delete mesh;
}

float rot = 0.0f;

void AssimpModel::Render(Vector3 Position)
{
	XMMATRIX worldMtx;
	worldMtx = XMMatrixRotationY(rot);
	worldMtx *= XMMatrixTranslation(Position.x,Position.y,Position.z);
	worldMtx *= XMMatrixScaling(1.0f,1.0f,1.0f);
	rot += 0.01f;

	D3DApp::Renderer::SetRasterize(D3D11_FILL_SOLID,D3D11_CULL_NONE);

	for(auto nodeMesh : this->_NodeMeshArray)
	{
		nodeMesh->_Mesh->SetVertexBuffer();
		nodeMesh->_Mesh->SetIndexBuffer();

		XMMATRIX local;
		local = nodeMesh->_OffsetMatrix * worldMtx;

		D3DApp::Renderer::SetWorldMatrix(&local);

		for (unsigned int s = 0; s < nodeMesh->_SubsetNum; s++)
		{
			ModelSubset& subset = nodeMesh->_SubsetArray[s];
			D3DApp::GetShader()->SetShader();

			subset._Material._Material.SetResource();
			D3DApp::GetConstantBuffer()->SetVSConstantBuffer(CONSTANT_BUFFER_MATERIAL,3);
			subset._Material._Texture[0].SetResource();

			D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			D3DApp::GetDeviceContext()->DrawIndexed(subset._IndexNum,subset._StartIndex,0);
		}
	}
}

AssimpModel::NodeMesh::~NodeMesh()
{
	delete _Mesh;
	delete[] _SubsetArray;
}
