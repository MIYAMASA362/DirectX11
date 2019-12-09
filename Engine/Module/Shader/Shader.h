#pragma once

class Light;
class Material;

enum VERTEX_INPUT_LAYOUT
{
	VSIL_POSITION,
	VSIL_NORMAL,
	VSIL_COLOR,
	VSIL_TEXCOORD
};

//コンスタントバッファ要素
enum CONSTANT_BUFFER_ELEMENT
{
	CONSTANT_BUFFER_WORLD = 0,
	CONSTANT_BUFFER_VIEW,
	CONSTANT_BUFFER_PROJECTION,
	CONSTANT_BUFFER_MATERIAL,
	CONSTANT_BUFFER_LIGHT,
	CONSTANT_BUFFER_NUM
};

//コンスタントバッファ生成
class ConstantBuffer
{
private:
	ID3D11Buffer* _ConstantBuffer[CONSTANT_BUFFER_NUM];
public:
	//コンスタントバッファの生成
	void CreateBuffer();
	//バッファの取得
	ID3D11Buffer* GetConstantBuffer(CONSTANT_BUFFER_ELEMENT bufferElement);
	//バッファの更新
	void UpdateSubresource(CONSTANT_BUFFER_ELEMENT bufferElement,const void* data);
	
	//バッファに設定
	void SetVSConstantBuffer(CONSTANT_BUFFER_ELEMENT bufferElement,UINT slot,UINT numBuffer = 1);
	void SetPSConstantBuffer(CONSTANT_BUFFER_ELEMENT bufferElement,UINT slot,UINT numBuffer = 1);
};

//シェーダ
class Shader
{
private:
	XMMATRIX _WorldMatrix;
	XMMATRIX _ViewMatrix;
	XMMATRIX _ProjectionMatrix;

private:
	Light* _Light;
	Material* _Material;

	ID3D11VertexShader* _VertexShader;
	ID3D11PixelShader* _PixelShader;
	ID3D11InputLayout* _VertexLayout;

	ID3D11Buffer* _WorldBuffer;
	ID3D11Buffer* _ViewBuffer;
	ID3D11Buffer* _ProjectionBuffer;

	ConstantBuffer* _ConstantBuffer;
public:
	Shader();
	virtual~Shader();

	void LoadShader(const char* VertexShader,const char* PixelShader,VERTEX_INPUT_LAYOUT* layout,unsigned int size);
	void SetShader();
	void Release();

	void SetWorldMatrix(XMMATRIX* WorldMatrix);
	void SetViewMatrix(XMMATRIX* ViewMatrix);
	void SetProjectionMatrix(XMMATRIX* ProjectionMatrix);

	//filenameのファイルをバイナリ形式で読み込み,サイズなどを返します。
	void LoadShaderFile(const char* filename,unsigned char** buffer,long int* fsize);

	Material* GetMaterial() { return _Material; };
};