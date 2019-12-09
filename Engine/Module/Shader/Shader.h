#pragma once

//���̓��C�A�E�g
enum VERTEX_INPUT_LAYOUT
{
	VSIL_POSITION,
	VSIL_NORMAL,
	VSIL_COLOR,
	VSIL_TEXCOORD
};

//���_�V�F�[�_�f�[�^�N���X
class VertexShaderData final
{
private:
	ID3D11VertexShader* _VertexShader;
	ID3D11InputLayout* _VertexLayout;
public:
	VertexShaderData();
	~VertexShaderData();

	void CrateVertexShader(const char* vertexShader, const VERTEX_INPUT_LAYOUT* InputLayout,unsigned int LayoutSize);
	void SetVertexShader();
};

//�s�N�Z���V�F�[�_�f�[�^�N���X
class PixelShaderData final
{
private:
	ID3D11PixelShader* _PixelShader;
public:
	PixelShaderData();
	~PixelShaderData();

	void CreatePixelShader(const char* pixelShader);
	void SetPixelShader();
};

//�R���X�g�o�b�t�@�v�f
enum CONSTANT_BUFFER_ELEMENT
{
	CONSTANT_BUFFER_WORLD = 0,
	CONSTANT_BUFFER_VIEW,
	CONSTANT_BUFFER_PROJECTION,
	CONSTANT_BUFFER_MATERIAL,
	CONSTANT_BUFFER_LIGHT,
	CONSTANT_BUFFER_NUM
};

//�R���X�g�o�b�t�@
class ConstantBuffer
{
private:
	ID3D11Buffer* _ConstantBuffer[CONSTANT_BUFFER_NUM];
public:
	void CreateBuffer();
	//�o�b�t�@�̎擾
	ID3D11Buffer** GetConstantBuffer(CONSTANT_BUFFER_ELEMENT bufferElement);
	//�o�b�t�@�̍X�V
	void UpdateSubresource(CONSTANT_BUFFER_ELEMENT bufferElement, const void* data);
	//�o�b�t�@�ɐݒ�
	void SetVSConstantBuffer(CONSTANT_BUFFER_ELEMENT bufferElement, UINT slot, UINT NumBuffer = 1);
	void SetPSConstantBuffer(CONSTANT_BUFFER_ELEMENT bufferElement, UINT slot, UINT NumBuffer = 1);
};

//�V�F�[�_
class Shader
{
private:
	VertexShaderData* _VertexShader;
	PixelShaderData* _PixelShader;
public:
	Shader();
	virtual~Shader();
	void SetShader();

	VertexShaderData* const GetVertexShader() { return _VertexShader; };
	PixelShaderData* const GetPixelShader() { return _PixelShader; };

	static void Shader::LoadShaderFile(const char * filename, unsigned char** buffer, long int * fsize);
};