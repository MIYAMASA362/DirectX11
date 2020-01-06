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

class D3DRenderer;




//*********************************************************************************************************************
//
//	RenderStatus
//
//*********************************************************************************************************************
class RenderStatus final
{
private:

	D3DRenderer* _D3DRenderer;

	IDXGISwapChain* _SwapChain = nullptr;

	ID3D11RenderTargetView* _RenderTargetView = nullptr;
	ID3D11DepthStencilView* _DepthStencilView = nullptr;

	ID3D11DepthStencilState* _DepthStateEnable = nullptr;
	ID3D11DepthStencilState* _DepthStateDisable = nullptr;

	bool _IsReleased;
public:
	RenderStatus(HWND hWnd, D3DRenderer* renderer,unsigned int fps);
	~RenderStatus();

	IDXGISwapChain* GetSwapChain() { return _SwapChain; };

	DXGI_SWAP_CHAIN_DESC GetSwapChainDesc();

	void CreateRenderTargetView();
	void CreateDepthStencilView();
	
	void SetViewport(UINT NumViewports);

	void CleanupRenderTargetView();
	void CleanupDepthStencilView();

	void ClearRenderTargetView(Color clearColor);
	void Begin();
	void End();

	void SetDepthEnable(bool Enable);
	unsigned int GetRefreshRate();

	void Release();
};




//*********************************************************************************************************************
//
//	D3DApp DirectX Application
//
//*********************************************************************************************************************
class D3DRenderer final
{
private:
	static D3DRenderer* pInstance;
	static RenderStatus* pTargetRenderStatus;

	//DirectX Graphics Infrastructure
	IDXGIDevice1* _pDXGI = nullptr;
	IDXGIAdapter* _pAdapter = nullptr;
	IDXGIFactory* _pFactory = nullptr;

	D3D_FEATURE_LEVEL featureLevel;

	ID3D11Device*			_D3DDevice = nullptr;
	ID3D11DeviceContext*	_ImmediateContext = nullptr;

	//マルチウィンドウ
	std::map<HWND,RenderStatus*> _ClientRenderStatus;


	ConstantBuffer* _ConstantBuffer;
	Light* _Light;
	Material* _Material;


	//コンストラクタ
	D3DRenderer();
	//デストラクタ
	~D3DRenderer();


public:

	static HRESULT Create();
	static HRESULT Destroy();

	static D3DRenderer* GetInstance() { return pInstance; };
	static RenderStatus* GetRenderStatus() { return pTargetRenderStatus; };
	static RenderStatus* GetRenderStatus(HWND hWnd);

	static void SetRenderStatus(RenderStatus* status) { pTargetRenderStatus = status; };

	IDXGIFactory* GetFactory() { return _pFactory; };

	//D3DDevice取得
	ID3D11Device* GetDevice() { return _D3DDevice; }
	//デバイスコンテキスト
	ID3D11DeviceContext* GetDeviceContext() { return _ImmediateContext; }

	ConstantBuffer* GetConstantBuffer() { return _ConstantBuffer; };

	//描画ターゲット作成
	HRESULT CreateRenderStatus(HWND hWnd, RenderStatus** renderstatus,unsigned int fps);


	void CreateBuffer(unsigned int BindFlag, unsigned int byteWidth, const void* subresource, ID3D11Buffer** buffer);
	void SetVertexBuffer(ID3D11Buffer * VertexBuffer, UINT stride, UINT offset);
	void SetIndexBuffer(ID3D11Buffer* IndexBuffer);

	void SetTexture(ID3D11ShaderResourceView* texture, UINT slot = 0, UINT numView = 1);
	void DrawIndexed(UINT IndexCount, UINT StartIndexLocation, int BaseVertexLocation);

	HRESULT CreateSwapChain(IDXGISwapChain** SwapChain, DXGI_SWAP_CHAIN_DESC* sd );

	void SetRasterize(D3D11_FILL_MODE fillmode, D3D11_CULL_MODE cullmode);
	

	//*********************************************************************************************************************
	//
	//	ConstantBuffer
	//
	//*********************************************************************************************************************
	void SetWorldViewProjection2D();
	void SetWorldMatrix(XMMATRIX* WorldMatrix);
	void SetViewMatrix(XMMATRIX* ViewMatrix);
	void SetProjectionMatrix(XMMATRIX* ProjectionMatrix);
	void SetProjectionMatrix2D();

	XMMATRIX GetProjectionMatrix2D();


};