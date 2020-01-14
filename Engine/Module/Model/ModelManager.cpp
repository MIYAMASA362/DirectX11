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
//	マテリアル設定
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

	//マテリアル設定
	m->_constant.Diffuse = Color(diffuse.r, diffuse.g, diffuse.b, 1.0f);
	m->_constant.Ambient = Color(ambient.r, ambient.g, ambient.b, 1.0f);
	m->_constant.Specular = Color(specular.r, specular.g, specular.b, 1.0f);
	m->_constant.Emission = Color(emission.r, emission.g, emission.b, 1.0f);
	m->_constant.Shininess = shininess;
}

//GetNodeMesh
//	ノードメッシュ取得
//
void GetNodeMesh(aiNode* node, Model* model, const aiScene* scene, std::string folderPath,const aiMatrix4x4& nodeMtx)
{
	aiMatrix4x4 rootMtx = nodeMtx * node->mTransformation;

	if (node->mNumMeshes > 0)
	{
		//メッシュ
		Mesh* mesh = new Mesh();

		//ノードメッシュ
		NodeMesh* nodeMesh = new NodeMesh();
		model->_NodeMeshArray.push_back(nodeMesh);

		//ノード名設定
		nodeMesh->_NodeName = node->mName.C_Str();

		//行列設定
		aiMatrix4x4 matrix = node->mTransformation;
		nodeMesh->_OffsetMatrix.r[0] = XMVectorSet(matrix.a1,matrix.a2,matrix.a3,matrix.a4);
		nodeMesh->_OffsetMatrix.r[1] = XMVectorSet(matrix.b1,matrix.b2,matrix.b3,matrix.b4);
		nodeMesh->_OffsetMatrix.r[2] = XMVectorSet(matrix.c1,matrix.c2,matrix.c3,matrix.c4);
		nodeMesh->_OffsetMatrix.r[3] = XMVectorSet(matrix.d1,matrix.d2,matrix.d3,matrix.d4);

		//配列長設定
		for (unsigned int m = 0; m < node->mNumMeshes; m++)
		{
			aiMesh* aimesh = scene->mMeshes[node->mMeshes[m]];
			mesh->_VertexNum += aimesh->mNumVertices;
			mesh->_IndexNum  += aimesh->mNumFaces * 3;
		}
		nodeMesh->_SubsetNum = node->mNumMeshes;

		//配列設定
		mesh->_VertexArray = new NodeMesh::MeshType::VertexType[mesh->_VertexNum];	//頂点配列
		mesh->_IndexArray = new unsigned short[mesh->_IndexNum];	//インデックス配列
		nodeMesh->_SubsetArray = new ModelSubset[nodeMesh->_SubsetNum];		//サブセット配列

		//インデックスカウント
		unsigned int iCount = 0;

		//メッシュ(多面体)
		for (unsigned int m = 0; m < node->mNumMeshes; m++)
		{
			//サブセット
			ModelSubset* subset = &nodeMesh->_SubsetArray[m];

			//メッシュを取得
			aiMesh* aimesh = scene->mMeshes[node->mMeshes[m]];
			{
				//マテリアルの取得 1つのメッシュに対してマテリアル一つ
				aiMaterial* material;
				material = scene->mMaterials[aimesh->mMaterialIndex];

				//マテリアル設定
				SetMaterial(&subset->_Material,material);

				//テクスチャ設定
				aiString texName;

				if (material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texName) == AI_SUCCESS)
				{
					subset->_Material._Texture  =TextureManager::LoadTexture(folderPath + texName.C_Str());
				}
				//頂点設定
#pragma region SetVertex

				for (unsigned int v = 0; v < aimesh->mNumVertices; v++)
				{
					VERTEX_3D* vertex = &mesh->_VertexArray[v];

					//頂点
					aiVector3D position(0.0f, 0.0f, 0.0f);
					if (aimesh->HasPositions()) position = aimesh->mVertices[v];
					vertex->Position = XMFLOAT3(position.x, position.y, position.z);

					//法線
					aiVector3D normal(0.0f, 1.0f, 0.0f);
					if (aimesh->HasNormals()) normal = aimesh->mNormals[v];
					vertex->Normal = XMFLOAT3(normal.x, normal.y, normal.z);

					//接線
					aiVector3D tangent(1.0f, 0.0f, 0.0f);
					if (aimesh->HasTangentsAndBitangents()) tangent = aimesh->mTangents[v];

					//従法線
					aiVector3D binormal(0.0f, 0.0f, 1.0f);
					if (aimesh->HasTangentsAndBitangents()) binormal = aimesh->mBitangents[v];

					//UV座標
					aiVector3D texCoord(0.0f, 0.0f, 0.0f);
					if (aimesh->HasTextureCoords(0)) texCoord = aimesh->mTextureCoords[0][v];
					vertex->TexCoord = XMFLOAT2(texCoord.x, texCoord.y);

					//頂点色
					aiColor4D diffuse(1.0f, 1.0f, 1.0f, 1.0f);
					if (aimesh->HasVertexColors(0)) diffuse = aimesh->mColors[0][v];
					vertex->Diffuse = XMFLOAT4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
				}
#pragma endregion

				//インデックス設定
#pragma region SetIndex

				//サブセット 開始インデックス
				subset->_StartIndex = iCount;

				//インデックス設定
				for (unsigned int f = 0; f < aimesh->mNumFaces; f++)
				{
					aiFace* face = &aimesh->mFaces[f];
					for (unsigned int i = 0; i < face->mNumIndices; i++)
					{
						mesh->_IndexArray[iCount] = face->mIndices[i];
						iCount++;
					}
				}

				//インデックス数
				subset->_IndexNum = iCount - subset->_StartIndex;

#pragma endregion
			}
		}

		//頂点バッファ生成
		mesh->CreateVertexBuffer();

		// インデックスバッファ生成
		mesh->CreateIndexBuffer();

		nodeMesh->_Mesh = MeshManager::RegisterIndex(nodeMesh->_NodeName,mesh);
	}

	//子のノードを取得
	for (unsigned int child = 0; child < node->mNumChildren; child++)
		GetNodeMesh(node->mChildren[child], model,scene,folderPath,rootMtx);
}

//LoadAssetForAssimp
//
//
Model* ModelManager::LoadAssetForAssimp(std::string fileName)
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
	size_t pos = fileName.find_last_of("\\/") + 1;
	std::string folderPath;
	if (pos == std::string::npos)
		folderPath = "";
	else
		folderPath = fileName.substr(0, pos);

	//モデル
	Model* model = new Model();

	aiMatrix4x4 mtx;

	//ノードの取得
	GetNodeMesh(scene->mRootNode,model,scene,folderPath,mtx);
	fileName = fileName.substr(pos, fileName.find_last_of(".") - pos);

	//追加
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
