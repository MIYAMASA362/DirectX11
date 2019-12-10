#pragma once

//�O���錾
class Light;
class Material;


//�R���X�^���g�o�b�t�@�v�f
enum CONSTANT_BUFFER_ELEMENT
{
	CONSTANT_BUFFER_WORLD = 0,
	CONSTANT_BUFFER_VIEW,
	CONSTANT_BUFFER_PROJECTION,
	CONSTANT_BUFFER_MATERIAL,
	CONSTANT_BUFFER_LIGHT,
	CONSTANT_BUFFER_NUM
};

//�R���X�^���g�o�b�t�@����
class ConstantBuffer
{
private:
	ID3D11Buffer* _ConstantBuffer[CONSTANT_BUFFER_NUM];
public:
	//�R���X�^���g�o�b�t�@�̐���
	void CreateBuffer();
	//�o�b�t�@�̎擾
	ID3D11Buffer* GetConstantBuffer(CONSTANT_BUFFER_ELEMENT bufferElement);
	//�o�b�t�@�̍X�V
	void UpdateSubresource(CONSTANT_BUFFER_ELEMENT bufferElement,const void* data);
	
	//�o�b�t�@�ɐݒ�
	void SetVSConstantBuffer(CONSTANT_BUFFER_ELEMENT bufferElement,UINT slot,UINT numBuffer = 1);
	void SetPSConstantBuffer(CONSTANT_BUFFER_ELEMENT bufferElement,UINT slot,UINT numBuffer = 1);
};



//���̓��C�A�E�g
enum VERTEX_INPUT_LAYOUT
{
	VSIL_POSITION,
	VSIL_NORMAL,
	VSIL_COLOR,
	VSIL_TEXCOORD
};

//���_�V�F�[�_
class VertexShaderData
{
private:
	ID3D11VertexShader* _VertexShader;
	ID3D11InputLayout* _InputLayout;
public:
	VertexShaderData();
	virtual~VertexShaderData();

	void LoadShader(const char* vertexShader,VERTEX_INPUT_LAYOUT* inputLayout,unsigned int layoutSize);
	void SetShader();
};



//�s�N�Z���V�F�[�_
class PixelShaderData
{
private:
	ID3D11PixelShader* _PixelShader;
public:
	PixelShaderData();
	virtual~PixelShaderData();

	void LoadShader(const char* pixelShader);
	void SetShader();
};



//�V�F�[�_
class Shader
{
private:
	Light* _Light;
	Material* _Material;

	VertexShaderData*	_VertexShader;
	PixelShaderData*	_PixelShader;
public:
	Shader();
	virtual~Shader();

	void LoadShader(const char* VertexShader,const char* PixelShader,VERTEX_INPUT_LAYOUT* layout,unsigned int size);
	void SetShader();
	void Release();

	Material* GetMaterial() { return _Material; };
};




//filename�̃t�@�C�����o�C�i���`���œǂݍ���,�T�C�Y�Ȃǂ�Ԃ��܂��B
void LoadShaderFile(const char* filename, unsigned char** buffer, long int* fsize);