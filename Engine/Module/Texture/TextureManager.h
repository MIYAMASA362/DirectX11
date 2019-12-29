#pragma once

//Texture�Ǘ�
//
//
class TextureManager final
{
	using TextureIndex = std::map<std::string, std::shared_ptr<Texture>>;
private:
	//Singleton Instance
	static TextureManager* g_pInstance;

	//�e�N�X�`���ۊ�
	TextureIndex _TextureIndex;

	//�R���X�g���N�^
	TextureManager();
	//�f�X�g���N�^
	~TextureManager();


public:
	//�C���X�^���X����
	static void Create();
	//�C���X�^���X�j��
	static void Release();

	//�e�N�X�`���ǂݍ��� aaa.xxx��aaaw���e�N�X�`�����Ƃ���
	static std::weak_ptr<Texture> LoadTexture(std::string filePath);
	//�e�N�X�`���擾
	static std::weak_ptr<Texture> GetTexture(std::string name);

	//�G�f�B�^�p�E�B���h�E
	static void EditorWindow();
};