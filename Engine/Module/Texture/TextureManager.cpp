#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<assert.h>
#include<string>
#include<map>
#include<memory>

#include"Module\DirectX\DirectX.h"
#include"Module\IMGUI\GUI_ImGui.h"

#include"cereal\cereal.hpp"
#include"cereal\archives\json.hpp"
#include<fstream>

#include"Module\Object\Object.h"
#include"texture.h"
#include"TextureManager.h"

using namespace DirectX;

//*********************************************************************************************************************
//
//	TextureManager
//
//*********************************************************************************************************************

//�C���X�^���X
TextureManager* TextureManager::g_pInstance = nullptr;

//TextureManager
//	�R���X�g���N�^
//
TextureManager::TextureManager()
{

}

//~TextureManager
//	�f�X�g���N�^
//
TextureManager::~TextureManager()
{
	_TextureIndex.clear();
}

//Create
//	�C���X�^���X����
//
void TextureManager::Create()
{
	if (g_pInstance) return;
	g_pInstance = new TextureManager();
}

//Release
//	�C���X�^���X�j��
//
void TextureManager::Release()
{
	if (g_pInstance == nullptr) return;
	delete g_pInstance;
	g_pInstance = nullptr;
}

//LoadTexture
//	Texture�ǂݍ���
//
std::weak_ptr<Texture> TextureManager::LoadTexture(std::string filePath)
{
	//�e�N�X�`����
	size_t pos = filePath.find_last_of("\\/") + 1;
	size_t len = filePath.find_last_of("\\.") - pos;
	std::string name = filePath.substr(pos,len);

	auto find = g_pInstance->_TextureIndex.find(name);
	if (find != g_pInstance->_TextureIndex.end())
		return find->second;

	//�e�N�X�`��
	Texture* texture = new Texture();
	texture->LoadTexture(filePath);

	//�C���f�b�N�X�ɓo�^
	return g_pInstance->_TextureIndex.emplace(name,std::shared_ptr<Texture>(texture)).first->second;
}


//GetTexture
//	�e�N�X�`���擾
//
std::weak_ptr<Texture> TextureManager::GetTexture(std::string name)
{
	auto find = g_pInstance->_TextureIndex.find(name);
	if (find == g_pInstance->_TextureIndex.end())
		MessageBox(NULL,("�e�N�X�`������������܂���ł����B:" + name).c_str(),"TextureManager",MB_OK);
	return find->second;
}


//EditorView�p�̃|�C���^
ID3D11ShaderResourceView* gView = nullptr;

//EditorWindow
//	ImGui�G�f�B�^�E�B���h�E
//
void TextureManager::EditorWindow()
{
	ImGui::Begin("TextureManager");
	for (auto obj : g_pInstance->_TextureIndex)
	{

		ImGui::Image((void*)obj.second->GetShaderResourceView(), ImVec2(64,64));
		ImGui::SameLine();
		ImGui::Text(obj.first.c_str());
	}

	if (gView) ImGui::Image((void*)gView, ImVec2(128, 128));
	ImGui::End();
}

void TextureManager::SetEditorView(ID3D11ShaderResourceView * view)
{
	gView = view;
}

