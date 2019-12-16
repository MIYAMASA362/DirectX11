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

//Model
#include"model.h"
#include"ModelManager.h"

const std::string ModelManager::AssetDataBase = DirectX::AssetManager::AssetPath+ "Model/";
std::map<std::string, std::shared_ptr<Model>> ModelManager::ModelIndex;

void DirectX::ModelManager::LoadAsset(ModelAsset asset)
{
	Model* data = new Model();

	MODEL model;
	LoadObj(asset, &model);

	// 頂点バッファ生成
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

	// インデックスバッファ生成
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

	// サブセット設定
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

	//メモリ削除
	{
		delete[] model.VertexArray;
		delete[] model.IndexArray;
		delete[] model.SubsetArray;
	}

	ModelIndex.emplace(asset.name,std::shared_ptr<Model>(data));
}

void GetNodeMesh(aiNode* node, AssimpModel* model, const aiScene* scene, std::string folderPath,const aiMatrix4x4& nodeMtx)
{
	aiMatrix4x4 rootMtx = nodeMtx * node->mTransformation;

	if (node->mNumMeshes > 0)
	{
		//ノードメッシュ
		AssimpModel::NodeMesh* NodeMesh = new AssimpModel::NodeMesh();
		model->_NodeMeshArray.push_back(NodeMesh);

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
			NodeMesh->_VertexNum += mesh->mNumVertices;
			NodeMesh->_IndexNum  += mesh->mNumFaces * 3;
		}
		NodeMesh->_SubsetNum = node->mNumMeshes;

		//配列設定
		NodeMesh->_VertexArray = new AssimpModel::VERTEX_TYPE[NodeMesh->_VertexNum];	//頂点配列
		NodeMesh->_IndexArray = new unsigned short[NodeMesh->_IndexNum];	//インデックス配列
		NodeMesh->_SubsetArray = new AssimpModel::Subset[NodeMesh->_SubsetNum];		//サブセット配列

		//インデックスカウント
		unsigned int iCount = 0;

		//メッシュ(多面体)
		for (unsigned int m = 0; m < node->mNumMeshes; m++)
		{
			//サブセット
			AssimpModel::Subset* subset = &NodeMesh->_SubsetArray[m];

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
				subset->_Material._constant.Diffuse = Color(diffuse.r, diffuse.g, diffuse.b, 1.0f);
				subset->_Material._constant.Ambient = Color(ambient.r, ambient.g, ambient.b, 1.0f);
				subset->_Material._constant.Specular = Color(specular.r, specular.g, specular.b, 1.0f);
				subset->_Material._constant.Emission = Color(emission.r, emission.g, emission.b, 1.0f);
				subset->_Material._constant.Shininess = shininess;

#pragma endregion

				//テクスチャ設定
#pragma region SetTexture

				aiString texName;
				subset->_Texture = new Texture();

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

					HRESULT hr = D3DApp::GetDevice()->CreateTexture2D(&td, &sd, &subset->_Texture->texture);
					if (FAILED(hr))
						assert(false);

					D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
					srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					srv.Texture2D.MipLevels = 1;

					hr = D3DApp::GetDevice()->CreateShaderResourceView(subset->_Texture->texture, &srv, &subset->_Texture->srv);

					if (FAILED(hr))
						assert(false);

					stbi_image_free(pixels);
				}

#pragma endregion

				//頂点設定
#pragma region SetVertex

				for (unsigned int v = 0; v < mesh->mNumVertices; v++)
				{
					VERTEX_3D* vertex = &NodeMesh->_VertexArray[v];

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
						NodeMesh->_IndexArray[iCount] = face->mIndices[i];
						iCount++;
					}
				}

				//インデックス数
				subset->_IndexNum = iCount - subset->_StartIndex;

#pragma endregion
			}
		}

		// 頂点バッファ生成
		{
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(VERTEX_3D) * NodeMesh->_VertexNum;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem =  NodeMesh->_VertexArray;

			D3DApp::GetDevice()->CreateBuffer(&bd, &sd, &NodeMesh->_VertexBuffer);
		}

		// インデックスバッファ生成
		{
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned short) * NodeMesh->_IndexNum;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = NodeMesh->_IndexArray;

			D3DApp::GetDevice()->CreateBuffer(&bd, &sd, &NodeMesh->_IndexBuffer);
		}

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



	//要素数カウント
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

			//四角は三角に分割
			if (in == 4)
				in = 6;

			indexNum += in;
		}
	}


	//メモリ確保
	positionArray = new XMFLOAT3[positionNum];
	normalArray = new XMFLOAT3[normalNum];
	texcoordArray = new XMFLOAT2[texcoordNum];


	Model->VertexArray = new VERTEX_3D[vertexNum];
	Model->VertexNum = vertexNum;

	Model->IndexArray = new unsigned short[indexNum];
	Model->IndexNum = indexNum;

	Model->SubsetArray = new SUBSET[subsetNum];
	Model->SubsetNum = subsetNum;




	//要素読込
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
			//マテリアルファイル
			fscanf(file, "%s", str);

			char path[256];
			
			strcpy(path, AssetDataBase.c_str());
			strcat(path, asset.folderPath.c_str());
			strcat(path,"/");

			LoadMaterial(path,str, &materialArray, &materialNum);
		}
		else if (strcmp(str, "o") == 0)
		{
			//オブジェクト名
			fscanf(file, "%s", str);
		}
		else if (strcmp(str, "v") == 0)
		{
			//頂点座標
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
			//法線
			fscanf(file, "%f", &normal->x);
			fscanf(file, "%f", &normal->y);
			fscanf(file, "%f", &normal->z);
			normal++;
		}
		else if (strcmp(str, "vt") == 0)
		{
			//テクスチャ座標
			fscanf(file, "%f", &texcoord->x);
			fscanf(file, "%f", &texcoord->y);
			//texcoord->y = 1.0f - texcoord->y;
			texcoord++;
		}
		else if (strcmp(str, "usemtl") == 0)
		{
			//マテリアル
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
			//面
			in = 0;

			do
			{
				fscanf(file, "%s", str);

				s = strtok(str, "/");
				Model->VertexArray[vc].Position = positionArray[atoi(s) - 1];
				if (s[strlen(s) + 1] != '/')
				{
					//テクスチャ座標が存在しない場合もある
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

			//四角は三角に分割
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

	//要素数カウント
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


	//メモリ確保
	materialArray = new ModelMaterial[materialNum];


	//要素読込
	int mc = -1;

	fseek(file, 0, SEEK_SET);

	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;


		if (strcmp(str, "newmtl") == 0)
		{
			//マテリアル名
			mc++;
			fscanf(file, "%s", materialArray[mc]._Name);
			strcpy(materialArray[mc]._TextureName, "");
		}
		else if (strcmp(str, "Ka") == 0)
		{
			//アンビエント
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Ambient.r);
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Ambient.g);
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Ambient.b);
			materialArray[mc]._Material._constant.Ambient.a = 1.0f;
		}
		else if (strcmp(str, "Kd") == 0)
		{
			//ディフューズ
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Diffuse.r);
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Diffuse.g);
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Diffuse.b);
			materialArray[mc]._Material._constant.Diffuse.a = 1.0f;
		}
		else if (strcmp(str, "Ks") == 0)
		{
			//スペキュラ
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Specular.r);
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Specular.g);
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Specular.b);
			materialArray[mc]._Material._constant.Specular.a = 1.0f;
		}
		else if (strcmp(str, "Ns") == 0)
		{
			//スペキュラ強度
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Shininess);
		}
		else if (strcmp(str, "d") == 0)
		{
			//アルファ
			fscanf(file, "%f", &materialArray[mc]._Material._constant.Diffuse.a);
		}
		else if (strcmp(str, "map_Kd") == 0)
		{
			//テクスチャ
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

AssimpModel::~AssimpModel()
{
	for(auto mesh : _NodeMeshArray)
	{
		delete[] mesh->_SubsetArray;

		delete[] mesh->_IndexArray;
		delete[] mesh->_VertexArray;

		if (mesh->_VertexBuffer) mesh->_VertexBuffer->Release();
		if (mesh->_IndexBuffer) mesh->_IndexBuffer->Release();
	}
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
		D3DApp::Renderer::SetVertexBuffer(nodeMesh->_VertexBuffer,sizeof(VERTEX_3D),0);
		D3DApp::Renderer::SetIndexBuffer(nodeMesh->_IndexBuffer);

		XMMATRIX local;
		local = nodeMesh->_OffsetMatrix * worldMtx;

		D3DApp::Renderer::SetWorldMatrix(&local);

		for (unsigned int s = 0; s < nodeMesh->_SubsetNum; s++)
		{
			Subset& subset = nodeMesh->_SubsetArray[s];
			D3DApp::GetShader()->SetShader();

			subset._Material.SetResource();
			D3DApp::GetConstantBuffer()->SetVSConstantBuffer(CONSTANT_BUFFER_MATERIAL,3);
			subset._Texture[0].SetResource();

			D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			D3DApp::GetDeviceContext()->DrawIndexed(subset._IndexNum,subset._StartIndex,0);
		}
	}
}
