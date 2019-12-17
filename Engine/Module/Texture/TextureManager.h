#pragma once

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

//Texture�Ǘ�
class TextureManager final
{
	using TextureIndex = std::map<std::string, std::shared_ptr<Texture>>;
private:
	//�e�N�X�`���ۊ�
	static TextureIndex textureIndex;

	TextureManager() = delete;
	~TextureManager() = delete;
public:
	//�e�N�X�`���ǂݍ��� aaa.xxx��aaaw���e�N�X�`�����Ƃ���
	static std::weak_ptr<Texture> LoadTexture(std::string filePath);
	//���
	static void Release();
	//�e�N�X�`���擾
	static std::weak_ptr<Texture> GetTexture(std::string name);
};