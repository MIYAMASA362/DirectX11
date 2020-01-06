#pragma once

class D3DRenderer;

//*********************************************************************************************************************
//
//	CONSTANT_BUFFER_ELEMENT
//
//*********************************************************************************************************************
enum CONSTANT_BUFFER_ELEMENT
{
	CONSTANT_BUFFER_WORLD = 0,
	CONSTANT_BUFFER_VIEW,
	CONSTANT_BUFFER_PROJECTION,
	CONSTANT_BUFFER_MATERIAL,
	CONSTANT_BUFFER_LIGHT,
	CONSTANT_BUFFER_NUM
};


//*********************************************************************************************************************
//
//	ConstantBuffer
//
//*********************************************************************************************************************
class ConstantBuffer
{
private:
	D3DRenderer* _Renderer;
	ID3D11Buffer* _ConstantBuffer[CONSTANT_BUFFER_NUM];
public:
	ConstantBuffer(D3DRenderer* renderer);
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






//*********************************************************************************************************************
//
//	VERTEZ_INPUT_LAYOUT
//
//*********************************************************************************************************************
enum VERTEX_INPUT_LAYOUT
{
	VSIL_POSITION,
	VSIL_NORMAL,
	VSIL_COLOR,
	VSIL_TEXCOORD
};


//*********************************************************************************************************************
//
//	VertexShaderData
//
//*********************************************************************************************************************
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




//*********************************************************************************************************************
//
//	PixelShaderData
//
//*********************************************************************************************************************
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





//*********************************************************************************************************************
//
//	Shader
//
//*********************************************************************************************************************
class Shader
{
private:
	VertexShaderData*	_VertexShader;
	PixelShaderData*	_PixelShader;
public:
	Shader();
	virtual~Shader();

	void SetVertexShader(const char* VertexShader,VERTEX_INPUT_LAYOUT* layout,unsigned int size);
	void SetPixelShader(const char* PixelShader);

	void SetShader();
	void Release();
};


//filename�̃t�@�C�����o�C�i���`���œǂݍ���,�T�C�Y�Ȃǂ�Ԃ��܂��B
void LoadShaderFile(const char* filename, unsigned char** buffer, long int* fsize);