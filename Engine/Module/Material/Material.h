#pragma once

class Texture;
struct aiMaterial;

//*********************************************************************************************************************
//
//	Material
//
//*********************************************************************************************************************
class Material final
{
public:
	struct Constant
	{
		Color Ambient;
		Color Diffuse;
		Color Specular;
		Color Emission;
		float Shininess;
		float Dummy[3];	//16bit�T�C�Y�ɍ��킹��
	};

	//�e�N�X�`��
	std::weak_ptr<Texture> _Texture;
	//�V�F�[�_
	std::weak_ptr<Shader> _Shader;
	//�}�e���A���F
	Material::Constant _constant;


public:
	//�R���X�g���N�^
	Material();
	//�f�X�g���N�^
	virtual ~Material();

	//���\�[�X�ݒ�
	void SetResource();

	void SetResourceMaterial();
	void SetResourceTexture();
	void SetResourceShader();


};