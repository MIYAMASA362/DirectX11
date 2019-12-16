#pragma once

#pragma warning(push)
#pragma warning(disable : 4005) 
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#pragma warning(pop)

// Direct3D extension.
#if ( _MSC_VER > 1000 ) || defined(_DEBUG)
#pragma comment(lib, "d3dx11.lib")
#else
#pragma comment(lib,"d3dx11.lib")
#endif

#pragma warning(push) 
#pragma warning(disable : 4005) 
#include <D3DX11.h>
#pragma warning(pop)
#include<DirectXMath.h>

using namespace DirectX;

class Shader;
class ConstantBuffer;
class Light;
class Material;

#include"Module\DirectX\DirectXStruct.h"

class D3DApp
{
private:
	static D3DApp* pInstance;
private:
	unsigned int ScreenWidth;
	unsigned int ScreenHeight;
	const std::string path = "";	//Applicaion Path
	unsigned int fps;
private:
	HWND hWnd;
	HINSTANCE hInstance;

	D3D_FEATURE_LEVEL featureLevel;

	ID3D11Device*			D3DDevice;
	ID3D11DeviceContext*	ImmediateContext;
	IDXGISwapChain*			SwapChain;
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11DepthStencilView* DepthStencilView;

	ID3D11DepthStencilState* DepthStateEnable;
	ID3D11DepthStencilState* DepthStateDisable;

	XMMATRIX _ProjectionMatrix;

	Shader* _Shader;					//シェーダ
	ConstantBuffer* _ConstantBuffer;	//定数バッファ
	Light* _Light;
	Material* _Material;
private:
	D3DApp() = default;
	~D3DApp() = default;
public:
	static HRESULT Create(HWND hWnd, HINSTANCE hInstance, unsigned int fps);
	static void Destroy();
public:
	static ID3D11Device* GetDevice();
	static ID3D11DeviceContext* GetDeviceContext();
	static HWND GetWindow();
	static HINSTANCE GethInstance();
	static unsigned int GetScreenWidth();
	static unsigned int GetScreenHeight();
	static unsigned int GetFps();

	static Shader* GetShader() { return pInstance->_Shader; };
	static ConstantBuffer* GetConstantBuffer() { return pInstance->_ConstantBuffer; };

	static void CreateBuffer(unsigned int BindFlag, unsigned int byteWidth, const void* subresource, ID3D11Buffer** buffer);
public:
	class Renderer
	{
	public:
		static void Begin();
		static void End();
		static void ClearRenderTargetView(Color clearColor);

		static void SetDepthEnable(bool Enable);
		static void SetRasterize(D3D11_FILL_MODE fillmode, D3D11_CULL_MODE cullmode);

		static void SetWorldViewProjection2D();
		static void SetWorldMatrix(XMMATRIX* WorldMatrix);
		static void SetViewMatrix(XMMATRIX* ViewMatrix);
		static void SetProjectionMatrix(XMMATRIX* ProjectionMatrix);
		static void SetProjectionMatrix2D();

		static void SetVertexBuffer(ID3D11Buffer* VertexBuffer,UINT stride,UINT offset);
		static void SetIndexBuffer(ID3D11Buffer* IndexBuffer);

		static void SetTexture(ID3D11ShaderResourceView* Texture, UINT slot = 0, UINT numView = 1);
		static void DrawIndexed(UINT IndexCount, UINT StartIndexLocation, int BaseVertexLocation);

		static XMMATRIX GetProjectionMatrix2D();
	};
};