#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<assert.h>
#include"Common.h"

//DirectX
#include"Module\DirectX\DirectX.h"

//Engine
#include"Module\ECSEngine.h"

#include"Module\Material\Material.h"
#include"Module\Texture\texture.h"

//Component Module
#include"Module\Renderer\Renderer.h"

#include"ModelManager.h"

#include"Module\Texture\TextureManager.h"

#include"Module\Mesh\Mesh.h"

#include"model.h"
#include"Module\Model\ModelManager.h"

#include"Module\Shader\Shader.h"

NodeMesh::~NodeMesh()
{
	delete _Mesh;
	delete[] _SubsetArray;
}

Model::~Model()
{
	for (auto mesh : _NodeMeshArray) delete mesh;
}