#pragma once

class Shader
{
private:
	XMMATRIX _WorldMatrix;
	XMMATRIX _ViewMatrix;
	XMMATRIX _ProjectionMatrix;

	MATERIAL _Material;
	LIGHT _Light;
private:
	ID3D11VertexShader* _VertexShader;
	ID3D11PixelShader* _PixelShader;
	ID3D11InputLayout* _VertexLayout;

	ID3D11Buffer* _WorldBuffer;
	ID3D11Buffer* _ViewBuffer;
	ID3D11Buffer* _ProjectionBuffer;

	ID3D11Buffer* _MaterialBuffer;
	ID3D11Buffer* _LightBuffer;

public:
	Shader();
	virtual~Shader();

	void LoadShader(const char* VertexShader,const char* PixelShader);
	void SetShader();
	void Release();

	void SetWorldMatrix(XMMATRIX* WorldMatrix);
	void SetViewMatrix(XMMATRIX* ViewMatrix);
	void SetProjectionMatrix(XMMATRIX* ProjectionMatrix);

	void SetMaterial(MATERIAL* material);
	void SetLight(LIGHT* light);
};