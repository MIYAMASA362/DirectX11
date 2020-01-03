#pragma once

//DirectX Compiler
//
//
#pragma warning(push)
#pragma warning(disable : 4005) 
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#pragma warning(pop)

// Direct3D extension
//
//
#if ( _MSC_VER > 1000 ) || defined(_DEBUG)
#pragma comment(lib, "d3dx11.lib")
#else
#pragma comment(lib,"d3dx11.lib")
#endif


//DirectX SDK
//
//
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

//*********************************************************************************************************************
//
//	D3DApp DirectX Application
//
//*********************************************************************************************************************
class D3DApp final
{
private:
	//DirectX Graphics Infrastructure
	IDXGIDevice1* _pDXGI = nullptr;
	IDXGIAdapter* _pAdapter = nullptr;
	IDXGIFactory* _pFactory = nullptr;
	IDXGISwapChain*	 SwapChain = nullptr;

	HWND hWnd;

	D3D_FEATURE_LEVEL featureLevel;

	ID3D11Device*			D3DDevice = nullptr;
	ID3D11DeviceContext*	ImmediateContext = nullptr;
	ID3D11RenderTargetView* RenderTargetView = nullptr;
	ID3D11DepthStencilView* DepthStencilView = nullptr;

	ID3D11DepthStencilState* DepthStateEnable = nullptr;
	ID3D11DepthStencilState* DepthStateDisable = nullptr;

public:
	//コンストラクタ
	D3DApp();
	//デストラクタ
	~D3DApp();

	//デバイス生成
	HRESULT Create(HWND hWnd, unsigned int fps);

	//D3DDevice取得
	ID3D11Device* GetDevice() { return D3DDevice; }
	//デバイスコンテキスト
	ID3D11DeviceContext* GetDeviceContext() { return ImmediateContext; }

	//HWND取得
	HWND GetWindow() { return hWnd; };
	unsigned int GetRefreshRate();

	void CreateBuffer(unsigned int BindFlag, unsigned int byteWidth, const void* subresource, ID3D11Buffer** buffer);

	IDXGISwapChain* GetSwapChain() { return SwapChain; };
	DXGI_SWAP_CHAIN_DESC GetSwapChainDesc();

	void CreateRenderTargetView();
	void CreateDepthStencilView();

	void CleanupRenderTargetView();
	void CleanupDepthStencilView();
public:

	//*********************************************************************************************************************
	//
	//	D3DApp::Renderer
	//
	//*********************************************************************************************************************
	class Renderer
	{
	private:
		static D3DApp* _pDevice;

		static Shader* _Shader;
		static ConstantBuffer* _ConstantBuffer;
		static Light* _Light;
		static Material* _Material;

	public:
		static void RegisterDevice(D3DApp* d3dapp);
		static D3DApp* GetD3DAppDevice() { return _pDevice; };
		static void Release();

		static void Begin();
		static void End();
		static void ClearRenderTargetView(Color clearColor);

		static Shader* GetShader() { return _Shader; };
		static ConstantBuffer* GetConstantBuffer() { return _ConstantBuffer; };

		static void SetDepthEnable(bool Enable);
		static void SetRasterize(D3D11_FILL_MODE fillmode, D3D11_CULL_MODE cullmode);

		static void SetWorldViewProjection2D();
		static void SetWorldMatrix(XMMATRIX* WorldMatrix);
		static void SetViewMatrix(XMMATRIX* ViewMatrix);
		static void SetProjectionMatrix(XMMATRIX* ProjectionMatrix);
		static void SetProjectionMatrix2D();

		static void SetVertexBuffer(ID3D11Buffer* VertexBuffer, UINT stride, UINT offset);
		static void SetIndexBuffer(ID3D11Buffer* IndexBuffer);

		static void SetTexture(ID3D11ShaderResourceView* Texture, UINT slot = 0, UINT numView = 1);
		static void DrawIndexed(UINT IndexCount, UINT StartIndexLocation, int BaseVertexLocation);

		static XMMATRIX GetProjectionMatrix2D();
	};


};