#pragma once

#define DEFAULT_SHADER ("Default")

//*********************************************************************************************************************
//
//	ShaderManager
//
//*********************************************************************************************************************
class ShaderManager final
{
private:
	//�C���X�^���X
	static ShaderManager* pInstance;
	//���\�[�X �C���f�b�N�X
	std::map<std::string, std::shared_ptr<Shader>> _ShaderIndex;

	//�R���X�g���N�^
	ShaderManager();
	//�f�X�g���N�^
	~ShaderManager();


public:
	//�C���X�^���X����
	static void Create();
	//�C���X�^���X�j��
	static void Release();

	//Shader�̒ǉ�
	static std::weak_ptr<Shader> RegisterShader(std::string name,Shader* shader);
	static std::weak_ptr<Shader> RegisterShader(std::string name,std::string vertexShader,VERTEX_INPUT_LAYOUT* layout,unsigned int size, std::string PixelShader);
	//Shader�̎擾
	static std::weak_ptr<Shader> GetShader(std::string name = DEFAULT_SHADER);


};