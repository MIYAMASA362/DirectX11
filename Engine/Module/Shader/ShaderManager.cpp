#include<memory>
#include<map>
#include<string>
#include<Windows.h>

#include"Module\DirectX\DirectX.h"

#include"Shader.h"
#include "ShaderManager.h"

//*********************************************************************************************************************
//
//	ShaderManager
//
//*********************************************************************************************************************

//�C���X�^���X
ShaderManager* ShaderManager::pInstance = nullptr;

//ShaderManager
//	�R���X�g���N�^
//
ShaderManager::ShaderManager()
{

}

//~ShaderManager
//	�f�X�g���N�^
//
ShaderManager::~ShaderManager()
{
	_ShaderIndex.clear();
}

//Create
//	�C���X�^���X����
//
void ShaderManager::Create()
{
	if (pInstance) return;
	pInstance = new ShaderManager();

	//Default Shader��o�^
	Shader* shader = new Shader();
	VERTEX_INPUT_LAYOUT layout[] =
	{
		VSIL_POSITION,
		VSIL_NORMAL,
		VSIL_COLOR,
		VSIL_TEXCOORD
	};
	shader->SetVertexShader("Asset/Shader/vertexShader.cso", layout, ARRAYSIZE(layout));
	shader->SetPixelShader("Asset/Shader/pixelShader.cso");

	RegisterShader(DEFAULT_SHADER,shader);
}

//Release
//	�C���X�^���X�j��
//
void ShaderManager::Release()
{
	if (pInstance == nullptr) return;
	delete pInstance;
	pInstance = nullptr;
}

//RegisterShader
//	�V�F�[�_�o�^
//
std::weak_ptr<Shader> ShaderManager::RegisterShader(std::string name, Shader * shader)
{
	auto find = pInstance->_ShaderIndex.find(name);
	if(find != pInstance->_ShaderIndex.end())
		MessageBox(NULL,("�d�����Ă���Shader������܂��B Name:" + name).c_str(),"ShaderManager",MB_OK);
	return pInstance->_ShaderIndex.emplace(name,std::shared_ptr<Shader>(shader)).first->second;
}

std::weak_ptr<Shader> ShaderManager::RegisterShader(std::string name, std::string vertexShader, VERTEX_INPUT_LAYOUT * layout, unsigned int size, std::string PixelShader)
{
	//Default Shader��o�^
	Shader* shader = new Shader();
	shader->SetVertexShader(vertexShader.c_str(), layout,size);
	shader->SetPixelShader(PixelShader.c_str());
	return RegisterShader(name,shader);
}

//GetShader
//	�V�F�[�_�擾
//
std::weak_ptr<Shader> ShaderManager::GetShader(std::string name)
{
	auto find = pInstance->_ShaderIndex.find(name);
	if (find == pInstance->_ShaderIndex.end())
		MessageBox(NULL,("��������܂���ł����BName:" + name).c_str(),"ShaderManager",MB_OK);
	return find->second;
}
