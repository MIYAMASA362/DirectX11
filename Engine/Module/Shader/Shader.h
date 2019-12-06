#pragma once

class Light;
class Material;

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
public:
	Shader();
	virtual~Shader();

	void LoadShader(const char* VertexShader,const char* PixelShader);
	void SetShader();
	void Release();

	void SetWorldMatrix(XMMATRIX* WorldMatrix);
	void SetViewMatrix(XMMATRIX* ViewMatrix);
	void SetProjectionMatrix(XMMATRIX* ProjectionMatrix);

	//filename�̃t�@�C�����o�C�i���`���œǂݍ���,�T�C�Y�Ȃǂ�Ԃ��܂��B
	void LoadShaderFile(const char* filename,unsigned char** buffer,long int* fsize);

	Material* GetMaterial() { return _Material; };
};