#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<assert.h>
#include<string>
#include<map>
#include<memory>

#include"Module\DirectX\DirectX.h"
#include"Module\IMGUI\GUI_ImGui.h"

#include"texture.h"
#include"TextureManager.h"

using namespace DirectX;

//*********************************************************************************************************************
//
//	TextureManager
//
//*********************************************************************************************************************

//インスタンス
TextureManager* TextureManager::g_pInstance = nullptr;

//TextureManager
//	コンストラクタ
//
TextureManager::TextureManager()
{

}

//~TextureManager
//	デストラクタ
//
TextureManager::~TextureManager()
{
	_TextureIndex.clear();
}

//Create
//	インスタンス生成
//
void TextureManager::Create()
{
	if (g_pInstance) return;
	g_pInstance = new TextureManager();
}

//Release
//	インスタンス破棄
//
void TextureManager::Release()
{
	if (g_pInstance == nullptr) return;
	delete g_pInstance;
	g_pInstance = nullptr;
}

//LoadTexture
//	Texture読み込み
//
std::weak_ptr<Texture> TextureManager::LoadTexture(std::string filePath)
{
	//テクスチャ
	Texture* texture = new Texture();
	texture->LoadTexture(filePath);

	//テクスチャ名
	size_t pos = filePath.find_last_of("\\/") + 1;
	size_t len = filePath.find_last_of("\\.") - pos;
	std::string name = filePath.substr(pos,len);

	//インデックスに登録
	return g_pInstance->_TextureIndex.emplace(name,std::shared_ptr<Texture>(texture)).first->second;
}


//GetTexture
//	テクスチャ取得
//
std::weak_ptr<Texture> TextureManager::GetTexture(std::string name)
{
	auto find = g_pInstance->_TextureIndex.find(name);
	if (find == g_pInstance->_TextureIndex.end())
		MessageBox(NULL,("テクスチャが見当たりませんでした。:" + name).c_str(),"TextureManager",MB_OK);
	return find->second;
}


//EditorView用のポインタ
ID3D11ShaderResourceView* gView = nullptr;

//EditorWindow
//	ImGuiエディタウィンドウ
//
void TextureManager::EditorWindow()
{
	ImGui::Begin("TextureManager");
	ImGui::BeginChild(ImGui::GetID((void*)0),ImVec2(256,128),ImGuiWindowFlags_NoTitleBar);
	for(auto obj : g_pInstance->_TextureIndex)
	{
		if(ImGui::Button(obj.first.c_str()))
		{
			gView = obj.second->GetShaderResourceView();
		};
	}
	ImGui::EndChild();

	if (gView) ImGui::Image((void*)gView, ImVec2(128, 128));
	ImGui::End();
}

