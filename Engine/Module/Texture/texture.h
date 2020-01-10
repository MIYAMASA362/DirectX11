#pragma once

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

//Texture
class Texture final
{
private:
	std::string _filePath;				//�t�@�C���p�X

	ID3D11Texture2D* _texture;			//�e�N�X�`��
	ID3D11ShaderResourceView* _srv;		//�V�F�[�_�[���\�[�X

public:
	Texture();
	Texture(ID3D11Texture2D* texture, ID3D11ShaderResourceView* srv);
	~Texture();

	ID3D11Texture2D* GetTexture(){ return _texture; };
	ID3D11ShaderResourceView* GetShaderResourceView() { return _srv; };

	//���\�[�X�ݒ�
	void SetResource();
	//�e�N�X�`���ǂݍ���
	void LoadTexture(std::string filePath);
};
