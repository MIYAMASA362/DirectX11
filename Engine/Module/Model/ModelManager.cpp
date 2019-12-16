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
		//ノードメッシュ
		AssimpModel::NodeMesh* NodeMesh = new AssimpModel::NodeMesh();
		model->_NodeMeshArray.push_back(NodeMesh);

		NodeMesh->_Mesh = new AssimpModel::MeshType();

		//行列設定
		aiMatrix4x4 matrix = node->mTransformation;
		NodeMesh->_OffsetMatrix.r[0] = XMVectorSet(matrix.a1,matrix.a2,matrix.a3,matrix.a4);
		NodeMesh->_OffsetMatrix.r[1] = XMVectorSet(matrix.b1,matrix.b2,matrix.b3,matrix.b4);
		NodeMesh->_OffsetMatrix.r[2] = XMVectorSet(matrix.c1,matrix.c2,matrix.c3,matrix.c4);
		NodeMesh->_OffsetMatrix.r[3] = XMVectorSet(matrix.d1,matrix.d2,matrix.d3,matrix.d4);

		//配列長設定
		for (unsigned int m = 0; m < node->mNumMeshes; m++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[m]];
			NodeMesh->_Mesh->_VertexNum += mesh->mNumVertices;
			NodeMesh->_Mesh->_IndexNum  += mesh->mNumFaces * 3;
		}
		NodeMesh->_SubsetNum = node->mNumMeshes;

		//配列設定
		NodeMesh->_Mesh->_VertexArray = new AssimpModel::MeshType::VertexType[NodeMesh->_Mesh->_VertexNum];	//頂点配列
		NodeMesh->_Mesh->_IndexArray = new unsigned short[NodeMesh->_Mesh->_IndexNum];	//インデックス配列
		NodeMesh->_SubsetArray = new ModelSubset[NodeMesh->_SubsetNum];		//サブセット配列

		//インデックスカウント
		unsigned int iCount = 0;

		//メッシュ(多面体)
		for (unsigned int m = 0; m < node->mNumMeshes; m++)
		{
			//サブセット
			ModelSubset* subset = &NodeMesh->_SubsetArray[m];

			//メッシュを取得
			aiMesh* mesh = scene->mMeshes[node->mMeshes[m]];
			{
				//マテリアル設定
#pragma region GetMaterial

				//マテリアルの取得 1つのメッシュに対してマテリアル一つ
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

				//サブセット設定
				subset->_Material._Material._constant.Diffuse = Color(diffuse.r, diffuse.g, diffuse.b, 1.0f);
				subset->_Material._Material._constant.Ambient = Color(ambient.r, ambient.g, ambient.b, 1.0f);
				subset->_Material._Material._constant.Specular = Color(specular.r, specular.g, specular.b, 1.0f);
				subset->_Material._Material._constant.Emission = Color(emission.r, emission.g, emission.b, 1.0f);
				subset->_Material._Material._constant.Shininess = shininess;

#pragma endregion

				//テクスチャ設定
#pragma region SetTexture

				aiString texName;
				subset->_Material._Texture = new Texture();

				if (material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texName) == AI_SUCCESS)
				{
					//ファイルパス
					std::string filePath = folderPath + texName.C_Str();
					filePath = filePath;

					//テクスチャ読み込み
					unsigned char* pixels;	//画素
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

				//頂点設定
#pragma region SetVertex

				for (unsigned int v = 0; v < mesh->mNumVertices; v++)
				{
					VERTEX_3D* vertex = &NodeMesh->_Mesh->_VertexArray[v];

					//頂点
					aiVector3D position(0.0f, 0.0f, 0.0f);
					if (mesh->HasPositions()) position = mesh->mVertices[v];
					vertex->Position = XMFLOAT3(position.x, position.y, position.z);

					//法線
					aiVector3D normal(0.0f, 1.0f, 0.0f);
					if (mesh->HasNormals()) normal = mesh->mNormals[v];
					vertex->Normal = XMFLOAT3(normal.x, normal.y, normal.z);

					//接線
					aiVector3D tangent(1.0f, 0.0f, 0.0f);
					if (mesh->HasTangentsAndBitangents()) tangent = mesh->mTangents[v];

					//従法線
					aiVector3D binormal(0.0f, 0.0f, 1.0f);
					if (mesh->HasTangentsAndBitangents()) binormal = mesh->mBitangents[v];

					//UV座標
					aiVector3D texCoord(0.0f, 0.0f, 0.0f);
					if (mesh->HasTextureCoords(0)) texCoord = mesh->mTextureCoords[0][v];
					vertex->TexCoord = XMFLOAT2(texCoord.x, texCoord.y);

					//頂点色
					aiColor4D diffuse(1.0f, 1.0f, 1.0f, 1.0f);
					if (mesh->HasVertexColors(0)) diffuse = mesh->mColors[0][v];
					vertex->Diffuse = XMFLOAT4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
				}
#pragma endregion

				//インデックス設定
#pragma region SetIndex

				//サブセット 開始インデックス
				subset->_StartIndex = iCount;

				//インデックス設定
				for (unsigned int f = 0; f < mesh->mNumFaces; f++)
				{
					aiFace* face = &mesh->mFaces[f];
					for (unsigned int i = 0; i < face->mNumIndices; i++)
					{
						NodeMesh->_Mesh->_IndexArray[iCount] = face->mIndices[i];
						iCount++;
					}
				}

				//インデックス数
				subset->_IndexNum = iCount - subset->_StartIndex;

#pragma endregion
			}
		}

		//頂点バッファ生成
		D3DApp::CreateBuffer(
			D3D11_BIND_VERTEX_BUFFER,
			sizeof(VERTEX_3D) * NodeMesh->_Mesh->_VertexNum,
			NodeMesh->_Mesh->_VertexArray,
			&NodeMesh->_Mesh->_VertexBuffer
		);

		// インデックスバッファ生成
		D3DApp::CreateBuffer(
			D3D11_BIND_INDEX_BUFFER,
			sizeof(unsigned short) * NodeMesh->_Mesh->_IndexNum,
			NodeMesh->_Mesh->_IndexArray,
			&NodeMesh->_Mesh->_IndexBuffer
		);
	}

	//子のノードを取得
	for (unsigned int child = 0; child < node->mNumChildren; child++)
		GetNodeMesh(node->mChildren[child], model,scene,folderPath,rootMtx);
}

AssimpModel* DirectX::ModelManager::LoadAssetForAssimp(std::string fileName)
{
	//シーン
	const aiScene* scene;
	
	//aiImportFile aiProcessPreset_TargetRealtime_MaxQuality 三角ポリゴン読み込み
	scene = aiImportFile(
		fileName.c_str(),
		aiProcessPreset_TargetRealtime_MaxQuality |	//リアルタイムレンダリング用にデータを最適化する
		aiProcess_ConvertToLeftHanded				//DirectX3Dベースのショートカットフラグ
	);

	if(!scene)
	{
		MessageBox(NULL,("NotFind:"+ fileName).c_str(),"エラー",MB_OK);
	}

	//フォルダパス設定
	size_t pos = fileName.find_last_of("\\/");
	std::string folderPath;
	if (pos == std::string::npos)
		folderPath = "";
	else
		folderPath = fileName.substr(0, pos+1);

	//モデル
	AssimpModel* model = new AssimpModel();

	aiMatrix4x4 mtx;
	//ノードの取得
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
